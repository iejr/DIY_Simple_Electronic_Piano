//-----------------------------------------------------------------------------
// F12x_UART0_Interrupt.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program demonstrates how to configure the C8051F120 to write to and read 
// from the UART interface. The program reads a word using the UART0 interrupts 
// and outputs that word to the screen, with all characters in uppercase
//
// How To Test:
//
// 1) Download code to a 'F12x device that is connected to a UART transceiver
// 2) Verify jumpers J6 and J9 are populated on the 'F12x TB.
// 3) Connect serial cable from the transceiver to a PC
// 4) On the PC, open HyperTerminal (or any other terminal program) and connect
//    to the COM port at <BAUDRATE> and 8-N-1
// 5) Download and execute code on an 'F12x target board.
// 6) Type up to 64 characters into the Terminal and press Enter.  The MCU 
//    will then print back the characters that were typed
//   
//
// Target:         C8051F12x
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (SM)
//    -11 JULY 2007
//


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <c8051f120.h>                 // SFR declarations
//#include <string.h>
#include <F120_UART0.h>
#include <common.h>                    

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F12x
//-----------------------------------------------------------------------------

sfr16 RCAP4    = 0xca;                 // Timer2 capture/reload
sfr16 TMR4     = 0xcc;                 // Timer2


//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and GPIO ports.
//
// P0.0   digital   push-pull     UART TX
// P0.1   digital   open-drain    UART RX
//-----------------------------------------------------------------------------
void UART0_PORT_Init (void)
{
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page

   SFRPAGE = CONFIG_PAGE;              // Set SFR page

   XBR0     = 0x04;                    // Enable UART0
   XBR1     = 0x00;
//   XBR2     = 0x40;                    // Enable crossbar and weak pull-up
                                       

   P0MDOUT |= 0x01;                    // Set TX pin to push-pull
//   P1MDOUT |= 0x40;                    // Set P1.6(LED) to push-pull

   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page
}


//-----------------------------------------------------------------------------
// UART0_Init   Variable baud rate, Timer 2, 8-N-1
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure UART0 for operation at <baudrate> 8-N-1 using Timer2 as
// baud rate source.
//
//-----------------------------------------------------------------------------
void UART0_Init (void)
{
   char SFRPAGE_SAVE;

   UART0_PORT_Init();

   SFRPAGE_SAVE = SFRPAGE;             // Preserve SFRPAGE

   SFRPAGE = TMR4_PAGE;

   TMR4CN = 0x00;                      // Timer in 16-bit auto-reload up timer
                                       // mode
   TMR4CF = 0x08;                      // SYSCLK is time base; no output;
                                       // up count only
   RCAP4 = - ((long) SYSTEMCLOCK/BAUDRATE/16);
   TMR4 = RCAP4;
   TR4= 1;                             // Start Timer4

   SFRPAGE = UART0_PAGE;

   SCON0 = 0x50;                       // 8-bit variable baud rate;
                                       // 9th bit ignored; RX enabled
                                       // clear all flags
   SSTA0 = 0x1F;                       // Clear all flags; enable baud rate
                                       // doubler (not relevant for these
                                       // timers);
                                       // Use Timer2 as RX and TX baud rate
                                       // source;
   ES0 = 1;  
   IP |= 0x10;

   SFRPAGE = SFRPAGE_SAVE;             // Restore SFRPAGE
}


/*********************************************************************/
/*****************iejr:·¢ËÍ×Ö½Ú***************************************/
void Send_Byte(uchar cSend)
{
   SBUF0 = cSend;
   while(!TI0);
   TI0 = 0;
}


/*********************************************************************/
/*****************iejr:·¢ËÍ×Ö·û´®*************************************/
void Send_String(uchar xdata* sSend)
{
   uchar i = 0;
   for(i = 0;i < 255;i++)
   {
      if(sSend[i] == '\0')
	     break;
	  else
         Send_Byte(sSend[i]);
   }
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------