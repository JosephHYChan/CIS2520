#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "listio.h"

/*
 * Local constant declarations section
 */
#define MAX_TAG_LEN 5
const char para_tag[MAX_TAG_LEN] = "<P>";
const char br_tag[MAX_TAG_LEN] = "<BR>";

/*
 * Private function declarations section
 */

void removesMultpleCharacters(char ch, char * str, int * length)
{
    char * buffer;
    if (strlen(str)>1) {
        if ((*str == ch) && (*(str+1) == ch)) {
            removesMultpleCharacters(ch, str+2, length);
            buffer = malloc((strlen(str+2)+1)*sizeof(char));
            memcpy(buffer, str+2, strlen(str+2)+1);
            memcpy(str+1, buffer, strlen(buffer)+1);
            free(buffer);
            removesMultpleCharacters(ch, str, length);
        } else {
            removesMultpleCharacters(ch, str+1, length);
            *length += 1;
        }    
    } else {
        *length = strlen(str);
    }   
}

void removeLeadingLineBreaks(char * str, int * count)
{
    //char * buffer;
    //int i;
    if (strlen(str)>0) {
        if ((*str == '\n') || (*str == '\r')) {
            *count += 1;
            memcpy(str, str+1, strlen(str));
            removeLeadingLineBreaks(str, count);
        }
    }
}   

int sizeDifference(char *str)
{
    int count = 0;
    int offset = 0;
    int lengthChange = 0;
    //int remainingChange = 0;
    char * buffer = malloc((strlen(str)+1)*sizeof(char));
    
    if (strlen(str)>0) {
        if ((*str == '\n') || (*str == '\r')) {
            /*
             * If one or multiple newlines or carriage returns are found, the string length
             * varies according to the replacement of proper html tag.
             */
            count = 0;
            memcpy(buffer, str, strlen(str)+1);
            removeLeadingLineBreaks(buffer, &count);
            if (count==1) {
                offset = strlen(br_tag);
            } else {
                offset = strlen(para_tag);
            }
            free(buffer);
        }
        lengthChange = (offset-count) + sizeDifference(str+count+1);
    }
    return lengthChange;
}

void convertToHTMLTags(char *str)
{
    int count;
    int offset = 1;
    bool needInsertTag = false;
    char * buffer;
    char htmltags[10] = "";
    
    if (strlen(str)>0) {
        if ((*str == '\n') || (*str == '\r')) {
            /*
             * If one or multiple newlines or carriage returns are found, replaces them
             * with the proper html tag.
             */
            count = 0;
            /*
             * Removes all leading newlines or carriage return and return a string
             * with its first character being neither newline nor carriage return.
             */
            removeLeadingLineBreaks(str, &count);
            if ((count)==1) {
                offset = strlen(br_tag);
                sprintf(htmltags, "%s", br_tag);
            } else {
                offset = strlen(para_tag);
                sprintf(htmltags, "%s", para_tag);
            }
            needInsertTag = true;
        }
        /*
         * Processes the rest of string to replace additional newlines or carriage returns.
         */
        convertToHTMLTags(str+1);
        if (needInsertTag == true) {
            buffer = malloc((strlen(str)+1)*sizeof(char));
            memcpy(buffer, str, (strlen(str)+1));
            memcpy(str+offset, buffer, strlen(buffer)+1);
            memcpy(str, htmltags, offset);
            free(buffer);
        }
    }    
}

bool isExpectedTag(char * str, const char * tag) {
    return (strcmp(str, tag) == 0);
}

int removeMultipleTags(struct dataString * text, const char * tag, int * length)
{
    char subString[strlen(tag)+1];
    //char * buffer;
    int status = SUCCESS;
    
    if (text != NULL) {
        if (text->next != NULL) {
            if ((strlen(text->string) >= strlen(tag)) && (strlen(text->next->string) >= strlen(tag))) {
                memcpy(subString, text->string + strlen(text->string) - strlen(tag), strlen(tag)+1);
                if (isExpectedTag(subString, tag)) {
                    memset(subString, 0, strlen(tag)+1);
                    memcpy(subString, text->next->string, strlen(tag));
                    if (isExpectedTag(subString, tag)) {
                        *(text->string + strlen(text->string) - strlen(tag)) = '\0';
                        text->string = realloc(text->string, (strlen(text->string)+1)*sizeof(char));
                        if (text->string == NULL) {
                            status = FAILURE;
                        }
                        *length -= strlen(tag);
                    }
                } 
            }
            return removeMultipleTags(text->next, tag, length);
        }
    }
    return printf("text is null\n");
}

