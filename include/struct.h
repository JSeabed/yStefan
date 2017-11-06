#ifndef _STRUCT_HEADER
#define _STRUCT_HEADER

#define STRUCTSIZE 150

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
#define TRUE 1
#define FALSE 0

// TODO might want to place this somewhere else
#define IP_ID 0
#define STATUS_ID 2
#define POSITION_ID 1
#define HEADING_ID 6
#define RTK_ID 3
#define SATALLITE_ID 7

struct data{
  char ip[STRUCTSIZE];
  char *status;
  char *position;
  char *heading;
  char *rtk;
  char *satallite;
};


int addStruct(struct data *newData, int id, char *dataStr);
void initStruct(struct data *newData);
void clearStruct(struct data *newData);
int isStructFull(struct data *newData);
void printStruct(struct data *newData);
void structManager(struct data *newData, int id, char* data);


#endif
