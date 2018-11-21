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
#define LABEL_IP_ID 0
#define LABEL_STATUS_ID 1
#define LABEL_POSITION_ID 2
#define LABEL_HEADING_ID 3
#define LABEL_RTK_ID 4
#define LABEL_SATALLITE_ID 5

#define IP_ID 0
#define POSITION_ID 1
#define STATUS_ID 2
#define RTK_ID 3
#define HEADING_ID 4
#define SATALLITE_ID 7

struct data{
  char ip[STRUCTSIZE];
  char status[STRUCTSIZE];
  char position[STRUCTSIZE];
  char heading[STRUCTSIZE];
  char rtk[STRUCTSIZE];
  char satallite[STRUCTSIZE];
};


int removeIdFromString(char *str);
int addStruct(struct data *newData, int id, char *dataStr);
void initStruct(struct data *newData);
void clearStruct(struct data *newData);
int isStructFull(struct data *newData);
void printStruct(struct data *newData);
void structManager(struct data *newData, int id, char* data);


#endif
