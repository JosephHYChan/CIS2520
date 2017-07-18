#ifndef H_FILEMANAGER_H
#define H_FILEMANAGER_H
#include <stdbool.h>
#include "TreeInterface.h"

#define NAME_SIZE 80
#define FULL_PATH_SIZE 256

typedef struct folder {
    char name[NAME_SIZE];
    Tree children;
} folder;
/*	Preconditions: An existing tree with nodes. Takes in the node and file path as parameters
	Postconditions: Node is detached from the tree and the file path is removed along with it
*/
void detachNode(char * node, char * path);
/*	Preconditions: None
	Postconditions: The desired value is copied and applied to the source which is the second parameter
*/
void * copyValue(void * target, void * source);
/*	Preconditions: None
	Postconditions: THe desired value is removed/destroyed
*/
void destroyValue(void * occupied);
/*	Preconditions: Takes in two values as parameters
	Postconditions: Two values are compared and based on the results will return an integer
*/
int compareValues(void * first, void * second);
/*	Preconditions: An existing tree
	Postconditions: The data is stored in the tree at the specified node which is identified by the path name
*/
void store(Tree * tree, char * path);
/*	Preconditions: An existing tree with nodes
	Postconditions: The child nodes are printed
*/
bool printChildren(Tree * tree, TreeNode * node, char * path);
/*	Preconditions:An existing tree
	Postconditions:All the paths that exist are printed
*/
void printAllPaths(Tree * tree, char * filter);
/*	Preconditions:An existing tree and node. The thrid parameter is the name of the root folder specified by the command line argument
	Postconditions: If the desired file is found, the entire path is printed
*/
void search(Tree * tree, char * nodeName, char * prefix);
/*	Preconditions:
	Postconditions:
*/
void moveNode(Tree * tree, char * source, char * target);
/*	Preconditions:
	Postconditions:
*/
void deleteNode(Tree * tree, char * nodeName);
/*	Preconditions:
	Postconditions:
*/
void renameNode(Tree * tree, char * nodeName, char * newname);

#endif

