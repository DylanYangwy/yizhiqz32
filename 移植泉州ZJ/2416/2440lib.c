#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

#include"includes.h"

int whichUart=0;
INT8U Flag_Comm;

//extern char Image$$RW$$Limit[];
//void *mallocPt=Image$$RW$$Limit;


extern SYSTEMCONFIG g_SystemConf;
extern GLOBALVAR g_GlobalVar;
extern U8 g_download_Flag;


extern TIMER2_V g_Timer2Uart;
extern TIMER2_V g_Timer2Virtual[MaxTimer];
extern U8 g_UartTimeOut;


////////******************Timer0---By TJ (2015-6-24)**********************///////////////////
void Timer0_Init(void)
{
}

void Timer0_Stop(void)
{
//	rINTMSK1 |= BIT_TIMER0;
//	rTCON &= ~(1 << 0);			// Timer0, stop						
}



// ÑÓÊ±º¯Êý
void Delay(int time)
{
	for (; time > 0; time--)
		wdt_feed_dog();
	main_enter_flag = 1;
}



