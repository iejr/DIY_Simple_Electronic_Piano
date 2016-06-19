#ifndef _F120_MIDI_
#define _F120_MIDI_

#include <common.h>
//#include <c8051f120.h>

#define MIDI_ADDITION    24

void KeyBoard_Init(void);
void Port_Res(void);
void Send_Midi_Cmd(uchar cc,uchar kk,uchar vv);

#endif