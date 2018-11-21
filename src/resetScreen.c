#include <stdio.h>
#include <geniePi.h>

#define PORT "/dev/ttyAMA0"
#define BAUDRATE 115200
#define INIT_FORM 0
#define INFO_FORM 1

/* 
   This file is used to reset the screen of the SGR7 at boot.
 */

int main (int argc, char** argv) {
    int status, id, ret; 

    struct genieReplyStruct reply;

    if(genieSetup(PORT ,BAUDRATE)<0) {
        printf("ViSi-Genie Failed to init display!\r\n");
        return(1); // Failed to initialize ViSi-Genie Display. Check Connections!
    }


    printf("reset screen\n");
    genieWriteObj(GENIE_OBJ_FORM,INIT_FORM, 1);
    printf("done\n");
}
