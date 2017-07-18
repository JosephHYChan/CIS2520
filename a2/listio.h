#ifndef H_STORAGE_H
#define H_STORAGE_H

#define FAILURE 0
#define SUCCESS 1

struct dataString {
    char *string;
    struct dataString *next;
};

struct dataHeader {
    char *name;
    int length;
    struct dataString *next;
};

struct returnStruct {
   int value;
   struct dataHeader *header;
};

struct returnStruct buildHeader();
int setName(struct dataHeader *header, char *name);
char * getName(struct dataHeader *header);
int getLength(struct dataHeader *header);
int addString(struct dataHeader *header, char *str);
int printString(struct dataHeader *header);
int processStrings(struct dataHeader *header);
int freeStructure(struct dataHeader *header);
int writeStrings(char *filename, struct dataHeader * header);
struct returnStruct readStrings(char *filename);

#endif
