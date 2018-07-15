
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
#include "includes.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/

//接收缓存区 	
u8 RS485_RX_BUF[USART_REC_LEN];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u8 RS485_RX_CNT=0;   		  
  

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

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
  
	/* RS485接收发送使能GPIO */
  GPIO_InitStructure.GPIO_Pin = RS485_REDE_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS485_REDE_PORT, &GPIO_InitStructure);
	  
	/* USART工作环境配置 */
  /* USART波特率:115200 */
	USART_InitStructure.USART_BaudRate = 9600;
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
	
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(RS485_USARTx,USART_IT_RXNE,ENABLE);
  /* 使能USART */
  USART_Cmd(RS485_USARTx, ENABLE);
	
  RS485_TX_EN=0;			//默认为接收模式
	
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
unsigned char USART_ReceiveChar(USART_TypeDef* USARTx1)
{
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx1));
  
  /* Receive Data */
  return (unsigned char)(USARTx1->DR & 0xFF);
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


//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
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

//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	Delay(3800);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
	if(rxlen==RS485_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//记录本次数据长度
		RS485_RX_CNT=0;		//清零
	}
}

//void RS485_Receive_Data(u8 *buf,u8 *len)
//{
//	u8 rxlen=RS485_RX_CNT;
//	u8 i=0;
//	*len=0;				//默认为0
//	Delay(300);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
//	if(rxlen==RS485_RX_CNT&&rxlen)//接收到了数据,且接收完成了
//	{
//		for(i=0;i<rxlen;i++)
//		{
//			buf[i]=RS485_RX_BUF[i];	
//		}		
//		*len=RS485_RX_CNT;	//记录本次数据长度
//		RS485_RX_CNT=0;		//清零
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
// 	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收到数据
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
////		res =USART_ReceiveChar(USART2); 	//读取接收到的数据
////		RS485_TX_EN=1;
////		Delay(100);
////		USART_SendChar(USART2,res);
////		while (USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
////		Delay(200);
////		RS485_TX_EN=0;
//		
////		if((RS485_RX_CNT&0x8000)==0)//接收未完成
////			{
////			if(RS485_RX_CNT&0x4000)//接收到了0x0d
////				{
////				if(res!=0x0a)RS485_RX_CNT=0;//接收错误,重新开始
////				else RS485_RX_CNT|=0x8000;	//接收完成了 
////				}
////			else //还没收到0X0D
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
////		RS485_TX_EN=0;				//设置为接收模式
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
		 res =USART_ReceiveData(USART2); 	//读取接收到的数据
		if(RS485_RX_CNT<64)
		{
			RS485_RX_BUF[RS485_RX_CNT]=res;		//记录接收到的值
			RS485_RX_CNT++;						//接收数据增加1 
		} 	 
		

//   		temp=USART_ReceiveData(USART2);
//		 if((RS485_RX_CNT&0x8000)==0)//接收未完成
//			{
//			if(RS485_RX_CNT&0x4000)//接收到了0x0d
//				{
//				if(temp!=0x0a)RS485_RX_CNT=0;//接收错误,重新开始
//				else 
//				{RS485_RX_CNT|=0x8000;	//接收完成了 
//				GPIO_SetBits(GPIOD,GPIO_Pin_7);
//				Delay(3500);
//				res=(RS485_RX_CNT>>8);
//				USART_SendData(USART2,res);
//				while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//				Delay(7500);
//				GPIO_ResetBits(GPIOD,GPIO_Pin_7);
//				}
//				}
//			else //还没收到0X0D
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
//					if(RS485_RX_CNT>(USART_REC_LEN-1))RS485_RX_CNT=0;//接收数据错误,重新开始接收	  
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
//			if((RS485_RX_CNT&0x8000)==0)//接收未完成
//			{
//			if(RS485_RX_CNT&0x4000)//接收到了0x0d
//				{
//				if(temp!=0x0a)RS485_RX_CNT=0;//接收错误,重新开始
//				else 
//				{RS485_RX_CNT|=0x8000;	//接收完成了 
//					
//				GPIO_SetBits(GPIOD,GPIO_Pin_7);
//			Delay(3500);

//			USART_SendData(USART2,0x0a);
//			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//			Delay(7000);

//			GPIO_ResetBits(GPIOD,GPIO_Pin_7);
//				}
//				}
//			else //还没收到0X0D
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
//		GPIO_ResetBits(GPIOD,GPIO_Pin_7);			//设置为接收模式
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
	auchMsg[1]=which_kind;// B W (KB KW) // 1234 最高是255
	
	auchMsg[2]=which_port&0x00ff;
	auchMsg[3]=which_port>>8;		//3492~3491  2016.1.3 quanzhou 
	//ID//超过265则也需要两个8位
	
		switch(which_kind)
	{
		case 1:
	  auchMsg[4]=0x00;
	  auchMsg[5]=g_InteralMemory.Bit[which_port];
		break;
		case 2:
		auchMsg[4]=g_InteralMemory.Word[which_port]&0x00ff;	//取右八位
		auchMsg[5]=g_InteralMemory.Word[which_port]>>8;//取左8位 高8位
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

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#if 1
//#pragma import(__use_no_semihosting)             
////标准库需要的支持函数                 
//struct __FILE 
//{ 
//	int handle; 
//	/* Whatever you require here. If the only file you are using is */ 
//	/* standard output using printf() for debugging, no file handling */ 
//	/* is required. */ 
//}; 
///* FILE is typedef’ d in stdio.h. */ 
//FILE __stdout;       
////定义_sys_exit()以避免使用半主机模式    
//void _sys_exit(int x) 
//{ 
//	x = x; 
//} 
////重定义fputc函数 

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

/************ (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/