int insert(struct dataString **text, char *str)
{
    if (*text == NULL) {
        /*
         * If end of the linked list is reached, reserve memory space to
         * create a new linked list member and store the pass in string.
         */
        *text = (struct dataString *)malloc(sizeof(struct dataString));
        if ((*text != NULL) && (str != NULL)) {
            (*text)->string = (char *)malloc((strlen(str)+1)*sizeof(char));
            if ((*text)->string != NULL) {
                memcpy((*text)->string, str, strlen(str)+1);
                (*text)->next = NULL;
                return SUCCESS;
            } else {
                return FAILURE;
            }
        } else {
            return FAILURE;
        }
    } else {
        /*
         * Otherwise, traverse to the next linked list member until the
         * end of the linked list is reached.
         */
        return insert(&(*text)->next, str);
    }
}

int print(struct dataString *text)
{
    if (text != NULL) {
        /*
         * If end of the linked list has not yet been reached, print the stored
         * string of the current linked list member before traversing to the 
         * next linked list member to do the same.
         */
        if (text->string != NULL) {
            printf("%s", text->string);
            return print(text->next);
        } else {
            return FAILURE;
        }   
    } else {
        /*
         * If end of the linked list is reached, print a newline to signify
         * printing of all strings has been done.
         */
         printf("\n");
         return SUCCESS;
    }
}

int processString(struct dataString *text, int *newlength)
{
    int i = 0;
    int length = 0;
    int adjustLength = 0;
    int originalLength = 0;
    //char htmltags[10] = "";
    char spCh[2] = {' ', '\t'};
    //char * string = NULL;
    int status = FAILURE;
    
    if (text != NULL) {
        for (i=0; i<2; i++) {
            /* 
             * Reduces multiple space or tab in each string to single one and
             * free the extra memory spaces
             */
            removesMultpleCharacters(spCh[i], text->string, &length);
            text->string = realloc(text->string, (1+length)*sizeof(char));
            if (text->string != NULL) {
                status = SUCCESS;
            }
        }
        adjustLength = sizeDifference(text->string);
        originalLength = strlen(text->string);

        if (adjustLength > 0) {
            /*
             * If string length needs increasing, reallocate the proper amount of 
             * reserve memory spaces before processing the string to replace newlines 
             * or carriage returns by html tags
             */
            text->string = realloc(text->string, (originalLength+adjustLength+1)*sizeof(char));
            if (text->string != NULL) {
                convertToHTMLTags(text->string);
                status = status && SUCCESS;
            }
        } else if (adjustLength < 0) {
            /*
             * If string length needs decreasing, reallocate the proper amount of 
             * reserve memory spaces after processing the string to replace newlines 
             * or carriage returns by html tags
             */
            convertToHTMLTags(text->string);
            text->string = realloc(text->string, (originalLength+adjustLength+1)*sizeof(char));
            if (text->string != NULL) {
                status = status && SUCCESS;
            }
        } else {
            convertToHTMLTags(text->string);
        }           
        status = status && processString(text->next, newlength);
        *newlength += strlen(text->string)+1;
    }
    return status;
}

int freeBody(struct dataString *text)
{
    if (text != NULL) {
        /*
         * If end of the linked list has not yet been reached, traversing to
         * free the next linked list member. Then free the memory allocated
         * for storing the string in the current linked list member.
         */
        free(text->string);
        freeBody(text->next);
        text->string = NULL;
        text->next = NULL;
    }
    return SUCCESS;
}

int writeBody(FILE * stream, struct dataString *text)
{
    int length = 0;
    int status = FAILURE;
    
    if (stream != NULL) {
        if (text != NULL) {
            /*
             * If end of the linked list has not yet been reached, calculate and write
             * the length of the string stored in current linked list member to file 
             * followed by writing the stored string to the file. Then traversing to
             * the next linked list member to do the same.
             */
            length = strlen(text->string)+1;
            if (fwrite(&length, sizeof(int), 1, stream) == 1) {
                status = SUCCESS;
            }
            if (fwrite(text->string, sizeof(char), length, stream) == length) {
                status = status && SUCCESS;         
            }
            status = status && writeBody(stream, text->next);
        }
        status = SUCCESS;
    }
    return status;
}

int writeStructure(FILE * stream, struct dataHeader *header)
{
    int length = 0;
    int status = FAILURE;
    
    if (stream != NULL) {
        if (header != NULL) {
            /*
             * If header has been created, write the number that records the
             * total length of all strings stored in the associated linked list
             * to file. Then write the name of the header to the file before
             * writing the string length and stored string of each member in 
             * the associated linked list.
             */
            length = strlen(header->name)+1;
            if (fwrite(&length, sizeof(int), 1, stream) == 1) {
                status = SUCCESS;
            }
            if (fwrite(header->name, sizeof(char), length, stream) == length) {
                status = status && SUCCESS;         
            }
            if (fwrite(&(header->length), sizeof(int), 1, stream) == 1) {
                status = status && SUCCESS;         
            }
            status = status && writeBody(stream, header->next);
        }
    }
    return status;
}

