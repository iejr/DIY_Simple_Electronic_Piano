#include <common.h>

static void UART0_PORT_Init (void);
void UART0_Init (void);//int mSystemClock,int mBaudrate);
void Send_Byte(uchar cSend);
void Send_String(uchar xdata* sSend);
