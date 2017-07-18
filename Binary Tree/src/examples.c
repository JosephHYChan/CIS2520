#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "restaurant.h"
#define NODE_INFO_SIZE 32
#define INTER_SPACE_SIZE 4

int maximum(int a, int b)
{
    return (a>=b)?a:b;    
}

int treeDepth(Tree * tree)
{
    if (tree == NULL) {
        return 0;
    } else {
        return 1 + maximum(treeDepth(getLeftSubtree(tree)), treeDepth(getRightSubtree(tree)));
    }
}

double power(double num, int iteration)
{
    if (iteration==0) {
        return 1;
    } else if (iteration>0) {
        return num*power(num, iteration-1);
    } else {
        return (1/power(num, -1*iteration));
    }
}

void printAllInfoInNode(TreeDataTypePtr data) 
{
    printf("Name : %s\nType : %s\nRating : %.1f / 5.0\n\n", ((restaurant *)data)->name, ((restaurant *)data)->type, ((restaurant *)data)->rate);
}

void printDuplicate(short amount, char prtch) 
{
    int count = 0;
    while (count<amount) {
        printf("%c", prtch);
        count++;
    }
}

void printNameAndRatingInNode(TreeDataTypePtr data, int nodeSize, bool depthFirst)
{
    if (depthFirst) {
        printf("%s (%.1f)", ((restaurant *)data)->name, ((restaurant *)data)->rate);
        printDuplicate(nodeSize-strlen(((restaurant *)data)->name)-7,'-');
        printf("+\n");
    } else {
        printf("+");
        printDuplicate((nodeSize-strlen(((restaurant *)data)->name)-7)/2,'-');
        if (strlen(((restaurant *)data)->name)%2 == 1) {
            printf("%s(%.1f)", ((restaurant *)data)->name, ((restaurant *)data)->rate);
        } else {
            printf("%s (%.1f)", ((restaurant *)data)->name, ((restaurant *)data)->rate);
        }
        printDuplicate((nodeSize-strlen(((restaurant *)data)->name)-7)/2,'-');
        printf("+");
    }
}

void printDepthFirst(Tree * tree, int layer, int nodeType, int ancestorType, int ancestorLevel)
{
    TreeDataTypePtr data = NULL;
    Tree * leftson = NULL;
    Tree * rightson = NULL;
    int offset = 1;
    
    if (tree != NULL) {
        data = getRootData(tree);
        leftson = getLeftSubtree(tree);
        rightson = getRightSubtree(tree);
        printDepthFirst(rightson, layer+1, 1, nodeType, layer);
        if (nodeType==0) {
            if (data != NULL) {
                printf("--+");
                printNameAndRatingInNode(data, NODE_INFO_SIZE, true);
            }
        } else if (nodeType==1) {
            offset = 1;
            printDuplicate(3,' ');
            if (ancestorType == 2) {
                printDuplicate((ancestorLevel-offset)*NODE_INFO_SIZE-1, ' ');
                printf("|");
                offset = ancestorLevel;
            }
            printDuplicate((layer-offset-1)*NODE_INFO_SIZE+NODE_INFO_SIZE-1,' ');
            printf("+");
            if (data != NULL) {
                printNameAndRatingInNode(data, NODE_INFO_SIZE, true);
                offset = 1;
                printDuplicate(3,' ');
                if (ancestorType == 2) {
                    printDuplicate((ancestorLevel-offset)*NODE_INFO_SIZE-1, ' ');
                    printf("|");
                    offset = ancestorLevel;
                }
                printDuplicate((layer-offset)*NODE_INFO_SIZE-1, ' ');
                printf("|");
            }
            if(leftson!=NULL) {
                printDuplicate(NODE_INFO_SIZE-1, ' ');
                printf("|");
            }
            printf("\n");
        } else {
            if (data != NULL) {
                offset = 1;
                printDuplicate(3,' ');
                if (ancestorType == 1) {
                    printDuplicate((ancestorLevel-offset)*NODE_INFO_SIZE-1, ' ');
                    printf("|");
                    offset = ancestorLevel;
                }
                printDuplicate((layer-offset)*NODE_INFO_SIZE-1, ' ');
                printf("|");
                if(rightson!=NULL) {
                    printDuplicate(NODE_INFO_SIZE-1, ' ');
                    printf("|");
                }
                printf("\n");
                offset = 1;
                printDuplicate(3,' ');
                if (ancestorType == 1) {
                    printDuplicate((ancestorLevel-offset)*NODE_INFO_SIZE-1, ' ');
                    printf("|");
                    offset = ancestorLevel;
                }
                printDuplicate((layer-offset-1)*NODE_INFO_SIZE+NODE_INFO_SIZE-1,' ');
                printf("+");
                printNameAndRatingInNode(data, NODE_INFO_SIZE, true);
            }
        }
        if ((ancestorType==1) && (nodeType==2)) {
            printDepthFirst(leftson, layer+1, 2, ancestorType, ancestorLevel);
        } else {
            printDepthFirst(leftson, layer+1, 2, nodeType, layer);
        }
    }
}

void validNodeExistence(Tree * tree, int nodeID, int level, bool nodeExists[127]) 
{
    int startNode = power(2,level)-1;
    int lsonID = 0;
    int rsonID = 0;
    
    if (tree != NULL) {
        nodeExists[nodeID] = true;
        lsonID = 2*(nodeID-startNode)+startNode+power(2,level);
        rsonID = 2*(nodeID-startNode)+startNode+power(2,level)+1;
        validNodeExistence(getLeftSubtree(tree), lsonID, level+1, nodeExists);
        validNodeExistence(getRightSubtree(tree), rsonID, level+1, nodeExists);
    } else {
        nodeExists[nodeID] = false;
    }       
}

