#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "restaurant.h"

char * ltrim(char * word)
{
    if (word[0] == '\0') {
        return word;
    } else if (word[0] == ' ') {
        return ltrim(word+1);
    } else {
        return word;
    }
}

char * rtrim(char * word) 
{
    int last = strlen(word)-1;
    
    if (word[0] == '\0') {
        return word;
    } else if (word[last] == ' ') {
        word[last] = word[last+1];
        return rtrim(word);
    } else {
        return word;
    }
}

char * trim(char * word)
{
    return rtrim(ltrim(word));
}

bool decode(char * aLine, char * delimeter, restaurant * data)
{
    bool valid = true;
    char * token[3];
    int pos = 0;
    
    for (pos = 0; pos < 3; pos++) 
    {
        if (pos == 0) {
            token[pos] = strtok(aLine, delimeter);
        } else {
            token[pos] = strtok(NULL, delimeter);
        }
        valid = valid && (token[pos] != NULL);
    }
    if (valid) {
        strcpy(data->name, trim(token[0]));
        strcpy(data->type, trim(token[1]));
        data->rate = atof(trim(token[2]));
    }
    return valid;
}

int main(int argc, char ** argv)
{
    FILE * instream = NULL;
    Tree * sortByName = NULL;
    Tree * sortLowerRating = NULL;
    Tree * sortByHigherRating = NULL;
    char aLine[120];
    restaurant * thisOne = NULL;
    
    if (argc == 2) {
        instream = fopen((char *)*(argv+1), "r");
        if (instream != NULL) {
            sortByName = createBinTree(compareName, destroyRatingInfo);
            sortLowerRating = createBinTree(compareForLowerRatingFirst, destroyRatingInfo);
            sortByHigherRating = createBinTree(compareForHigherRatingFirst, destroyRatingInfo);

            while (!feof(instream)) {
                thisOne = malloc(sizeof(restaurant));
                fgets(aLine, 120, instream);
                if (decode(aLine, ",", thisOne)) {
                    addToTree(sortByName, (void *)thisOne);
                    addToTree(sortLowerRating, (void *)thisOne);
                    addToTree(sortByHigherRating, (void *)thisOne);
                }
            }
            fclose(instream);
            printf("List of restaurants sorted by name:\n");
            printInOrder(sortByName, printAllInfoInNode);
            printf("\n\n");
            printf("List of restaurants sorted by rating in descending order:\n");
            printInOrder(sortByHigherRating, printAllInfoInNode);
            printf("\n\n");
            printf("Tree printed in depth first order\n\n");
            printDepthFirst(sortByName, 1, 0, 0, 0);
            printf("\n\n");
            printf("Tree printed in breadth first order\n\n");
            printBreadthFirst(sortByName);
            return EXIT_SUCCESS;
        } else {
            printf("Failed to open %s\n", (char *)*(argv+1));
        return EXIT_FAILURE;
        }
        
    } else {
        printf("Usage: %s %s\n", (char *)(*argv), "<restaurant profile>");
        return EXIT_FAILURE;
    }
}