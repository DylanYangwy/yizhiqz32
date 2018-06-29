#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//#include "2416addr.h"
//#include "2416lib.h"
//#include "..\inc\def.h"
//#include "..\inc\option.h"
//#include "..\inc\communication.h"
//#include "..\inc\systemconf.h"

//#include <stdio.h>
#include "includes.h"

extern char PlcType;
extern INTERALMEMORY g_InteralMemory;
extern int whichUart;

extern U8 com_frequence;
extern U8 g_com_Flag ;

extern TIMER2_V g_Timer2Uart;
extern TIMER2_V g_Timer2Virtual[MaxTimer];
extern U8 g_UartTimeOut;


/******************************************
*用于MODBUS(RTU mode)通信的CRC检验函数
*********************************************************************************************************
*	函 数 名: CRC16_Modbus
*	功能说明: 计算CRC。 用于Modbus协议。
*	形    参: _pBuf、auchMsg : 参与校验的数据
*			  _usLen : 数据长度
*	返 回 值: 16位整数值。 对于Modbus ，此结果高字节先传送，低字节后传送。
*
*   所有可能的CRC值都被预装在两个数组当中，当计算报文内容时可以简单的索引即可；
*   一个数组包含有16位CRC域的所有256个可能的高位字节，另一个数组含有低位字节的值；
*   这种索引访问CRC的方式提供了比对报文缓冲区的每一个新字符都计算新的CRC更快的方法；
*
*  注意：此程序内部执行高/低CRC字节的交换。此函数返回的是已经经过交换的CRC值；也就是说，该函数的返回值可以直接放置
*        于报文用于发送；
****************************************************************
******************************************/
INT16U CRC1(U8 *auchMsg,INT8U _usLen)  
{
   INT16U j;
   INT16U reg_crc=0xFFFF;/* 高低CRC 字节初始化 */
   while(_usLen--)  /* CRC循环中的索引 */
   	{
   	  reg_crc^=*auchMsg++;   
			/* 计算CRC */
	  for(j=0;j<8;j++)
	  	{
	  	  if(reg_crc&0x01)
		  	reg_crc=(reg_crc>>1)^0xA001;
		  else
		  	reg_crc=(reg_crc>>1);
	  	}
   	}
//  {
//  usIndex = ucCRCHi ^ *_pBuf++; /* 计算CRC */
//  ucCRCHi = ucCRCLo ^ s_CRCHi[usIndex];
//  ucCRCLo = s_CRCLo[usIndex];
//  }
   return reg_crc;
}


