#ifndef BINTREE_GUARD
#define BINTREE_GUARD
/**************************************
Interface for a Binary Tree ADT

Author:  Judi McCuaig
October, 2012
****************************************/

/*********************************************
This ADT provides the basic operations for a binary tree. It takes a void pointer (void *)
as the data type.   

Two important typedefs are made available via this interface, one for a data type and 
on for a boolean type.  Feel free to make use of them if you wish.

typedef void * TreeDataTypePtr;
typedef int bool;
enum {false, true};

You must provide a function pointer to a compare function and a destroy function when
you initialize the tree.

the compare function must have following signature (but can be called anything you like)

int compare (void * data1, void * data2);

The compare function must return a number greater than zero if data1  is 'larger' than 
data2,  a number less than zero of data2 is 'larger' than data1, and zero if the two are
equal.  'larger' can be interpreted any way you like

the destroy function must have the signature (but can be called anything you like)
void destroyDataNode(void * node);

Finally,  if you wish to use the built in print functionality, you must write a print
function for your data.  The print function must be a void function that takes a 
single void* parameter
*/
struct BinTree;
typedef struct BinTree Tree;
typedef void * TreeDataTypePtr;
typedef int bool;
enum {false, true};

/************************
createBinTree
this function returns a pointer to a binary tree.  You must send pointers to the compare
and destroy functions when you create the tree.
createBinTree returns null if the memory allocation fails.

An example call to this function is: myTree = createBinTree(&compare, &destroyDataNode);
*********************/

Tree * createBinTree(int (*comparePointer) (TreeDataTypePtr data1, TreeDataTypePtr data2), void (*destroyPointer) (TreeDataTypePtr));


/*************************
destroyBinTree

this function destroys the binary tree and all data that is in the tree
when destroy is called.
****************************/

void  destroyBinTree(Tree * toDie);

/****************************
addToTree

This function adds the data item to the tree. The item is added in-order using the
compare function

*******************************/
void addToTree(Tree * theTree, TreeDataTypePtr data);

/******************************
removeFromTree

This function removes a data item from the tree and adjusts the remaining 
branches of the tree to keep the tree in the proper order

****************************/
void removeFromTree(Tree * theTree, TreeDataTypePtr data);

/*****************************
getLeftSubtree

this function returns a Tree that is rooted in the left subtree of the 
parameter passed in.   This function mallocs memory for the return value.  You 
must free the returned value in your code or your program will leak memory
*****************************/

Tree * getLeftSubtree (Tree *);

/*****************************
getLeftSubtree

this function returns a Tree that is rooted in the right subtree of the 
parameter passed in.   This function mallocs memory for the return value.  You 
must free the returned value in your code or your program will leak memory
*****************************/
Tree * getRightSubtree (Tree *);

/****************************
getRootData

this function returns a pointer to the data that is contained in the root
node of the tree that is passed in as a parameter
******************************/
TreeDataTypePtr getRootData(Tree *);

/*******************************
printInOrder

This function prints the tree using an inOrder traversal
A sample call to this function is: printInOrder(myTree, &printNode);

where printNode is the function to print a data item
*********************************/
void printInOrder(Tree * theTree, void (* printNode) (TreeDataTypePtr data));

/*******************************
printPreOrder

This function prints the tree using a preOrder traversal
A sample call to this function is: printPreOrder(myTree, &printNode);

where printNode is the function to print a data item
*********************************/
void printPreOrder(Tree * theTree, void (* printNode) (TreeDataTypePtr data));



/*********************************
isTreeEmpty

this function returns true if the tree has no data and false otherwise
**********************************/
bool isTreeEmpty(Tree * theTree);

#endif