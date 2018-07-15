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
	u8 t;
	u16 bit_num,word_number;
	u8 which_kind;
	int Count = 0;
	u8 RecArray[25] = {0};
	u8 auchMsg[25] = {0};
	int read_bit_num = 0;
	int write_word_num = 0;
	int write_bit_num = 0;
	int read_word_num = 0;
	
	
	u8 len,key;
	u8 i=0;
	u8 cnt=0;
	u8 rs485buf[5]; 
	
 // App_Init ();
	RCC_Configuration();
	GPIO_Configuration();
	Timer0_init_quanzhou();
	SysTick_Init();
	RS485_USARTx_Config();
	
	/* 嵌套向量中断控制器组选择 */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	PVD_Init();
	KEY_Init();
	//RS232_USARTx_Config();
	//printf("正点原子@ALIENTEK\r\n\r\n");
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
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);
	//首次读取FLASH
	//定时每 Nms写入FLASH  每Nms读取 
  /* 无限循环 */
  while (1)
  { 
	  RS485_Receive_Data(rs485buf,&len);
		if((len)&&(rs485buf[0]==0x99))//接收到有数据
		{
			if(len>9)len=9;//最大是5个数据.
			which_kind=rs485buf[1];
			bit_num=(RecArray[2]&0x00ff)|(RecArray[3]<<8);
			word_number = (RecArray[4]&0x00ff)|(RecArray[5]<<8);
					
				switch(which_kind)
				{
					case 1:
					g_InteralMemory.Bit[bit_num]=word_number;break;//how?
					//word_number>1,就会出现dug，这里可以弄个转换
					
					case 2:
					g_InteralMemory.Word[bit_num]=word_number;break;	
					case 3:
					g_InteralMemory.KeepBit[bit_num]=word_number;break;
					//word_number>1,就会出现dug，这里可以弄个转换
					//写入flash
					case 4:
					g_InteralMemory.KeepWord[bit_num]=word_number;break;
					default:
						break;
				}		
					
				auchMsg[0]=0x66;//ack回复66 send 回复99
				auchMsg[1]=rs485buf[1];
				auchMsg[2]=rs485buf[2];
				auchMsg[3]=rs485buf[3];
				auchMsg[4]=rs485buf[4];
				auchMsg[5]=rs485buf[5];
				auchMsg[6]=(CRC1(auchMsg,6)) & 0xff;
				auchMsg[7]=(CRC1(auchMsg,6))>>8;
			 	auchMsg[8]='\r';
			RS485_Send_Data(auchMsg,9);//发送5个字节 	

		}
		
  	key=KEY_Scan(0);
		if(key==KEY0_PRES)//KEY0按下,发送一次数据
		{
				for(i=0;i<5;i++)
			{
				auchMsg[0]=0x99;//ack回复66 send 回复99
				auchMsg[1]=0x02;
				auchMsg[2]=0x0c;
			
				auchMsg[3]=0x00;
				auchMsg[4]=0x0c;
				auchMsg[5]=0x00;
				auchMsg[6]=(CRC1(auchMsg,6)) & 0xff;
				auchMsg[7]=(CRC1(auchMsg,6))>>8;
 			  auchMsg[8]='\r';
			RS485_Send_Data(auchMsg,9);//发送5个字节 		
			}
		}		
		
	Delay(1000);
  }
}




/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/	

