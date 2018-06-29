/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: RS485双机通信--主机设备实现
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */  


/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f10x.h"

#include "includes.h"
#include "string.h"
SYSTEMCONFIG g_SystemConf;

TIMER2_V g_Timer2Uart;
TIMER2_V g_Timer2Virtual[MaxTimer];

extern unsigned int a;
extern DELAY_STRUCTURE delay_fac;

unsigned char main_enter_flag=1;
__IO uint16_t timer_count=0;


#if 0
#define FLASH_ADR 0x0807F800


//32位int 转为4个8位char
unsigned char sendint_char(unsigned int shuru,int i)
{
	unsigned char s[4];
	s[0]=(unsigned char)((a&0xff000000)>>24);
	s[1]=(unsigned char)((a&0xff0000)>>16);
	s[2]=(unsigned char)((a&0xff00)>>8);
	s[3]=(unsigned char)(a&0xff);
	return s[i];
}
/*
程序while里面，一开始通讯变量存入FLASH里面，这个是改了之后读值
？？判断一下如何不一直进入这个程序，
比如没改，就不存也就不读？？
*/
void dezhi(void);
{
	g_InteralMemory.KeepBit[0]=(*(__IO uint32_t*)(0x0807F800));
	//bit是unsigned char，一个地址可以留多大数据，
	g_InteralMemory.KeepBit[1]=(*(__IO uint32_t*)(0x0807F801));//
	
	//0x0x0807F800+10??
	//16进制与十进制数相加得到的，最终是以2形式表示的
	g_InteralMemory.KeepBit[i]=(*(__IO uint32_t*)(base+i));//
}
#endif
/**
  * 函数功能: 主函数.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 本工程为主机，实验时先给从机上电
  */
int main(void)
{  
 // DEBUG_USART_Init();  
//	GENERAL_TIMx_Configuration();  
//	TIM_SetAutoreload(TIM6,1999);
	
	unsigned int data=0x12345678;
	unsigned char data_char[4];
	int num,num1;
 // App_Init ();
	RCC_Configuration();
	GPIO_Configuration();
	Timer0_init_quanzhou();
	RS485_USARTx_Config();
	RS232_USARTx_Config();
	
/*#if 0	
	if(1)
	{
	for(t=0;t<len;t++)
				{
					RecArray[t] = (USART_RX_BUF[t]);
				}
				
				if(RecArray[0] == '%' && RecArray[4]=='R' && RecArray[5] == 'C' &&  RecArray[6] == 'S')// && RecArray[14]=='\r' 	ReadBit_Feedback
				{//RCS读取单个触点的状态信息
					read_bit_num = (CharToData(RecArray[11])+(CharToData(RecArray[10])*10)+(CharToData(RecArray[9])*100)+(CharToData(RecArray[8])*1000));
					
					auchMsg[0]='%';
					auchMsg[1]=48;
					auchMsg[2]=49;
					auchMsg[3]='$';
					auchMsg[4]='R';
					auchMsg[5]='C';
					auchMsg[6]=DataToChar(write_bit_reg[read_bit_num]);//？？0
					auchMsg[7]=DataToChar(BCC(auchMsg,7)/16);
					auchMsg[8]=DataToChar(BCC(auchMsg,7)%16);
					auchMsg[9]='\r';
					
					for (Count=0;Count<10;Count++)
						SendArray[Count]=auchMsg[Count];
					
						for(Count=0;Count<10;Count++)
						{
							USART_SendChar(USART3, SendArray[Count]);//向串口3发送数据
							while(USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);//等待发送结束
						}
				//解锁
				FLASH_Unlock();
				//清除标志位
				FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
				//擦除页的起始地址
				FLASH_ErasePage(FLASH_ADR);
						
				//写数据
				FLASH_ProgramWord(FLASH_ADR,data);
				//48 49  R    ...R[1]		1/0
				//flash里面bit[1]的值改变了		
				FLASH_Lock();			
						
	}
	
	dezhi();  //g_InteralMemory所有的值遍历读取一次
	
	
	data=0;
	data=(*(__IO uint32_t*)(FLASH_ADR));
	
	for(num1=0;num1<4;num1++)
	{
	data_char[num1]=sendint_char(data,num1);
	}
#endif*/
	
	
	
	X1_EXIT_Config();
  /* 无限循环 */
  while (1)
  { 
//	hanshu1();
//			Set_Y_Value(2,HIGH);
//			Set_Y_Value(1,1);
 //  		Set_Y_Value(6,1);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_5);

//		if(X5)
//		{
//   jidianqi_write_single(0x02,1);
//		}
   //GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	//	GPIO_ResetBits(GPIOD,LED2_GPIO_PIN|LED3_GPIO_PIN);
		
  }
}


/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/	

