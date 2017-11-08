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
#define WAIT 25

/*#define checksum(x) (x ^= x)*/

#define ZERO "0"


#define MAIN_SCREEN 0
#define INFO_SCREEN 1

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

#if DEMO
    void demo();
    void demoRead(int, struct genieReplyStruct *);
#endif
/* remove eventually
   if(reply->object == GENIE_OBJ_4DBUTTON) {
   */

#if GENIE
void handleEvent (struct genieReplyStruct *reply) {


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
    usleep(20);
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
      i = genieWriteStr(IP_ID, ";;;");
  #endif
  /*genieWriteStr(STATUS_ID, "...");
  genieWriteStr(POSITION_ID, "...");
  genieWriteStr(HEADING_ID, "...");
  genieWriteStr(RTK_ID, "...");
  genieWriteStr(SATALLITE_ID, "...");*/
}

/*Change init form to info form on display */
void goToInfo(){
    genieWriteObj(GENIE_OBJ_FORM,INFO_FORM, 1);
}

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


void sentData(char* data, int id){
#if GENIE
    genieWriteStr(id, data);
#else
    //diablo code
#endif
    usleep(50);
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

#if DEMO
void demo(){
    int i = 0;
    if(genieSetup(PORT ,BAUDRATE)<0) {
        printf("ViSi-Genie Failed to init display!\r\n");
        exit(1); // Failed to initialize ViSi-Genie Display. Check Connections!
    }

    struct genieReplyStruct reply;
    goToInfo(); // go to next form on display

    struct data newData0;
    struct data newData1;
    struct data newData2;
    struct data newData3;

    strcpy(newData0.ip , "172.16.45.5");
    strcpy(newData0.status , "Ins solution good");
    strcpy(newData0.position , "Finesteering");
    strcpy(newData0.heading , "OK");
    strcpy(newData0.rtk , "Fixed");
    strcpy(newData0.satallite , "19");

    strcpy(newData1.ip , "172.16.45.5");
    strcpy(newData1.status , "Ins solution good");
    strcpy(newData1.position , "Finesteering");
    strcpy(newData1.heading , "OK");
    strcpy(newData1.rtk , "Fixed");
    strcpy(newData1.satallite , "21");

    strcpy(newData2.ip , "172.16.45.5");
    strcpy(newData2.status , "Ins solution good");
    strcpy(newData2.position , "Finesteering");
    strcpy(newData2.heading , "OK");
    strcpy(newData2.rtk , "Fixed");
    strcpy(newData2.satallite , "20");

    strcpy(newData3.ip , "172.16.45.5");
    strcpy(newData3.status , "Ins solution good");
    strcpy(newData3.position , "Finesteering");
    strcpy(newData3.heading , "OK");
    strcpy(newData3.rtk , "Fixed");
    strcpy(newData3.satallite , "17");

    for(;;){
        i = 0;
        dataReady(&newData0);    
        demoRead(2, &reply);
        dataReady(&newData1);    
        demoRead(7, &reply);
        dataReady(&newData2);    
        demoRead(12, &reply);
        dataReady(&newData1);    
        demoRead(3, &reply);
        dataReady(&newData2);    
        demoRead(8, &reply);
        dataReady(&newData3);    
        demoRead(11, &reply);

    }
}
void demoRead(int wait, struct genieReplyStruct *reply ){
        int i = 0;
        for(i <= wait; i++;){
        sleep(1);
        printf("ik kom hier");
        if(genieReplyAvail()) {
            genieGetReply(reply);
            handleEvent(reply);
            sleep(wait-i); // wait 20ms between polls to save CPU
            break;
        } // handle input from display
        }
}
#endif


int main (int argc, char** argv) {

#if DEBUG

    printf("Debug mode on\n");

#endif
#if DEMO
    demo();
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
    goToInfo(); // go to next form on display

    int fd_child[2], fd_parent[2];
    int status, id, ret;

    struct data newData;

    initStruct(&newData);
    clearStruct(&newData);
    clearStruct(&oldData);

    char readBuffer[BUFFSIZE];
    char writeBuffer[BUFFSIZE];

    pipe(fd_child);
    pipe(fd_parent);

    pid_t child;
    child = fork();

    if(child == (pid_t)-1){
        perror("Failed to create child\n");
        exit(EXIT_FAILURE);
    } /* failed to create child*/


    if(!child){
        close(fd_child[1]);
        close(fd_parent[0]);

        for(;;){
            childGetData(fd_child[0], fd_parent[1]);
        } // if something goes wrong, initalise new named pipe
    } // child enters here

    close(fd_parent[1]);
    close(fd_child[0]);

    usleep(20);


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
        } // handle input from display

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
    return 1;
}
