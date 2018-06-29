/**
  ******************************************************************************
  * �ļ�����: bsp_GeneralTIM.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: ͨ�ö�ʱ��TIM2 & TIM3 & TIM4 & TIM5�ײ���������
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
#include "delay.h" 


DELAY_STRUCTURE delay_fac;

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ͨ�ö�ʱ�� TIMx,x[2,3,4,5]�ж����ȼ�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void GENERAL_TIMx_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  /* �����ж���Ϊ0 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
  /* �����ж���Դ */
  NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ;	
  /* ���������ȼ�Ϊ 0 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
  /* ������ռ���ȼ�Ϊ3 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	 /*��ʱ��ʹ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

//void GENERAL_TIM2_NVIC_Configuration(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure; 
//  /* �����ж���Ϊ0 */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
//  /* �����ж���Դ */
//  NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ;	
//  /* ���������ȼ�Ϊ 0 */
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
//  /* ������ռ���ȼ�Ϊ3 */
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//	 /*��ʱ��ʹ�� */
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//}
/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--7199 
 * �ж�����Ϊ = 1/(72MHZ /7200) * 1000 = 100ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */
/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{  TIM_Prescaler         ����
 *	TIM_CounterMode			   TIMx,x[6,7]û�У���������
 *  TIM_Period             ����
 *  TIM_ClockDivision      TIMx,x[6,7]û�У���������
 *  TIM_RepetitionCounter  TIMx,x[1,8,15,16,17]����
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */
void GENERAL_TIMx_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  /* ����TIMx_CLK,x[2,3,4,5],���ڲ�ʱ��CK_INT=72M */
  GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);
  
  /* ͨ�ö�ʱ�� TIMx,x[2,3,4,5]�ж����ȼ����� */
  GENERAL_TIMx_NVIC_Configuration();
  
  /* �Զ���װ�ؼĴ����ܵ�ֵ(����ֵ) */
  TIM_TimeBaseStructure.TIM_Period=1000;
  /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж�
     ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (7199+1)=0.1ms */
  TIM_TimeBaseStructure.TIM_Prescaler= 7199;
  /* ʱ�ӷ�Ƶ���� ��û���õ������ù� */
//  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  /* ����������ģʽ��������ʱ��TIM6��TIM7ֻ�����ϼ�����û�м���ģʽ�����ã����ù� */
 // TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
  /* �ظ���������ֵ��ͨ�ö�ʱ��û�У����ù� */
  //TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
  /* ��ʼ����ʱ��TIMx, x[2,3,4,5] */
  TIM_TimeBaseInit(GENERAL_TIMx, &TIM_TimeBaseStructure);

  /* ����������жϱ�־λ */
  TIM_ClearITPendingBit(GENERAL_TIMx, TIM_IT_Update);
  /* �����������ж� */
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
Function(��������): __irq	Timer0_Proc(void)
Description(�������ܡ����ܵȵ�����): Timer0�жϷ������1.��ʹ�ñ�Ƶ��������£���ֹ�������������жϳ���������
													  2.WDT����������encoder1.c�У�����main�����Ѿ������룬
														�����жϺ������ڽ��룬������ֹͣ
													  3.ʵʱ��������ٶȵ���������¼,������ٶȵļ���
													  4.������ʽ��ʱ��delay_qz(U8 delay_rank,U32 delay_t,U8 status)
													  5.�ⲿ����(delay_qz(0,30,1))���ú���
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/



/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/

