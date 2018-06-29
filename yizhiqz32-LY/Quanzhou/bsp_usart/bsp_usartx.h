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
#define RS485_REDE_PIN                         GPIO_Pin_0

#define RS485_USARTx                           UART5
#define RS485_USARTx_BAUDRATE                  115200

#define RS485_USARTx_ClockCmd                  RCC_APB1PeriphClockCmd
#define RS485_USARTx_CLK                       RCC_APB1Periph_UART5 

#define RS485_USARTx_GPIO_ClockCmd             RCC_APB2PeriphClockCmd
#define RS485_USARTx_TX_PORT                   GPIOC   
#define RS485_USARTx_TX_PIN                    GPIO_Pin_12
#define RS485_USARTx_TX_CLK                    RCC_APB2Periph_GPIOC 

#define RS485_USARTx_RX_PORT                   GPIOD 
#define RS485_USARTx_RX_PIN                    GPIO_Pin_2
#define RS485_USARTx_RX_CLK                    RCC_APB2Periph_GPIOD

// 232  使能引脚GPIO 宏定义


#define RS232_USARTx                           USART1
#define RS232_USARTx_BAUDRATE                  9600

#define RS232_USARTx_ClockCmd                  RCC_APB2PeriphClockCmd
#define RS232_USARTx_CLK                       RCC_APB2Periph_USART1


#define RS232_USARTx_GPIO_ClockCmd             RCC_APB2PeriphClockCmd
#define RS232_USARTx_TX_PORT                   GPIOA   
#define RS232_USARTx_TX_PIN                    GPIO_Pin_9
#define RS232_USARTx_TX_CLK                    RCC_APB2Periph_GPIOA 

#define RS232_USARTx_RX_PORT                   GPIOA 
#define RS232_USARTx_RX_PIN                    GPIO_Pin_10
#define RS232_USARTx_RX_CLK                    RCC_APB2Periph_GPIOA


/* 扩展变量 ------------------------------------------------------------------*/
#define USART_REC_LEN  		200  	//定义最大接收字节数 200
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	


//typedef struct
//{
//  __IO uint16_t SR;
//  uint16_t  RESERVED0;
//  __IO uint16_t DR;
//  uint16_t  RESERVED1;
//  __IO uint16_t BRR;
//  uint16_t  RESERVED2;
//  __IO uint16_t CR1;
//  uint16_t  RESERVED3;
//  __IO uint16_t CR2;
//  uint16_t  RESERVED4;
//  __IO uint16_t CR3;
//  uint16_t  RESERVED5;
//  __IO uint16_t GTPR;
//  uint16_t  RESERVED6;
//} USART_TypeDef;

/* 函数声明 ------------------------------------------------------------------*/

void RS485_USARTx_Config(void);
void RS232_USARTx_Config(void);

void USART_SendChar(USART_TypeDef* USART_x, char Data);
char USART_ReceiveChar(USART_TypeDef* USARTx1);
	
void Usart_SendByte(uint8_t ch);
void Usart_SendStr_length(uint8_t *str,uint32_t strlen);
void Usart_SendString(uint8_t *str);



#endif  // __BSP_USARTX_H__

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/

