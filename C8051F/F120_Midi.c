#include <common.h>
#include <c8051f120.h>
#include <F120_Midi.h>
#include <F120_UART0.h>


void KeyBoard_Init(void)
{
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page

   SFRPAGE = CONFIG_PAGE;              // Set SFR page

   P3MDOUT = 0x00;
   P2MDOUT = 0x00;
   P3 = 0xff;
   P2 = 0xff;
   P1MDOUT |= 0xff;

//   XBR2     = 0x40;                    // Enable crossbar and weak pull-up
                                       
   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page
}

void Port_Res(void)
{
    SFRPAGE = CONFIG_PAGE;
    P2MDOUT = 0xff;
	P3MDOUT = 0xff;
	P1 = 0xff;
	P2 = 0xff;
	P3 = 0xff;
	P2MDOUT = 0x00;
	P3MDOUT = 0x00;
	P2 = 0xff;
	P3 = 0xff;
}

void Send_Midi_Cmd(uchar cc,uchar kk,uchar vv)
{
    SFRPAGE = UART0_PAGE;
    Send_Byte(cc);
	Send_Byte(kk);
	Send_Byte(vv);
}