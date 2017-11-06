#include "../include/struct.h"
#include "../include/shell.h"
#include <fcntl.h>
#include <sys/stat.h>
//#include <stdlib.h>
#include <sys/types.h> // pid
#include <sys/time.h> // pid
//#include <sys/poll.h> // pid
#include <sys/wait.h> //  pid
#include <unistd.h> // for usleep and used for pid_t
#if GENIE
    #include <geniePi.h>
#else
    #include <diabloSerial.h>
    #include <Diablo_Serial_4DLibrary.h>
    #include <Diablo_Types4D.h>
    #include <Diablo_const4D.h>
#endif

#if GENIE
    #define GENIE_OBJ_FORM 10
    #define GENIE_OBJ_USERBUTTON 33
    #define GENIE_OBJ_4DBUTTON 30
#else
// diablo code
#endif

#define PORT "/dev/ttyAMA0"
//#define BAUDRATE 9600
#define BAUDRATE 115200

#define FROM(x) (0x010a + x + 0000) // TODO needs to be checked

//#define BUFFSIZE 4096
#define BUFFSIZE 2048
#define TIMEOUT 500
#define WAIT 250

/*#define checksum(x) (x ^= x)*/

#define ZERO "0"


#define MAIN_SCREEN 0
#define INFO_SCREEN 1

#define toggle(x) (x = !x)
#define isIdentical(a, b) (a == b)

int FORM = 0;

//struct data oldData;
//oldData = (char*)malloc(sizeof(char)*STRUCTSIZE*6);
//struct data *oldData = malloc(STRUCTSIZE*6);
struct data oldData;
//initStruct(struct data *oldData);
//struct data Newdata; //TODO replace

void sentData(char* data, int id);
void dataReady(struct data *newData);
void structManager(struct data *newData, int id, char* data);

/* remove eventually
   if(reply->object == GENIE_OBJ_4DBUTTON) {
   */

#if GENIE
void handleEvent (struct genieReplyStruct *reply) {
  printf("Ik kom hier\n");
	if(reply->object == GENIE_OBJ_4DBUTTON) {
		switch (reply->index) {
			case 0:
				/* Main screen. Show no data. Save data.*/
				genieWriteStr(1,"You pressed the RED button.");
				//genieWriteObj(GENIE_OBJ_FORM, 1, 1);
				#if DEBUG
					printf("RED");
					printf("%d\n");
				#endif
				break;
			case 1:
				/* Screen with data. Obtain old data? */
				genieWriteStr(2,"You pressed the GREEN button.");
				//genieWriteObj(GENIE_OBJ_FORM,0, 1);
				#if DEBUG
					printf("Green");
					printf("%d\n");
				#endif
				// sentData();
				break;
			default:
				printf("Error, index not in range or found.");
				exit(0);
				break;
		}
	}
}
#endif



//TODO change namae
void dataReady(struct data *newData){
  sleep(1);
  if(strncmp(newData->ip, ZERO, 1) !=0)
    if(strcmp(newData->ip, oldData.ip) != 0)
	sentData(newData->ip, IP_ID);
  if(strncmp(newData->status, ZERO, 1) != 0)
    if(strcmp(newData->status, oldData.status) != 0)
  	sentData(newData->status, STATUS_ID);
  if(strncmp(newData->position, ZERO, 1) != 0)
    if(strcmp(newData->position, oldData.position) != 0)
  	sentData(newData->position, POSITION_ID);
  if(strncmp(newData->heading, ZERO, 1) != 0)
    if(strcmp(newData->heading, oldData.heading) != 0)
  	sentData(newData->heading, HEADING_ID);
  if(strncmp(newData->rtk, ZERO, 1) != 0)
    if(strcmp(newData->rtk, oldData.rtk) != 0)
  	sentData(newData->rtk, RTK_ID);
  if(strncmp(newData->satallite, ZERO, 1) != 0)
    if(strcmp(newData->satallite, oldData.satallite) != 0)
  	sentData(newData->satallite, SATALLITE_ID);
    //if(isIdentical)
  printf("Komt hier de segmentation fault?\n");
  oldData = *newData;
}


void clearScreen(){
  #if GENIE
  int i = 0;
  i = genieWriteStr(IP_ID, ";;;");
  //printf("i = %d \n", i);
  #else
  // diablo code 
  #endif
  //genieWriteStr(STATUS_ID, "...");
  //genieWriteStr(POSITION_ID, "...");
  //genieWriteStr(HEADING_ID, "...");
  //genieWriteStr(RTK_ID, "...");
  //genieWriteStr(SATALLITE_ID, "...");
}


int changeForm(){
  toggle(FORM);
  #if GENIE
    genieWriteObj(GENIE_OBJ_FORM,FORM, 1);
  #else
    //diablo code
  #endif
  
    clearStruct(&oldData);
  return 1;
}


void sentData(char* data, int id){
#if GENIE
    genieWriteStr(id, data);
#else
    //diablo code
#endif
  usleep(500);
  //genieWriteStr(STATUS_ID, newData->status);
  //perror("sentData");
  //genieWriteStr(POSITION_ID, newData->position);
  //perror("sentData");

  //genieWriteStr(HEADING_ID, newData->heading);
  //perror("sentData");
  //genieWriteStr(RTK_ID, newData->rtk);
  //perror("sentData");
  //genieWriteStr(SATALLITE_ID, newData->satallite);
  //perror("sentData");
}

/*
int checkFifo(FILE *file){
	int retval;
	struct pollfd fd1;
	fd1.fd = file;
	retval = poll(fd1, 1, TIMEOUT);
	if(retval < 0){
	} else if(retval > 0){
		getData(file);
	} else {
	}
	return true;
}
*/

