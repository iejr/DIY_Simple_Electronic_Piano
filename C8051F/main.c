


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <c8051f120.h>                 // SFR declarations
#include <stdio.h> 
#include <common.h>                    
#include <F120_UART0.h>
#include <F120_Oscillators.h>
#include <F120_Midi.h>

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F12x
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
extern void OSCILLATOR_Init(void);
extern void UART0_Init(void);

extern void KeyBoard_Init(void);
extern void Port_Res(void);

void Port_Enable(void);
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
uchar cKeyNumberH[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
uchar cKeyNumberL[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
//-----------------------------------------------------------------------------
// main() Routine
//-----------------------------------------------------------------------------

void main (void)
{
   uchar i = 0;
   uint j = 0;

	uchar cZone = 0x01;
	uchar cKeyInput1,cKeyInput2;
	uint iKeyInput1,iKeyInput2;

	uchar cKeyChange1,cKeyChange2;
	uint  uKeyChange1,uKeyChange2;

	uint iTestBit = 1;

   SFRPAGE = CONFIG_PAGE;

   WDTCN = 0xDE;                       // Disable watchdog timer
   WDTCN = 0xAD;

   OSCILLATOR_Init ();                 // Initialize oscillator

   UART0_Init ();                      // Initialize UART0

   KeyBoard_Init();

   Port_Enable();

   EA = 0;

   while (1)
   {  	  
      cZone = 0x01;
	  
	  for(i = 0;i < 6;i++)
	  {
	      SFRPAGE = CONFIG_PAGE;
	      P1 = 0xff & (~cZone);
		  for(j=0;j<20;j++);
		  cKeyInput1 = P2;
		  cKeyInput2 = P3;
		  iKeyInput1 = cKeyInput2;
		  iKeyInput1 <<= 8;
		  iKeyInput1 += cKeyInput1;

//		  Port_Res();
          P2MDOUT = 0xff;
		 P3MDOUT = 0xff;
		 P1 = 0xff;
		 P2 = 0xff;
		 P3 = 0xff;
		 P2MDOUT = 0x00;
		 P3MDOUT = 0x00;
		 P2 = 0xff;
		 P3 = 0xff;
/*
		  if(iIn != 65535)
		  {
	          for(i = 0;i < 8270;i++);
			  SFRPAGE = CONFIG_PAGE;
		      P1 = k;
			  
			  cIn1 = P2;
			  cIn2 = P3;
			  iInOK = cIn2;
			  iInOK <<= 8;
			  iInOK += cIn1;
			  
			  iKeyNumberBuf[i] = iIn;
	
			  P2MDOUT = 0xff;
			  P3MDOUT = 0xff;
			  P1 = 0xff;
			  P2 = 0xff;
			  P3 = 0xff;
			  P2MDOUT = 0x00;
			  P3MDOUT = 0x00;
			  P2 = 0xff;
			  P3 = 0xff;

			  if(iIn == iInOK)
			  {
				  SFRPAGE = UART0_PAGE;
				  Send_Byte(iIn);
				  Send_Byte(0x00);
			  }
		  }
*/

//		  SFRPAGE = UART0_PAGE;
//		  Send_Byte(cKeyInput2);
//		  Send_Byte(cKeyInput1);
//		  Send_Byte(0x00);

		  cKeyChange2 = cKeyNumberH[i] ^ cKeyInput2;
		  cKeyChange1 = cKeyNumberL[i] ^ cKeyInput1;
		   
		  uKeyChange1 = cKeyChange2;
		  uKeyChange1 <<= 8;
		  uKeyChange1 += cKeyChange1;
		  if(uKeyChange1 != 0)
		  {
		      iTestBit = 1;
//			  SFRPAGE = UART0_PAGE;
//			  Send_Byte(iKeyInput1);
		      for(j = 0;j < 12;j++)
			  {
			       if((uKeyChange1 & iTestBit) != 0)
				   {
				       SFRPAGE = UART0_PAGE;
					   if((iKeyInput1 & iTestBit) != 0)
					   {
//					       Send_Byte(12*i + j + MIDI_ADDITION);//·¢ËÍÍ£Ö¹ÃüÁî
//						   Send_Byte(0x00);
						   Send_Midi_Cmd(0x81,12*i + j + MIDI_ADDITION,0x7e);
					   }
					   else
					   {
//					       Send_Byte(12*i + j + MIDI_ADDITION);//·¢ËÍµ¯×àÃüÁî
//						   Send_Byte(0xff);
                           Send_Midi_Cmd(0x91,12*i + j + MIDI_ADDITION,0x6f);
					   }
				   }
				   iTestBit <<= 1;
			  }

			  cKeyNumberH[i] = cKeyInput2;
			  cKeyNumberL[i] = cKeyInput1;
		  }
		  
		  cZone <<= 1;
	  }

	  for(i = 0;i < 127;i++)
	     for(j = 0;j < 1000;j++);
   }
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------
void Port_Enable(void)
{
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page

   SFRPAGE = CONFIG_PAGE;              // Set SFR page

   XBR2     = 0x40;                    // Enable crossbar and weak pull-up
                                       
   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------