/**
  ******************************************************************************
  * 文件名程: bsp_usartx_fifo.c 
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
#include "bsp_usartx_fifo.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有变量 ------------------------------------------------------------------*/
/* 定义每个串口结构体变量 */
#if USART1_FIFO_EN == 1
	static USART_FIFO_Typedef g_tUsart1;
	static uint8_t g_TxBuf1[USART1_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf1[USART1_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if USART2_FIFO_EN == 1
	static USART_FIFO_Typedef g_tUsart2;
	static uint8_t g_TxBuf2[USART2_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf2[USART2_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

#if USART3_FIFO_EN == 1
	static USART_FIFO_Typedef g_tUsart3;
	static uint8_t g_TxBuf3[USART3_TX_BUF_SIZE];		/* 发送缓冲区 */
	static uint8_t g_RxBuf3[USART3_RX_BUF_SIZE];		/* 接收缓冲区 */
#endif

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
void RS485_SendOver(void);
void RS485_SendBefor(void);
void RS485_ReciveNew(uint8_t _byte);

/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 初始化串口相关的变量
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
static void UsartVarInit(void)
{
#if USART1_FIFO_EN == 1
	g_tUsart1.usart = USART1;						        /* STM32 串口设备 */
	g_tUsart1.pTxBuf = g_TxBuf1;					        /* 发送缓冲区指针 */
	g_tUsart1.pRxBuf = g_RxBuf1;					        /* 接收缓冲区指针 */
	g_tUsart1.usTxBufSize = USART1_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUsart1.usRxBufSize = USART1_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUsart1.usTxWrite = 0;			          			/* 发送FIFO写索引 */
	g_tUsart1.usTxRead = 0;						          /* 发送FIFO读索引 */
	g_tUsart1.usRxWrite = 0;						          /* 接收FIFO写索引 */
	g_tUsart1.usRxRead = 0;						          /* 接收FIFO读索引 */
	g_tUsart1.usRxCount = 0;						          /* 接收到的新数据个数 */
	g_tUsart1.usTxCount = 0;						          /* 待发送的数据个数 */
	g_tUsart1.SendBefor = 0;						          /* 发送数据前的回调函数 */
	g_tUsart1.SendOver = 0;						          /* 发送完毕后的回调函数 */
	g_tUsart1.ReciveNew = 0;						          /* 接收到新数据后的回调函数 */
#endif
  
#if USART2_FIFO_EN == 1
	g_tUsart2.usart = USART2;						        /* STM32 串口设备 */
	g_tUsart2.pTxBuf = g_TxBuf2;					        /* 发送缓冲区指针 */
	g_tUsart2.pRxBuf = g_RxBuf2;					        /* 接收缓冲区指针 */
	g_tUsart2.usTxBufSize = USART2_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUsart2.usRxBufSize = USART2_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUsart2.usTxWrite = 0;			          			/* 发送FIFO写索引 */
	g_tUsart2.usTxRead = 0;						          /* 发送FIFO读索引 */
	g_tUsart2.usRxWrite = 0;						          /* 接收FIFO写索引 */
	g_tUsart2.usRxRead = 0;						          /* 接收FIFO读索引 */
	g_tUsart2.usRxCount = 0;						          /* 接收到的新数据个数 */
	g_tUsart2.usTxCount = 0;						          /* 待发送的数据个数 */
	g_tUsart2.SendBefor = 0;						          /* 发送数据前的回调函数 */
	g_tUsart2.SendOver = 0;						          /* 发送完毕后的回调函数 */
	g_tUsart2.ReciveNew = 0;						          /* 接收到新数据后的回调函数 */
#endif

#if USART3_FIFO_EN == 1
	g_tUsart3.usart = USART3;						        /* STM32 串口设备 */
	g_tUsart3.pTxBuf = g_TxBuf3;					        /* 发送缓冲区指针 */
	g_tUsart3.pRxBuf = g_RxBuf3;					        /* 接收缓冲区指针 */
	g_tUsart3.usTxBufSize = USART3_TX_BUF_SIZE;	/* 发送缓冲区大小 */
	g_tUsart3.usRxBufSize = USART3_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUsart3.usTxWrite = 0;			          			/* 发送FIFO写索引 */
	g_tUsart3.usTxRead = 0;						          /* 发送FIFO读索引 */
	g_tUsart3.usRxWrite = 0;						          /* 接收FIFO写索引 */
	g_tUsart3.usRxRead = 0;						          /* 接收FIFO读索引 */
	g_tUsart3.usRxCount = 0;						          /* 接收到的新数据个数 */
	g_tUsart3.usTxCount = 0;						          /* 待发送的数据个数 */
	g_tUsart3.SendBefor = RS485_SendBefor;						          /* 发送数据前的回调函数 */
	g_tUsart3.SendOver = RS485_SendOver;						          /* 发送完毕后的回调函数 */
	g_tUsart3.ReciveNew = RS485_ReciveNew;						          /* 接收到新数据后的回调函数 */
#endif

}

/*
*********************************************************************************************************
*	函 数 名: InitHardUsart
*	功能说明: 配置串口的硬件参数（波特率，数据位，停止位，起始位，校验位，中断使能）适合于STM32-F4开发板
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitHardUsart(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

#if USART1_FIFO_EN == 1		/* 串口1 TX = PA9   RX = PA10 */

	/* 第1步：打开GPIO和USART部件的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* 第2步：将USART Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 第3步：将USART Rx的GPIO配置为浮空输入模式
		由于CPU复位后，GPIO缺省都是浮空输入模式，因此下面这个步骤不是必须的
		但是，我还是建议加上便于阅读，并且防止其它地方修改了这个口线的设置参数
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 第4步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = USART1_BAUD;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(USART1, ENABLE);		/* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
#endif

#if USART2_FIFO_EN == 1		/* 串口2 TX = PA2， RX = PA3  */
	/* 第1步：打开GPIO和USART部件的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* 第2步：将USART Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 第3步：将USART Rx的GPIO配置为浮空输入模式
		由于CPU复位后，GPIO缺省都是浮空输入模式，因此下面这个步骤不是必须的
		但是，我还是建议加上便于阅读，并且防止其它地方修改了这个口线的设置参数
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*  第3步已经做了，因此这步可以不做
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 第4步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = USART2_BAUD;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		/* 仅选择接收模式 */
	USART_Init(USART2, &USART_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(USART2, ENABLE);		/* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART2, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
#endif

#if USART3_FIFO_EN == 1			/* 串口3 TX = PB10   RX = PB11 */
	/* 第1步： 开启GPIO和UART时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* 第2步：将USART Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* 第3步：将USART Rx的GPIO配置为浮空输入模式
		由于CPU复位后，GPIO缺省都是浮空输入模式，因此下面这个步骤不是必须的
		但是，我还是建议加上便于阅读，并且防止其它地方修改了这个口线的设置参数
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*  第3步已经做了，因此这步可以不做
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* 第4步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = USART3_BAUD;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	/* 使能接收中断 */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		注意: 不要在此处打开发送中断
		发送中断使能在SendUart()函数打开
	*/
	USART_Cmd(USART3, ENABLE);		/* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART3, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */
#endif

}

/**
  * 函数功能: 配置NVIC，设定USART接收中断优先级.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
static void NVIC_Configuration_USART(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	 /* 嵌套向量中断控制器组选择 */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
#if USART1_FIFO_EN == 1
	/* 使能串口1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#if USART2_FIFO_EN == 1
	/* 使能串口2中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#if USART3_FIFO_EN == 1
	/* 使能串口3中断t */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

}

/*
*********************************************************************************************************
*	函 数 名: RS485_InitTXE
*	功能说明: 配置RS485发送使能口线 TXE
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void RS485_InitTXEN(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RS485_TXEN_RCC_CLOCKGPIO, ENABLE);	/* 打开GPIO时钟 */

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* 推挽输出模式 */
	GPIO_InitStructure.GPIO_Pin = RS485_TXEN_GPIO_PIN;
	GPIO_Init(RS485_TXEN_GPIO, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: RS485_SendBefor
*	功能说明: 发送数据前的准备工作。对于RS485通信，请设置RS485芯片为发送状态，
*			  并修改 UartVarInit()中的函数指针等于本函数名，比如 g_tUart2.SendBefor = RS485_SendBefor
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_SendBefor(void)
{
	RS485_TX_EN();	/* 切换RS485收发芯片为发送模式 */
}

/*
*********************************************************************************************************
*	函 数 名: RS485_SendOver
*	功能说明: 发送一串数据结束后的善后处理。对于RS485通信，请设置RS485芯片为接收状态，
*			  并修改 UartVarInit()中的函数指针等于本函数名，比如 g_tUart2.SendOver = RS485_SendOver
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_SendOver(void)
{
	RS485_RX_EN();	/* 切换RS485收发芯片为接收模式 */
}


/*
*********************************************************************************************************
*	函 数 名: RS485_SendBuf
*	功能说明: 通过RS485芯片发送一串数据。注意，本函数不等待发送完毕。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_SendBuf(uint8_t *_ucaBuf, uint16_t _usLen)
{
	comSendBuf(COM3, _ucaBuf, _usLen);
}

/*
*********************************************************************************************************
*	函 数 名: RS485_ReciveNew
*	功能说明: 接收到新的数据
*	形    参: _byte 接收到的新数据
*	返 回 值: 无
*********************************************************************************************************
*/
extern void MODH_ReciveNew(uint8_t _data);
void RS485_ReciveNew(uint8_t _byte)
{
	MODH_ReciveNew(_byte);
}


/**
  * 函数功能: 串口参数配置.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void Usart_FIFO_Init(void)
{  
  /* 初始化串口相关的变量 */
  UsartVarInit();
  
  RS485_InitTXEN();
  
  /* 配置NVIC，设定USART接收中断优先级 */
  NVIC_Configuration_USART();
  
  /* 初始化USART对应GPIO和USART外设 */
  InitHardUsart();  
}

/*
*********************************************************************************************************
*	函 数 名: ComToUsart
*	功能说明: 将COM端口号转换为UART指针
*	形    参: _ucPort: 端口号(COM1 - COM6)
*	返 回 值: uart指针
*********************************************************************************************************
*/
USART_FIFO_Typedef *ComToUsart(COM_PORT_E _ucPort)
{
	if (_ucPort == COM1)
	{
#if USART1_FIFO_EN == 1
			return &g_tUsart1;
#else
			return 0;
#endif
	}
	else if (_ucPort == COM2)
	{
#if USART2_FIFO_EN == 1
			return &g_tUsart2;
#else
			return 0;
#endif
	}
	else if (_ucPort == COM3)
	{
#if USART3_FIFO_EN == 1
			return &g_tUsart3;
#else
			return 0;
#endif
	}
	else
	{
		/* 不做任何处理 */
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: UsartGetChar
*	功能说明: 从串口接收缓冲区读取1字节数据 （用于主程序调用）
*	形    参: _pUsart : 串口设备
*			  _pByte : 存放读取数据的指针
*	返 回 值: 0 表示无数据  1表示读取到数据
*********************************************************************************************************
*/
static uint8_t UsartGetChar(USART_FIFO_Typedef *_pUsart, uint8_t *_pByte)
{
	uint16_t usCount;

	/* usRxWrite 变量在中断函数中被改写，主程序读取该变量时，必须进行临界区保护 */
	DISABLE_INT();
	usCount = _pUsart->usRxCount;
	ENABLE_INT();

	if (usCount == 0)	/* 已经没有数据 */
	{
		return 0;
	}
	else
	{
		*_pByte = _pUsart->pRxBuf[_pUsart->usRxRead];		/* 从串口接收FIFO取1个数据 */

		/* 改写FIFO读索引 */
		DISABLE_INT();
		if (++_pUsart->usRxRead >= _pUsart->usRxBufSize)
		{
			_pUsart->usRxRead = 0;
		}
		_pUsart->usRxCount--;
		ENABLE_INT();
		return 1;
	}
}

/*
*********************************************************************************************************
*	函 数 名: UartSend
*	功能说明: 填写数据到UART发送缓冲区,并启动发送中断。中断处理函数发送完毕后，自动关闭发送中断
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void UsartSend(USART_FIFO_Typedef *_pUsart, uint8_t *_ucaBuf, uint16_t _usLen)
{
	uint16_t i;

	for (i = 0; i < _usLen; i++)
	{
		/* 如果发送缓冲区已经满了，则等待缓冲区空 */
		/* 当 _pUsart->usTxBufSize == 1 时, 下面的函数会死掉(待完善) */
		while (1)
		{
			__IO uint16_t usCount;

			DISABLE_INT();
			usCount = _pUsart->usTxCount;
			ENABLE_INT();

			if (usCount < _pUsart->usTxBufSize)
			{
				break;
			}
		}
		/* 将新数据填入发送缓冲区 */
		_pUsart->pTxBuf[_pUsart->usTxWrite] = _ucaBuf[i];

		DISABLE_INT();
		if (++_pUsart->usTxWrite >= _pUsart->usTxBufSize)
		{
			_pUsart->usTxWrite = 0;
		}
		_pUsart->usTxCount++;
		ENABLE_INT();
	}
	USART_ITConfig(_pUsart->usart, USART_IT_TXE, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: comSendBuf
*	功能说明: 向串口发送一组数据。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
*	形    参: _ucPort: 端口号(COM1 - COM3)
*			  _ucaBuf: 待发送的数据缓冲区
*			  _usLen : 数据长度
*	返 回 值: 无
*********************************************************************************************************
*/
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen)
{
	USART_FIFO_Typedef *pUsart;

	pUsart = ComToUsart(_ucPort);
	if (pUsart == 0)
	{
		return;
	}
	if (pUsart->SendBefor != 0)
	{
		pUsart->SendBefor();
	}
	UsartSend(pUsart, _ucaBuf, _usLen);
}

/*
*********************************************************************************************************
*	函 数 名: comSendChar
*	功能说明: 向串口发送1个字节。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
*	形    参: _ucPort: 端口号(COM1 - COM3)
*			  _ucByte: 待发送的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte)
{
	comSendBuf(_ucPort, &_ucByte, 1);
}

/*
*********************************************************************************************************
*	函 数 名: comGetChar
*	功能说明: 从串口缓冲区读取1字节，非阻塞。无论有无数据均立即返回
*	形    参: _ucPort: 端口号(COM1 - COM3)
*			  _pByte: 接收到的数据存放在这个地址
*	返 回 值: 0 表示无数据, 1 表示读取到有效字节
*********************************************************************************************************
*/
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte)
{
	USART_FIFO_Typedef *pUsart;

	pUsart = ComToUsart(_ucPort);
	if (pUsart == 0)
	{
		return 0;
	}
	return UsartGetChar(pUsart, _pByte);
}

/*
*********************************************************************************************************
*	函 数 名: comClearTxFifo
*	功能说明: 清零串口发送缓冲区
*	形    参: _ucPort: 端口号(COM1 - COM3)
*	返 回 值: 无
*********************************************************************************************************
*/
void comClearTxFifo(COM_PORT_E _ucPort)
{
	USART_FIFO_Typedef *pUsart;

	pUsart = ComToUsart(_ucPort);
	if (pUsart == 0)
	{
		return;
	}
	pUsart->usTxWrite = 0;
	pUsart->usTxRead = 0;
	pUsart->usTxCount = 0;
}

/*
*********************************************************************************************************
*	函 数 名: comClearRxFifo
*	功能说明: 清零串口接收缓冲区
*	形    参: _ucPort: 端口号(COM1 - COM3)
*	返 回 值: 无
*********************************************************************************************************
*/
void comClearRxFifo(COM_PORT_E _ucPort)
{
	USART_FIFO_Typedef *pUsart;

	pUsart = ComToUsart(_ucPort);
	if (pUsart == 0)
	{
		return;
	}
	pUsart->usRxWrite = 0;
	pUsart->usRxRead = 0;
	pUsart->usRxCount = 0;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SetUart1Baud
*	功能说明: 修改UART1波特率
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetUsart1Baud(uint32_t _baud)
{
	USART_InitTypeDef USART_InitStructure;

	/* 第2步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = _baud;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SetUart2Baud
*	功能说明: 修改UART2波特率
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetUsart2Baud(uint32_t _baud)
{
	USART_InitTypeDef USART_InitStructure;

	/* 第2步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = _baud;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_Set485Baud
*	功能说明: 修改UART3波特率
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetUsart3Baud(uint32_t _baud)
{
	USART_InitTypeDef USART_InitStructure;

	/* 第2步： 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = _baud;	/* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
}



/*
*********************************************************************************************************
*	函 数 名: UsartIRQ
*	功能说明: 供中断服务程序调用，通用串口中断处理函数
*	形    参: _pUsart : 串口设备
*	返 回 值: 无
*********************************************************************************************************
*/
static void UsartIRQ(USART_FIFO_Typedef *_pUsart)
{
	/* 处理接收中断  */
	if (USART_GetITStatus(_pUsart->usart, USART_IT_RXNE) != RESET)
	{
		/* 从串口接收数据寄存器读取数据存放到接收FIFO */
		uint8_t ch;

		ch = USART_ReceiveData(_pUsart->usart);
		_pUsart->pRxBuf[_pUsart->usRxWrite] = ch;
		if (++_pUsart->usRxWrite >= _pUsart->usRxBufSize)
		{
			_pUsart->usRxWrite = 0;
		}
		if (_pUsart->usRxCount < _pUsart->usRxBufSize)
		{
			_pUsart->usRxCount++;
		}
		/* 回调函数,通知应用程序收到新数据,一般是发送1个消息或者设置一个标记 */
    if (_pUsart->ReciveNew)
    {
      _pUsart->ReciveNew(ch);
    }
	}

	/* 处理发送缓冲区空中断 */
	if (USART_GetITStatus(_pUsart->usart, USART_IT_TXE) != RESET)
	{
		if (_pUsart->usTxCount == 0)
		{
			/* 发送缓冲区的数据已取完时， 禁止发送缓冲区空中断 （注意：此时最后1个数据还未真正发送完毕）*/
			USART_ITConfig(_pUsart->usart, USART_IT_TXE, DISABLE);

			/* 使能数据发送完毕中断 */
			USART_ITConfig(_pUsart->usart, USART_IT_TC, ENABLE);
		}
		else
		{
			/* 从发送FIFO取1个字节写入串口发送数据寄存器 */
			USART_SendData(_pUsart->usart, _pUsart->pTxBuf[_pUsart->usTxRead]);
			if (++_pUsart->usTxRead >= _pUsart->usTxBufSize)
			{
				_pUsart->usTxRead = 0;
			}
			_pUsart->usTxCount--;
		}

	}
	/* 数据bit位全部发送完毕的中断 */
	else if (USART_GetITStatus(_pUsart->usart, USART_IT_TC) != RESET)
	{
		if (_pUsart->usTxCount == 0)
		{
			/* 如果发送FIFO的数据全部发送完毕，禁止数据发送完毕中断 */
			USART_ITConfig(_pUsart->usart, USART_IT_TC, DISABLE);

			/* 回调函数, 一般用来处理RS485通信，将RS485芯片设置为接收模式，避免抢占总线 */
			if (_pUsart->SendOver)
			{
				_pUsart->SendOver();
			}
		}
		else
		{
			/* 正常情况下，不会进入此分支 */
			/* 如果发送FIFO的数据还未完毕，则从发送FIFO取1个数据写入发送数据寄存器 */
			USART_SendData(_pUsart->usart, _pUsart->pTxBuf[_pUsart->usTxRead]);
			if (++_pUsart->usTxRead >= _pUsart->usTxBufSize)
			{
				_pUsart->usTxRead = 0;
			}
			_pUsart->usTxCount--;
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler
*	功能说明: USART中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#if USART1_FIFO_EN == 1
void USART1_IRQHandler(void)
{
	UsartIRQ(&g_tUsart1);
}
#endif

#if USART2_FIFO_EN == 1
void USART2_IRQHandler(void)
{
	UsartIRQ(&g_tUsart2);
}
#endif

#if USART3_FIFO_EN == 1
void USART3_IRQHandler(void)
{
	UsartIRQ(&g_tUsart3);
}
#endif

/*
*********************************************************************************************************
*	函 数 名: fputc
*	功能说明: 重定义putc函数，这样可以使用printf函数从串口1打印输出
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
#if 1	/* 将需要printf的字符通过串口中断FIFO发送出去，printf函数会立即返回 */
	comSendChar(COM1, ch);
	return ch;
  
#else	/* 采用阻塞方式发送每个字符,等待数据发送完毕 */
	/* 写一个字节到USART1 */
	USART_SendData(USART1, (uint8_t)ch);

	/* 等待发送结束 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}
	return ch;
#endif
}

/*
*********************************************************************************************************
*	函 数 名: fgetc
*	功能说明: 重定义getc函数，这样可以使用getchar函数从串口1输入数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fgetc(FILE *f)
{
#if 1	/* 从串口接收FIFO中取1个数据, 只有取到数据才返回 */
	uint8_t ucData;
	while(comGetChar(COM1, &ucData) == 0);
	return ucData;
  
#else
	/* 等待串口1输入数据 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USART1);
#endif
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
