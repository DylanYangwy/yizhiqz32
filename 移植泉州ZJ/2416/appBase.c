#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

//#include "def.h"
//#include "2416addr.h"
//#include "2416lib.h"
//#include "option.h"
//#include "LCD.h"
//#include "mmu.h"
//#include "Nand.h"
//#include "..\inc\download.h"
//#include "..\inc\systemconf.h"
//#include "..\inc\systemmenu.h"
//#include "..\inc\function.h"
//#include "..\inc\jiema.h"
//#include "..\inc\communication.h"
//#include "..\inc\test.h"
//#include "..\inc\Rtc.h"
//#include "..\inc\encrypt.h"

//#include "..\inc\appBase.h"

#include "includes.h"
extern U8 shoubu_previous_speed;
extern U8 shoubu_previous_status;
extern U8 songsha_previous_speed;
extern U8 songsha_previous_status;


extern INTERALMEMORY g_InteralMemory;

//extern GLOBALVAR g_GlobalVar;

//extern INT16U PreScrNum;

//unsigned int kMotorTarget[7]={0};
volatile unsigned int k_motor[7]={8000,20000,20000,0,0,0,0};			//电机K值存放的数组(实际)
volatile unsigned int motor_factor[7]={0};		//用于和1000000比较，仅仅是一个中间变量

unsigned int InPulseCount[7]={0};		//记录6路高速输入的脉冲数

unsigned int OutPulse_PreSet[4]={0};	// 3路脉冲输出的  脉冲预设值
unsigned int OutPulse_Counter[4]={0};	// 3路脉冲输出的  当前输出脉冲数

unsigned int a=0;


/***********/
//系统时钟配置
/***********/
void RCC_Configuration(void)
{
	SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO,ENABLE);
}


/**************/
//GPIO配置文件
/*************/
void GPIO_Configuration(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;	
//A口配置
	GPIO_InitStructure.GPIO_Pin=X2_GPIO_PIN|X3_GPIO_PIN|X4_GPIO_PIN|X5_GPIO_PIN|X6_GPIO_PIN|
															X7_GPIO_PIN|X8_GPIO_PIN|X9_GPIO_PIN;//PA8不知道干嘛的
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;

	GPIO_Init(GPIOA,&GPIO_InitStructure);


	
	//B口配置
	GPIO_InitStructure.GPIO_Pin=X16_GPIO_PIN|X17_GPIO_PIN|X18_GPIO_PIN;//PA8不知道干嘛的
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;

	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//B口输出
	GPIO_InitStructure.GPIO_Pin=Y1_GPIO_PIN|Y2_GPIO_PIN|Y5_GPIO_PIN|Y6_GPIO_PIN|LED1_GPIO_PIN;
		
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;

	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_SetBits(GPIOB,Y1_GPIO_PIN|Y6_GPIO_PIN|LED1_GPIO_PIN);

//D口输出
	GPIO_InitStructure.GPIO_Pin=LED3_GPIO_PIN;//PA8不知道干嘛的
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;

	GPIO_Init(GPIOD,&GPIO_InitStructure);
	GPIO_SetBits(GPIOD,LED3_GPIO_PIN);


	//E口配置
	GPIO_InitStructure.GPIO_Pin=X10_GPIO_PIN|X11_GPIO_PIN|X12_GPIO_PIN|X13_GPIO_PIN|X14_GPIO_PIN|X15_GPIO_PIN;//PA8不知道干嘛的
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;

	GPIO_Init(GPIOE,&GPIO_InitStructure);

	//E口输出
	GPIO_InitStructure.GPIO_Pin=LED2_GPIO_PIN|Y3_GPIO_PIN|Y4_GPIO_PIN|Y7_GPIO_PIN|Y8_GPIO_PIN;
		
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;

	GPIO_Init(GPIOE,&GPIO_InitStructure);

	GPIO_SetBits(GPIOE,LED2_GPIO_PIN|Y3_GPIO_PIN|Y4_GPIO_PIN|Y7_GPIO_PIN|Y8_GPIO_PIN);

}

 void X1_EXIT_Config(void)
{
  /* 定义IO硬件初始化结构体变量 */
  GPIO_InitTypeDef GPIO_InitStructure;
  /* 定义外部中断线初始化结构体变量 */
  EXTI_InitTypeDef EXTI_InitStructure;
  /* 定义嵌套向量中断控制器初始化结构体变量 */
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 为启用IO引脚中断功能需要使能复用功能时钟 */
  X1_RCC_CLOCKCMD(X1_RCC_CLOCKGPIO | X1_RCC_CLOCKAFIO,ENABLE);
  
  /* 设定KEY1对应引脚IO编号 */
  GPIO_InitStructure.GPIO_Pin = X1_GPIO_PIN;  
  /* 设定KEY1对应引脚IO最大操作速度 ：GPIO_Speed_50MHz */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  /* 设定KEY1对应引脚IO为浮空输入模式 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  /* 初始化KEY1对应引脚IO */
  GPIO_Init(X1_GPIO, &GPIO_InitStructure);

  /* 选择PA0作为中断输入源 */
  GPIO_EXTILineConfig(X1_GPIO_PORTSOURCE,X1_GPIO_PINSOURCE);
  
  /* KEY1对应的断线 */
  EXTI_InitStructure.EXTI_Line=X1_EXITLINE;
  /* 外部中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  /* 下降沿触发方式 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* 使能中断 */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  /* 根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器 */
  EXTI_Init(&EXTI_InitStructure);
  
  /* 选择中断优先级配置组为4个抢占式优先级和4个子优先级，可以参考misc.h文件了解相关设置 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 使能KEY1所在的外部中断通道 */
  NVIC_InitStructure.NVIC_IRQChannel = X1_IRQCHANNEL;
  /* 设置抢占式优先级为2 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; 
  /* 设置子优先级为3 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
  /* 使能外部中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
  /* 初始化配置嵌套向量中断控制器 */
  NVIC_Init(&NVIC_InitStructure); 
}

void X1_IRQHANDLER(void)
{
	unsigned int jj;//,signal;,zushu //友峰去除组数
  /* 确保是否产生了EXTI Line中断 */
	if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
	
    /* 清除中断标志位	*/
		
				for (jj=0;jj<3;jj++)
		{
			motor_factor[jj] += k_motor[jj];
			
			/***拉低3个电机电平(或者拉高，具体看原理图，有修改过)***/
			if (motor_factor[jj]>=1000000)
			{
					Set_Y_Value(jj,LOW);
				motor_factor[jj] -= 1000000;
				songsha_num[jj]++;			//shachang_xianshi()中调用，用于纱长显示
			}
			
			/***拉高3个电机电平(或者拉低，具体看原理图，有修改过)，
				并且优化占空比(步进电机的响应时间会变长一些，但是一个周期之内不影响)
				友峰：jj != 3 改为jj == 0,if(jj != 3) && --> if ((jj == 0) && 
			***/
//			if ((jj == 0) && (motor_factor[jj + 4] >= 500000 || k_motor[jj + 4] >= 500000)){
//				Set_Y_Value(jj,HIGH);
//			}
		}
		EXTI_ClearITPendingBit(EXTI_Line7);     
	}
}

