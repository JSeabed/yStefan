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
#include <geniepi.h>

#define genie_obj_form 10
#define genie_obj_userbutton 33
#define genie_obj_4dbutton 30

#define from(x) (0x010a + x + 0000) // todo needs to be checked

//#define buffsize 4096
#define buffsize 2048
#define structsize 150
#define timeout 500
#define wait 250

/*#define checksum(x) (x ^= x)*/

typedef int bool;
#define true 1
#define false 0

#define ip_id 0
#define status_id 2
#define position_id 1
#define heading_id 6
#define rtk_id 3
#define satallite_id 7

#define main_screen 0
#define info_screen 1

#define toggle(x) (x = !x)
#define isidentical(a, b) (a == b)

int form = 0;

struct data{
  char *ip;
  char *status;
  char *position;
  char *heading;
  char *rtk;
  char *satallite;
};

//struct data newdata; //todo replace
struct data olddata;

void sentdata(char* data, int id);
void dataready(struct data *newdata, struct geniereplystruct *reply);
void structmanager(struct data *newdata, int id, char* data);

/* remove eventually
   if(reply->object == genie_obj_4dbutton) {
   */

void handleevent (struct geniereplystruct *reply) {
	if(reply->object == genie_obj_4dbutton) {
		switch (reply->index) {
			case 0:
				/* main screen. show no data. save data.*/
				geniewritestr(1,"you pressed the red button.");
				geniewriteobj(genie_obj_form, 1, 1);
				#if debug
					printf("red");
					printf("%d\n");
				#endif
				break;
			case 1:
				/* screen with data. obtain old data? */
				geniewritestr(2,"you pressed the green button.");
				geniewriteobj(genie_obj_form,0, 1);
				#if debug
					printf("green");
					printf("%d\n");
				#endif
				// sentdata();
				break;
			default:
				printf("error, index not in range or found.");
				exit(0);
				break;
		}
	}
}


int addstruct(struct data *newdata, int id, char *datastr){
  // first remove id from string
  datastr += 3;
  #if debug
  printf("add to struct: %s\n", datastr);
  #endif
  switch(id){
  case ip_id:
    strcpy(newdata->ip, datastr);
    return;
  case status_id:
    strcpy(newdata->status, datastr);
    return;
  case position_id:
    strcpy(newdata->position, datastr);
    return;
  case heading_id:
    strcpy(newdata->heading, datastr);
    return;
  case rtk_id:
    strcpy(newdata->rtk, datastr);
    return;
  case satallite_id:
    strcpy(newdata->satallite, datastr);
    return;
  default:
    printf("error: addtostruct");
  }
}

// allocate memory for struct
void initstruct(struct data *newdata){
  newdata->ip = (char*)malloc(sizeof(char)*structsize);
  newdata->status = (char*)malloc(sizeof(char)*structsize);
  newdata->position = (char*)malloc(sizeof(char)*structsize);
  newdata->heading = (char*)malloc(sizeof(char)*structsize);
  newdata->rtk = (char*)malloc(sizeof(char)*structsize);
  newdata->satallite = (char*)malloc(sizeof(char)*structsize);
}

void clearstruct(struct data *newdata){
  #if debug
  printf("clearstruct \n");
  #endif
  char str[10] = "0";
  strcpy(newdata->ip, str);
  strcpy(newdata->status, str);
  strcpy(newdata->position, str);
  strcpy(newdata->heading, str);
  strcpy(newdata->rtk, str);
  strcpy(newdata->satallite, str);
  // strncpy(newdata->&ip , null, 1);
  //strncpy(newdata->&status , null , 1);
  //strncpy(newdata->position , (char*)'0', 1);
  //strncpy(newdata->heading , (char*)'0', 1);
  //strncpy(newdata->rtk , (char*)'0', 1);
  //strncpy(newdata->satallite , (char*)'0', 1);
}


int isstructfull(struct data *newdata){
#if debug
  printf("isstructfull: %s", newdata->ip);
  if(newdata->ip == 0)
    printf("passed second test. ip is 0\n");

#endif
}


void printstruct(struct data *newdata){
  #if debug
  printf("structure: \n");
  printf("ip: %s\n", newdata->ip);
  printf("status: %s\n", newdata->status);
  printf("position: %s\n", newdata->position);
  printf("heading: %s\n", newdata->heading);
  printf("rtk: %s\n", newdata->rtk);
  printf("satallite: %s\n", newdata->satallite);
  #endif
}


void structmanager(struct data *newdata, int id, char* data){
  // compare
  #if debug
  printf("structmanager\n");
  #endif
  addstruct(newdata, id, data);
  printstruct(newdata);
  //dataready(newdata);
}

//todo change name
void dataready(struct data *newdata, struct geniereplystruct *reply){
  char *zero = "0";
  printf("te vergelijken %s en %s \n", zero, newdata->ip);
  if(strncmp(newdata->ip, zero, 1) != 0){
    //sentdata(newdata->ip, ip_id);
    printf("data is send :( \n ");
  }
  else printf("data not send\n");
  if(strncmp(newdata->status, zero, 1) != 0){
    sentdata(newdata->status, status_id);
  }
  else printf("data not send\n");
  if(strncmp(newdata->position, zero, 1) != 0){
    sentdata(newdata->position, position_id);
  }
  else printf("data not send\n");
  if(strncmp(newdata->heading, zero, 1) != 0){
    sentdata(newdata->heading, ip_id);
  }
  else printf("data not send\n");
    //if(isidentical)
    
}

