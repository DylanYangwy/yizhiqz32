/**
  ******************************************************************************
  * 文件名程: bsp_GeneralTIM.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 通用定时器TIM2 & TIM3 & TIM4 & TIM5底层驱动程序
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
#include "delay.h" 


DELAY_STRUCTURE delay_fac;

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 通用定时器 TIMx,x[2,3,4,5]中断优先级配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void GENERAL_TIMx_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  /* 设置中断组为0 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
  /* 设置中断来源 */
  NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ;	
  /* 设置主优先级为 0 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
  /* 设置抢占优先级为3 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	 /*定时器使能 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

//void GENERAL_TIM2_NVIC_Configuration(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure; 
//  /* 设置中断组为0 */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
//  /* 设置中断来源 */
//  NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ;	
//  /* 设置主优先级为 0 */
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
//  /* 设置抢占优先级为3 */
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//	 /*定时器使能 */
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//}
/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--7199 
 * 中断周期为 = 1/(72MHZ /7200) * 1000 = 100ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
 */
/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{  TIM_Prescaler         都有
 *	TIM_CounterMode			   TIMx,x[6,7]没有，其他都有
 *  TIM_Period             都有
 *  TIM_ClockDivision      TIMx,x[6,7]没有，其他都有
 *  TIM_RepetitionCounter  TIMx,x[1,8,15,16,17]才有
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */
void GENERAL_TIMx_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  /* 开启TIMx_CLK,x[2,3,4,5],即内部时钟CK_INT=72M */
  GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);
  
  /* 通用定时器 TIMx,x[2,3,4,5]中断优先级配置 */
  GENERAL_TIMx_NVIC_Configuration();
  
  /* 自动重装载寄存器周的值(计数值) */
  TIM_TimeBaseStructure.TIM_Period=1000;
  /* 累计 TIM_Period个频率后产生一个更新或者中断
     时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (7199+1)=0.1ms */
  TIM_TimeBaseStructure.TIM_Prescaler= 7199;
  /* 时钟分频因子 ，没有用到，不用管 */
//  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  /* 计数器计数模式，基本定时器TIM6和TIM7只能向上计数，没有计数模式的设置，不用管 */
 // TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
  /* 重复计数器的值，通用定时器没有，不用管 */
  //TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
  /* 初始化定时器TIMx, x[2,3,4,5] */
  TIM_TimeBaseInit(GENERAL_TIMx, &TIM_TimeBaseStructure);

  /* 清除计数器中断标志位 */
  TIM_ClearITPendingBit(GENERAL_TIMx, TIM_IT_Update);
  /* 开启计数器中断 */
  TIM_ITConfig(GENERAL_TIMx,TIM_IT_Update,ENABLE);

  TIM_Cmd(GENERAL_TIMx, ENABLE); 
	
	
}



void Delay_qz_sub(void)
{
	u8 delay_rank;
	for (delay_rank=0;delay_rank<5;delay_rank++)
	{
		if (delay_fac.delay_permit[delay_rank]==1)
		{
			if (delay_fac.delay_init_flag[delay_rank]==0)
			{
				delay_fac.delay_flag[delay_rank]=0;
				delay_fac.delay_init_flag[delay_rank]=1;
			}
			else
			{
				delay_fac.delay_flag[delay_rank]++;
				if (delay_fac.delay_flag[delay_rank]>=delay_fac.delay_time_qz[delay_rank])
				{
					delay_fac.delay_flag[delay_rank]=0;
					//delay_fac.delay_permit[delay_rank]=0;
				}
			}
		}
		else
		{
			delay_fac.delay_flag[delay_rank]=0;
			delay_fac.delay_init_flag[delay_rank]=0;
		}
	}
}

void delay_qz(u8 delay_rank,u8 delay_t,u8 status)
{
	delay_fac.delay_time_qz[delay_rank]=delay_t;
	delay_fac.delay_permit[delay_rank]=status;
}


/*************************************************
Function(函数名称): __irq	Timer0_Proc(void)
Description(函数功能、性能等的描述): Timer0中断服务程序：1.不使用变频器的情况下，防止屏死机，但是中断程序还在运行
													  2.WDT计数，用于encoder1.c中，用于main函数已经不进入，
														但是中断函数还在进入，将大盘停止
													  3.实时计算大盘速度的脉冲数记录,即完成速度的计算
													  4.非阻塞式延时：delay_qz(U8 delay_rank,U32 delay_t,U8 status)
													  5.外部报警(delay_qz(0,30,1))调用函数
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/



/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/