void Y_TOGGLE(int T_num)
{

	
	switch(T_num)
	{
//	 case 1:
//		 Y1_GPIO->ODR ^=Y1_GPIO_PIN;
//		break;
//	 case 2:
//		 Y2_GPIO->ODR ^=Y2_GPIO_PIN;
//		break;
//	 case 3:
//		 Y3_GPIO->ODR ^=Y3_GPIO_PIN;
//		break;
	 case 1:
		 LED1_GPIO->ODR ^=LED1_GPIO_PIN;
		break;
	 case 2:
		 LED2_GPIO->ODR ^=LED2_GPIO_PIN;
		break;
	 case 3:
		 LED3_GPIO->ODR ^=LED3_GPIO_PIN;
		break;
//	 case 7:
//		 (Value) ? GPIO_SetBits(Y7_GPIO,Y7_GPIO_PIN): GPIO_ResetBits(Y7_GPIO,Y7_GPIO_PIN);
//		break;
//	 case 8:
//		 (Value) ? GPIO_SetBits(Y8_GPIO,Y8_GPIO_PIN): GPIO_ResetBits(Y8_GPIO,Y8_GPIO_PIN);
//		break;
 
//	case 9:
//		(Value) ? (rGPEDAT |= (1<<5)) : (rGPEDAT &= ~(1<<5));	// Y9	
//		break;
//	case 10:
//		(Value) ? (rGPEDAT |= (1<<6)) : (rGPEDAT &= ~(1<<6));	// Y10	
//		break;
//	case 11:
//		(Value) ? (rGPEDAT |= (1<<7)) : (rGPEDAT &= ~(1<<7));	// Y11	
//		break;
  }
	
}

//void EXTI_9_5_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(X1_EXITLINE) != RESET)
//		{
//			
//////  	/**********此处为上升沿中断(((rGPFDAT >> 1) & 0x1)保证了其上升沿读出来的数值是1)
//////			  signal!=((rGPFDAT >> 2) & 0x1 此条件用来进行消抖(具体是停机的时候，
//////			  增量式编码器的性质，导致外部抖动会造成编码器信号一直存在，导致电机运动)
//////			  此种方法可以省去对中断信号的延时进入，因为通过编码器B相判断。
//////			  所以此方法只能用于双边沿的中断。
//////	**********/
//////		if (signal!=Get_X_Value(2)){//Get_X_Value(2)
//////		signal=Get_X_Value(2);//Get_X_Value(2)，获得B相信号
//////		encoder1_speed_pulse++;
//////		encoder1_pulse_number++;	//编码器脉冲数记录
//////		//友峰调线功能需要使用Y9/Y10作为压针电机，所以得去除两路电机
//////		/**将7组电机分为上下沿两次进行判断，以减小每次循环次数(上半部分)**/		
////	  	encoder1_speed_pulse++;
////					if(encoder1_speed_pulse>=65530)
////			encoder1_speed_pulse=0;
////					if (encoder1_speed_pulse%3==0)
////					{
////						a=!a;
////					Set_Y_Value(6,a);
//			
//		    LED1_TOGGLE;
////			 
////			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
//				EXTI_ClearITPendingBit(X1_EXITLINE);
//			}
//			
//		     
//}



void APP_init(void)
{
	X1_EXIT_Config();
}

#if 1
///////////////////////////////////////////////////////////////////
////	高速脉冲输入X1 中断服务程序    ///////////////
////	InPulseCount[1] 为X1  输入的脉冲数	//////////////////
///////////////////////////////////////////////////////////////////
/*void __irq PulseIn_1_Process(void)	
{
	int i;
	for(i=0; i<20; i++);

	if((rGPFDAT >> 1) & 0x1)
	{		
		InPulseCount[1]++;
		encoder1_number++;				//2016.1.6 quanzhou
	}

	rEINTPEND=(1<<1);
	ClearPending(BIT_EINT1);
	
	
	
}	*/	//2016.1.2	quanzhou
///////////////////////////////////////////////////////////////////
////	高速脉冲输入X2 中断服务程序    ///////////////
////	InPulseCount[2] 为X2  输入的脉冲数	//////////////////
///////////////////////////////////////////////////////////////////
/*void __irq PulseIn_2_Process(void)	
{
	int i;
	for(i=0; i<20; i++);

	if((rGPFDAT >> 2) & 0x1)
	{
		InPulseCount[2]++;
		encoder2_number++;			//2016.1.6 quanzhou
	}

	rEINTPEND=(1<<2);
	ClearPending(BIT_EINT2);
}*/				//2016.1.2	quanzhou
///////////////////////////////////////////////////////////////////
////	高速脉冲输入X3 中断服务程序    ///////////////
////	InPulseCount[3] 为X3  输入的脉冲数	//////////////////
///////////////////////////////////////////////////////////////////
void __irq PulseIn_3_Process(void)	
{
//	int i;
//	for(i=0; i<20; i++);

//	if((rGPFDAT >> 3) & 0x1)
//	{
//		InPulseCount[3]++;
//	}

//	rEINTPEND=(1<<3);
//	ClearPending(BIT_EINT3);
//	
	
	
	
}				



