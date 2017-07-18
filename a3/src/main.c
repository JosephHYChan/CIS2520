#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "filemanager.h"

Tree tree;
char prefix[FULL_PATH_SIZE] = "";

/*
 * Removes leading and trailing '\'
 */
void normalize(char * prefix)
{   
    int index = 0;
    
    if ((prefix[0] == '\\') || (prefix[0] == '/')){
        while (prefix[index] != '\0') {
            prefix[index] = prefix[index+1];
            index++;
        }
    }
    if ((prefix[strlen(prefix)-1] == '\\') || (prefix[strlen(prefix)-1] == '/')) {
        prefix[strlen(prefix)-1] = '\0';
    }
}

/*
 * Separate path into first node and rest of path
 */
void decode(char * aLine, char * command, char parameters[][FULL_PATH_SIZE])
{
    char * token;
    int index = 0;
    
    token = strtok(aLine, " ");
    if (token != NULL) {
        strcpy(command, token);
        token = strtok(NULL, " ");
        while (token != NULL) {
            strcpy(parameters[index], token);
            token = strtok(NULL, " ");
            index++;
        }
    }
}

/*
 * Read a line until newline, eol & eof is encountered.
 * Returns true when eof is encountered; otherwise false.
 */
bool readLine(FILE * instream, char * aLine)
{
    char inChr;
    int pos = 0;
    
    while (((inChr = fgetc(instream)) != '\n') && (inChr != '\0') && !feof(instream)) {
        aLine[pos++] = inChr;
    }
    aLine[pos] = '\0';
    return feof(instream);
}

bool absolutePath(char * path)
{
    return (path[0] == '\\');
}

/*
 * Read a supported command and performs the command, then retry unless the
 * supported command is exit.
 * If command being read is not supported, an error message will be printed
 */
void execute(char * prefix)
{   char command[20];
    char aLine[FULL_PATH_SIZE];
    char source[FULL_PATH_SIZE] = "";
    char target[FULL_PATH_SIZE] = "";
    char parameters[2][FULL_PATH_SIZE] = {"", ""};
    
    printf("%s> ", prefix);
    readLine(stdin, aLine);
    decode(aLine, command, parameters);
    if (strcmp(command, "dir")==0) {
        if ((strlen(parameters[0]) == 0) && (strlen(parameters[1]) == 0)) {
            strcat(source, prefix);
            printChildren(&tree, tree.root, source);
        } else if (strlen(parameters[1]) == 0) {
            if ((strcmp("-r", parameters[0]) == 0) || (strcmp("-recursive", parameters[0]) == 0)) {
                printAllPaths(&tree, prefix);
            } else {
                if (!absolutePath(parameters[0])) {
                    strcat(source, prefix);
                }
                strcat(source, parameters[0]);
                printChildren(&tree, tree.root, source);
            }           
        } else {
            if ((strcmp("-r", parameters[0]) == 0) || (strcmp("-recursive", parameters[0]) == 0)) {
                if (!absolutePath(parameters[1])) {
                    strcat(source, prefix);
                }
                strcat(source, parameters[1]);
                printAllPaths(&tree, source);
            } else {
                if (!absolutePath(parameters[0])) {
                    strcat(source, prefix);
                }
                strcat(source, parameters[0]);
                printChildren(&tree, tree.root, source);
            }           
        }       
        execute(prefix);
    } else if (strcmp(command, "move")==0) {
        if ((strlen(parameters[0]) == 0) || (strlen(parameters[1]) == 0)) {
            printf("move needs [filename in original path] and [new path] as parameters\n");
        } else {
            if (!absolutePath(parameters[0])) {
                strcat(source, prefix);
            }
            strcat(source, parameters[0]);
            if (!absolutePath(parameters[1])) {
                strcat(target, prefix);
            }
            strcat(target, parameters[1]);
            moveNode(&tree, source, target);
        }
        execute(prefix);
    } else if (strcmp(command, "rename")==0) {
        if ((strlen(parameters[0]) == 0) || (strlen(parameters[1]) == 0)) {
            printf("rename needs [filename in path] and [new filename] as parameters\n");
        } else {
            if (!absolutePath(parameters[0])) {
                strcat(source, prefix);
            }
            strcat(source, parameters[0]);
            strcat(target, parameters[1]);
            renameNode(&tree, source, target);
        }
        execute(prefix);
    } else if (strcmp(command, "delete")==0) {
        if (strlen(parameters[0]) == 0) {
            printf("delete needs [filename in path] as parameters\n");
        } else {
            if (!absolutePath(parameters[0])) {
                strcat(target, prefix);
            }
            strcat(target, parameters[0]);
            deleteNode(&tree, target);
        }
        execute(prefix);
    } else if (strcmp(command, "search")==0) {
        if (strlen(parameters[0]) == 0) {
            printf("search needs [optional search path] and [filename] as parameters\n");
        } else if (strlen(parameters[1]) == 0) {
            strcat(target, parameters[0]);
            search(&tree, target, prefix);
        } else {
            if (!absolutePath(parameters[0])) {
                strcat(source, prefix);
            }
            strcat(source, parameters[0]);
            strcat(target, parameters[1]);
            search(&tree, target, source);
        }
        execute(prefix);
    } else if (strcmp(command, "exit")==0) {
        exit(0);
    } else {
        if (strlen(command) > 0) {
            printf("%s is an unsupported command\n", command);
        }
        execute(prefix);
    }
}
    

