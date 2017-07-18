/*********************************************************************
 * FILE NAME: TreeInterface.h
 * PURPOSE: Interface of a generic Binary Search Tree ADT
 * AUTHOR: Joseph Chan
 * DATE: 13/11/2015
 *********************************************************************/
#include "TreeInterface.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/*********************************************************************
 * Local functions
 *********************************************************************/
int treeHeight(TreeNode * root) {
    int leftHeight;
    int rightHeight;

    if (root == NULL) {
        return -1;
    } else {
        leftHeight = treeHeight(root->left);
        rightHeight = treeHeight(root->right);
        if (leftHeight>rightHeight) {
            return leftHeight+1;
        } else {
            return rightHeight+1;
        }
    }
}


void rotateRight(Tree * T, TreeNode * unbalancedNode) {
    TreeNode * grandParent = unbalancedNode->parent; 
    TreeNode * newParent;
    
    newParent = unbalancedNode->left;
    unbalancedNode->left = newParent->right;
    if (newParent->right != NULL) {
        newParent->right->parent = unbalancedNode;
    }
    newParent->right = unbalancedNode;
    unbalancedNode->parent = newParent;

    if (grandParent==NULL) {
        T->root = newParent;
        newParent->parent = NULL;
    } else if (grandParent->left == unbalancedNode) {
        grandParent->left =  newParent;
        newParent->parent = grandParent;
    } else {
        grandParent->right =  newParent;
        newParent->parent = grandParent;
    }
}


void rotateLeft(Tree * T, TreeNode * unbalancedNode) {
    TreeNode * grandParent = unbalancedNode->parent; 
    TreeNode * newParent;
    
    newParent = unbalancedNode->right;
    unbalancedNode->right = newParent->left;
    if (newParent->left != NULL) {
        newParent->left->parent = unbalancedNode;
    }
    newParent->left = unbalancedNode;
    unbalancedNode->parent = newParent;

    if (grandParent==NULL) {
        T->root = newParent;
        newParent->parent = NULL;
    } else if (grandParent->left == unbalancedNode) {
        grandParent->left =  newParent;
        newParent->parent = grandParent;
    } else {
        grandParent->right =  newParent;
        newParent->parent = grandParent;
    }
}


void rotateRightThenLeft(Tree * T, TreeNode * unbalancedNode) {
    rotateRight(T, unbalancedNode->right);
    rotateLeft(T, unbalancedNode);
}


void rotateLeftThenRight(Tree * T, TreeNode * unbalancedNode) {
    rotateLeft(T, unbalancedNode->left);
    rotateRight(T, unbalancedNode);
}


int balancedTree(TreeNode * root) {
    if (root == NULL) {
        return 1;
    } else if (abs(treeHeight(root->left)-treeHeight(root->right))<=1) {
        return balancedTree(root->left) && balancedTree(root->right);
    } else {
        return 0;
    }
}
        
        
TreeNode * getUnbalancedAncestor(TreeNode * node) {
    if (node==NULL) {
        return NULL;
    } else if (balancedTree(node->parent)==0) {
        return node->parent;
    } else {
        return getUnbalancedAncestor(node->parent);
    }
}

        
void balanceIt(Tree * T, TreeNode * node) {
    TreeNode * ancestor = getUnbalancedAncestor(node);
    TreeNode * parent = node->parent;
    
    if (Balanced(T)==0) {
        if ((T->compareValues(node->value, ancestor->value) == -1) &&
            (T->compareValues(node->value, parent->value) == -1)) {
            /* 
             Left left case 
             */
            rotateRight(T, ancestor);
        } else if ((T->compareValues(node->value, ancestor->value) == -1) &&
                   (T->compareValues(node->value, parent->value) != -1)) {
            /* 
             Left right case 
             */
            rotateLeftThenRight(T, ancestor);
        } else if ((T->compareValues(node->value, ancestor->value) != -1) &&
                   (T->compareValues(node->value, parent->value) != -1)) {
            /* 
             Right right case 
             */
            rotateLeft(T, ancestor);
        } else if ((T->compareValues(node->value, ancestor->value) != -1) &&
                   (T->compareValues(node->value, parent->value) == -1)) {
            /* 
             Right left case 
             */
            rotateRightThenLeft(T, ancestor);
        }
        balanceIt(T, node);
    }
}

        
TreeNode * addNode(Tree * T, void * I, TreeNode * node, TreeNode * parent) {
    if (node==NULL) {
        node = malloc(sizeof(TreeNode));
        node->value = T->copyValue(NULL, I);
        node->parent = parent;
        node->left = NULL;
        node->right = NULL;
        T->current = node;
    } else if (T->compareValues(I, node->value) == -1) {
        node->left = addNode(T, I, node->left, node);
    } else if (T->compareValues(I, node->value) == 1) {
        node->right = addNode(T, I, node->right, node);
    }
    return node;
}


