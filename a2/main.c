/* 
 * File:   main.c
 * Author: Joseph Chan
 *
 * Created on September 14, 2016, 6:48 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "listio.h"
/*
 * 
 */

void readOneString(FILE *source, char *string)
{
    int pos = 0;
    char currentChar, previousChar;
    bool quit = false;
    
    while ((!quit) && (currentChar = fgetc(source))) {
        if (currentChar == EOF) {
            quit = true;
            ungetc(currentChar, source);
            *(string+pos) = '\0';
        } else if ((currentChar == '\n') || (currentChar == '\r')) {
            previousChar = currentChar;
            *(string+pos) = currentChar;
            pos++;
        } else {
            if ((pos>0) && ((previousChar == '\n') || (previousChar == '\r'))) {
                quit = true;
                ungetc(currentChar, source);
                *(string+pos) = '\0';
            } else {
                previousChar = currentChar;
                *(string+pos) = currentChar;
                pos++;
            }
        }
    }
}

void concatenateText(struct dataString *text, char * buffer)
{
    if (text != NULL) {
        strcat(buffer, text->string);
        concatenateText(text->next, buffer);
    }   
}

void handOver(struct dataHeader *book, FILE *portal)
{
    /*
     * open a fifo queue, write filename, header, body text to
     * send all information through the fifo queue to the python
     * program before closing the queue
     */
    char * buffer = malloc((book->length+1)*sizeof(char));
    portal = fopen("./q2", "w");
    fprintf(portal, "%s\n", book->name);
    concatenateText(book->next, buffer);
    fprintf(portal, "%s\n", buffer);
    fclose(portal);
    printf("Processed text sent over to Python program to be formatted as HTML\n");
}

int main(int argc, char** argv) {
    int i = 0;
    int fileLength = 0;
    FILE * instream, *outstream, *portal;
    struct returnStruct returnValue;
    struct dataHeader *book = NULL;
    char oneline[4800];
    char htmlFileName[80];
    char inchar;

    sprintf(htmlFileName, "%s%s", *(argv+1), ".html");
    
    if ((instream = fopen(htmlFileName, "r")) != NULL) {
        while (!feof(instream)) {
            inchar = fgetc(instream);
            if (inchar != EOF) {
                printf("%c", inchar);
            } else {
				printf("\n");
		    }
        }
        fclose(instream);
    } else if ((instream = fopen(*(argv+1), "r")) != NULL) {
        returnValue = buildHeader();
        if (returnValue.value == SUCCESS) {
            book = returnValue.header;
            setName(book, *(argv+1));
            i = 0;
            while (!feof(instream)) {
                readOneString(instream, oneline);
                i++;
                addString(book, oneline);
            }
            processStrings(book);
            handOver(book, portal);
        }
        fclose(instream);

        if ((outstream = fopen(htmlFileName, "w")) != NULL) {
            mkfifo("./q1", 0777);
            instream = fopen("./q1", "r");
            fscanf(instream, "%d", &fileLength);
            for (i=0; i<fileLength; i++) {
               fscanf(instream, "%c", &inchar);
               fprintf(outstream, "%c", inchar);
            }
            fclose(instream);
            remove("./q1");
            fclose(outstream);
            printf("Formatted text received from Python program to create %s\n", htmlFileName);
        }
    }     
    return (EXIT_SUCCESS);
}

