
/**
  ******************************************************************************
  * �ļ�����: bsp_usartx.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ���ڵײ���������
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
#include "bsp_usartx.h"
#include "includes.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/

//���ջ����� 	
u8 RS485_RX_BUF[USART_REC_LEN];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u8 RS485_RX_CNT=0;   		  
  

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ���ڲ�������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����ʹ�ú궨�巽������������źţ����������ֲ��ֻҪ���޸�bsp_led.h
  *           �ļ���غ궨��Ϳ��Է����޸����š�
  */
void RS485_USARTx_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

	/* config RS485_USARTx clock */
	RS485_USARTx_GPIO_ClockCmd(RS485_USARTx_TX_CLK|RS485_USARTx_RX_CLK, ENABLE);
	RS485_REDE_GPIO_ClockCmd(RS485_REDE_GPIO_CLK, ENABLE);
	RS485_USARTx_ClockCmd(RS485_USARTx_CLK, ENABLE);

	/* RS485_USARTx GPIO config */
   /* Configure RS485_USARTx Tx  as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = RS485_USARTx_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS485_USARTx_TX_PORT, &GPIO_InitStructure);
	    
  /* Configure RS485_USARTx Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = RS485_USARTx_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(RS485_USARTx_RX_PORT, &GPIO_InitStructure);
  
	/* RS485���շ���ʹ��GPIO */
  GPIO_InitStructure.GPIO_Pin = RS485_REDE_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS485_REDE_PORT, &GPIO_InitStructure);
	  
	/* USART������������ */
  /* USART������:115200 */
	USART_InitStructure.USART_BaudRate = 9600;
	/* USART�ֳ�(��Чλ)��8λ */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	/* USARTֹͣλ��1λ */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/* USARTУ��λ���� */
  USART_InitStructure.USART_Parity = USART_Parity_No ;
	/* USARTӲ������������(Ӳ���źſ��ƴ���ֹͣ)���� */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/* USART����ģʽʹ�ܣ�������պͷ��� */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* ��ʼ��USART */
  USART_Init(RS485_USARTx, &USART_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(RS485_USARTx,USART_IT_RXNE,ENABLE);
  /* ʹ��USART */
  USART_Cmd(RS485_USARTx, ENABLE);
	
  RS485_TX_EN=0;			//Ĭ��Ϊ����ģʽ
	
}


/**
  * ��������: ���ڷ���һ���ַ��ͱ���
  * �������:USARTx�����봮��
						 Data���������ַ��ͱ���
  * �� �� ֵ: ��
  * ˵    ������
  */
void USART_SendChar(USART_TypeDef* USART_x, char Data)
{
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_DATA(Data)); 
    
  /* Transmit Data */
  USART_x->DR = (Data & 0xFF);
}


/**
  * ��������: ���ڷ���һ���ַ��ͱ���
  * �������: USARTx��485�Ĵ���
  * �� �� ֵ: ��
  * ˵    ������
  */
unsigned char USART_ReceiveChar(USART_TypeDef* USARTx1)
{
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx1));
  
  /* Receive Data */
  return (unsigned char)(USARTx1->DR & 0xFF);
}

/**
  * ��������: ���ڷ���һ���ֽ����� 
  * �������: ch���������ַ�
  * �� �� ֵ: ��
  * ˵    ������
  */
void Usart_SendByte(uint8_t ch)
{
  /* ����һ���ֽ����ݵ�USART1 */
  USART_SendData(RS485_USARTx,ch);

  /* �ȴ�������� */
  while (USART_GetFlagStatus(RS485_USARTx, USART_FLAG_TXE) == RESET);	
}

/**
  * ��������: ���ڷ���ָ�����ȵ��ַ���
  * �������: str���������ַ���������
  *           strlen:ָ���ַ�������
  * �� �� ֵ: ��
  * ˵    ������
  */
void Usart_SendStr_length(uint8_t *str,uint32_t strlen)
{
  unsigned int k=0;
  do 
  {
    Usart_SendByte(*(str + k));
    k++;
  } while(k < strlen);
}

