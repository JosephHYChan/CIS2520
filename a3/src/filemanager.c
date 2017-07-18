#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "filemanager.h"

void detachNode(char * node, char * path)
{
    char * token;
    char rest[FULL_PATH_SIZE] = "";
    
    token = strtok(path, "\\");
    if (token != NULL) {
        strcpy(node, token);
        token = strtok(NULL, "\\");
        while (token != NULL) {
            strcat(rest, "\\");
            strcat(rest, token);
            token = strtok(NULL, "\\");
        }
    }
    strcpy(path, rest);
}

void * copyValue(void * target, void * source) {
    if (target == NULL) {
        target = malloc(NAME_SIZE*sizeof(char)+sizeof(Tree));
        memcpy(target, source, NAME_SIZE*sizeof(char)+sizeof(Tree));
        return target;
    } else if (source == NULL) {
        return NULL;
    } else {
        memcpy(target, source, NAME_SIZE*sizeof(char)+sizeof(Tree));
        return target;
    }
}

void destroyValue(void * occupied) {
    TreeNode * node = (TreeNode *)occupied;
    free((int *)node->value);
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    free(node);
}


int compareValues(void * first, void * second) 
{
    int result = 0;
    
    if (strcmp(((folder *)first)->name, ((folder *)second)->name) > 0) {
        result = 1;
    } else if (strcmp(((folder *)first)->name, ((folder *)second)->name) < 0) {
        result = -1;
    } else {
        result = 0;
    }
    return result;
}

void printTree(Tree * tree, TreeNode * node) {
    folder content;
    if (node != NULL) {
        printTree(tree, node->left);
        tree->copyValue(&content, node->value);
        printf("%s\n", content.name);
        printTree(tree, node->right);
    }
}

bool searchNodeInTree(Tree * tree, TreeNode * node, char * nodeName)
{
    TreeNode * subtree;
    char source[80] = "" ;
    char currentPath[FULL_PATH_SIZE];
    bool found = false;
    folder leaf;

    strcpy(currentPath, nodeName);   
    detachNode(source, nodeName);
    strcpy(leaf.name, source);
    leaf.children.root = NULL;
    if (node != NULL) {
        if (strlen(source)>0 && strlen(nodeName)>0) {
            if (strcmp(((folder *)node->value)->name, source) == 0) {
                found = searchNodeInTree(&(((folder *)node->value)->children), ((folder *)node->value)->children.root, nodeName);
            } else {
                found = false;
            }
            if (!found) {
                strcpy(nodeName, currentPath);   
                found = searchNodeInTree(tree, node->left, nodeName);
            }
            if (!found) {
                strcpy(nodeName, currentPath);   
                found = searchNodeInTree(tree, node->right, nodeName);
            }
        } else if (strlen(nodeName) == 0) {
            Locate(tree, (void *)&leaf);
            subtree = tree->current;
            if (subtree != NULL) {
                found = true;
            } else {
                found = false;
            }
        }
        return found;
    }
    return false;
}

void replaceLeaf(char * oldname, char * newname, char * newpath)
{
    char node[80];
    char source[FULL_PATH_SIZE];
    
    strcpy(newpath, "");
    strcpy(source, oldname);
    detachNode(node, source);
    while (strlen(source)>0) {
        strcat(newpath, "\\");
        strcat(newpath, node);
        detachNode(node, source);
    }
    strcat(newpath, "\\");
    strcat(newpath, newname);
}

void replaceDirectory(char * oldname, char * target, char * newpath)
{
    char node[80];
    char source[FULL_PATH_SIZE];
    
    strcpy(newpath, "");
    strcpy(source, oldname);
    detachNode(node, source);
    while (strlen(source)>0) {
        detachNode(node, source);
    }
    strcat(newpath, target);
    strcat(newpath, "\\");
    strcat(newpath, node);
}