//////////////////////////////////////////////////////////////////////
////	高速脉冲输入X4-X6  中断服务程序     //////////////
////	InPulseCount[4] 为X4  输入的脉冲数	//////////////////
////	InPulseCount[5] 为X5  输入的脉冲数	//////////////////
////	InPulseCount[6] 为X6  输入的脉冲数	//////////////////
/////////////////////////////////////////////////////////////////////
void __irq PulseIn_4_7_Process(void)
{
//	int i;
//	for(i=0; i<20; i++);

//		// Eint4	// 高速输入X4
//	if ( !( rEINTMASK >>4  & 0x1 )  && (rEINTPEND & (1<<4)) )//取消屏蔽标志+挂起有效
//	{
//		if((rGPFDAT >> 4) & 0x1)
//		{
//			InPulseCount[4]++;
//		}

//		rEINTPEND=(1<<4);		
//	}
//	// Eint5	// 高速输入X5
//	else if ( !( rEINTMASK >>5  & 0x1 )  && (rEINTPEND & (1<<5)) )//取消屏蔽标志+挂起有效	
//	
//	{
//		if((rGPFDAT >> 5) & 0x1)
//		{
//			InPulseCount[5]++;
//		}

//		rEINTPEND=(1<<5);		
//	}
//	// Eint6	// 高速输入X6
//	else if ( !( rEINTMASK >>6  & 0x1 )  && (rEINTPEND & (1<<6)) )//取消屏蔽标志+挂起有效	
//	
//	{
//		if((rGPFDAT >> 6) & 0x1)
//		{
//			InPulseCount[6]++;  //InPulseCount[n] 只定义了7个 InPulseCount[0]-----InPulseCount[6]
//		}

//		rEINTPEND=(1<<6);		
//	}
//	// Eint7	// 高速输入X7
//	else if ( !( rEINTMASK >>7  & 0x1 )  && (rEINTPEND & (1<<7)) )//取消屏蔽标志+挂起有效	
//	
//	{
//		if((rGPFDAT >> 7) & 0x1)
//		{
//		  /////////
//		}
//		rEINTPEND=(1<<7);		
//	}
// 
//	ClearPending(BIT_EINT4_7);
}

//////////////////////////////////////////////////////////////////////
////	高速脉冲输入X8-X23  中断服务程序     //////////////
/////////////////////////////////////////////////////////////////////
void __irq PulseIn_8_23_Process(void)   // 高速输入X8-x10   
{	 
//	 if ( !( rEINTMASK >>8  & 0x1 )  && (rEINTPEND & (1<<8)) ) //取消屏蔽+挂起有效
//	
//	{	
//	    /*if((rGPGDAT >> 0) & 0x1) //判断沿触发
//	    {
//	      //////////
//	      
//	    }*/
//		
//		rEINTPEND=(1<<8);
//		 
//	}
//	else if ( !( rEINTMASK >>9  & 0x1 )  && (rEINTPEND & (1<<9)) ) //取消屏蔽+挂起有效   
//	{ 
//		//////////
//		rEINTPEND=(1<<9);
//	}
//	else if ( !( rEINTMASK >>10  & 0x1 )  && (rEINTPEND & (1<<10)) ) //取消屏蔽+挂起有效  
//	{
//		////////
//		rEINTPEND=(1<<10);
//	}
//	else if ( !( rEINTMASK >>11  & 0x1 )  && (rEINTPEND & (1<<11)) ) //取消屏蔽+挂起有效  
//	{
//		////////
//		rEINTPEND=(1<<11);
//	}
//	else if ( !( rEINTMASK >>12  & 0x1 )  && (rEINTPEND & (1<<12)) ) //取消屏蔽+挂起有效  
//	{
//		////////
//		rEINTPEND=(1<<12);
//	}
//	else if ( !( rEINTMASK >>13  & 0x1 )  && (rEINTPEND & (1<<13)) ) //取消屏蔽+挂起有效  
//	{
//		////////
//		rEINTPEND=(1<<13);
//	}
//	else if ( !( rEINTMASK >>14  & 0x1 )  && (rEINTPEND & (1<<14)) ) //取消屏蔽+挂起有效  
//	{
//		////////
//		rEINTPEND=(1<<14);
//	} 
//	else if ( !( rEINTMASK >>15  & 0x1 )  && (rEINTPEND & (1<<15)) ) //取消屏蔽+挂起有效  
//	{
//		////////
//		rEINTPEND=(1<<15);
//	}
//	
//	
//	
//	ClearPending(BIT_EINT8_23);

}







