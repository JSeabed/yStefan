#include "../include/struct.h"
#include "../include/demo.h"

volatile int DFORM = 0;

void demoRead(int wait, struct genieReplyStruct *reply ){
        int i = 0;
        for(i = 0; i <= (wait*750); i++){
        usleep(200);
        if(genieReplyAvail()) {
            printf("ik kom hier\n");
            genieGetReply(reply);
            handleEvent(reply);
            if(FORM == INFO_FORM){
                break;
            }
            //usleep(wait-i);
        } // handle input from display
        }
}

void demo(){
    if(genieSetup(PORT ,BAUDRATE)<0) {
        printf("ViSi-Genie Failed to init display!\r\n");
        exit(1); // Failed to initialize ViSi-Genie Display. Check Connections!
    }

    struct genieReplyStruct reply;
    goToInfo(); // go to next form on display
    DFORM = 1;

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
       // clearStruct(&oldData);
        dataReady(&newData0);    
        demoRead(16, &reply);
        dataReady(&newData1);    
        demoRead(12, &reply);
        dataReady(&newData2);    
        demoRead(18, &reply);
        dataReady(&newData1);    
        demoRead(20, &reply);
        dataReady(&newData2);    
        demoRead(18, &reply);
        dataReady(&newData3);    
        demoRead(11, &reply);

    }
}

