#ifndef H_LINKEDLIST_H
#define H_LINKEDLIST_H

typedef struct list {
    void * info;
    struct list * next;
} LinkedList;

LinkedList * add(LinkedList ** root, void * info, int (*compare)(void * a, void * b));
LinkedList * removeHead(LinkedList ** root);
void printAll(LinkedList * root, void (*print)(LinkedList * node));

#endif