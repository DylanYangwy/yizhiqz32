#ifndef __DELAY_H__
#define __DELAY_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "includes.h"

typedef struct 
{
	u8 delay_init_flag[6];
	u8 delay_flag[6];
	u8 delay_time_qz[6];
	u8 delay_permit[6];
}DELAY_STRUCTURE;

extern DELAY_STRUCTURE delay_fac;
/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
/********************通用定时器TIM参数定义，只限TIM2 & TIM3 & TIM4 & TIM5************/

#define GENERAL_TIMx                 TIM6
#define GENERAL_TIM_APBxClock_FUN    RCC_APB1PeriphClockCmd
#define GENERAL_TIM_CLK              RCC_APB1Periph_TIM6
#define GENERAL_TIM_IRQ              TIM6_IRQn
#define GENERAL_TIM_INT_FUN          TIM6_IRQHandler

//#define GENERAL_TIM2                 TIM7
//#define GENERAL_TIM2_APBxClock_FUN    RCC_APB1PeriphClockCmd
//#define GENERAL_TIM2_CLK              RCC_APB1Periph_TIM7
//#define GENERAL_TIM2_IRQ              TIM7_IRQn
//#define GENERAL_TIM2_INT_FUN          TIM7_IRQHandler

#define  GENERAL_TIMx_Configuration Timer0_init_quanzhou

#define	delay_0_count				delay_fac.delay_flag[0]
#define	delay_1_count				delay_fac.delay_flag[1]
#define	delay_2_count				delay_fac.delay_flag[2]
#define	delay_3_count				delay_fac.delay_flag[3]
#define	delay_4_count				delay_fac.delay_flag[4]
#define	delay_5_count				delay_fac.delay_flag[5]


#define	init_stop_time_kw			g_InteralMemory.KeepWord[149]			//改机开机停机时间0.1s

/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void GENERAL_TIMx_Configuration(void);
void Delay_qz_sub(void);
	
void delay_qz(u8 delay_rank,u8 delay_t,u8 status);


/************************非阻塞式Delay基本用法***********************************/
/**
{
	delay_qz(n,t,1);						//n号定时器,t段时间,1表示开启
	int x;								//x：延时中间时间段
	if (delay_fac.delay_permit[n]==1)
	{
		if (delay_0_count<(t-x))
		{
			//此处添加功能代码
		}
		else
		{
			//此处添加功能代码
		}
		
	}
}

{
	delay_qz(n,t,0);						//关闭n号定时器
}

**/
#endif	/* __GENERAL_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/