///////////////////////////////////////////////////////////////
////	高速脉冲输入X1  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse1_Start(void)
{
//	rEINTPEND=(1<<1);
//	ClearPending(BIT_EINT1);
//	
//	rEINTMASK &= ~BIT_EINT1;
//	rINTMSK1 &= ~BIT_EINT1;
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X1  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse1_Stop(void)
{
//	rEINTMASK |=(1<<1);
//	rINTMSK1 |=BIT_EINT1;
//	
//	rEINTPEND=(1<<1);
//	ClearPending(BIT_EINT1);
	
}

///////////////////////////////////////////////////////////////
////	高速脉冲输入X2  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse2_Start(void)
{
//	rEINTPEND=(1<<2);
//	ClearPending(BIT_EINT2);
//	
//	rEINTMASK &= ~(1<<2);
//	rINTMSK1 &= ~BIT_EINT2;
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X2  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse2_Stop(void)
{
//	rEINTMASK |=(1<<2);
//	rINTMSK1 |=BIT_EINT2;
//	
//	rEINTPEND=(1<<2);
//	ClearPending(BIT_EINT2);
}

///////////////////////////////////////////////////////////////
////	高速脉冲输入X3  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse3_Start(void)
{
//	rEINTPEND=(1<<3);
//	ClearPending(BIT_EINT3);

//	rEINTMASK &= ~(1<<3);
//	rINTMSK1 &= ~BIT_EINT3;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X3  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse3_Stop(void)
{
//	rEINTMASK |=(1<<3);
//	rINTMSK1 |=BIT_EINT3;
//	
//	rEINTPEND=(1<<3);
//	ClearPending(BIT_EINT3);
}

///////////////////////////////////////////////////////////////
////	高速脉冲输入X4  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse4_Start(void)
{
//	rEINTPEND=(1<<4);
//	ClearPending(BIT_EINT4_7);
//	
//	rEINTMASK &= ~(1<<4);
//	rINTMSK1 &= ~BIT_EINT4_7;
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X4  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse4_Stop(void)
{
//	rEINTMASK |=(1<<4);
//	//rINTMSK1 |=BIT_EINT4_7;
//	
//	rEINTPEND=(1<<4);
//	ClearPending(BIT_EINT4_7);
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X5  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse5_Start(void)
{
//	rEINTPEND=(1<<5);
//	ClearPending(BIT_EINT4_7);
//	
//	rEINTMASK &= ~(1<<5);
//	rINTMSK1 &= ~BIT_EINT4_7;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X5  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse5_Stop(void)
{
//	rEINTMASK |=(1<<5);
//	//rINTMSK1 |=BIT_EINT4_7;
//	
//	
//	rEINTPEND=(1<<5);
//	ClearPending(BIT_EINT4_7);
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X6  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse6_Start(void)
{
//	rEINTPEND=(1<<6);
//	ClearPending(BIT_EINT4_7);
//	
//	rEINTMASK &= ~(1<<6);
//	rINTMSK1 &= ~BIT_EINT4_7;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X6  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse6_Stop(void)
{
//	rEINTMASK |=(1<<6);
//	//rINTMSK1 |=BIT_EINT4_7;
//	
//	rEINTPEND=(1<<6);
//	ClearPending(BIT_EINT4_7);
}


///////////////////////////////////////////////////////////////
////	高速脉冲输入X7  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse7_Start(void)
{
//	rEINTPEND=(1<<7);
//	ClearPending(BIT_EINT4_7);
//	
//	rEINTMASK &= ~(1<<7);
//	rINTMSK1 &= ~BIT_EINT4_7;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X7  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse7_Stop(void)
{
//	rEINTMASK |=(1<<7);
//	//rINTMSK1 |=BIT_EINT4_7;
//	rEINTPEND=(1<<7);
//	ClearPending(BIT_EINT4_7);
	
}

///////////////////////////////////////////////////////////////
////	高速脉冲输入X8  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse8_Start(void)
{  
//	rEINTPEND=(1<<8);
//	ClearPending(BIT_EINT8_23);
//	 
//	rEINTMASK &= ~(1<<8);
//	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X8  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse8_Stop(void)
{
//	rEINTMASK |=(1<<8);
//	//rINTMSK1 |=BIT_EINT8_23;
//	
//	rEINTPEND=(1<<8);
//	ClearPending(BIT_EINT8_23);
	
}


///////////////////////////////////////////////////////////////
////	高速脉冲输入X9  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse9_Start(void)
{
//	rEINTPEND=(1<<9);
//	ClearPending(BIT_EINT8_23);
//	
//	rEINTMASK &= ~(1<<9);
//	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X9  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse9_Stop(void)
{
//	rEINTMASK |=(1<<9);
//	//rINTMSK1 |=BIT_EINT8_23;
//	
//	rEINTPEND=(1<<9);
//	ClearPending(BIT_EINT8_23);
	
}
 
///////////////////////////////////////////////////////////////
////	高速脉冲输入X10  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse10_Start(void)
{
//	rEINTPEND=(1<<10);
//	ClearPending(BIT_EINT8_23);
//	
//	rEINTMASK &= ~(1<<10);
//	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X10  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse10_Stop(void)
{
//	rEINTMASK |=(1<<10);
//	//rINTMSK1 |=BIT_EINT8_23;
//	
//	rEINTPEND=(1<<10);
//	ClearPending(BIT_EINT8_23);
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X11  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse11_Start(void)
{
//	rEINTPEND=(1<<11);
//	ClearPending(BIT_EINT8_23);
//	
//	rEINTMASK &= ~(1<<11);
//	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X11  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse11_Stop(void)
{
//	rEINTMASK |=(1<<11);
//	//rINTMSK1 |=BIT_EINT8_23;
//	
//	rEINTPEND=(1<<11);
//	ClearPending(BIT_EINT8_23);
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X12  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse12_Start(void)
{
//	rEINTPEND=(1<<12);
//	ClearPending(BIT_EINT8_23);
//	
//	rEINTMASK &= ~(1<<12);
//	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X12  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse12_Stop(void)
{
//	rEINTMASK |=(1<<12);
//	//rINTMSK1 |=BIT_EINT8_23;
//	
//	rEINTPEND=(1<<12);
//	ClearPending(BIT_EINT8_23);
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X13  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse13_Start(void)
{
//	rEINTPEND=(1<<13);
//	ClearPending(BIT_EINT8_23);
//	
//	rEINTMASK &= ~(1<<13);
//	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X13  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse13_Stop(void)
{
//	rEINTMASK |=(1<<13);
//	//rINTMSK1 |=BIT_EINT8_23;
//	
//	rEINTPEND=(1<<13);
//	ClearPending(BIT_EINT8_23);
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X14  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse14_Start(void)
{
//	rEINTPEND=(1<<14);
//	ClearPending(BIT_EINT8_23);
//	
//	rEINTMASK &= ~(1<<14);
//	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X14  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse14_Stop(void)
{
//	rEINTMASK |=(1<<14); 
//	//rINTMSK1 |=BIT_EINT8_23;
//	
//	rEINTPEND=(1<<14);
//	ClearPending(BIT_EINT8_23);
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X15  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse15_Start(void)
{
//	rEINTPEND=(1<<15);
//	ClearPending(BIT_EINT8_23);
//	
//	rEINTMASK &= ~(1<<15);
//	rINTMSK1 &= ~BIT_EINT8_23;
//}
/////////////////////////////////////////////////////////////////
//////	高速脉冲输入X15  停止//////////////////////////
/////////////////////////////////////////////////////////////////
//void InPulse15_Stop(void)
//{
//	rEINTMASK |=(1<<15);
//	//rINTMSK1 |=BIT_EINT8_23;
//	
//	rEINTPEND=(1<<15);
//	ClearPending(BIT_EINT8_23);
	
} 
 
 



/////////////////////////////////////////////////
//////	高速脉冲输入初始化     //////////////
//////	IN 1~6   6路高速输入	IN7     //////////////
////////////////////////////////////////////////

/*************************************************
Function(函数名称): Pulse_In_Init(void)
Description(函数功能、性能等的描述): X口初始化
Calls (被本函数调用的函数清单): InPulse1_Stop();
Called By (调用本函数的函数清单): app.c

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): InPulse1_Stop();
Output(对输出参数的说明):
Return: None
Others: None
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/

void Pulse_In_Init(void)	
{
//	DWORD tmp;
//	
//	//配置X1为中断模式
//	tmp = rGPFCON & (~(0x3<< 2)); //& (~(0x3<< 12)) & (~(0x3<< 14));& (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10))
//	rGPFCON = tmp | (0x2<<2);//| (0x2<<12) | (0x2<<14);	| (0x2<<4)|   | (0x2<<10)
//	
//	//配置X3/X4为中断模式
//	tmp = rGPFCON &(~(0x3<< 6)) & (~(0x3<< 8));
//	rGPFCON = tmp | (0x2<<6) | (0x2<<8);
//	//GPF1~7 up down disable x1-x7
//	rGPFUDP &=  (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 12)) & (~(0x3<< 14));
//	//GPG 0 1 2---up down disable---  X9 X10 X11 X12 X13 X14 X15
//    rGPGUDP &= (~(0x3<< 0)) & (~(0x3<< 2))& (~(0x3<< 4)) & (~(0x3<< 6))& (~(0x3<< 8))& (~(0x3<< 10))& (~(0x3<< 12))& (~(0x3<< 14));
//	
//	//配置X11为中断模式，压针零位传感器
//	tmp = rGPGCON &(~(0x3<<6));						//by FJW
//	rGPGCON = tmp | (0x2<<6);						//SET GPG3 AS EINT X11	//by FJW
//	
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<4))) | (0x6<<4);		// Eint1	Both edge triggered
//	//rEXTINT0 = (rEXTINT0 & (~(0x7<<4))) | (0x2<<4);		// Eint1	Falling edge triggered
//	
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<8))) | (0x4<<8);		// Eint2	Rising edge triggered	
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<12))) | (0x4<<12);	// Eint3	Rising edge triggered	
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<16))) | (0x4<<16);	// Eint4	Rising edge triggered
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<20))) | (0x2<<20);	// Eint5	Falling edge triggered
//	//rEXTINT0 = (rEXTINT0 & (~(0x7<<20))) | (0x4<<20);	// Eint5	Rising edge triggered
//	
//	//rEXTINT0 = (rEXTINT0 & (~(0x7<<24))) | (0x2<<24);	// Eint6	Falling edge triggered	
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<24))) | (0x2<<24);	// Eint6	Rising edge triggered	
//	//GPF1~6  x1-x6 上升沿触发
//	/* rEXTINT0 = (rEXTINT0 & (~(0x7<<4))) | (0x4<<4);		// Eint1	Rising edge triggered
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<8))) | (0x4<<8);		// Eint2	Rising edge triggered	
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<12))) | (0x4<<12);	// Eint3	Rising edge triggered	
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<16))) | (0x4<<16);	// Eint4	Rising edge triggered
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<20))) | (0x4<<20);	// Eint5	Rising edge triggered	
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<24))) | (0x4<<24);	// Eint6	Rising edge triggered	 */
//				
//	//GPF1~6  x1-x6 下降沿触发
///*	rEXTINT0 = (rEXTINT0 & (~(0x7<<4))) | (0x2<<4);		// Eint1	Falling edge triggered
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<8))) | (0x2<<8);		// Eint2	Falling edge triggered	
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<12))) | (0x2<<12);	// Eint3	Falling edge triggered	
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<16))) | (0x2<<16);	// Eint4	Falling edge triggered
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<20))) | (0x2<<20);	// Eint5	Falling edge triggered	
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<24))) | (0x2<<24);	// Eint6	Falling edge triggered	
// */
//	//rEXTINT1 = (rEXTINT1 & (~(0x7<<12))) | (0x2<<12);	    // Eint11	下降沿触发		压针零位传感器by FJW
//	
//	//GPF1~6  x1-x6 双边沿触发
//	/*rEXTINT0 = (rEXTINT0 & (~(0x7<<4))) | (0x6<<4);		// Eint1	Both edge triggered
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<8))) | (0x6<<8);		// Eint2	Both edge triggered	
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<12))) | (0x6<<12);		// Eint3	Both edge triggered	
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<16))) | (0x6<<16);		// Eint4	Both edge triggered
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<20))) | (0x6<<20);		// Eint5	Both edge triggered	
//	rEXTINT0 = (rEXTINT0 & (~(0x7<<24))) | (0x6<<24);		// Eint6	Both edge triggered	
//	*/
//	//pISR_EINT1= (U32)PulseIn_1_Process;				// X1
//	//pISR_EINT2= (U32)PulseIn_2_Process;				// X2
//	//pISR_EINT1= (U32)encoder1_A;						// X1	Pos_check_375
//	//pISR_EINT2= (U32)encoder1_B;						// X2 	2016.1.2 quanzhou 
//	//pISR_EINT3= (U32)PulseIn_3_Process;				// X3
//	//pISR_EINT4_7= (U32)PulseIn_4_7_Process;			// X4_6
//	pISR_EINT1= (U32)encoder1_process;				// X1	
//	pISR_EINT2= (U32)encoder2_process;				// X2	
//	pISR_EINT3= (U32)shangyazhen_zero_process;			// X3	
//	pISR_EINT4_7= (U32)pwrDownHandler;
//	//pISR_EINT8_23= (U32)pressing_zero_process;		//压针零位 By FJW
//	rGPFDAT=0Xff;
//	rEINTPEND = 0xffffff;
//	
//	rSRCPND1 |= BIT_EINT1|BIT_EINT2|BIT_EINT3|BIT_EINT4_7|BIT_EINT8_23; //to clear the previous pending states //BIT_EINT8_23 by FJW
//	
//	//rSRCPND1 |= BIT_EINT1|BIT_EINT2|BIT_EINT3|BIT_EINT4_7; //to clear the previous pending states
//	rINTPND1 |= BIT_EINT1|BIT_EINT2|BIT_EINT3|BIT_EINT4_7;
//	InPulse1_Stop();
//	InPulse2_Stop();
//	InPulse3_Stop();
//	InPulse4_Stop();
//	InPulse5_Stop();
//	InPulse6_Stop();
//	
//	//InPulse5_Start();
}





