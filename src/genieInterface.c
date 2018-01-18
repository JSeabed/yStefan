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
#if GENIE
    #include <geniePi.h>
#else
    #include <diabloSerial.h>
    //#include <Diablo_Serial_4DLibrary.h>
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

#define INIT_FORM 0
#define INFO_FORM 1
#define SUPPORT_FORM 2

//#define BUFFSIZE 4096
#define BUFFSIZE 2048
#define TIMEOUT 500
#define INTERFALL 3 // seconds
#define WAIT 25

/*#define checksum(x) (x ^= x)*/

#define ZERO "0"

#define toggle(x) (x = !x)
#define isIdentical(a, b) (a == b)

volatile int FORM = 0;

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

printf("HandleEvent()\n");

    if(reply->object == GENIE_OBJ_USERBUTTON) {
        switch (reply->index) {
            case 0:
                /* Main screen. Show no data. Save data.*/
                changeForm();
                break;
            case 1:
                /* Screen with data. Obtain old data? */
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
#endif



//TODO change namae
/*Check and send data to display */
void dataReady(struct data *newData){
	if(strncmp(newData->ip, ZERO, 1) !=0) // check if empty
		if(strcmp(newData->ip, oldData.ip) != 0) // check if identical
			sentData(newData->ip, LABEL_IP_ID); // send data
	if(strncmp(newData->status, ZERO, 1) != 0)
		if(strcmp(newData->status, oldData.status) != 0)
			sentData(newData->status, LABEL_STATUS_ID);

	if(strncmp(newData->position, ZERO, 1) != 0)
		if(strcmp(newData->position, oldData.position) != 0)
			sentData(newData->position, LABEL_POSITION_ID);

	if(strncmp(newData->heading, ZERO, 1) != 0)
		if(strcmp(newData->heading, oldData.heading) != 0)
			sentData(newData->heading, LABEL_HEADING_ID);

	if(strncmp(newData->rtk, ZERO, 1) != 0)
		if(strcmp(newData->rtk, oldData.rtk) != 0)
			sentData(newData->rtk, LABEL_RTK_ID);

	if(strncmp(newData->satallite, ZERO, 1) != 0)
		if(strcmp(newData->satallite, oldData.satallite) != 0)
			sentData(newData->satallite, LABEL_SATALLITE_ID);

	oldData = *newData;
}


void clearScreen(){
  #if GENIE
      int i = 0;
      genieWriteObj(GENIE_OBJ_FORM,FORM, 1);
  #endif
  /*genieWriteStr(STATUS_ID, "...");
  genieWriteStr(POSITION_ID, "...");
  genieWriteStr(HEADING_ID, "...");
  genieWriteStr(RTK_ID, "...");
  genieWriteStr(SATALLITE_ID, "...");*/
}


#if DEBUG
/*Change init form to info form on display */
void goToInfo(){
    genieWriteObj(GENIE_OBJ_FORM,INFO_FORM, 1);
    printf("Function: goToInfo\n");
}
#endif


/*Change display screen*/
int changeForm(){

  (FORM == INFO_FORM) ? (FORM = SUPPORT_FORM) : (FORM = INFO_FORM);

  #if GENIE
    genieWriteObj(GENIE_OBJ_FORM,FORM, 1);

    if(FORM == INFO_FORM){
            printf("INFO_FORM\n");
    } // load data for INFO FORM

  #else
    //diablo code
  #endif
  
    clearStruct(&oldData);

  return 1;
}


/* Sent data to label with id on display*/
void sentData(char* data, int id){
#if GENIE
printf("check sendData");
    genieWriteStr(id, data);
printf("checked\n");
#else
    //diablo code
#endif
    sleep(INTERFALL);
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

    for(;;){

        if(file == NULL){
            file = fopen(myfifo, "r");
        } // if no file is opened yet. Open it.
	sleep(INTERFALL);

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
		usleep(WAIT);
    }
    unlink(myfifo);
}


/* Wait for input from the display.
   Change form if button is pressed.
*/
void getDisplayInput(struct genieReplyStruct reply){
#if DEBUG
	int data_ready = 0;
	for(;;){
	    sleep(1);

	    printf("displayinput()\n");
	    data_ready = genieReplyAvail();
	    printf("input = %d \n", data_ready);
        if(data_ready) {
	    printf("GET DISPLAY DATA\n");
            genieGetReply(&reply);
            //handleEvent(&reply);
    		if(reply.object == GENIE_OBJ_USERBUTTON) {
			changeForm();
		}
            usleep(WAIT); // wait 20ms between polls to save CPU
            } // handle input from display
	}
#endif
}


/* Subtract and return ID from string */
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
    printf("Id not found\n");

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


/* */
int main (int argc, char** argv) {
    int status, id, ret; 

    pthread_t myThread ;
    struct genieReplyStruct reply;
    struct data newData;

    char readBuffer[BUFFSIZE];
    char writeBuffer[BUFFSIZE];

    int fd_child[2], fd_parent[2];
    int rc;

    pipe(fd_child);
    pipe(fd_parent);

    pid_t child, displayChild;

    child = fork();

    if(child == (pid_t)-1){
        perror("Failed to create child\n");
        exit(EXIT_FAILURE);
    } /* failed to create child*/


    if(!child){
      printf("Child here! \n");
        close(fd_child[1]);
        close(fd_parent[0]);

	sleep(10);
        for(;;){
             childGetData(fd_child[0], fd_parent[1]);
        } // if something goes wrong, initalise new named pipe
    } // child enters here


    displayChild = fork();

    if(!displayChild){
      printf("display here! \n");
	sleep(10);
	getDisplayInput(reply);
    }

    if(genieSetup(PORT ,BAUDRATE)<0) {
        printf("ViSi-Genie Failed to init display!\r\n");
        return(1); // Failed to initialize ViSi-Genie Display. Check Connections!
    }
/*
#else
    // diablo init code
    rc = OpenComm(PORT, BAUDRATE);
    if(rc != 0){
        printf("Failed to init display\n");
        exit(EXIT_FAILURE);
    }
#endif
*/
    // shell: use for testing
    /*const char* c = (const char* )argv[1];
      if((strcmp(c, "-s")) == 0 ){
      shell();
      exit(0);
      }*/
    wiringPiSetup () ;
    pinMode (12, OUTPUT) ;
    pinMode (13, OUTPUT) ;
    digitalWrite (12, HIGH) ; delay (500) ;
    digitalWrite (13, HIGH) ; delay (500) ;

    printf("----Ik kom hier nog 2-----\n");

    initStruct(&newData);
    clearStruct(&newData);
    clearStruct(&oldData);

    close(fd_parent[1]);
    close(fd_child[0]);

    goToInfo();
    //goToInfo(); // go to next form on display
    for(;;) {
	printf("Kom ik hier?\n");
        if(ret = checkFd(fd_parent[0])){
            printf("check parent read");
            read(fd_parent[0], &readBuffer, BUFFSIZE);
	    printf("checked\n");
            id = getID(readBuffer);
#if DEBUG

            printf("\n parent: %s", readBuffer);

#endif
            structManager(&newData, id, readBuffer);
            printf("check dataReady");
            dataReady(&newData);
	    printf("checked\n");
    	    usleep(20);
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
    } 
        /* Timeout */
	return 0;
}