/**
  * ��������: ���ڷ����ַ�����ֱ�������ַ���������
  * �������: str���������ַ���������
  * �� �� ֵ: ��
  * ˵    ������
  */
void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
    Usart_SendByte(*(str + k));
    k++;
  } while(*(str + k)!='\0');
}


//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN=1;
	Delay(3000);
	for(t=0;t<len;t++)
	{
		while (USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
		USART_SendChar(USART2,buf[t]);
	//	Delay(1000);
	}
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
  Delay(7000);
	RS485_RX_CNT=0;
  RS485_TX_EN=0;
}

//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	Delay(3800);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//��¼�������ݳ���
		RS485_RX_CNT=0;		//����
	}
}

//void RS485_Receive_Data(u8 *buf,u8 *len)
//{
//	u8 rxlen=RS485_RX_CNT;
//	u8 i=0;
//	*len=0;				//Ĭ��Ϊ0
//	Delay(300);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
//	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
//	{
//		for(i=0;i<rxlen;i++)
//		{
//			buf[i]=RS485_RX_BUF[i];	
//		}		
//		*len=RS485_RX_CNT;	//��¼�������ݳ���
//		RS485_RX_CNT=0;		//����
//	}
//}

//void USART2_IRQHandler(void)
//{
//	u8 temp=0;
//	u8 res;
//	u16 len,bit_num,word_number;
//  u8 RecArray[64];
//	u8 t,Count;
//	u8 which_kind;
//	u8 SendArray[25] = {0};
//	u8 auchMsg[20] = {0};	
// 	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //���յ�����
//	{	 

//		temp=USART_ReceiveData(USART2);
//		
//		
//		GPIO_SetBits(GPIOD,GPIO_Pin_7);
//		Delay_ms(1);

//		USART_SendData(USART2,temp);
//		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//		Delay_ms(2);

//		GPIO_ResetBits(GPIOD,GPIO_Pin_7);
//		
////		temp=USART_ReceiveChar(USART2);
////		
////		GPIO_SetBits(GPIOD,GPIO_Pin_7);
////		Delay(100);

////		USART_SendChar(USART2,temp);
////		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
////		Delay(100);

////		GPIO_ResetBits(GPIOD,GPIO_Pin_7);
//		
////		res =USART_ReceiveChar(USART2); 	//��ȡ���յ�������
////		RS485_TX_EN=1;
////		Delay(100);
////		USART_SendChar(USART2,res);
////		while (USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
////		Delay(200);
////		RS485_TX_EN=0;
//		
////		if((RS485_RX_CNT&0x8000)==0)//����δ���
////			{
////			if(RS485_RX_CNT&0x4000)//���յ���0x0d
////				{
////				if(res!=0x0a)RS485_RX_CNT=0;//���մ���,���¿�ʼ
////				else RS485_RX_CNT|=0x8000;	//��������� 
////				}
////			else //��û�յ�0X0D
////				{	
////				if(res==0x0d)RS485_RX_CNT|=0x4000;
////				else
////					{
////					RS485_RX_BUF[RS485_RX_CNT&0X3FFF]=res ;	
////					RS485_RX_CNT++;
////					}
////				}
////			}
////			RS485_TX_EN=1;
////			Delay(200);
////			
////			USART_SendChar(USART2,0x01);
////			while (USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
////			RS485_TX_EN=0;
////			
////		if(RS485_RX_CNT&0x8000){
////		len=RS485_RX_CNT&0x3fff;
////			for(t=0;t<len;t++)
////			{
////				RecArray[t] = (RS485_RX_BUF[t]);
////			}
////		
////		RS485_TX_EN=1;
////		Delay(200);
////		for (Count=0;Count<len;Count++)
////		{
////		USART_SendData(USART2,res);
////		while (USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
////		}
////		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		
////		RS485_RX_CNT=0;	  
////		RS485_TX_EN=0;				//����Ϊ����ģʽ
////	}
//	USART_ClearFlag(USART2,USART_IT_RXNE);
//	}  
//	
//} 

