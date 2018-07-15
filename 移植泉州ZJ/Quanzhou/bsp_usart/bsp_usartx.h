#ifndef __BSP_USARTX_H__
#define __BSP_USARTX_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include <stm32f10x.h>
#include "stdio.h"

#include "includes.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
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

// 485  ʹ������GPIO RE/DE�궨��
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

// 232  ʹ������GPIO �궨��


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


/* ��չ���� ------------------------------------------------------------------*/
#define USART_REC_LEN  		200  	//�����������ֽ��� 200
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

extern u8 RS485_RX_BUF[USART_REC_LEN]; 		//���ջ���,���64���ֽ�
extern u8 RS485_RX_CNT;   			//���յ������ݳ���

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C


//ģʽ����
#define RS485_TX_EN		 BIT_ADDR(GPIOD_ODR_Addr,7)	//485ģʽ����.0,����;1,����.
//����봮���жϽ��գ��벻Ҫע�����º궨��

/*
#define FLASH_ADR 0x0807F800
	ADDR(unsigned long addr,int num)
{
//����
	FLASH_Unlock();
	//�����־λ
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|
					FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	
	addr_now=FLASH_ADR+num;
	//Ҫ����ҳ����ʼ��ַ
	FLASH_ErasePage(addr_now);
	//д����
	FLASH_ProgramWord(FLASH_ADR,data);

	
	//����
	FLASH_Lock();
	
#define BITBAND(addr, bitnum) (addr+bitnum) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))
	//�������
	word[n]=BIT_ADDR(FLASH_ADR,n)
*/


/* �������� ------------------------------------------------------------------*/

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

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/