///////////////////////////////////////////////////////////////////////////
////	Y1 脉冲发送中断服务程序////////
///////////////////////////////////////////////////////////////////////////
 void __irq PulseOut_1_Process(void)
{
//	OutPulse_Counter[1]++;	
//	
//	
//	ClearPending((U32)BIT_TIMER1);
	
} 



///////////////////////////////////////////////////////////////////////////
////	Y1 脉冲输出启动//////////////////////////////
////	 每发送一段脉冲 PulseOut_1_Start 只需启动一次//////
////	frequence: 脉冲频率///////////////////////////////
////	pulse_PreSet: 发送脉冲数 ///////////////////////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_1_Start(unsigned int frequence, int pulse_PreSet)
{
//	DWORD tmp;
//	
//	if(0 == pulse_PreSet)
//	{
//		PulseOut_1_Stop();
//		return;
//	}
//	else
//	{
//		OutPulse_PreSet[1] = pulse_PreSet;
//		OutPulse_Counter[1] = 0;	// 确保每次启动PulseOut_1_Start ，发送脉冲数 pulse_PreSet		//2016.1.2 quanzhou
//	}
//	
//	if(0 == frequence)
//	{
//		frequence = 1;
//		PulseOut_1_Stop();
//		return;
//	}
//	
//	rTCNTB1= 300300/frequence;	// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}
//	
//	rTCMPB1 = rTCNTB1/2;
//	
//	rSRCPND1 = rSRCPND1 | ((U32)0x1<<11);   //清空定时器1源请求
//    rINTPND1 = rINTPND1 | ((U32)0x1<<11);   //清空定时器1中断请求
//   
//	rINTMSK1 |=(BIT_TIMER1);
//	//rINTMSK1 &=~(BIT_TIMER1);
//	tmp = rTCON & (~(0xf << 8));	// dead zone Disable
//	rTCON = tmp | (2 << 8);		/* update TCVNTB0, stop					*/
//	rTCON = tmp | (9 << 8);		/* interval mode,  start				*/	
}

