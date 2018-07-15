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
	
	/* Ƕ�������жϿ�������ѡ�� */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	PVD_Init();
	KEY_Init();
	//RS232_USARTx_Config();
	//printf("����ԭ��@ALIENTEK\r\n\r\n");
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
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);
	//�״ζ�ȡFLASH
	//��ʱÿ Nmsд��FLASH  ÿNms��ȡ 
  /* ����ѭ�� */
  while (1)
  { 
	  RS485_Receive_Data(rs485buf,&len);
		if((len)&&(rs485buf[0]==0x99))//���յ�������
		{
			if(len>9)len=9;//�����5������.
			which_kind=rs485buf[1];
			bit_num=(RecArray[2]&0x00ff)|(RecArray[3]<<8);
			word_number = (RecArray[4]&0x00ff)|(RecArray[5]<<8);
					
				switch(which_kind)
				{
					case 1:
					g_InteralMemory.Bit[bit_num]=word_number;break;//how?
					//word_number>1,�ͻ����dug���������Ū��ת��
					
					case 2:
					g_InteralMemory.Word[bit_num]=word_number;break;	
					case 3:
					g_InteralMemory.KeepBit[bit_num]=word_number;break;
					//word_number>1,�ͻ����dug���������Ū��ת��
					//д��flash
					case 4:
					g_InteralMemory.KeepWord[bit_num]=word_number;break;
					default:
						break;
				}		
					
				auchMsg[0]=0x66;//ack�ظ�66 send �ظ�99
				auchMsg[1]=rs485buf[1];
				auchMsg[2]=rs485buf[2];
				auchMsg[3]=rs485buf[3];
				auchMsg[4]=rs485buf[4];
				auchMsg[5]=rs485buf[5];
				auchMsg[6]=(CRC1(auchMsg,6)) & 0xff;
				auchMsg[7]=(CRC1(auchMsg,6))>>8;
			 	auchMsg[8]='\r';
			RS485_Send_Data(auchMsg,9);//����5���ֽ� 	

		}
		
  	key=KEY_Scan(0);
		if(key==KEY0_PRES)//KEY0����,����һ������
		{
				for(i=0;i<5;i++)
			{
				auchMsg[0]=0x99;//ack�ظ�66 send �ظ�99
				auchMsg[1]=0x02;
				auchMsg[2]=0x0c;
			
				auchMsg[3]=0x00;
				auchMsg[4]=0x0c;
				auchMsg[5]=0x00;
				auchMsg[6]=(CRC1(auchMsg,6)) & 0xff;
				auchMsg[7]=(CRC1(auchMsg,6))>>8;
 			  auchMsg[8]='\r';
			RS485_Send_Data(auchMsg,9);//����5���ֽ� 		
			}
		}		
		
	Delay(1000);
  }
}




/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/	

