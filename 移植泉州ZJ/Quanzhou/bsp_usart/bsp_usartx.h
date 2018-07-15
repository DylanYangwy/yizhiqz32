#ifndef __BSP_USARTX_H__
#define __BSP_USARTX_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include <stm32f10x.h>
#include "stdio.h"

#include "includes.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
//#define USARTx_BAUDRATE                        115200

//#define USARTx_ClockCmd                        RCC_APB1PeriphClockCmd
//#define USARTx_CLK                             RCC_APB1Periph_UART5

//#define USARTx_GPIO_ClockCmd                   RCC_APB2PeriphClockCmd
//    
//#define USARTx_TX_PORT                         GPIOA   
//#define USARTx_TX_PIN                          GPIO_Pin_9
//#define USARTx_TX_CLK                          RCC_APB2Periph_GPIOA 
//#define USARTx_RX_PORT                         GPIOA 
//#define USARTx_RX_PIN                          GPIO_Pin_10
//#define USARTx_RX_CLK                          RCC_APB2Periph_GPIOA

//#define USARTx_IRQHANDLER                      UART5_IRQHandler
//#define USARTx_IRQn                            UART5_IRQn
//#define USARTx                                 UART5

// 485  使能引脚GPIO RE/DE宏定义
#define RS485_REDE_GPIO_ClockCmd               RCC_APB2PeriphClockCmd
#define RS485_REDE_GPIO_CLK                    RCC_APB2Periph_GPIOD     
#define RS485_REDE_PORT                        GPIOD   
#define RS485_REDE_PIN                         GPIO_Pin_7

#define RS485_USARTx                           USART2
#define RS485_USARTx_BAUDRATE                  9600

#define RS485_USARTx_ClockCmd                  RCC_APB1PeriphClockCmd
#define RS485_USARTx_CLK                       RCC_APB1Periph_USART2 

#define RS485_USARTx_GPIO_ClockCmd             RCC_APB2PeriphClockCmd
#define RS485_USARTx_TX_PORT                   GPIOA   
#define RS485_USARTx_TX_PIN                    GPIO_Pin_2
#define RS485_USARTx_TX_CLK                    RCC_APB2Periph_GPIOA 

#define RS485_USARTx_RX_PORT                   GPIOA 
#define RS485_USARTx_RX_PIN                    GPIO_Pin_3
#define RS485_USARTx_RX_CLK                    RCC_APB2Periph_GPIOA

#define RS485_USARTx_IRQHANDLER                      USART2_IRQHandler
#define RS_485USARTx_IRQn                            USART2_IRQn

// 232  使能引脚GPIO 宏定义


#define RS232_USARTx                           USART3
#define RS232_USARTx_BAUDRATE                  9600

#define RS232_USARTx_ClockCmd                  RCC_APB1PeriphClockCmd
#define RS232_USARTx_CLK                       RCC_APB1Periph_USART3


#define RS232_USARTx_GPIO_ClockCmd             RCC_APB2PeriphClockCmd
#define RS232_USARTx_TX_PORT                   GPIOB   
#define RS232_USARTx_TX_PIN                    GPIO_Pin_10
#define RS232_USARTx_TX_CLK                    RCC_APB2Periph_GPIOB 

#define RS232_USARTx_RX_PORT                   GPIOB 
#define RS232_USARTx_RX_PIN                    GPIO_Pin_11
#define RS232_USARTx_RX_CLK                    RCC_APB2Periph_GPIOB


/* 扩展变量 ------------------------------------------------------------------*/
#define USART_REC_LEN  		200  	//定义最大接收字节数 200
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

extern u8 RS485_RX_BUF[USART_REC_LEN]; 		//接收缓冲,最大64个字节
extern u8 RS485_RX_CNT;   			//接收到的数据长度

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C


//模式控制
#define RS485_TX_EN		 BIT_ADDR(GPIOD_ODR_Addr,7)	//485模式控制.0,接收;1,发送.
//如果想串口中断接收，请不要注释以下宏定义

/*
#define FLASH_ADR 0x0807F800
	ADDR(unsigned long addr,int num)
{
//解锁
	FLASH_Unlock();
	//清除标志位
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|
					FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	
	addr_now=FLASH_ADR+num;
	//要擦出页的起始地址
	FLASH_ErasePage(addr_now);
	//写数据
	FLASH_ProgramWord(FLASH_ADR,data);

	
	//锁定
	FLASH_Lock();
	
#define BITBAND(addr, bitnum) (addr+bitnum) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))
	//外面调用
	word[n]=BIT_ADDR(FLASH_ADR,n)
*/


/* 函数声明 ------------------------------------------------------------------*/

void RS485_USARTx_Config(void);
void RS485_Send_Data(u8 *buf,u8 len);
void RS485_Receive_Data(u8 *buf,u8 *len);



void USART_SendChar(USART_TypeDef* USART_x, char Data);
unsigned char USART_ReceiveChar(USART_TypeDef* USARTx1);
	
void Usart_SendByte(uint8_t ch);
void Usart_SendStr_length(uint8_t *str,uint32_t strlen);
void Usart_SendString(uint8_t *str);
void send(U8 which_kind,U16 which_port);


#endif  // __BSP_USARTX_H__

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/

