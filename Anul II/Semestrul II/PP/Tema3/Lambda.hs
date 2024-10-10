module Lambda where

import Expr
import Data.List
import Data.Maybe

-- TODO 1.1. find free variables of a Expr
free_vars :: Expr -> [String]
free_vars (Variable x) = [x]
free_vars (Function x e) = filter (/=x) (free_vars e)
free_vars (Application e1 e2) = nub (free_vars e1 ++ free_vars e2)

-- TODO 1.2. reduce a redex
reduce :: Expr -> String -> Expr -> Expr
reduce (Variable y) x e2
  | y == x = e2
  | otherwise = Variable y
reduce (Function y e) x e2 
  | y == x = Function y e
  | y `elem` freeVarsE2 = let y' = avoid_collision y (freeVarsE2 ++ free_vars e2)
                          in Function y' (reduce (rename_var e y y') x e2)
  | otherwise = Function y (reduce e x e2)
  where
    freeVarsE2 = free_vars e2
reduce (Application e1 e2) x e3 =
  Application (reduce e1 x e3) (reduce e2 x e3)

avoid_collision :: String -> [String] -> String
avoid_collision var vars
  | var `elem` vars = avoid_collision (var ++ "'") vars
  | otherwise = var

rename_var :: Expr -> String -> String -> Expr
rename_var (Variable y) x newY
  | y == x = Variable newY
  | otherwise = Variable y
rename_var (Function y e) x newY
  | y == x = Function newY (rename_var e x newY)
  | otherwise = Function y (rename_var e x newY)
rename_var (Application e1 e2) x newY =
  Application (rename_var e1 x newY) (rename_var e2 x newY)

-- Normal Evaluation
-- TODO 1.3. perform one step of Normal Evaluation
stepN :: Expr -> Expr
stepN (Variable x) = Variable x 
stepN (Function x e) = Function x (stepN e)
stepN (Application (Function x e1) e2) = reduce e1 x e2
stepN (Application e1 e2) = case stepN e1 of
           (Function x e) -> Application (Function x e) e2
           e' -> Application e' (stepN e2)
stepN (_) = error ("Invalid ecuation")

-- TODO 1.4. perform Normal Evaluation
reduceN :: Expr -> Expr
reduceN expr = case stepN expr of
  expr' | expr' == expr -> expr
        | otherwise -> reduceN expr' 

reduceAllN :: Expr -> [Expr]
reduceAllN expr = expr : if is_normal_form expr
                          then []
                          else reduceAllN (stepN expr)
  where
    is_normal_form :: Expr -> Bool
    is_normal_form (Variable _)       = True
    is_normal_form (Function _ _)     = True
    is_normal_form (Application _ _)  = False

-- Applicative Evaluation
-- TODO 1.5. perform one step of Applicative Evaluation
stepA :: Expr -> Expr
stepA (Variable x) = Variable x
stepA (Function x e) = Function x (stepA e)
stepA (Application (Function x e1) e2)
  | isApplication e2 = Application (Function x e1) (stepA e2)
  | otherwise = reduce e1 x e2
stepA (Application e1 e2)
  | isApplication e1 = Application (stepA e1) e2
  | otherwise = Application e1 (stepA e2)

isApplication :: Expr -> Bool
isApplication(Application _ _) = True
isApplication(_) =  False

-- TODO 1.6. perform Applicative Evaluation
reduceA :: Expr -> Expr
reduceA expr = case stepA expr of
  expr' | expr == expr' -> expr
        | otherwise -> reduceA expr' 

reduceAllA :: Expr -> [Expr]
reduceAllA expr = expr : if is_apl_form expr
                          then []
                          else reduceAllA (stepA expr)
  where
    is_apl_form :: Expr -> Bool
    is_apl_form (Variable _)       = True
    is_apl_form (Function _ _)     = True
    is_apl_form (Application _ _)  = False

-- TODO 3.1. make substitutions into a expression with Macros
evalMacros :: [(String, Expr)] -> Expr -> Expr
evalMacros _ (Variable name) = Variable name 
evalMacros context (Function param body) = Function param (evalMacros context body)
evalMacros context (Application func arg) = Application (evalMacros context func) (evalMacros context arg) 
evalMacros context (Macro name) = case lookup name context of
    Just expr -> evalMacros context expr 
    Nothing -> Variable name  

-- TODO 4.1. evaluate code sequence using given strategy
evalCode :: (Expr -> Expr) -> [Code] -> [Expr]
evalCode strategy codes = evalHelper strategy codes []
  where
    evalHelper _ [] _ = []
    evalHelper strategy (code : codes) acc = case code of
      Assign name expr -> evalHelper strategy codes ((name, expr) : acc)
      Evaluate expr -> (strategy (evalMacros acc expr)) : (evalHelper strategy codes acc)