bool renameNodeInTree(Tree * tree, TreeNode * node, char * oldname, char * newname)
{
    TreeNode * subtree;
    char source[80] = "" ;
    char currentPath[FULL_PATH_SIZE];
    bool done = false;
    folder leaf;

    strcpy(currentPath, oldname);   
    detachNode(source, oldname);
    strcpy(leaf.name, source);
    leaf.children.root = NULL;
    if (node != NULL) {
        if (strlen(source)>0 && strlen(oldname)>0) {
            if (strcmp(((folder *)node->value)->name, source) == 0) {
                done = renameNodeInTree(&(((folder *)node->value)->children), ((folder *)node->value)->children.root, oldname, newname);
            } else {
                done = false;
            }
            if (!done) {
                strcpy(oldname, currentPath);   
                done = renameNodeInTree(tree, node->left, oldname, newname);
            }
            if (!done) {
                strcpy(oldname, currentPath);   
                done = renameNodeInTree(tree, node->right, oldname, newname);
            }
        } else if (strlen(oldname) == 0) {
            Locate(tree, (void *)&leaf);
            subtree = tree->current;
            if (subtree != NULL) {
                strcpy(((folder *)node->value)->name, newname);
                done = true;
            } else {
                done = false;
            }
        }
        return done;
    }
    return false;
}

bool deleteNodeFromTree(Tree * tree, TreeNode * node, char * nodeName)
{
    char source[80] = "" ;
    char currentPath[FULL_PATH_SIZE];
    bool done = false;
    folder leaf;

    strcpy(currentPath, nodeName);   
    detachNode(source, nodeName);
    strcpy(leaf.name, source);
    leaf.children.root = NULL;
    if (node != NULL) {
        if (strlen(source)>0 && strlen(nodeName)>0) {
            if (strcmp(((folder *)node->value)->name, source) == 0) {
                done = deleteNodeFromTree(&(((folder *)node->value)->children), ((folder *)node->value)->children.root, nodeName);
            } else {
                done = false;
            }
        } else if (strlen(nodeName) == 0) {
            if (strcmp(((folder *)node->value)->name, source) == 0) {
                Remove (tree, (void *)&leaf);
                done = true;
            } else {
                done = false;
            }
        }
        if (!done) {
            strcpy(nodeName, currentPath);   
            done = deleteNodeFromTree(tree, node->left, nodeName);
        }
        if (!done) {
            strcpy(nodeName, currentPath);   
            done = deleteNodeFromTree(tree, node->right, nodeName);
        }
        return done;
    }
    return false;
}

bool printChildren(Tree * tree, TreeNode * node, char * path)
{
    TreeNode * subtree;
    char source[80] = "" ;
    char currentPath[FULL_PATH_SIZE];
    bool done = false;
    folder leaf;

    strcpy(currentPath, path);   
    detachNode(source, path);
    strcpy(leaf.name, source);
    leaf.children.root = NULL;
    if (node != NULL) {
        if (strlen(source)>0 && strlen(path)>0) {
            if (strcmp(((folder *)node->value)->name, source) == 0) {
                done = printChildren(&(((folder *)node->value)->children),((folder *)node->value)->children.root, path);
            } else {
                done = false;
            }
            if (!done) {
                strcpy(path, currentPath);   
                done = printChildren(tree, node->left, path);
            }
            if (!done) {
                strcpy(path, currentPath);   
                done = printChildren(tree, node->right, path);
            }
        } else if (strlen(path) == 0) {
            Locate(tree, (void *)&leaf);
            subtree = tree->current;
            if (subtree != NULL) {
                if (((folder *)subtree->value)->children.root != NULL) {
                    printTree(tree, ((folder *)subtree->value)->children.root);
                } else {
                    printf("%s\n", ((folder *)subtree->value)->name);
                }
                done = true;
            } else {
                done = false;
            }
        }
        return done;
    }
    return false;
}

void printPath(Tree * tree, TreeNode * root, char * path, char * filter)
{
    char currentPath[FULL_PATH_SIZE];
    folder content;

    strcpy(currentPath, path);
    if (root != NULL) {
        printPath(tree, root->left, path, filter);
        copyValue(&content, root->value);
        strcpy(path, currentPath);
        if ((content.children).root != NULL) {
            strcat(path, "\\");
            strcat(path, content.name);
            printPath(tree, (content.children).root, path, filter);
            if (strstr(path, filter) != NULL){
                printf("%s:\n", path);
                printChildren(tree, tree->root, path);
                printf("\n");
            }
        }
        strcpy(path, currentPath);
        printPath(tree, root->right, path, filter);
    }
}

