
#include "PVD.h"
#include "stdio.h"
#include "bsp_beep.h"
//extern  volatile u16 Word_Buffer[SIZE_OF_WORD][4];
//extern  volatile u16 Bit_Buffer[SIZE_OF_BIT];

//extern  unsigned char write_data_reg[SIZE_OF_WORD][4];
//extern  unsigned char write_bit_reg[SIZE_OF_BIT];

extern void BEEP_Init(void);
u16 PVD_Word_Buffer[SIZE_OF_PVD_WORD][4] = {0};
u16 PVD_Bit_Buffer[SIZE_OF_PVD_BIT] = {0};

void PVD_Init(void)
{  
   
	 NVIC_InitTypeDef NVIC_InitStruct;
   EXTI_InitTypeDef EXTI_InitStructure;
	
//	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); //_PWR, ENABLE); // 这个是必需的
   PWR_PVDLevelConfig(PWR_PVDLevel_2V9);  
	 PWR_PVDCmd(ENABLE); 
	
	 NVIC_InitStruct.NVIC_IRQChannel = PVD_IRQn;
   NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
   
	 NVIC_Init(&NVIC_InitStruct);//允许中断
	
		EXTI_ClearITPendingBit(EXTI_Line16);
		EXTI_InitStructure.EXTI_Line = EXTI_Line16; 
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//EXTI_Trigger_Rising_Falling   ;EXTI_Trigger_Rising////EXTI_Trigger_Rising; //EXTI_Trigger_Falling
		EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
		EXTI_Init(&EXTI_InitStructure);

}

//PVD中断函数如下
void PVD_IRQHandler(void)
{
		
				BEEP_ON;	
	

//		if(PWR_GetFlagStatus(PWR_FLAG_PVDO)) //
//		 {
//			PWR_ClearFlag(PWR_FLAG_PVDO);	//别人写的放在这里
////			STMFLASH_Write(FLASH_SAVE_WORD_ADDR,(u16*)write_data_reg,sizeof(write_data_reg));//FLASH_SAVE_BIT_PVD_ADDR

//		 }	

		EXTI_ClearITPendingBit(EXTI_Line16); //放在上面保证下面没做完也要把中断清了，不然上电进中断了
}
//static void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)
//{


//}
