
/**
  ******************************************************************************
  * 文件名程: bsp_usartx.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 串口底层驱动程序
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
#include "bsp_usartx.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/

u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目

u16 USART_RX_STA=0;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 配置NVIC，设定USART接收中断优先级.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
//void NVIC_Configuration(void)
//{
//	NVIC_InitTypeDef NVIC_InitStructure; 
//	 /* 嵌套向量中断控制器组选择 */  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
//	
//   /* 配置USART为中断源 */
//	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;	 
//	 /* 抢断优先级为0 */
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	 /* 子优先级为1 */
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	 /* 使能中断 */
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	 /* 初始化配置NVIC */
//	NVIC_Init(&NVIC_InitStructure);
//}

/**
  * 函数功能: 串口参数配置.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：使用宏定义方法代替具体引脚号，方便程序移植，只要简单修改bsp_led.h
  *           文件相关宏定义就可以方便修改引脚。
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
  
	/* RS485接收发送使能GPIO */
  GPIO_InitStructure.GPIO_Pin = RS485_REDE_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS485_REDE_PORT, &GPIO_InitStructure);
	  
	/* USART工作环境配置 */
  /* USART波特率:115200 */
	USART_InitStructure.USART_BaudRate = RS485_USARTx_BAUDRATE;
	/* USART字长(有效位)：8位 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	/* USART停止位：1位 */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/* USART校验位：无 */
  USART_InitStructure.USART_Parity = USART_Parity_No ;
	/* USART硬件数据流控制(硬件信号控制传输停止)：无 */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/* USART工作模式使能：允许接收和发送 */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* 初始化USART */
  USART_Init(RS485_USARTx, &USART_InitStructure); 
  /* 使能USART */
  USART_Cmd(RS485_USARTx, ENABLE);
	
	
}

void RS232_USARTx_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	 /* 嵌套向量中断控制器组选择 */  
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
  
	/* USART工作环境配置 */
  /* USART波特率:115200 */
	USART_InitStructure.USART_BaudRate = RS232_USARTx_BAUDRATE;
	/* USART字长(有效位)：8位 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	/* USART停止位：1位 */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/* USART校验位：无 */
  USART_InitStructure.USART_Parity = USART_Parity_No ;
	/* USART硬件数据流控制(硬件信号控制传输停止)：无 */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/* USART工作模式使能：允许接收和发送 */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* 初始化USART */
  USART_Init(RS232_USARTx, &USART_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
  /* 使能USART */
  USART_Cmd(RS232_USARTx, ENABLE);
	
	 /* 清除发送完成标志 */
	USART_ClearFlag(USART1, USART_FLAG_TC|USART_FLAG_TXE|USART_FLAG_RXNE);
}


/**
  * 函数功能: 串口发送一个字符型变量
  * 输入参数:USARTx：输入串口
						 Data：待发送字符型变量
  * 返 回 值: 无
  * 说    明：无
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
  * 函数功能: 串口发送一个字符型变量
  * 输入参数: USARTx：485的串口
  * 返 回 值: 无
  * 说    明：无
  */
char USART_ReceiveChar(USART_TypeDef* USARTx1)
{
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx1));
  
  /* Receive Data */
  return (char)(USARTx1->DR & 0xFF);
}

/**
  * 函数功能: 串口发送一个字节数据 
  * 输入参数: ch：待发送字符
  * 返 回 值: 无
  * 说    明：无
  */
void Usart_SendByte(uint8_t ch)
{
  /* 发送一个字节数据到USART1 */
  USART_SendData(RS485_USARTx,ch);

  /* 等待发送完毕 */
  while (USART_GetFlagStatus(RS485_USARTx, USART_FLAG_TXE) == RESET);	
}

/**
  * 函数功能: 串口发送指定长度的字符串
  * 输入参数: str：待发送字符串缓冲器
  *           strlen:指定字符串长度
  * 返 回 值: 无
  * 说    明：无
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
  * 函数功能: 串口发送字符串，直到遇到字符串结束符
  * 输入参数: str：待发送字符串缓冲器
  * 返 回 值: 无
  * 说    明：无
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

//串口中断接收
void USART1_IRQHandler(void)                	//串口3中断服务程序 by FJW
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
		int Count_2 = 0;	//仅读取FLASH中记录的变量的时候使用
		static int read_memory_status = 0;
		u8 Res;
		static u8 Res_start = 0;

		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//清除中断标志位
		LED2_TOGGLE;
		Res =USART_ReceiveChar(USART1);	//读取接收到的数据
				if((USART_RX_STA&0x8000)== 0 && Res_start == 1)//接收未完成 	//还没收到0X0D
		{
				USART_RX_BUF[USART_RX_STA&0X3FFF]=Res;
				
				USART_RX_STA++;
				if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
		}		
		}	  
				
		if(USART_RX_STA&0x8000)
		{					   
		len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
		
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
				//word_number>1,就会出现dug，这里可以弄个转换
				
				case 2:
				g_InteralMemory.Word[bit_num]=word_number;break;	
				
				case 3:
				g_InteralMemory.KeepBit[bit_num]=word_number;break;
				//word_number>1,就会出现dug，这里可以弄个转换
				
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
			while(USART_GetFlagStatus(RS232_USARTx,USART_FLAG_TC) == RESET);//等待发送结束
			}
			USART_RX_STA=0;
						
	 }

}
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/