TreeNode * removeNode(Tree * T, void * I, TreeNode * node, TreeNode * parent)
{   
    TreeNode * current = node;
    
    if (node != NULL) {
        if (T->compareValues(I, node->value) == 0) {
            if ((node->left == NULL) && (node->right == NULL)) {
                node = NULL;
                T->destroyValue(current);
            } else if (node->right == NULL) {
                node = node->left;
                node->parent = parent;
                current->left = NULL;
                T->destroyValue(current);
            } else if (node->left == NULL) {
                node = node->right;
                node->parent = parent;
                current->right = NULL;
                T->destroyValue(current);
            } else {
                current = current->left;
                while (current->right != NULL) current = current->right;
                current->right = node->right;
                current = node;
                node = node->left;
                node->parent = parent;
                current->left = NULL;
                current->right = NULL;
                T->destroyValue(current);
            }   
            T->current = parent;            
        } else if (T->compareValues(I, node->value) > 0) {
            node->right = removeNode(T, I, node->right, node);
        } else {
            node->left = removeNode(T, I, node->left, node);
        }
    }
    return node;
}


TreeNode * getSubTree(Tree * T, void * I, TreeNode * node)
{
    TreeNode * lefttree = NULL;
    TreeNode * righttree = NULL;
    
    if (node == NULL) {
        return NULL;
    } else if (T->compareValues(I, node->value) == 0) {
        return node;
    } else {
        lefttree = getSubTree(T, I, node->left);
        righttree = getSubTree(T, I, node->right);
        return (lefttree!=NULL)?lefttree:righttree;
    }
}


void killTree(TreeNode * root, void (*destroyValue) (void *)) {
    if (root != NULL) {
        killTree(root->left, destroyValue);
        killTree(root->right, destroyValue);
        destroyValue(root);
    }
}

/*********************************************************************
 * FUNCTION NAME: Initialize
 * PURPOSE: Sets a Tree variable to the empty Binary Search Tree.
 * ARGUMENTS: . The address of the Tree variable to be initialized
 *              (Tree *) 
 *            . A pointer to a copy function
 *              ---------------------------- 
 *              PURPOSE: Makes a copy of a value.
 *              ARGUMENTS: . NULL, or the address (void *) of  
 *                           the location in memory where the 
 *                           copy must be stored
 *                         . The address (void *) of the value 
 *                           to be copied
 *              RETURNS: The address (void *) of the location in
 *                       memory where the copy has been stored
 *              NOTES: If the first argument is NULL, the copy
 *                     function allocates memory for the copy,
 *                     and this memory should later be freed
 *                     by the destroy function below.
 *              -------------------------------
 *            . A pointer to a destroy function
 *              -------------------------------
 *              PURPOSE: Frees memory that may have been 
 *                       allocated by the copy function above.
 *              ARGUMENT: The address of the value to be destroyed
 *                        (void *); this address must have been
 *                        returned by the copy function called
 *                        with NULL as its first argument
 *              -------------------------------
 *            . A pointer to a compare function
 *              -------------------------------
 *              PURPOSE: Compares two values.
 *              ARGUMENTS: . A pointer to a first value (void *)
 *                         . A pointer to a second value (void *)
 *              RETURNS: The integer
 *                       -1 if the 1st value is less than the 2nd value,
 *                        0 if the two values are equal,
 *                        1 otherwise
 *              -----------------------
 * NOTES: Initialize is the only function that may be used right
 *        after the declaration of the Tree variable or a call
 *        to Destroy, and it should not be used otherwise.
 *********************************************************************/
void Initialize (Tree *T,
                 void * (*copyValue) (void *, void *),
                 void (*destroyValue) (void *),
                 int (*compareValues) (void *, void *)) {
    T->root = NULL;
    T->current = NULL;
    T->copyValue = copyValue;
    T->destroyValue = destroyValue;
    T->compareValues = compareValues;
    T->size = 0;
}


/*********************************************************************
 * FUNCTION NAME: Insert
 * PURPOSE: Inserts a value in a Binary Search Tree.
 * ARGUMENTS: . The address of the Tree (Tree *)
 *            . The address of the value to be inserted (void *)
 *********************************************************************/
void Insert (Tree *T, void *I) {
    T->root = addNode(T, I, T->root, NULL);
    balanceIt(T, T->current);
    T->size++;
}

