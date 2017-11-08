#ifndef _DEMO_HEADER
#define _DEMO_HEADER


#define INIT_FORM 0
#define INFO_FORM 1
#define SUPPORT_FORM 2

#define PORT "/dev/ttyAMA0"
#define BAUDRATE 115200

void demo(void);
void demoRead(int , struct genieReplyStruct);
/**/

#endif