///////////////////////////////////////////////////////////////////////////
////	Y1 脉冲发送停止////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_1_Stop(void)
{
//	//DWORD tmp;
//	//rGPEDAT &= ~(1<<3);//by FJW 压针
//	/* if(mode_choose == yazheng_mode)
//	{	
//		Set_Y_Value(pressing_pulse_port,0);	//代替上面一行直接进行未操作

//		////////手动调节结束/////////压针 by FJW
//		if(positive_button == 1){//正向压针脉冲记录
//			jog_positive_status = 0;
//			positive_button = 0;
//		}		
//		if(negative_button == 1){
//			jog_negative_status = 0;
//			negative_button = 0;
//		}
//		
//		OutPulse_Counter[1] = 0;
//	/////////////////////////////////////////////////////
//	} */
//	rTCON &= ~(1 << 8);			/* Timer1, stop							*/
//	rINTMSK1 |= BIT_TIMER1;
//	ClearPending((U32)BIT_TIMER1);
//	/* shoubu_previous_speed=0;
//	shoubu_previous_status=0;
//	shoubu_run_status=0; */
//	
//	//rTCNTB1=0;
//	//Delay(500);
//	//OutPulse_Counter[1] = 0;		//2016.1.2 quanzhou
}


///////////////////////////////////////////////////////////////////////////
////	Y2 脉冲发送中断服务程序////////
///////////////////////////////////////////////////////////////////////////
/*void __irq PulseOut_2_Process(void)
{
	OutPulse_Counter[2]++;
	
	//if(OutPulse_Counter[2] >= OutPulse_PreSet[2])
	if  ((  OutPulse_PreSet[2] > 0   )&&(OutPulse_Counter[2] >= OutPulse_PreSet[2]))
	//OutPulse_PreSet[2]<0用于无限发脉冲
	
	{
		PulseOut_2_Stop();
	}

	ClearPending((U32)BIT_TIMER2);

}*/



///////////////////////////////////////////////////////////////////////////
////	Y2 脉冲输出启动//////////////////////////////
////	 每发送一段脉冲 PulseOut_2_Start 只需启动一次//////
////	frequence: 脉冲频率///////////////////////////////
////	pulse_PreSet: 发送脉冲数 ///////////////////////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_2_Start(unsigned int frequence, int pulse_PreSet)
{
//	DWORD tmp;
//	
//	if(0 == pulse_PreSet)
//	{
//		PulseOut_2_Stop();
//		return;
//	}
//	else
//	{
//		OutPulse_PreSet[2] = pulse_PreSet;
//		OutPulse_Counter[2] = 0;	// 确保每次启动PulseOut_2_Start ，发送脉冲数 pulse_PreSet
//	}
//	
//	if(0 == frequence)
//	{
//		frequence = 1;
//		PulseOut_2_Stop();
//		return;
//	}
//	
//	rTCNTB2= 300300/frequence;	// // Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}
//	rTCMPB2 = rTCNTB2/2;
//	
//	rSRCPND1 = rSRCPND1 | ((U32)0x1<<12);   //清空定时器2源请求
//    rINTPND1 = rINTPND1 | ((U32)0x1<<12);    //清空定时器2中断请求
//  
//	
//	//rINTMSK1 &=~(BIT_TIMER2);
//	tmp = rTCON & (~(0xf << 12));	// dead zone Disable
//	rTCON = tmp | (2 << 12)	;/* update TCVNTB0, stop					*/
//	rTCON = tmp | (9 << 12)	;/* interval mode,  start				*/	
}

///////////////////////////////////////////////////////////////////////////
////	Y2 脉冲发送停止////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_2_Stop(void)
{
//	//DWORD tmp;

//	rINTMSK1 |= BIT_TIMER2;
//	rTCON &= ~(1 << 12);		/* Timer2, stop							*/
//	/* songsha_run_status=0;
//	songsha_previous_speed=0;
//	songsha_previous_status=0; */
//	//OutPulse_Counter[2] = 0;

	
}


/////////////高速Y3未配置////////////////////
///////////////////////////////////////////////////////////////////////////
////	Y3 脉冲发送中断服务程序////////
///////////////////////////////////////////////////////////////////////////
void __irq PulseOut_3_Process(void)
{
	
//	OutPulse_Counter[3]++; 
//	
//	//if(OutPulse_Counter[3] >= OutPulse_PreSet[3])
//	if  ((  OutPulse_PreSet[3] > 0   )&&(OutPulse_Counter[3] >= OutPulse_PreSet[3]))
//	//OutPulse_PreSet[3]<0用于无限发脉冲
//	{	
//		
//		OutPulse_Counter[3]=0;
//		PulseOut_3_Stop();
//	 
//	}
//  	ClearPending((U32)BIT_TIMER3);

}
///////////////////////////////////////////////////////////////////////////
////	Y3 脉冲输出启动//////////////////////////////
////	 每发送一段脉冲 PulseOut_3_Start 只需启动一次//////
////	frequence: 脉冲频率///////////////////////////////
////	pulse_PreSet: 发送脉冲数 ///////////////////////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_3_Start(unsigned int frequence, int pulse_PreSet)
{
//	DWORD tmp;
//	
//	if(0 == pulse_PreSet)
//	{
//		PulseOut_3_Stop();
//		return;
//	}
//	else
//	{
//		OutPulse_PreSet[3] = pulse_PreSet;
//	//	OutPulse_Counter[3] = 0;	// 确保每次启动PulseOut_3_Start ，发送脉冲数 pulse_PreSet
//	}
//	
//	if(0 == frequence)
//	{
//		frequence = 1;
//		PulseOut_3_Stop();
//		return;
//	}
//	
//	rTCNTB3= 300300/frequence;	// // Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}
//	rTCMPB3 = rTCNTB3/2;
//	
//	rSRCPND1 = rSRCPND1 | ((U32)0x1<<13);   //清空定时器3源请求
//	rINTPND1 = rINTPND1 | ((U32)0x1<<13);    //清空定时器3中断请求
//  
//	
//	rINTMSK1 &=~(BIT_TIMER3);
//	tmp = rTCON & (~(0xf << 16));	// dead zone Disable
//	rTCON = tmp | (2 << 16);		//update TCVNTB0, stop				
//	rTCON = tmp | (9 << 16);		// interval mode,  start
}