void childGetData(int fd_child, int fd_parent ){
	/* Get data from python script */
	char readBuffer[BUFFSIZE];
	//printf("Child here\n");
	//read(fd_child, &readBuffer, BUFFSIZE);
	//printf("%s", readBuffer);

	int n;
	//int fd;
	char buf[BUFFSIZE];
	FILE *file;
	int ret;
	char * myfifo = "/tmp/mypipe";
	/* create the FIFO (named pipe) */
	mkfifo(myfifo, 0666);
	file = fopen(myfifo, "r");
	/* write "Hi" to the FIFO */
	//fd = open(myfifo, O_WRONLY);
	//write(fd, "Hi", sizeof("Hi"));

	for(;;){
			if(fgets(buf, BUFFSIZE, file) > 0){
			//  printf("%s \n", buf);
			n = write(fd_parent, &buf, sizeof(buf));
			if(n > 0){
			  printf("verstuurd!: %s \n", buf);
			}
			if(n < 0){
			  perror("Error: ");
			}
		}
	  	//file = open(myfifo, O_WRONLY);
	  	//fclose(file);
		//fflush(file);
	  usleep(WAIT);
	}
	unlink(myfifo);
}


int getID(char *str){
  char *strMask = "%*[^0123456789]%d";
  int id;
  int i = 0;
  while(sizeof(str) > i){
    if(sscanf(str, strMask, &id) == 1){
    printf("id is = %d", id);
    return id;
    }
    i++;
  }
  //error
  printf("ID NOT FOUND\n");
  return -1;
}
	/* remove the FIFO */
	/* fill data struct*/
/************************************************************************
 * Fetch the received data from the python script to the data structure *
 * *********************************************************************/
/*int fetchData(struct data, char *buf){
	//data.ip =
	return 1;
}*/


void errorExit(char* error){
	printf("%s\n", error);
	//exit(0);
}


int main (int argc, char** argv) {
  #if DEBUG
    printf("Debug mode on\n");
  #endif
  #if GENIE
	struct genieReplyStruct reply;
  #else
    int rc;
  #endif

	#if GENIE
	if(genieSetup(PORT ,BAUDRATE)<0) {
		printf("ViSi-Genie Failed to init display!\r\n");
		return(1); // Failed to initialize ViSi-Genie Display. Check Connections!
	}
	#else
	// diablo init code
	rc = OpenComm(PORT, BAUDRATE);
	if(rc != 0){
	  printf("Failed to init display\n");
	  exit(EXIT_FAILURE);
	}
	#endif
    // shell: use for testing
    /*const char* c = (const char* )argv[1];
  if((strcmp(c, "-s")) == 0 ){
    shell();
    exit(0);
    }*/
	// fd_child = child read | fd_parent = parent_read
	int fd_child[2], fd_parent[2];
	int status, id, ret;

	struct data newData;
	initStruct(&newData);
	clearStruct(&newData);
	printf("clear struct old data\n");
	clearStruct(&oldData);
	//dataReady(&newData, &reply);


	char readBuffer[BUFFSIZE];
	char writeBuffer[BUFFSIZE];

	pipe(fd_child);
	pipe(fd_parent);

	pid_t child;
	child = fork();

	//clearScreen();

	  printf("test");
		if(child == (pid_t)-1){
			/* failed to create child*/

		}

		if(!child){
			/* Here enters the child */
			/* create pipe to python script */
			/* check if named pipe if filled*/
			//printf("Child pid = %d \n", (int)child);
			close(fd_child[1]);
			close(fd_parent[0]);
			childGetData(fd_child[0], fd_parent[1]);
		}

		close(fd_parent[1]);
		close(fd_child[0]);

		usleep(20);
		//write(fd_child[1], &test, sizeof(test));
		/*#if GENIE
		while(genieReplyAvail()) {
			genieGetReply(&reply);
			handleEvent(&reply);
			usleep(WAIT); // wait 20ms between polls to save CPU
		}
		#endif*/
	for(;;) {
		if(ret = checkFd(fd_parent[0])){
			#if DEBUG
			    printf("Data is available\n");
			#endif
			read(fd_parent[0], &readBuffer, BUFFSIZE);
			id = getID(readBuffer);
			#if DEBUG
			    printf("\n parent: %s", readBuffer);
			#endif
			structManager(&newData, id, readBuffer);
			dataReady(&newData);
			//void structManager(struct data *newData, int id, char* data, char dataReady){
			//genieWriteStr(1, readBuffer);
			//fflush(myfifo* fd_parent[0]);
			// fetchData();
		} else if(ret == -1){
			/* error */
		  perror("Error - parent: ");
		} else{
			#if DEBUG
				//printf("Timeout!\n");
			#endif
			usleep(WAIT);
		}
		//struct data Newdata; //TODO replace
		usleep(WAIT);
		if(genieReplyAvail()) {
			genieGetReply(&reply);
			handleEvent(&reply);
			usleep(WAIT); // wait 20ms between polls to save CPU
		}
		//if(isStructFull(&newData)) sentData(&newData);




}
	return(0);
}
/************************************
 * Check file descriptor with child.*
 ***********************************/
int checkFd(int fd_parent){
	//Init timeout
	struct timeval tv;
	// set timeout to x Sec
	tv.tv_usec = TIMEOUT;

	fd_set set;
	FD_ZERO(&set);
	FD_SET(fd_parent, &set);

	int retval = select(FD_SETSIZE, &set, NULL, NULL, &tv);
	if(retval == -1){
		printf("error: select()\n");
		return -1;
	}
	else if(retval){
		/* Data is available */
		return 1;
	} else {
		/* Timeout */
		return 0;
	}
}