void printBreadthFirst(Tree * tree)
{
    int depth = treeDepth(tree);
    int layer = 1;
    int count = 0;
    int nodePosition = 0;
    int nodesInCurrentLevel = 1;
    int nodesInNextLevel = 0;
    int nodesFound = 0;
    int nodeLength = 0;
    int paddingOffset = INTER_SPACE_SIZE/2;
    int paddingSpaceSize =0;
    int interSpaceSize = 0;
    int maxNumOfNodes = 0;
    int startNodePos = 0;
    int endNodePos = 0;
    int latestNodePos = 0;
    bool nodeExists[127] = {false};
    bool needPadding= true;
    Tree * node = tree;
    Tree * leftson = NULL;
    Tree * rightson = NULL;
    LinkedList * queue = NULL;
    LinkedList * head = NULL;
    
    if (tree != NULL) {
        add(&queue, (void *)tree, NULL);
        nodeExists[nodePosition++] = true;
        maxNumOfNodes = power(2,layer-1); 
        startNodePos = maxNumOfNodes - 1;
        endNodePos = startNodePos + maxNumOfNodes -1;
        latestNodePos = startNodePos;
        validNodeExistence(tree, 0, 0, nodeExists);
        while (queue != NULL) {
            head = removeHead(&queue);
            node = (Tree *)head->info;
            if (layer<depth) {
                nodeLength = power(2,depth-layer-1)*(NODE_INFO_SIZE+INTER_SPACE_SIZE);
                paddingSpaceSize = ((power(2, depth-layer)*NODE_INFO_SIZE+(power(2, depth-layer)-1)*INTER_SPACE_SIZE)-nodeLength)/2;
                interSpaceSize = nodeLength;
            } else {
                nodeLength = NODE_INFO_SIZE;
                paddingSpaceSize = paddingOffset;
                interSpaceSize = INTER_SPACE_SIZE;
            }   
            if (needPadding == true) {
                printDuplicate(paddingSpaceSize, ' ');
                needPadding = false;
            } else {
                printDuplicate(interSpaceSize, ' ');
            }
            while (!nodeExists[latestNodePos]) {
                printDuplicate(nodeLength+interSpaceSize, ' ');
                latestNodePos += 1;
            }
            printNameAndRatingInNode(getRootData(node), nodeLength, false);
            nodesInCurrentLevel -= 1;
            nodesFound += 1;
            latestNodePos += 1;
            leftson = getLeftSubtree(node);
            rightson = getRightSubtree(node);
            if (leftson != NULL) {
                add(&queue, (void *)leftson, NULL);
                nodesInNextLevel += 1;
            }           
            if (rightson != NULL) {
                add(&queue, (void *)rightson, NULL);
                nodesInNextLevel += 1;
            }
            if (nodesInCurrentLevel == 0) {
                printf("\n");
                printDuplicate(paddingSpaceSize, ' ');
                for (count = startNodePos; count <= endNodePos; count++) {
                    if (nodeExists[count]) {
                        printf("%c",'|');
                        printDuplicate(nodeLength-2, ' ');
                        printf("%c",'|');
                        printDuplicate(interSpaceSize, ' ');
                    } else {
                        printDuplicate(nodeLength+interSpaceSize, ' ');
                    }
                }
                printf("\n");
                needPadding = true;
                layer += 1;
                nodesInCurrentLevel = nodesInNextLevel ;
                nodesInNextLevel  = 0;
                nodesFound = 0;
                maxNumOfNodes = power(2,layer-1); 
                startNodePos = maxNumOfNodes - 1;
                endNodePos = startNodePos + maxNumOfNodes -1;
                latestNodePos = startNodePos;
            }
        }
    }
}

int compareName(void * data1, void *data2) 
{
    int result = 0;
    
    if (strcmp(((restaurant *)data1)->name, ((restaurant *)data2)->name) > 0) {
        result = 1;
    } else if (strcmp(((restaurant *)data1)->name, ((restaurant *)data2)->name) < 0) {
        result = -1;
    } else {
        result = 0;
    }
    return result;
}

int compareForLowerRatingFirst(void * data1, void *data2) 
{
    int result = 0;
    
    if (((restaurant *)data1)->rate == ((restaurant *)data2)->rate) {
        result = compareName(data1, data2);
    } else if (((restaurant *)data1)->rate > ((restaurant *)data2)->rate) {
        result = 1;
    } else {
        result = -1;
    }
    return result;
}

int compareForHigherRatingFirst(void * data1, void *data2) 
{
    int result = 0;
    
    if (((restaurant *)data1)->rate == ((restaurant *)data2)->rate) {
        result = compareName(data1, data2);
    } else if (((restaurant *)data1)->rate < ((restaurant *)data2)->rate) {
        result = 1;
    } else {
        result = -1;
    }
    return result;
}

void destroyRatingInfo(TreeDataTypePtr data)
{
    memset(((restaurant *)data)->name, 0, 50);
    memset(((restaurant *)data)->type, 0, 25);
    ((restaurant *)data)->rate = 0.0;
    free(data);
    data = NULL;
}