/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: RS485˫��ͨ��--�����豸ʵ��
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */  


/* ����ͷ�ļ� ----------------------------------------------------------------*/
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


//32λint תΪ4��8λchar
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
����while���棬һ��ʼͨѶ��������FLASH���棬����Ǹ���֮���ֵ
�����ж�һ����β�һֱ�����������
����û�ģ��Ͳ���Ҳ�Ͳ�������
*/
void dezhi(void);
{
	g_InteralMemory.KeepBit[0]=(*(__IO uint32_t*)(0x0807F800));
	//bit��unsigned char��һ����ַ������������ݣ�
	g_InteralMemory.KeepBit[1]=(*(__IO uint32_t*)(0x0807F801));//
	
	//0x0x0807F800+10??
	//16������ʮ��������ӵõ��ģ���������2��ʽ��ʾ��
	g_InteralMemory.KeepBit[i]=(*(__IO uint32_t*)(base+i));//
}
#endif
/**
  * ��������: ������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ������Ϊ������ʵ��ʱ�ȸ��ӻ��ϵ�
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
				{//RCS��ȡ���������״̬��Ϣ
					read_bit_num = (CharToData(RecArray[11])+(CharToData(RecArray[10])*10)+(CharToData(RecArray[9])*100)+(CharToData(RecArray[8])*1000));
					
					auchMsg[0]='%';
					auchMsg[1]=48;
					auchMsg[2]=49;
					auchMsg[3]='$';
					auchMsg[4]='R';
					auchMsg[5]='C';
					auchMsg[6]=DataToChar(write_bit_reg[read_bit_num]);//����0
					auchMsg[7]=DataToChar(BCC(auchMsg,7)/16);
					auchMsg[8]=DataToChar(BCC(auchMsg,7)%16);
					auchMsg[9]='\r';
					
					for (Count=0;Count<10;Count++)
						SendArray[Count]=auchMsg[Count];
					
						for(Count=0;Count<10;Count++)
						{
							USART_SendChar(USART3, SendArray[Count]);//�򴮿�3��������
							while(USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);//�ȴ����ͽ���
						}
				//����
				FLASH_Unlock();
				//�����־λ
				FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
				//����ҳ����ʼ��ַ
				FLASH_ErasePage(FLASH_ADR);
						
				//д����
				FLASH_ProgramWord(FLASH_ADR,data);
				//48 49  R    ...R[1]		1/0
				//flash����bit[1]��ֵ�ı���		
				FLASH_Lock();			
						
	}
	
	dezhi();  //g_InteralMemory���е�ֵ������ȡһ��
	
	
	data=0;
	data=(*(__IO uint32_t*)(FLASH_ADR));
	
	for(num1=0;num1<4;num1++)
	{
	data_char[num1]=sendint_char(data,num1);
	}
#endif*/
	
	
	
	X1_EXIT_Config();
  /* ����ѭ�� */
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


/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/	