///////////////////////////////////////////////////////////////////////////
////	Y3 脉冲发送停止////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_3_Stop(void)
{
//	//DWORD tmp;

//	//rINTMSK1 |= BIT_TIMER3;
//	rTCON &= ~(1 << 16);		// Timer3, stop		
//	//bianpingqi_run_status=0;
//	//OutPulse_Counter[2] = 0;

	
}

/*************************************************
Function(函数名称): Pulse_Out_Init(void)
Description(函数功能、性能等的描述): 配置输出口(TOUT(PWM)或者OutPut)
Calls (被本函数调用的函数清单): PulseOut_1_Stop;
Called By (调用本函数的函数清单): app.c

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
/////////////////////////////////////////////////
//////	高速脉冲输出初始化     //////////////
//////	OUT 1~3   3路高速输出 //////////////
////////////////////////////////////////////////
void Pulse_Out_Init()	
{
//	DWORD tmp;
//	/***以下为配置输出口的GPIO***/
//    //set GPB1 2 3 as TOUT
//	// tmp = rGPBCON & (~(0x3<< 2)) & (~(0x3<< 4))& (~(0x3<< 6));
//	// rGPBCON = tmp | (0x2<<2) | (0x2<<4) | (0x2<<6);		
//	
//	//set GPB1 2 3 as 普通输出Output
//	tmp = rGPBCON & (~(0x3<< 2)) & (~(0x3<< 4))& (~(0x3<< 6));
//	rGPBCON = tmp | (0x1<<2) | (0x1<<4) | (0x1<<6);	

//	/***以下为配置三个定时器***/
//	// Timer1 Initialize	----HS Out1
//	pISR_TIMER1 = (int)PulseOut_1_Process;	// Timer ISR for HS Out1	2016.1.2 quanzhou 
//	//pISR_TIMER1 = (int)bianpingqi_motor_pulse_handler;
//	rTCFG0 &= ~(0xff << 0); 
//	rTCFG0 |= (110<< 0); 	// Dead zone=0, Prescaler0=10(0xff)
//	rTCFG1 &= ~(0xf << 4); 
//	rTCFG1 |= (0x0 << 4); 	// Interrupt, Mux0=1/2
//	//rTCNTB1 = 30;    //30.27273	// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}	100K
//	//rTCMPB1 = 15;

//	PulseOut_1_Stop();

//	
//	// Timer2 Initialize	----HS Out2
//	//pISR_TIMER2 = (int)PulseOut_2_Process;	// Timer ISR for HS Out2
//	rTCFG0 &= ~(0xff << 8); 
//	rTCFG0 |= (110 << 8); 	// Dead zone=0, Prescaler0=110(0xff)   注意注意注意！！！不能变 Timer 2 3 4 共用
//	rTCFG1 &= ~(0xf << 8); 
//	rTCFG1 |= (0x0 << 8); 	// Interrupt, Mux0=1/2
//	
//	PulseOut_2_Stop(); 

//	
//	// Timer3 Initialize	----HS Out3
// 	//pISR_TIMER3 = (int)PulseOut_3_Process;	// Timer ISR for HS Out3
//	rTCFG0 &= ~(0xff << 8); 
//	rTCFG0 |= (110 << 8); 	// Dead zone=0, Prescaler0=110(0xff)   注意注意注意！！！不能变 Timer 2 3 4 共用
//	rTCFG1 &= ~(0xf << 12); 
// 	rTCFG1 |= (0x0 << 12); 	// Interrupt, Mux0=1/2
// 	
// 	PulseOut_3_Stop();

}






//////////////////////////////////////////////////////////////////////
////	DA 输出中断服务程序///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void __irq DA0_10_Process(void) // 可通过示波器确定频率
{
//	/* if(0xffff == (g_InteralMemory.Word[32] & 0xffff))
//	{
//		g_InteralMemory.Word[32] = 0;
//		
//	}
//	else
//		g_InteralMemory.Word[32]++; */

//	ClearPending((U32)BIT_TIMER3);
}


//////////////////////////////////////////////////////////////////////
////	DA 输出启动(只需启动一次)//////////////////////////
////	voltage: 输出电压(0.000~10.000V )  小数点后3 位//////////
//////////////////////////////////////////////////////////////////////

void DA0_10_Start(float voltage)
{
//	DWORD tmp;

//	if(voltage>10*1000)
//		voltage = 10*1000;
//	else if(voltage<0)
//		voltage = 0;
//	
//	rTCNTB3= 300;	
//	//rTCMPB3 = (rTCNTB3*g_InteralMemory.Word[30])/(10000*1.326);// 最大10V , 小数点后3 位10*1000
//	rTCMPB3 = (rTCNTB3*voltage)/(10*1000*1.326);// 最大10V , 小数点后3 位10*1000
//	if(rTCMPB3 == rTCNTB3)
//		rTCMPB3 -= 1;
//	
//	tmp = rGPBCON & (~(0x3<< 6));
//	rGPBCON = tmp | (0x2<<6);	//set GPB3 as TOUT	

//	rINTMSK1 &=~(BIT_TIMER3);
//	tmp = rTCON & (~(0xf << 16));	// dead zone Disable
//	rTCON = tmp | (2 << 16);		/* update TCVNTB3, stop					*/
//	rTCON = tmp | (9 << 16);		/* interval mode,  start				*/	
}


//////////////////////////////////////////////////////////////////////
////	DA 输出停止//////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void DA0_10_Stop(void)
{
//	DWORD tmp;

//	rTCNTB3= 300;	
//	rINTMSK1 |= BIT_TIMER3;
//	rTCON &= ~(1 << 16);			/* Timer3, stop							*/

//	tmp = rGPBCON & (~(0x3<< 6));
//	rGPBCON = tmp | (1<<6);	//set GPB3 as OUT	
//	rGPBDAT &= ~(1<<3);
}


//////////////////////////////////////////////////////////////////////
////	DA 输出初始化///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void DA0_10_Init(void)
{
//	DWORD tmp;

//	tmp = rGPBCON & (~(0x3<< 6));
//	rGPBCON = tmp | (0x2<<6);	//set GPB3 as TOUT	

//	// Timer3 Initialize	----DA
//	pISR_TIMER3 = (int)DA0_10_Process;	// Timer ISR for DA
//	rTCFG0 &= ~(0xff << 8); 
//	rTCFG0 |= (110 << 8); 	// Dead zone=0, Prescaler0=110(0xff)   Timer 2 3 4 共用
//	rTCFG1 &= ~(0xf << 12); 
//	rTCFG1 |= (0x0 << 12); 	// Interrupt, Mux0=1/2
//	rTCNTB3 = 300;    			// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}	1K
//	rTCMPB3 = 150;
//	DA0_10_Stop();
}



