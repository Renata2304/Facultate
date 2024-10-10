module Parser (parse_expr, parse_code) where

import Control.Monad
import Control.Applicative
import Expr
import Data.Char

-- Parser data type
newtype Parser a = Parser {
    parse :: String -> Maybe(a, String)
}

--- type declaration ---

instance Monad Parser where
    return x = Parser $ \s -> Just (x, s)
    (>>=) mp f = Parser $ \s ->
        case parse mp s of
            Nothing -> Nothing
            Just (x, s') -> parse (f x) s'

instance Applicative Parser where
    pure x = return x
    pf <*> px = do
        f <- pf
        x <- px
        return $ f x

instance Functor Parser where
    fmap f px = do
        x <- px
        return $ f x

instance Alternative Parser where
    empty = Parser $ const Nothing
    p <|> q = Parser $ \s ->
        case parse p s of
            Nothing -> parse q s
            just -> just
            
--- type declaration over ---

charParser :: Char -> Parser Char
charParser c = Parser $ \s -> case s of 
                                [] -> Nothing
                                (x:xs) -> if x == c then Just (c, xs) else Nothing

predicateParser :: (Char -> Bool) -> Parser Char
predicateParser pred = Parser $ \s -> case s of
                                        [] -> Nothing
                                        (x:xs) -> if pred x then Just (x, xs) else Nothing

manyParser :: Parser a -> Parser [a]
manyParser p = do
    x <- p
    xs <- manyParser p <|> return []
    return (x:xs)

name :: Expr -> String
name (Variable a) = a
name (_) = ""

exprParser :: Parser Expr
exprParser = varParser <|> functionExprParser <|> appParser <|> macroParser

varParser :: Parser Expr
varParser = takeParan <|> do
    var <- predicateParser isLetter
    return (Variable [var])

functionExprParser :: Parser Expr
functionExprParser = takeParan <|> do
    charParser '\\'
    str <- varParser
    charParser '.'
    e <- exprParser
    return (Function (name str) e)

macroParser :: Parser Expr
macroParser = takeParan <|> do
    charParser '$'
    nameMacro <- some (predicateParser isLetter) 
    return (Macro nameMacro)

appParser :: Parser Expr
appParser = takeParan <|> do
    charParser '('
    str <- exprParser
    e <- some addExpr
    charParser ')'
    return (foldl Application str e)

addExpr :: Parser Expr
addExpr = do
  charParser ' '
  exprParser

-- TODO 2.1. parse an expression
parse_expr :: String -> Expr
parse_expr s = case parse exprParser (addParan s) of
    Just (expr, _) -> expr
    Nothing -> error ("Invalid expression" ++ s)

-- pt cazurile in care nu exista paranteza la application
addParan :: String -> String
addParan(s) = "(" ++ s ++ ")"

takeParan :: Parser Expr
takeParan = do
    charParser '('
    expr <- exprParser
    charParser ')'
    return expr

-- TODO 4.2. parse code
parse_code :: String -> Code
parse_code (str) = 
    if (elem '=' str) then case parse parse_aux (str) of
        Just (expr, _) -> expr
        Nothing -> error ("Invalid expression")
    else Evaluate(parse_expr str)  

parse_aux :: Parser Code
parse_aux = do
    str <- manyParser (predicateParser isLetter)
    manyParser (charParser ' ') <|> (return [])
    manyParser (charParser '=') <|> (return [])
    manyParser (charParser ' ') <|> (return [])
    e <- exprParser
    return (Assign str e)