void USART2_IRQHandler(void)
{

	u8 temp=0;
	u8 res,Res;
	u16 bit_num,word_number;
  u8 RecArray[64];
	u8 Count;
	u8 which_kind;
	u8 SendArray[25] = {0};
	u8 auchMsg[20] = {0};	
	u8 rs485buf[8]; 
   if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
   {
		 res =USART_ReceiveData(USART2); 	//��ȡ���յ�������
		if(RS485_RX_CNT<64)
		{
			RS485_RX_BUF[RS485_RX_CNT]=res;		//��¼���յ���ֵ
			RS485_RX_CNT++;						//������������1 
		} 	 
		

//   		temp=USART_ReceiveData(USART2);
//		 if((RS485_RX_CNT&0x8000)==0)//����δ���
//			{
//			if(RS485_RX_CNT&0x4000)//���յ���0x0d
//				{
//				if(temp!=0x0a)RS485_RX_CNT=0;//���մ���,���¿�ʼ
//				else 
//				{RS485_RX_CNT|=0x8000;	//��������� 
//				GPIO_SetBits(GPIOD,GPIO_Pin_7);
//				Delay(3500);
//				res=(RS485_RX_CNT>>8);
//				USART_SendData(USART2,res);
//				while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//				Delay(7500);
//				GPIO_ResetBits(GPIOD,GPIO_Pin_7);
//				}
//				}
//			else //��û�յ�0X0D
//				{	
//				if(temp==0x0d)
//				{RS485_RX_CNT|=0x4000;
//				GPIO_SetBits(GPIOD,GPIO_Pin_7);
//				Delay(3500);
//				res=(RS485_RX_CNT>>8);
//				USART_SendData(USART2,res);
//				while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//				Delay(7500);
//				GPIO_ResetBits(GPIOD,GPIO_Pin_7);
//					
//				}
//					else
//					{
//					RS485_RX_BUF[RS485_RX_CNT&0X3FFF]=temp ;
//					RS485_RX_CNT++;
//					if(RS485_RX_CNT>(USART_REC_LEN-1))RS485_RX_CNT=0;//�������ݴ���,���¿�ʼ����	  
//					}		 
//				}
//			}   

//		if(RS485_RX_CNT==0x8000){
//		LED2_TOGGLE;		
//		GPIO_SetBits(GPIOD,GPIO_Pin_7);
//		Delay(3500);

//		USART_SendData(USART2,0x01);
//		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//		Delay(7000);

//		GPIO_ResetBits(GPIOD,GPIO_Pin_7);
			
//		}
//			if((RS485_RX_CNT&0x8000)==0)//����δ���
//			{
//			if(RS485_RX_CNT&0x4000)//���յ���0x0d
//				{
//				if(temp!=0x0a)RS485_RX_CNT=0;//���մ���,���¿�ʼ
//				else 
//				{RS485_RX_CNT|=0x8000;	//��������� 
//					
//				GPIO_SetBits(GPIOD,GPIO_Pin_7);
//			Delay(3500);

//			USART_SendData(USART2,0x0a);
//			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//			Delay(7000);

//			GPIO_ResetBits(GPIOD,GPIO_Pin_7);
//				}
//				}
//			else //��û�յ�0X0D
//				{	
//				if(temp==0x0d)
//				{
//					RS485_RX_CNT|=0x4000;
//						GPIO_SetBits(GPIOD,GPIO_Pin_7);
//		Delay(3500);
//			res=(RS485_RX_CNT>>8);
//		USART_SendData(USART2,res);
//		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//		Delay(3500);

//		GPIO_ResetBits(GPIOD,GPIO_Pin_7);
//				}else
//					{
//					RS485_RX_BUF[RS485_RX_CNT&0X3FFF]=temp ;	
//					RS485_RX_CNT++;
//					}
//				}
//			}
			
//		if(RS485_RX_CNT&0x8000){
//		  len=RS485_RX_CNT&0x3fff;
//			for(t=0;t<len;t++)
//			{
//				RecArray[t] = (RS485_RX_BUF[t]);
//			}
//		
//		GPIO_SetBits(GPIOD,GPIO_Pin_7);
//		Delay(7000);
//		for (Count=0;Count<len;Count++)
//		{
//		USART_SendData(USART2,RecArray[Count]);
//		while (USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
//		}
//		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		
//		Delay(7000);
//		RS485_RX_CNT=0;	  
//		GPIO_ResetBits(GPIOD,GPIO_Pin_7);			//����Ϊ����ģʽ
//	  Delay(7000);
//		}
	//USART_ClearFlag(USART2,USART_IT_RXNE);	
	
//		if(temp ==0x0d)
//	  RS485_RX_CNT=0x800;
//	  else 
//		{
//		RS485_RX_CNT++;
//			LED2_TOGGLE;
//			
//		RS485_RX_BUF[RS485_RX_CNT]=temp;
//		}
//		
//		if(RS485_RX_CNT==0x800)
//		{
//		for(Count=0;Count<RS485_RX_CNT;Count++)
//		{
//		GPIO_SetBits(GPIOD,GPIO_Pin_7);
//		 Delay(1000);

//		USART_SendData(USART2,RS485_RX_BUF[RS485_RX_CNT]);
//		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//		Delay(7500);

//		GPIO_ResetBits(GPIOD,GPIO_Pin_7);
//		}
//		}
//		
//	 
		USART_ClearFlag(USART2,USART_IT_RXNE);
	 }
}

