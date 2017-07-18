#ifndef H_RESTAURANT_H
#define H_RESTAURANT_H
#include "treeInterface.h"
#include "LinkedList.h"

typedef struct restaurant {
    char name[50];
    char type[15];
    double rate;    
} restaurant;

void printAllInfoInNode(TreeDataTypePtr data);
int compareName(void * data1, void *data2);
int compareForLowerRatingFirst(void * data1, void *data2);
int compareForHigherRatingFirst(void * data1, void *data2);
void destroyRatingInfo(TreeDataTypePtr data);
void printDepthFirst(Tree * tree, int layer, int nodeType, int ancestorType, int ancestorLevel);
void printBreadthFirst(Tree * tree);

#endif
