/* 
 * Seabed 2017-2018 - Stefan van Delft and Jeroen Komen.
 * This file manages the communication between the diablo display 
 * and the python scripts/webserver. Two child processes are created to
 * manage the data input from the python scripts and the data input from the
 * diablo display. Data is stored in a structure and is then sent to the display 
 * and added to old structure. When new data arrives, it is compared to the old data.
 * Only data which is different from the old data is sent to the display to reduce
 * data transfer.
*/

#include "../include/struct.h"
#include "../include/shell.h"
#include <fcntl.h>
#include <wiringPi.h>
#include <sys/stat.h>
//#include <stdlib.h>
#include <sys/types.h> // pid
#include <sys/time.h> // pid
//#include <sys/poll.h> // pid
#include <sys/wait.h> //  pid
#include <unistd.h> // for usleep and used for pid_t
#include <pthread.h>
#include <geniePi.h>
#if GENIE
// todo remove some if GENIE
#endif

#define GENIE_OBJ_FORM 10
#define GENIE_OBJ_USERBUTTON 33
#define GENIE_OBJ_4DBUTTON 30

#define PORT "/dev/ttyAMA0"
//#define BAUDRATE 9600
#define BAUDRATE 115200

/* Now in headerfile
#define INIT_FORM 0
#define INFO_FORM 1
#define SUPPORT_FORM 2
*/

//#define BUFFSIZE 4096
#define BUFFSIZE 2048
//#define INTERFALL 175000 // mili seconds - old
#define INTERFALL 17500 // mili seconds


#define ZERO "0"


volatile int FORM = 0;

//struct data oldData;
//oldData = (char*)malloc(sizeof(char)*STRUCTSIZE*6);
//struct data *oldData = malloc(STRUCTSIZE*6);
struct data oldData;
//void initStruct(struct data);
//struct data Newdata; //TODO replace

void goToInfo(void);
void writeOldDataToDisplay(void);
void sendDataToDisplay(char* data, int id);
void sendDataToDisplayToDisplay(struct data *newData);
void structManager(struct data *newData, int id, char* data);

/* remove eventually
   if(reply->object == GENIE_OBJ_4DBUTTON) {
   */

/* */
int main (int argc, char** argv) {
    int status, id, ret; 

    pthread_t displayThread ;
    struct data newData;
    struct genieReplyStruct *reply = malloc(sizeof(struct genieReplyStruct));

    char readBuffer[BUFFSIZE];
    char writeBuffer[BUFFSIZE];

    int fd_child[2], fd_parent[2];
    int rc;

    if(genieSetup(PORT ,BAUDRATE)<0) {
        printf("ViSi-Genie Failed to init display!\r\n");
        return(1); // Failed to initialize ViSi-Genie Display. Check Connections!
    }

    if(pthread_create(&displayThread, NULL, getDisplayInput, &reply)) {
	fprintf(stderr, "Error creating thread\n");
	return 1;
    }
    
    pipe(fd_child);
    pipe(fd_parent);

    pid_t child, displayChild;

    child = fork();

    if(child == (pid_t)-1){
        perror("Failed to create child\n");
        exit(EXIT_FAILURE);
    } /* failed to create child*/


    if(!child){
      //printf("Child here! \n");
        close(fd_child[1]);
        close(fd_parent[0]);

	sleep(3);
        for(;;){
             childGetData(fd_child[0], fd_parent[1]);
        } // if something goes wrong, initalise new named pipe
    } // child enters here


    //    displayChild = fork();

    //  if(displayChild){
    //genieWriteContrast(0); // turn the display backlight on again
    //sleep(1);
    ////printf("display here! \n");
	//getDisplayInput(reply);
	//}

/*
#endif
*/
    // shell: use for testing
    /*const char* c = (const char* )argv[1];
      if((strcmp(c, "-s")) == 0 ){
      shell();
      exit(0);
      }*/
    /*
    wiringPiSetup () ;
    pinMode (12, OUTPUT) ;
    pinMode (13, OUTPUT) ;
    digitalWrite (12, HIGH) ; delay (500) ;
    digitalWrite (13, HIGH) ; delay (500) ;
    */


    initStruct(&newData);
    clearStruct(&newData);
    clearStruct(&oldData);

    close(fd_parent[1]);
    close(fd_child[0]);

    usleep(INTERFALL);
    goToInfo(); // go to next form on display


    for(;;) {
        if(checkFd(fd_parent[0])){
            read(fd_parent[0], &readBuffer, BUFFSIZE);
            id = getIdFromString(readBuffer);
	#if DEBUG
            //printf("\n parent: %s", readBuffer);
	#endif
		if(FORM == INFO_FORM){
			structManager(&newData, id, readBuffer);
			sendDataToDisplayToDisplay(&newData);
		} else {
		    usleep(INTERFALL);
		}

	if(ret == -1){
		/* error */
		perror("Error - parent: ");
	    } else{
	#if DEBUG
	    //printf("Timeout!\n");
	#endif
	usleep(INTERFALL);
        }
      }
    }
    return(0);
}


/*
 *Get data from a named pipe (mypipe) 
 *and pass it to the parent process through fd.
 */
void childGetData(int fd_child, int fd_parent ){
    /* Get data from python script */
    char readBuffer[BUFFSIZE];
    char buf[BUFFSIZE];
    int n;
    int ret;
    FILE *file;

    char * myfifo = "/tmp/mypipe";

    /* create the FIFO (named pipe) */
    if(mkfifo(myfifo, 0666) == -1){
        perror("mkfifo: \n");
    }

    file = fopen(myfifo, "r");
    if(file = NULL){
      printf("genieInterface.c: failed to open pipe\n");
    }

    for(;;){

        if(file == NULL){
            file = fopen(myfifo, "r");
        } // if no file is opened yet. Open it.
	usleep(INTERFALL);

		if(fgets(buf, BUFFSIZE, file) > 0){
		  //printf("%s \n", buf);
		    n = write(fd_parent, &buf, sizeof(buf));
		    if(n > 0){
			//successfully written to partent process
		    }
		    if(n < 0){
			perror("Error: failed to write to parent process through file descriptor ");
		    }
		}
		usleep(INTERFALL);
    }
    unlink(myfifo);
}