int readBody(FILE * stream, struct dataHeader *header, int remains)
{
    int length = 0;
    char *string = NULL;
    int status = FAILURE;
    
    if (remains > 0) {
        /*
         * If there is still string remaining in file, read the next string length and
         * string from the file and add the string to the associated linked list. The
         * length of remaining unread strings is reduced by read in string length before
         * repeating the same action.
         */
         
        if (fread(&length, sizeof(int), 1, stream) == 1) {
            status = status && SUCCESS;
        }
        string = (char *)malloc(length*sizeof(char));
        if (string != NULL) {
            if (fread(string, sizeof(char), length, stream) == length) {
                status = status && SUCCESS;
            }
            status = status && addString(header, string);
            free(string);
        } else {
            status = FAILURE;
        }
        status = status && readBody(stream, header, remains-length);
        return status;
    } else {
        return SUCCESS;
    }
}

int readStructure(FILE * stream, struct dataHeader *header)
{
    int length = 0;
    char *name = NULL;
    int status = FAILURE;
    
    if (stream != NULL) {
        if (header != NULL) {
            /*
             * If header has been created, read the number that represent the
             * total length of all strings to be stored in the associated linked
             * list from file and store it. Then read the name of the header from
             * file and store it before reading all string lengths and strings 
             * from file and store them in the associated linked list.
             */
            if (fread(&length, sizeof(int), 1, stream) == 1) {
                status = SUCCESS;
            }
            name = (char *)malloc(length*sizeof(char));
            if (name != NULL) {
                if (fread(name, sizeof(char), length, stream) == length) {
                    status = status && SUCCESS;
                }
                status = status && setName(header, name);
                free(name);
            } else {
                status = FAILURE;
            }
            if (fread(&length, sizeof(int), 1, stream) == 1) {
                status = status && SUCCESS;
            }
            status = status && readBody(stream, header, length);
        }
    }
    return status;
}

/*
 * Public function declarations section
 */
struct returnStruct buildHeader()
{
    struct dataHeader *newheader = malloc(sizeof(struct dataHeader));
    struct returnStruct retVal;
    
    if (newheader == NULL) {
        retVal.value = FAILURE;
        retVal.header = NULL;
    } else {
        newheader->name = NULL;
        newheader->next = NULL;
        newheader->length = 0;
        retVal.value = SUCCESS;
        retVal.header = newheader;
    }
    return retVal;
}

int setName(struct dataHeader *header, char *name)
{
    if ((header != NULL) && (name != NULL)) {
        header->name = (char *)malloc((strlen(name)+1)*sizeof(char));
        if (header->name != NULL) {
            memcpy((void *)header->name, (void *)name, (strlen(name)+1));
            return SUCCESS;
        } else {
            return FAILURE;
        }
    } else {
        return FAILURE;
    }
}

char * getName(struct dataHeader *header)
{
    if (header != NULL) {
        return header->name;
    } else {
        return NULL;
    }
}

int getLength(struct dataHeader *header)
{
    if (header == NULL) {
        return 0;
    } else {
        return header->length;
    }
}

int addString(struct dataHeader *header, char *str)
{
    int status = FAILURE;
    
    if ((header != NULL) && (str != NULL)) {
        status = insert(&(header->next), str);
        if (status == SUCCESS) {
            header->length += strlen(str)+1;
        }
        return status;
    } else {
        return status;
    }
}

int printString(struct dataHeader *header){
    if (header != NULL) {
        return print(header->next);
    } else {
        return FAILURE;
    }
}

int processStrings(struct dataHeader *header)
{
    int status = FAILURE;
    
    if (header != NULL) {
        header->length = 0;
        status = processString(header->next, &(header->length));
        status = status && removeMultipleTags(header->next, para_tag, &(header->length));
        status = status && removeMultipleTags(header->next, br_tag, &(header->length));
    }
    return status;
}

int freeStructure(struct dataHeader *header)
{
    int status;
    
    status = freeBody(header->next);
    header->next = NULL;
    free(header->name);
    header->name = NULL;
    free(header);
    return status;
}

int writeStrings(char *filename, struct dataHeader * header)
{
    FILE * outstream;
    int status = FAILURE;

    if ((outstream = fopen(filename, "w")) != NULL) {
        status = writeStructure(outstream, header);
        fclose(outstream);
    }
    return status;
}

struct returnStruct readStrings(char *filename)
{
    FILE * instream;
    struct returnStruct retVal;

    if ((instream = fopen(filename, "r")) != NULL) {
        retVal = buildHeader();
        if (retVal.value == SUCCESS) {
            retVal.value = readStructure(instream, retVal.header);
        }   
        fclose(instream);
        if (retVal.value == FAILURE) {
            retVal.header = NULL;
        }
    } else {
        retVal.value = FAILURE;
        retVal.header = NULL;
    }
    return retVal;
}