int main(int argc, char ** argv)
{
    FILE * instream = NULL;
    bool quit = false;
    int count = 0;
    char inFile[NAME_SIZE] = "";
    char aLine[FULL_PATH_SIZE] = "";
    char path[][FULL_PATH_SIZE] = {
                                   "\\mnt\\sdcard\\folder1\\a\\b\\file3",
                                   "\\mnt\\sdcard\\folder1\\a\\b\\file1",
                                   "\\mnt\\sdcard\\folder1\\a\\b\\file5",
                                   "\\mnt\\sdcard\\folder1\\a\\b\\file6",
                                   "\\mnt\\sdcard\\folder1\\a\\b\\file2",
                                   "\\mnt\\sdcard\\folder1\\e\\c\\file4",
                                   "\\mnt\\sdcard\\folder2\\d\\file6",
                                   "\\mnt\\sdcard\\folder2\\d\\file8",
                                   "\\mnt\\sdcard\\file9",
                                   "\\etc\\sdcard\\folder1\\a\\b\\file3",
                                   "\\etc\\sdcard\\folder1\\a\\b\\file1",
                                   "\\etc\\sdcard\\folder1\\a\\b\\file5",
                                   "\\etc\\sdcard\\folder1\\a\\b\\file6",
                                   "\\etc\\sdcard\\folder1\\a\\b\\file2",
                                   "\\etc\\sdcard\\folder1\\e\\c\\file4",
                                   "\\etc\\sdcard\\folder2\\d\\file6",
                                   "\\etc\\sdcard\\folder2\\d\\file8",
                                   "\\etc\\sdcard\\file9",
                                  };
    
    if (argc == 2) 
    {
       /*
        * If one argument is passed in during execution of the
        * application, it is interpreted as the root directory
        */
        strcat(aLine, *(argv+1));
        normalize(aLine);
        strcat(prefix, "\\");
        strcat(prefix, aLine);
        strcat(prefix, "\\");
    } else if (argc == 3) {
       /*
        * If two argument is passed in during execution of the
        * application, the first is interpreted as the root directory,
        * the second is interpreted as a data file containing the 
        * simulated file hierarchy.
        */
        strcat(aLine, *(argv+1));
        normalize(aLine);
        strcat(prefix, aLine);
        strcat(prefix, "\\");
        strcpy(inFile, *(argv+2));
        printf("inFile = %s\n", inFile);
    }   
    
    Initialize (&tree, copyValue, destroyValue, compareValues); 
    if (strlen(inFile) == 0) {
        for (count=0; count<(sizeof(path)/FULL_PATH_SIZE); count++) {
            store(&tree, path[count]);
        }
    } else {
        instream = fopen(inFile, "r");
        if (instream != NULL) {
            while (!quit) {
                quit = readLine(instream, aLine);
                store(&tree, aLine);
            }   
            fclose(instream);
        } else {
            for (count=0; count<9; count++) {
                store(&tree, path[count]);
            }
        }
    }
    execute(prefix);
    return 0;
}