void store(Tree * tree, char * path)
{
    Tree * childtree;
    TreeNode * subtree =  NULL;
    char node[80] = "";
    folder subdir;
    
    detachNode(node, path);
    if (strlen(node)>0) {
        strcpy(subdir.name, node);
        childtree = &(subdir.children);
        Initialize (&(subdir.children), tree->copyValue, tree->destroyValue, tree->compareValues); 
        Insert (tree, (void *)&subdir);
        
        Locate (tree, (void *)&subdir);

        subtree = tree->current;
        if (subtree != NULL) {
            store(&(((folder *)((TreeNode *)subtree)->value)->children), path);
        }

    }
}

TreeNode * searchNode(TreeNode * node, bool * found, char * nodeName, char * prefix, char * path)
{
    TreeNode * current = NULL;
    char currentPath[FULL_PATH_SIZE];
    folder content;
    
    strcpy(currentPath, path);
    if (node == NULL) {
        current = NULL;
    } else {
        copyValue(&content, node->value);
        strcat(path, "\\");
        strcat(path, content.name);
        if (strcmp(content.name, nodeName) == 0) {
            *found = true;
            current = node;
            if (strstr(path, prefix) != NULL) {
                printf("%s is found in %s\n", nodeName, path);
            }
        } else if (content.children.root != NULL) {
            current = searchNode(content.children.root, found, nodeName, prefix, path);
        }
        strcpy(path, currentPath);
        current = searchNode(node->left, found, nodeName, prefix, path);
        strcpy(path, currentPath);
        current = searchNode(node->right, found, nodeName, prefix, path);
    }
    return current;
}

void search(Tree * tree, char * nodeName, char * prefix)
{
    TreeNode * node = NULL;
    char blank[FULL_PATH_SIZE] = "";
    bool found = false;

    strcpy(blank, "");
    node = searchNode(tree->root, &found, nodeName, prefix, blank);
    
    if (!found) {
        printf("%s cannot be found\n", nodeName);
    }
}

void renameNode(Tree * tree, char * nodeName, char * newname)
{
    char source[128];
    char newpath[128];
    char path[128];
    
    strcpy(source, nodeName);
    replaceLeaf(nodeName, newname, newpath);
    strcpy(path, nodeName);
    if (searchNodeInTree(tree, tree->root, path) == false) {
        printf("%s cannot be renamed as it cannot be found\n", source);
    } else {
        strcpy(path, newpath);
        if (searchNodeInTree(tree, tree->root, path)) {
            printf("%s cannot be renamed as %s already exists\n", source, newpath);
        } else {
            renameNodeInTree(tree, tree->root, source, newname);
        }
    }
}

void deleteNode(Tree * tree, char * nodeName)
{
    char path[128];
    
    strcpy(path, nodeName);
    if (deleteNodeFromTree(tree, tree->root, path) ==  false) {
        printf("%s cannot be deleted\n", nodeName);
    }
}

void moveNode(Tree * tree, char * source, char * target)
{
    /*bool done = false;*/
    char path[128] = "";
    char newpath[128] = "";
    
    replaceDirectory(source, target, newpath);
    strcpy(path, source);
    if (searchNodeInTree(tree, tree->root, path) == false) {
        printf("%s cannot be moved as it cannot be found\n", source);
    } else {
        strcpy(path, newpath);
        if (searchNodeInTree(tree, tree->root, path) == true) {
            printf("%s cannot be moved as %s already exists\n", source, newpath);
        } else {
            deleteNode(tree, source);
            strcpy(path, newpath);            
            store(tree, path);
        }
    }    
}

void printAllPaths(Tree * tree, char * filter)
{
    char path[FULL_PATH_SIZE] = "";

    strcpy(path, "");
    printPath(tree, tree->root, path, filter);
}