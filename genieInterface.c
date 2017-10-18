#include <stdio.h>
#include <unistd.h> // for usleep
#include <geniePi.h>

#define GENIE_OBJ_FORM 10

/*
   ip
   gpgga
   ins_active
   ins_inactive
   ins_aligning
   ins_high_variance
   ins_solution_good
   ins_solution_free
   ins_alignment_complete
   determining_orientation
   waiting_initialpos
   waiting_azimuth
   initializing_biases
   motion_detect
   finesteering
   coarsesteering
   unknown
   aproximate
   coarseadjusting
   coarse
   freewheeling
   fineadjusting
   fine
   finebackupsteering
   sattime
   gpgga
   ins
   */

typedef int bool;
#define true 1
#define false 0

struct data{
	char* ip; 
	float[] gpgga;
	bool ins_active; 
	bool ins_aligning; 
	bool ins_high_variance
	bool ins_solution_good
	bool ins_solution_free
	bool ins_alignment_complete
	bool determining_orientation
	bool waiting_initialpos
	bool waiting_azimuth
	bool initializing_biases
	bool motion_detect
	bool finesteering
	bool coarsesteering
	bool unknown
	bool aproximate
	bool coarseadjusting
	bool coarse
	bool freewheeling
	bool fineadjusting
	bool fine
	bool finebackupsteering
	bool sattime
	bool gpgga
	bool ins
};


void handleEvent (struct genieReplyStruct *reply) {
	if(reply->object == GENIE_OBJ_FORM) {
    switch (reply->index) {
      case 0:
	/* Main screen. Show no data. Save data.*/
        genieWriteStr(0,"You pressed the RED button.");
	printf("RED");
        break;
      case 1:
	/* Screen with data. Obtain old data? */
        genieWriteStr(0,"You pressed the GREEN button.");
	printf("Green");
        break;
     default:
	printf("Error, index not in range or found.")
	break;
    }
  }
}

void getData(void){
	/* Get data from python script */
}

int main (int argc, char** argv) {
  struct genieReplyStruct reply;

  if(genieSetup("/dev/ttyAMA0",115200)<0) {
    printf("ViSi-Genie Failed to init display!\r\n");
    return(1); // Failed to initialize ViSi-Genie Display. Check Connections!
  }
  while(1) {
usleep(20000);
    while(genieReplyAvail()) {
      genieGetReply(&reply);
      handleEvent(&reply);
      printf("Ik kom hier");
      usleep(20000); // wait 20ms between polls to save CPU
    }
  }

  return(0);
}
