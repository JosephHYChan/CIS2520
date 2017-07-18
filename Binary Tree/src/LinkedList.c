#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "LinkedList.h"

LinkedList * add(LinkedList ** root, void * info, int (*compare)(void * a, void * b))
{   
    LinkedList * node = *root;
    
    if (*root == NULL) {
        *root = (LinkedList *)malloc(sizeof(LinkedList));
        (*root)->info = info;
        (*root)->next = NULL;
        node = *root;
    } else if ((compare == NULL) || (compare(info, (*root)->info) > 0)) {
        node = add(&(*root)->next, info, compare);
    } else if (compare(info, (*root)->info) < 0) {
        *root = (LinkedList *)malloc(sizeof(LinkedList));
        (*root)->info = info;
        (*root)->next = node;
        node = *root;
    }
    return node;
}

LinkedList * removeHead(LinkedList ** root)
{
    LinkedList * node;
    
    node = *root;
    if (*root != NULL) {
        *root = (*root)->next;
    }
    return node;
}

void printAll(LinkedList * root, void (*print)(LinkedList * node))
{
    if(root != NULL) {
        print(root);
        printAll(root->next, print);
    } else {
        printf("\n");
    }
}