/////////////////////////////////////////////////
//////	普通输入初始化     //////////////
//////	IN7~IN15 共9 路		//////////////
////////////////////////////////////////////////
void X_In_Init(void)	
{
//	DWORD tmp;

//	tmp = rGPFCON & (~(0x3<< 10))& (~(0x3<< 12))& (~(0x3<< 14));
//	rGPFCON = tmp | (0x0<<10)|(0x0<<12)|(0x0<<14);			
//	//////////////
//	tmp = rGPGCON & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8))
//					 & (~(0x3<< 10)) & (~(0x3<< 12)) & (~(0x3<< 14));
//	
//	rGPGCON = tmp | (0x0<<0) | (0x0<<2) | (0x0<<4) | (0x0<<6) | (0x0<<8) | (0x0<<10) | (0x0<<12) | (0x0<<14);			
//	//////////	
}



/////////////////////////////////////////////////
//////	普通输出初始化     //////////////
//////	Y2~Y15 共14 路	 //////////////
////////////////////////////////////////////////
void Y_Out_Init(void)		
{
//	DWORD tmp;

//	tmp = rGPECON & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 12)) & (~(0x3<< 14));
//	
//	
//	rGPECON = tmp | (0x1<<0) | (0x1<<2) | (0x1<<4) | (0x1<<6) | (0x1<<8) | (0x1<<10) | (0x1<<12) | (0x1<<14);
//	rGPEUDP &= (~(1<<0)) & (~(1<<1)) & (~(1<<2)) & (~(1<<3)) & (~(1<<4)) & (~(1<<5)) & (~(1<<6)) & (~(1<<7));
//	// 输入输出都反向
//	rGPEDAT |= (1<<0);	// OUT4		
//	rGPEDAT |= (1<<1);	// OUT5		
//	rGPEDAT |= (1<<2);	// OUT6		
//	rGPEDAT |= (1<<3);	// OUT7		
//	rGPEDAT |= (1<<4);	// OUT8		
//	rGPEDAT |= (1<<5);	// OUT9		
//	rGPEDAT |= (1<<6);	// OUT10	
//	rGPEDAT |= (1<<7);	// OUT11	


}

#endif



////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////	输入   IN7-IN15//////////////////////
////	返回: 端口状态
////////////////////////////////////////////////////////////////
unsigned char Get_X_Value(unsigned char X_num)
{
	unsigned char X_value=0xff;

	switch(X_num)
	{	
		case 1:
			X_value = X1;
			break;
			
		case 2:
			X_value = X2;
			break;
//			
//		case 3:
//			X_value = (rGPFDAT >> 3) & 0x1;	// X3
//			break;
//			
//		case 4:
//			X_value = (rGPFDAT >> 4) & 0x1;	// X4
//			break;
//				
//		case 5:
//			X_value = (rGPFDAT >> 5) & 0x1;	// X5
//			break;
//				
//		case 6:
//			X_value = (rGPFDAT >> 6) & 0x1;	// X6
//			break;

//		case 7:
//			X_value = (rGPFDAT >> 7) & 0x1;	// X7
//			break;
//		case 8:
//			X_value = (rGPGDAT >> 0) & 0x1;	// X8	
//			break;
//		case 9:
//			X_value = (rGPGDAT >> 1) & 0x1;	// X9
//			break;
//		case 10:
//			X_value = (rGPGDAT >> 2) & 0x1;	// X10
//			break;
//		case 11:
//			X_value = (rGPGDAT >> 3) & 0x1;	// X11
//			break;
//		case 12:
//			X_value = (rGPGDAT >> 4) & 0x1;	// X12
//			break;
//		case 13:
//			X_value = (rGPGDAT >> 5) & 0x1;	// X13
//			break;
//		case 14:
//			X_value = (rGPGDAT >> 6) & 0x1;	// X14
//			break;
//		case 15:
//			X_value = (rGPGDAT >> 7) & 0x1;	// X15
//			break;
	}

	return X_value;// xuzhiqin tixing xiugai
}


////////////////////////////////////////////////////////////////
////	输出   Y2~Y15    /////////////////////////
////	Y_num:2~15		Value:   0或1    		//////
////////////////////////////////////////////////////////////////
void Set_Y_Value(unsigned char Y_num, unsigned char Value)
{
	Value = !Value;// xuzhiqin tixing xiugai
	
	switch(Y_num)
	{
	 case 1:
		 (Value) ? GPIO_SetBits(Y1_GPIO,Y1_GPIO_PIN): GPIO_ResetBits(Y1_GPIO,Y1_GPIO_PIN);
		break;
	 case 2:
		 (Value) ? GPIO_SetBits(Y2_GPIO,Y2_GPIO_PIN): GPIO_ResetBits(Y2_GPIO,Y2_GPIO_PIN);
		break;
	 case 3:
		 (Value) ? GPIO_SetBits(Y3_GPIO,Y3_GPIO_PIN): GPIO_ResetBits(Y3_GPIO,Y3_GPIO_PIN);
		break;
	 case 4:
		 (Value) ? GPIO_SetBits(Y4_GPIO,Y4_GPIO_PIN): GPIO_ResetBits(Y4_GPIO,Y4_GPIO_PIN);
		break;
	 case 5:
		 (Value) ? GPIO_SetBits(Y5_GPIO,Y5_GPIO_PIN): GPIO_ResetBits(Y5_GPIO,Y5_GPIO_PIN);
		break;
	 case 6:
		 (Value) ? GPIO_SetBits(Y6_GPIO,Y6_GPIO_PIN): GPIO_ResetBits(Y6_GPIO,Y6_GPIO_PIN);
		break;
	 case 7:
		 (Value) ? GPIO_SetBits(Y7_GPIO,Y7_GPIO_PIN): GPIO_ResetBits(Y7_GPIO,Y7_GPIO_PIN);
		break;
	 case 8:
		 (Value) ? GPIO_SetBits(Y8_GPIO,Y8_GPIO_PIN): GPIO_ResetBits(Y8_GPIO,Y8_GPIO_PIN);
		break;
 
//	case 9:
//		(Value) ? (rGPEDAT |= (1<<5)) : (rGPEDAT &= ~(1<<5));	// Y9	
//		break;
//	case 10:
//		(Value) ? (rGPEDAT |= (1<<6)) : (rGPEDAT &= ~(1<<6));	// Y10	
//		break;
//	case 11:
//		(Value) ? (rGPEDAT |= (1<<7)) : (rGPEDAT &= ~(1<<7));	// Y11	
//		break;
  }
	
}