int changeform(){
  toggle(form);
  geniewriteobj(genie_obj_form,form, 1);
  return 1;
}


  void sentdata(char* data, int id){
  geniewritestr(id, data);
  usleep(250);
  //geniewritestr(status_id, newdata->status);
  //perror("sentdata");
  //geniewritestr(position_id, newdata->position);
  //perror("sentdata");

  //geniewritestr(heading_id, newdata->heading);
  //perror("sentdata");
  //geniewritestr(rtk_id, newdata->rtk);
  //perror("sentdata");
  //geniewritestr(satallite_id, newdata->satallite);
  //perror("sentdata");
}

/*
int checkfifo(file *file){
	int retval;
	struct pollfd fd1;
	fd1.fd = file;
	retval = poll(fd1, 1, timeout);
	if(retval < 0){
	} else if(retval > 0){
		getdata(file);
	} else {
	}
	return true;
}
*/

void childgetdata(int fd_child, int fd_parent ){
	/* get data from python script */
	char readbuffer[buffsize];
	//printf("child here\n");
	//read(fd_child, &readbuffer, buffsize);
	//printf("%s", readbuffer);

	int n;
	//int fd;
	char buf[buffsize];
	file *file;
	int ret;
	char * myfifo = "/tmp/mypipe";
	/* create the fifo (named pipe) */
	mkfifo(myfifo, 0666);
	file = fopen(myfifo, "r");
	/* write "hi" to the fifo */
	//fd = open(myfifo, o_wronly);
	//write(fd, "hi", sizeof("hi"));

	for(;;){
			if(fgets(buf, buffsize, file) > 0){
			//  printf("%s \n", buf);
			n = write(fd_parent, &buf, sizeof(buf));
			if(n > 0){
			  printf("verstuurd!: %s \n", buf);
			}
			if(n < 0){
			  perror("error: ");
			}
		}
	  	//file = open(myfifo, o_wronly);
	  	//fclose(file);
		//fflush(file);
	  usleep(wait);
	}
	unlink(myfifo);
}


int getid(char *str){
  char *strmask = "%*[^0123456789]%d";
  int id;
  int i = 0;
  while(sizeof(str) > i){
    if(sscanf(str, strmask, &id) == 1){
    printf("id is = %d", id);
    return id;
    }
    i++;
  }
  //error
  printf("id not found\n");
  return -1;
}
	/* remove the fifo */
	/* fill data struct*/
/************************************************************************
 * fetch the received data from the python script to the data structure *
 * *********************************************************************/
/*int fetchdata(struct data, char *buf){
	//data.ip =
	return 1;
}*/


void errorexit(char* error){
	printf("%s\n", error);
	//exit(0);
}


int main (int argc, char** argv) {
  #if debug
  printf("debug mode on\n");
  #endif
	struct geniereplystruct reply;
	// fd_child = child read | fd_parent = parent_read
	int fd_child[2], fd_parent[2];
	int status, id, ret;

	struct data newdata;
	initstruct(&newdata);
	clearstruct(&newdata);
	isstructfull(&newdata);
	dataready(&newdata, &reply);

	char readbuffer[buffsize];
	char writebuffer[buffsize];

	pipe(fd_child);
	pipe(fd_parent);

	pid_t child, p;
	child = fork();

	if(geniesetup("/dev/ttyama0",9600)<0) {
		printf("visi-genie failed to init display!\r\n");
		return(1); // failed to initialize visi-genie display. check connections!
	}

		if(child == (pid_t)-1){
			/* failed to create child*/

		}

		if(!child){
			/* here enters the child */
			/* create pipe to python script */
			/* check if named pipe if filled*/
			//printf("child pid = %d \n", (int)child);
			close(fd_child[1]);
			close(fd_parent[0]);
			childgetdata(fd_child[0], fd_parent[1]);
		}

		close(fd_parent[1]);
		close(fd_child[0]);

		//write(fd_child[1], &test, sizeof(test));
	for(;;) {
		if(ret = checkfd(fd_parent[0])){
			#if debug
			    printf("data is available\n");
			#endif
			read(fd_parent[0], &readbuffer, buffsize);
			id = getid(readbuffer);
			#if debug
			    printf("\n parent: %s", readbuffer);
			#endif
			structmanager(&newdata, id, readbuffer);
			dataready(&newdata, &reply);
			//void structmanager(struct data *newdata, int id, char* data, char dataready){
			//geniewritestr(1, readbuffer);
			//fflush(myfifo* fd_parent[0]);
			// fetchdata();
		} else if(ret == -1){
			/* error */
		  perror("error - parent: ");
		} else{
			#if debug
				printf("timeout!\n");
			#endif
			usleep(wait);
		}
		#if debug
		#endif
		//struct data newdata; //todo replace
		usleep(wait);
		//if(isstructfull(&newdata)) sentdata(&newdata);
		while(geniereplyavail()) {
			geniegetreply(&reply);
			handleevent(&reply);
			usleep(wait); // wait 20ms between polls to save cpu
		}

}
	return(0);
}
/************************************
 * check file descriptor with child.*
 ***********************************/
int checkfd(int fd_parent){
	//init timeout
	struct timeval tv;
	// set timeout to x sec
	tv.tv_usec = timeout;

	fd_set set;
	fd_zero(&set);
	fd_set(fd_parent, &set);

	int retval = select(fd_setsize, &set, null, null, &tv);
	if(retval == -1){
		printf("error: select()\n");
		return -1;
	}
	else if(retval){
		/* data is available */
		return 1;
	} else {
		/* timeout */
		return 0;
	}
}