/*********************************************************************
 * FUNCTION NAME: Remove
 * PURPOSE: Removes a value in a Binary Search Tree.
 * ARGUMENTS: . The address of the Tree (Tree *)
 *            . The address of the value to be removed (void *)
 *********************************************************************/
void Remove (Tree *T, void *I) {
    T->root = removeNode(T, I, T->root, NULL);
    balanceIt(T, T->current);
    T->size--;
}


/*********************************************************************
 * FUNCTION NAME: Locate
 * PURPOSE: Locate a value in a Binary Search Tree.
 * ARGUMENTS: . The address of the Tree (Tree *)
 *            . The address of the value to be located (void *)
 *********************************************************************/
void Locate (Tree *T, void *I) {
    T->current = getSubTree(T, I, T->root);
}
 
/*********************************************************************
 * FUNCTION NAME: Minimum
 * PURPOSE: Finds the least value (according to the compare 
 *          function whose address was passed to Initialize)
 *          of a Binary Search Tree.
 * ARGUMENTS: . The address of the Tree (Tree *)
 *            . The address (void *) where a copy 
 *              of the least value should be stored 
 * RETURNS: 1 if the minimum has been found,
 *          i.e., if the Tree is not empty,
 *          0 otherwise
 *********************************************************************/
int Minimum (Tree *T, void *I) {
    bool quit = false;
    if (T->root == NULL) {
        return -1;
    } else {
        T->current = T->root;
        while (quit==false) {
            if (T->current->left == NULL) {
                quit = true;
            } else {
                T->current = T->current->left;
            }
        }
        T->copyValue(I, T->current->value);
        return 1;
    }
}


/*********************************************************************
 * FUNCTION NAME: Successor
 * PURPOSE: Finds the successor (according to the compare 
 *          function whose address was passed to Initialize)
 *          in a Binary Search Tree of the last value found by
 *          Successor or Minimum (whichever was called last).
 * ARGUMENTS: . The address of the Tree (Tree *)
 *            . The address (void *) where a copy 
 *              of the successor should be stored 
 * RETURNS: 1 if the successor has been found,
 *          i.e., if the Tree is not empty and if the
 *          last value found by Minimum or Successor
 *          was not the greatest value in the Tree,
 *          0 otherwise
 * NOTES: A call to Successor must be immediately
 *        preceded by a call to Successor or Minimum.
 *********************************************************************/
int Successor (Tree *T, void *I) {
    TreeNode * parent;
    bool quit = false;
    
    if (T->root == NULL) {
        return 0;
    } else {
        parent = T->current->parent;
        quit = (parent == NULL);
        while (quit == false) {
            if (T->compareValues(parent->value, T->current->value) == 1) {
                quit = true;
            } else {
                parent = parent->parent;
                quit = (parent == NULL);
            }
        }
        if (parent == NULL) {
            return 0;
        } else {
            T->current = parent;
            T->copyValue(I, parent->value);
            return 1;
        }
    }
}

/*********************************************************************
 * FUNCTION NAME: Size
 * PURPOSE: Finds the number of values stored in a Binary Search Tree.
 * ARGUMENT: The address of the Tree (Tree *) 
 * RETURNS: The number of values stored in the Tree
 *********************************************************************/
int Size (Tree *T) {
    return T->size;
}

/*********************************************************************
 * FUNCTION NAME: Height
 * PURPOSE: Calculates the height of a Binary Search Tree.
 * ARGUMENT: The address of the Tree (Tree *) 
 * RETURNS: The height of the Tree
 * NOTES: The height of the empty Tree is -1.
 *********************************************************************/
int Height (Tree *T) {
    return treeHeight(T->root);
}
 

/*********************************************************************
 * FUNCTION NAME: Balanced
 * PURPOSE: Checks whether a Binary Search Tree is balanced.
 * ARGUMENT: The address of the Tree (Tree *) 
 * RETURNS: 1 if the Tree is balanced, 0 otherwise
 * NOTES: A Tree is balanced if it is empty or if for any node N
 *        the heights of N's subTrees are equal or differ by 1.
 *********************************************************************/
int Balanced (Tree *T) {
    return balancedTree(T->root);
}


/*********************************************************************
 * FUNCTION NAME: Destroy
 * PURPOSE: Frees memory that may have been 
 *          allocated by Initialize and Insert.
 * ARGUMENTS: The address of the Tree to be destroyed (Tree *) 
 * NOTES: The last function to be called should always be Destroy. 
 *********************************************************************/
void Destroy (Tree *T) {
    killTree(T->root, T->destroyValue);
}


