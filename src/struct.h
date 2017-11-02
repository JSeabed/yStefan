#ifndef _STRUCT_HEADER
#define _STRUCT_HEADER

#define STRUCTSIZE 150

struct data{
  char *ip;
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
