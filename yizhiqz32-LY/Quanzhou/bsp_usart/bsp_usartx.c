
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

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/

u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ

u16 USART_RX_STA=0;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ����NVIC���趨USART�����ж����ȼ�.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
//void NVIC_Configuration(void)
//{
//	NVIC_InitTypeDef NVIC_InitStructure; 
//	 /* Ƕ�������жϿ�������ѡ�� */  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
//	
//   /* ����USARTΪ�ж�Դ */
//	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;	 
//	 /* �������ȼ�Ϊ0 */
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	 /* �����ȼ�Ϊ1 */
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	 /* ʹ���ж� */
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	 /* ��ʼ������NVIC */
//	NVIC_Init(&NVIC_InitStructure);
//}

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
	USART_InitStructure.USART_BaudRate = RS485_USARTx_BAUDRATE;
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
  /* ʹ��USART */
  USART_Cmd(RS485_USARTx, ENABLE);
	
	
}

void RS232_USARTx_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	 /* Ƕ�������жϿ�������ѡ�� */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	
	/* config RS485_USARTx clock */
	RS232_USARTx_GPIO_ClockCmd(RS232_USARTx_TX_CLK|RS232_USARTx_RX_CLK, ENABLE);
	RS232_USARTx_ClockCmd(RS232_USARTx_CLK, ENABLE);

	/* RS485_USARTx GPIO config */
   /* Configure RS485_USARTx Tx  as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = RS232_USARTx_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS232_USARTx_TX_PORT, &GPIO_InitStructure);
	    
  /* Configure RS485_USARTx Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = RS232_USARTx_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(RS232_USARTx_RX_PORT, &GPIO_InitStructure);
  
	/* USART������������ */
  /* USART������:115200 */
	USART_InitStructure.USART_BaudRate = RS232_USARTx_BAUDRATE;
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
  USART_Init(RS232_USARTx, &USART_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
  /* ʹ��USART */
  USART_Cmd(RS232_USARTx, ENABLE);
	
	 /* ���������ɱ�־ */
	USART_ClearFlag(USART1, USART_FLAG_TC|USART_FLAG_TXE|USART_FLAG_RXNE);
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
char USART_ReceiveChar(USART_TypeDef* USARTx1)
{
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx1));
  
  /* Receive Data */
  return (char)(USARTx1->DR & 0xFF);
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


//void RS485_Send_Data(u8 *buf,u8 len)
//{
//	u8 t;
////	RS485_TX_EN=1;
//	for(t=0,t<len,t++)
//	{
//	while (USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);
//		USART_SendData(USART2.buf[t]);
//	}
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
////	RS485_RX_CNT=0;
////	RS485_TX_EN=0;
//}

//�����жϽ���
void USART1_IRQHandler(void)                	//����3�жϷ������ by FJW
	{
	 	u16 t;  
		u16 len;	
		u8 which_kind;
		u16 bit_num ,word_number;
		u8 RecArray[25] = {0};
		u8 SendArray[25] = {0};
		u8 auchMsg[20] = {0};
		int read_bit_num = 0;
		int write_word_num = 0;
		int write_bit_num = 0;
		int read_word_num = 0;
	
		int Count = 0;
		int Count_2 = 0;	//����ȡFLASH�м�¼�ı�����ʱ��ʹ��
		static int read_memory_status = 0;
		u8 Res;
		static u8 Res_start = 0;

		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//����жϱ�־λ
		LED2_TOGGLE;
		Res =USART_ReceiveChar(USART1);	//��ȡ���յ�������
				if((USART_RX_STA&0x8000)== 0 && Res_start == 1)//����δ��� 	//��û�յ�0X0D
		{
				USART_RX_BUF[USART_RX_STA&0X3FFF]=Res;
				
				USART_RX_STA++;
				if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
		}		
		}	  
				
		if(USART_RX_STA&0x8000)
		{					   
		len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
		
		for(t=0;t<len;t++)
		{
			RecArray[t] = (USART_RX_BUF[t]);
		}
					
				
		which_kind=RecArray[0];
		bit_num=(RecArray[1]&0x00ff)|(RecArray[2]<<8);
		word_number = (RecArray[3]&0x00ff)|(RecArray[4]<<8);
		
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
				
				case 4:
				g_InteralMemory.KeepWord[bit_num]=word_number;break;
			}

		
			auchMsg[0]=RecArray[0];
			auchMsg[1]=RecArray[1];
			auchMsg[2]=RecArray[2];
			auchMsg[3]=RecArray[3];
			auchMsg[4]=RecArray[4];
			auchMsg[5]=0x02;//ac
			auchMsg[6]=(CRC1(auchMsg,6)) & 0xff;
			auchMsg[7]=(CRC1(auchMsg,6))>>8;

			for (Count=0;Count<8;Count++)
			{
			USART_SendChar(RS232_USARTx,auchMsg[Count]);
			while(USART_GetFlagStatus(RS232_USARTx,USART_FLAG_TC) == RESET);//�ȴ����ͽ���
			}
			USART_RX_STA=0;
						
	 }

}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/


