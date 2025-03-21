// C program to insert a node in AVL tree
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
 
// An AVL tree node
struct Node
{
    int key;
    struct Node *left;
    struct Node *right;
    int height;
};

// A utility function to get maximum of two integers
int max(int a, int b);
  
// A utility function to get height of the tree
int height(struct Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}
  
// A utility function to get maximum of two integers
int max(int a, int b) {
    return (a > b)? a : b;
}
  
/* Helper function that allocates a new node with the given key and
    NULL left and right pointers. */
struct Node* newNode(int key) {
    struct Node* node = (struct Node*)
                        malloc(sizeof(struct Node));
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;  // new node is initially added at leaf
    return(node);
}
  
// A utility function to right rotate subtree rooted with y
// See the diagram given above.
struct Node *rightRotate(struct Node *y) {
    struct Node *x = y->left;
    struct Node *T2 = x->right;
  
    // Perform rotation
    x->right = y;
    y->left = T2;
  
    // Update heights
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
  
    // Return new root
    return x;
}
  
// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct Node *leftRotate(struct Node *x) {
    struct Node *y = x->right;
    struct Node *T2 = y->left;
  
    // Perform rotation
    y->left = x;
    x->right = T2;
  
    //  Update heights
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
  
    // Return new root
    return y;
}
  
// Get Balance factor of node N
int getBalance(struct Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}
  
struct Node* insert(struct Node* node, int key) {
    /* 1.  Perform the normal BST rotation */
    if (node == NULL)
        return(newNode(key));
  
    if (key < node->key)
        node->left  = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else // Equal keys not allowed
        return node;
  
    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left),
                           height(node->right));
  
    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(node);
  
    // If this node becomes unbalanced, then there are 4 cases
  
    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);
  
    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
  
    // Left Right Case
    if (balance > 1 && key > node->left->key)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
  
    // Right Left Case
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
  
    /* return the (unchanged) node pointer */
    return node;
}
  
/* Given a non-empty binary search tree, return the
   node with minimum key value found in that tree.
   Note that the entire tree does not need to be
   searched. */
struct Node * minValueNode(struct Node* node) {
    struct Node* current = node;
  
    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;
  
    return current;
}

/* Given a non-empty binary search tree, return the
   node with minimum key value found in that tree.
   Note that the entire tree does not need to be
   searched. */
struct Node * maxValueNode(struct Node* node) {
    struct Node* current = node;
  
    /* loop down to find the leftmost leaf */
    while (current->right != NULL)
        current = current->right;
  
    return current;
}
  
// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
struct Node* deleteNode(struct Node* root, int key) {
    // STEP 1: PERFORM STANDARD BST DELETE
    if (root == NULL)
        return root;

    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if ( key < root->key )
        root->left = deleteNode(root->left, key);
  
    // If the key to be deleted is greater than the
    // root's key, then it lies in right subtree
    else if( key > root->key )
        root->right = deleteNode(root->right, key);
  
    // if key is same as root's key, then This is
    // the node to be deleted
    else
    {
        // node with only one child or no child
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct Node *temp = root->left ? root->left :
                                             root->right;
  
            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
             *root = *temp; // Copy the contents of
                            // the non-empty child
            free(temp);
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            struct Node* temp = minValueNode(root->right);
  
            // Copy the inorder successor's data to this node
            root->key = temp->key;
  
            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->key);
        }
    }
  
    // If the tree had only one node then return
    if (root == NULL)
      return root;
  
    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left),
                           height(root->right));
  
    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
    int balance = getBalance(root);
  
    // If this node becomes unbalanced, then there are 4 cases
  
    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
  
    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }
  
    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
  
    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
  
    return root;
}

void destroyTree(struct Node* root) {
	if (root != NULL) {
		destroyTree(root->left);
		destroyTree(root->right);
		free(root);
	}
}

// A utility function to print preorder traversal of
// the tree.
// The function also prints height of every node
void preOrder(FILE* output, struct Node *root) {
    if(root != NULL) {
        fprintf(output, "%d ", root->key);
        preOrder(output, root->left);
        preOrder(output, root->right);
    }
}

int verifIfContains(struct Node *root, int val) {
    if (root) {
        while (root) {
            if (val < root->key) {
                root = root->left;
            } else if (val > root->key) {
                root = root->right;
            } else {
                return 1;
            }
        }
        return 0;
    }
}

/* Driver program to test above function*/
int main() {
    FILE *input, *output;

    char in[20], out[20];

    strcpy(in, "test.in");
    strcpy(out, "test.out");

    input = fopen(in, "r");
    output = fopen(out, "w");

    int N = 0;
    fscanf(input, "%d", &N);

    struct Node *root = NULL;

    int start_time = clock();

    for (int i = 0; i < N; i++) {
        int choice = 0, a = 0, b = 0; 
        fscanf(input, "%d", &choice);
        if (choice != 4) {
            fscanf(input, "%d", &a);
        }
        if (choice == 3) {
            fscanf(input, "%d", &b);
        }

        switch (choice) {
            case 0: {
                root = insert(root, a);
                break;
            }     
            case 1: {
                root = deleteNode(root, a);
                break;
            }
            case 2: {
                fprintf(output, "%d\n", verifIfContains(root, a));
                break;
            }
            case 3: {
                if (verifIfContains(root, a)) {
                    root = deleteNode(root, a);
                    root = insert(root, b);
                }
                break;
            }
            case 4: {
                preOrder(output, root);
                if (root) {
                    fprintf(output, "\n");
                }
                break;
            }
        }
    }
    
    int end_time = clock();

    FILE *time = fopen ("timeAVL", "a");

    fprintf(time, "%f\n",
    (double) (end_time - start_time) * 1000 / CLOCKS_PER_SEC);
    
    fclose (time);

    destroyTree(root);
    root = NULL;
    fclose(input); fclose(output);

    return 0;
}