void send(U8 which_kind,U16 which_port)
{
	U8 auchMsg[8];  
	U8 Count,jdqCheck;
	
	auchMsg[0]=0x01;
	auchMsg[1]=which_kind;// B W (KB KW) // 1234 �����255
	
	auchMsg[2]=which_port&0x00ff;
	auchMsg[3]=which_port>>8;		//3492~3491  2016.1.3 quanzhou 
	//ID//����265��Ҳ��Ҫ����8λ
	
		switch(which_kind)
	{
		case 1:
	  auchMsg[4]=0x00;
	  auchMsg[5]=g_InteralMemory.Bit[which_port];
		break;
		case 2:
		auchMsg[4]=g_InteralMemory.Word[which_port]&0x00ff;	//ȡ�Ұ�λ
		auchMsg[5]=g_InteralMemory.Word[which_port]>>8;//ȡ��8λ ��8λ
		break;
		case 3:
		auchMsg[4]=0x00;
		auchMsg[5]=g_InteralMemory.KeepBit[which_port];
		case 4:
		auchMsg[4]=g_InteralMemory.KeepWord[which_port]&0x00ff;	
		auchMsg[5]=g_InteralMemory.KeepWord[which_port]>>8;break;
		default:
			break;
	}
	auchMsg[6]=(CRC1(auchMsg,6)) & 0xff;
	auchMsg[7]=(CRC1(auchMsg,6))>>8;
	
	RS485_Send_Data(auchMsg,8);

}
////////////////////////////////////////////////////////////////////

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#if 1
//#pragma import(__use_no_semihosting)             
////��׼����Ҫ��֧�ֺ���                 
//struct __FILE 
//{ 
//	int handle; 
//	/* Whatever you require here. If the only file you are using is */ 
//	/* standard output using printf() for debugging, no file handling */ 
//	/* is required. */ 
//}; 
///* FILE is typedef�� d in stdio.h. */ 
//FILE __stdout;       
////����_sys_exit()�Ա���ʹ�ð�����ģʽ    
//void _sys_exit(int x) 
//{ 
//	x = x; 
//} 
////�ض���fputc���� 

//int fputc(int ch, FILE *f)
//{      

//	while((USART2->SR&0X40)==0);
//	USART2->DR = (u8) ch;      
//  return ch;
//}
//#endif 

int fputc(int ch, FILE *f)
{
		GPIO_SetBits(GPIOD,GPIO_Pin_7);
		Delay_ms(1);
	USART_SendData(USART2, (uint8_t) ch);

	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) {}	
   GPIO_ResetBits(GPIOD,GPIO_Pin_7);
		Delay_ms(2);
    return ch;
}
int GetKey (void)  { 

    while (!(USART2->SR & USART_FLAG_RXNE));

    return ((int)(USART2->DR & 0x1FF));
}

/************ (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/


