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
*����MODBUS(RTU mode)ͨ�ŵ�CRC���麯��
*********************************************************************************************************
*	�� �� ��: CRC16_Modbus
*	����˵��: ����CRC�� ����ModbusЭ�顣
*	��    ��: _pBuf��auchMsg : ����У�������
*			  _usLen : ���ݳ���
*	�� �� ֵ: 16λ����ֵ�� ����Modbus ���˽�����ֽ��ȴ��ͣ����ֽں��͡�
*
*   ���п��ܵ�CRCֵ����Ԥװ���������鵱�У������㱨������ʱ���Լ򵥵��������ɣ�
*   һ�����������16λCRC�������256�����ܵĸ�λ�ֽڣ���һ�����麬�е�λ�ֽڵ�ֵ��
*   ������������CRC�ķ�ʽ�ṩ�˱ȶԱ��Ļ�������ÿһ�����ַ��������µ�CRC����ķ�����
*
*  ע�⣺�˳����ڲ�ִ�и�/��CRC�ֽڵĽ������˺������ص����Ѿ�����������CRCֵ��Ҳ����˵���ú����ķ���ֵ����ֱ�ӷ���
*        �ڱ������ڷ��ͣ�
****************************************************************
******************************************/
INT16U CRC1(U8 *auchMsg,INT8U _usLen)  
{
   INT16U j;
   INT16U reg_crc=0xFFFF;/* �ߵ�CRC �ֽڳ�ʼ�� */
   while(_usLen--)  /* CRCѭ���е����� */
   	{
   	  reg_crc^=*auchMsg++;   
			/* ����CRC */
	  for(j=0;j<8;j++)
	  	{
	  	  if(reg_crc&0x01)
		  	reg_crc=(reg_crc>>1)^0xA001;
		  else
		  	reg_crc=(reg_crc>>1);
	  	}
   	}
//  {
//  usIndex = ucCRCHi ^ *_pBuf++; /* ����CRC */
//  ucCRCHi = ucCRCLo ^ s_CRCHi[usIndex];
//  ucCRCLo = s_CRCLo[usIndex];
//  }
   return reg_crc;
}


