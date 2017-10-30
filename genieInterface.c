#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h> // pid
#include <sys/time.h> // pid
//#include <sys/poll.h> // pid
#include <sys/wait.h> //  pid
#include <unistd.h> // for usleep and used for pid_t
#include <geniePi.h>

#define GENIE_OBJ_FORM 10
#define GENIE_OBJ_USERBUTTON 33
#define GENIE_OBJ_4DBUTTON 30

#define FROM(x) (0x010a + x + 0000) // TODO needs to be checked

//#define BUFFSIZE 4096
#define BUFFSIZE 2048
#define TIMEOUT 500
#define WAIT 250

#define MAIN_SCREEN 0
#define INFO_SCREEN 1

/*#define checksum(x) (x ^= x)*/

typedef int bool;
#define true 1
#define false 0

struct data{
	char* ip;
	float gpgga;
	bool ins_active;
	bool ins_aligning;
	bool ins_high_variance;
	bool ins_solution_good;
	bool ins_solution_free;
	bool ins_alignment_complete;
	bool determining_orientation;
	bool waiting_initialpos;
	bool waiting_azimuth;
	bool initializing_biases;
	bool motion_detect;
	bool finesteering;
	bool coarsesteering;
	bool unknown;
	bool aproximate;
	bool coarseadjusting;
	bool coarse;
	bool freewheeling;
	bool fineadjusting;
	bool fine;
	bool finebackupsteering;
	bool sattime;
	bool ins;
};

//struct data Newdata; //TODO replace
struct data oldData;

/* remove eventually
   if(reply->object == GENIE_OBJ_4DBUTTON) {
   */

void handleEvent (struct genieReplyStruct *reply) {
	if(reply->object == GENIE_OBJ_4DBUTTON) {
		switch (reply->index) {
			case 0:
				/* Main screen. Show no data. Save data.*/
				genieWriteStr(1,"You pressed the RED button.");
				genieWriteObj(GENIE_OBJ_FORM, 1, 1);
				#if DEBUG
					printf("RED");
					printf("%d\n");
				#endif
				break;
			case 1:
				/* Screen with data. Obtain old data? */
				genieWriteStr(2,"You pressed the GREEN button.");
				genieWriteObj(GENIE_OBJ_FORM,0, 1);
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


int sentData(int isString, struct data *newData, char *str){
  if(isString){
    genieWriteStr(1, str);
  } else {
    // change form
    genieWriteObj(GENIE_OBJ_FORM,0, 1);
  }
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
	struct genieReplyStruct reply;
	// fd_child = child read | fd_parent = parent_read
	int fd_child[2], fd_parent[2];
	int status;
	int ret;

	char readBuffer[BUFFSIZE];
	char writeBuffer[BUFFSIZE];

	pipe(fd_child);
	pipe(fd_parent);

	pid_t child, p;
	child = fork();

	if(genieSetup("/dev/ttyAMA0",9600)<0) {
		printf("ViSi-Genie Failed to init display!\r\n");
		return(1); // Failed to initialize ViSi-Genie Display. Check Connections!
	}

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

		//write(fd_child[1], &test, sizeof(test));
	for(;;) {
		if(ret = checkFd(fd_parent[0])){
			#if DEBUG
				printf("Data is available\n");
				perror("Error parent: ");
			#endif
			printf("Laatste keer parent");
			read(fd_parent[0], &readBuffer, BUFFSIZE);
			printf("\n parent: %s", readBuffer);
			perror("Error parent: ");
			genieWriteStr(1, readBuffer);
			//fflush(myfifo* fd_parent[0]);
			// fetchData();
		} else if(ret == -1){
			/* error */
		  perror("Error - parent: ");
		} else{
			#if DEBUG
				printf("Timeout!\n");
			#endif
			usleep(WAIT);
		}
		#if DEBUG
		printf("FD is: %d ",fcntl(fd_parent[0], F_GETFD));
		printf("parent komt hier nog steeds!\n");
		#endif
		//struct data Newdata; //TODO replace
		usleep(WAIT);
		while(genieReplyAvail()) {
			genieGetReply(&reply);
			handleEvent(&reply);
			usleep(WAIT); // wait 20ms between polls to save CPU
		}
	}
	printf("Error - parent: exit ");
	perror("Error - parent: ");
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
