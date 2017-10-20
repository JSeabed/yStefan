#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h> // pid
#include <sys/time.h> // pid
#include <sys/poll.h> // pid
#include <sys/wait.h> //  pid
#include <unistd.h> // for usleep and used for pid_t
#include <geniePi.h>

#define GENIE_OBJ_FORM 10
#define GENIE_OBJ_USERBUTTON 33
#define GENIE_OBJ_4DBUTTON 30

#define FROM(x) (0x010a + x + 0000) // TODO needs to be checked

#define BUFFSIZE 512
#define TIMEOUT 5

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
				printf("RED");
				printf("%d\n");
				break;
			case 1:
				/* Screen with data. Obtain old data? */
				genieWriteStr(2,"You pressed the GREEN button.");
				genieWriteObj(GENIE_OBJ_FORM,0, 1);
				printf("Green");
				printf("%d\n");
				break;
			default:
				printf("Error, index not in range or found.");
				exit(0);
				break;
		}
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

void getData(int fd_child, int fd_parent ){
	/* Get data from python script */
	char readBuffer[BUFFSIZE];

	read(fd_child, &readBuffer, BUFFSIZE);
	printf("%s", readBuffer);

	int n;
	//int fd;
	char buf[BUFFSIZE];
	FILE *file;
	char * myfifo = "/tmp/mypipe";

	/* create the FIFO (named pipe) */
	mkfifo(myfifo, 0666);
	file = fopen(myfifo, "r");
	/* write "Hi" to the FIFO */
	//fd = open(myfifo, O_WRONLY);
	//write(fd, "Hi", sizeof("Hi"));

	fgets(buf, BUFFSIZE, file);
	printf("%s", buf);

	write(fd_parent, buf, sizeof(buf));

	//file = open(myfifo, O_WRONLY);
	//fclose(file);

	/* remove the FIFO */
	unlink(myfifo);

	/* fill data struct*/
}


void errorExit(char* error){
	printf("%s\n", error);
	//exit(0);
}


int main (int argc, char** argv) {
	struct genieReplyStruct reply;
	// fd_child = child read | fd_parent = parent_read
	int fd_child[2], fd_parent[2];
	int status;

	char test[20];
	strcpy(test, "nigger");
	char readBuffer[512];
	char writeBuffer[128];

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
			close(fd_child[0]);
			close(fd_parent[1]);
			for(;;)
				getData(fd_child[0], fd_parent[1]);
		}

		close(fd_parent[1]);
		close(fd_child[0]);

		write(fd_child[1], &test, sizeof(test));
	for(;;) {
		read(fd_parent[0], &readBuffer, BUFFSIZE);
		struct data Newdata; //TODO replace
		usleep(20000);
		while(genieReplyAvail()) {
			genieGetReply(&reply);
			handleEvent(&reply);
			usleep(20000); // wait 20ms between polls to save CPU
		}
	}

	return(0);
}
