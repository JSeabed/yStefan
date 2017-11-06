#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/* The shell can be used to test 
 * certain parts of the program */
void shell(){
  int done = 0;
  int input;
  do{
    printf("> ");
    scanf("%d", &input);
    switch(input){
    case 0:
      #if GENIE
	printf("test string\n");
	genieWriteStr(1,"Test string");
      #endif
      break;
    case 1:
      done = 1;
      break;
    default:
      printf("input is not valid. \n");
      break;
    }
  } while(!done);
  exit(1);
}