void handleEvent (struct genieReplyStruct *reply) {
    if(reply->object == GENIE_OBJ_USERBUTTON) {
        switch (reply->index) {
            case 0:
                /* Main screen. Show no data. Save data.*/
                changeForm();
                break;
            case 1:
                /* Display with data. Obtain old data? */
                changeForm();
                break;
            default:
                // TODO error screen
                printf("Error, index not in range or found.");
                exit(0);
                break;
        }
    }
}



//TODO change namae
/*Check and send data to display */
void sendDataToDisplayToDisplay(struct data *newData){
	if(strncmp(newData->ip, ZERO, 1) !=0) // check if empty
	  if(strcmp(newData->ip, oldData.ip) != 0){ // check if identical
			sendDataToDisplay(newData->ip, LABEL_IP_ID); // send data
			strcpy(oldData.ip, newData->ip);
	  }
	if(strncmp(newData->status, ZERO, 1) != 0)
	  if(strcmp(newData->status, oldData.status) != 0){
			sendDataToDisplay(newData->status, LABEL_STATUS_ID);
			strcpy(oldData.status, newData->status);
	  }
	if(strncmp(newData->position, ZERO, 1) != 0)
	  if(strcmp(newData->position, oldData.position) != 0){
			sendDataToDisplay(newData->position, LABEL_POSITION_ID);
			strcpy(oldData.position, newData->position);
	  }
	if(strncmp(newData->heading, ZERO, 1) != 0)
	  if(strcmp(newData->heading, oldData.heading) != 0){
			sendDataToDisplay(newData->heading, LABEL_HEADING_ID);
			strcpy(oldData.heading, newData->heading);
	  }
	if(strncmp(newData->rtk, ZERO, 1) != 0)
	  if(strcmp(newData->rtk, oldData.rtk) != 0){
			sendDataToDisplay(newData->rtk, LABEL_RTK_ID);
			strcpy(oldData.rtk, newData->rtk);
	  }
	if(strncmp(newData->satallite, ZERO, 1) != 0)
	  if(strcmp(newData->satallite, oldData.satallite) != 0){
			sendDataToDisplay(newData->satallite, LABEL_SATALLITE_ID);
			strcpy(oldData.satallite, newData->satallite);
	  }
	//oldData = *newData;
}


void clearDisplay(){
      int i = 0;
      genieWriteObj(GENIE_OBJ_FORM,FORM, 1);
  /*genieWriteStr(STATUS_ID, "...");
  genieWriteStr(POSITION_ID, "...");
  genieWriteStr(HEADING_ID, "...");
  genieWriteStr(RTK_ID, "...");
  genieWriteStr(SATALLITE_ID, "...");*/
}


/*Change init form to info form on display */
void goToInfo(){
    genieWriteObj(GENIE_OBJ_FORM,INFO_FORM, 1);
    FORM = INFO_FORM;
}


/*Change display screen*/
int changeForm(){

  (FORM == INFO_FORM) ? (FORM = SUPPORT_FORM) : (FORM = INFO_FORM);

    genieWriteObj(GENIE_OBJ_FORM,FORM, 1);
    if(FORM == INFO_FORM){
      writeOldDataToDisplay();
    } // load data for INFO FORM
  return 1;
}


/* 
 * write old data to screen when screens returns to info_form.
*/
void writeOldDataToDisplay(void){
  sendDataToDisplay(oldData.ip, LABEL_IP_ID);
  usleep(10);
  sendDataToDisplay(oldData.position, LABEL_POSITION_ID);
  usleep(10);
  sendDataToDisplay(oldData.status, LABEL_STATUS_ID);
  usleep(10);
  sendDataToDisplay(oldData.heading, LABEL_HEADING_ID);
  usleep(10);
  sendDataToDisplay(oldData.rtk, LABEL_RTK_ID);
  usleep(10);
  sendDataToDisplay(oldData.satallite, LABEL_SATALLITE_ID);
}


/* Sent data to label with id on display*/
void sendDataToDisplay(char* data, int id){
    genieWriteStr(id, data);
}


/* Wait for input from the display.
   Change form if button is pressed.
struct genieReplyStruct reply
*/
void *getDisplayInput(void *reply){
  struct genieReplyStruct * replyStruct = (struct genieReplyStruct *)reply;

  for(;;){
  while(genieReplyAvail()){
	    if(genieReplyAvail ())
	    {
	    genieGetReply    (reply) ;
	    handleEvent      (reply) ;
	    usleep (100000) ; // 10mS - Don't hog the CPU in-case anything else is happening...
	}
  }
  }
	    return NULL;
}


/* Subtract and return ID from string */
int getIdFromString(char *str){
    // regex to get integer from string
    char *strMask = "%*[^0123456789]%d";
    int id;
    int i = 0;

    while(sizeof(str) > i){
        if(sscanf(str, strMask, &id) == 1){
            return id;
        }
        i++;
    }
    //error
    printf("error: Id not found\n");

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




/************************************
 * Check file descriptor with child.*
 ***********************************/
int checkFd(int fd_parent){
    //Init timeout
    struct timeval tv;
    // set timeout to x Sec
    tv.tv_usec = INTERFALL;

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
    } 
        /* Timeout */
	return 0;
}
