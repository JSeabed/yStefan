#include "../include/struct.h"



char* removeGarbage(char *str){
	int i = 0;
	char* pchr;
	char* tmpStr;

	pchr = strchr(str, '[');
	if(pchr != NULL){
		for(i < (int)(pchr - str + 1); i++;){
			tmpStr[i] += str[i];
		}
		printf("tmpStr = %s", tmpStr);
	return tmpStr;
	} // found [ char. remove everything behind it.

	// No garbage found. Remove original string
	return str;
}


/*
Return TRUE (1) if data was added to struct. 
Otherwise return FALSE (0) 
*/
int addStruct(struct data *newData, int id, char *dataStr){
  // first remove id from string
  dataStr += 3;
  dataStr = removeGarbage(dataStr);
  #if DEBUG
  printf("Add to struct: %s\n", dataStr);
  #endif
  switch(id){
  case IP_ID:
    strcpy(newData->ip, dataStr);
    return TRUE;
  case STATUS_ID:
    strcpy(newData->status, dataStr);
    return TRUE;
  case POSITION_ID:
    strcpy(newData->position, dataStr);
    return TRUE;
  case HEADING_ID:
    strcpy(newData->heading, dataStr);
    return TRUE;
  case RTK_ID:
    strcpy(newData->rtk, dataStr);
    return TRUE;
  case SATALLITE_ID:
    strcpy(newData->satallite, dataStr);
    return TRUE;
  default:
    printf("Error: addToStruct");
    return FALSE;
  }
}

// allocate memory for struct
void initStruct(struct data *newData){
  /*newData->ip = (char*)malloc(sizeof(char)*STRUCTSIZE);
  newData->status = (char*)malloc(sizeof(char)*STRUCTSIZE);
  newData->position = (char*)malloc(sizeof(char)*STRUCTSIZE);
  newData->heading = (char*)malloc(sizeof(char)*STRUCTSIZE);
  newData->rtk = (char*)malloc(sizeof(char)*STRUCTSIZE);
  newData->satallite = (char*)malloc(sizeof(char)*STRUCTSIZE);*/
}

void clearStruct(struct data *newData){
  #if DEBUG
    printf("ClearStruct \n");
  #endif
  char str[10] = "0";
  strcpy(newData->ip, str);
  strcpy(newData->status, str);
  strcpy(newData->position, str);
  strcpy(newData->heading, str);
  strcpy(newData->rtk, str);
  strcpy(newData->satallite, str);
  // strncpy(newData->&ip , NULL, 1);
  //strncpy(newData->&status , NULL , 1);
  //strncpy(newData->position , (char*)'0', 1);
  //strncpy(newData->heading , (char*)'0', 1);
  //strncpy(newData->rtk , (char*)'0', 1);
  //strncpy(newData->satallite , (char*)'0', 1);
}


int isStructFull(struct data *newData){
#if DEBUG
  printf("isStructFull: %s", newData->ip);
  if(newData->ip == 0)
    printf("passed second test. IP is 0\n");

#endif
}


void printStruct(struct data *newData){
  #if DEBUG
  printf("STRUCTURE: \n");
  printf("ip: %s\n", newData->ip);
  printf("status: %s\n", newData->status);
  printf("position: %s\n", newData->position);
  printf("heading: %s\n", newData->heading);
  printf("rtk: %s\n", newData->rtk);
  printf("satallite: %s\n", newData->satallite);
  #endif
}


void structManager(struct data *newData, int id, char* data){
  // compare
  #if DEBUG
  printf("structManager\n");
  #endif
  addStruct(newData, id, data);
  printStruct(newData);
  //dataReady(newData);
}

