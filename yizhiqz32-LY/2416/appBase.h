#ifndef	_APPBASE_H_
#define	_APPBASE_H_

//宏定义   
//定义输入端引脚
//
#define X1_GPIO GPIOA  //TIM3-CH2 光耦输入
#define X1_GPIO_PIN GPIO_Pin_7
#define X1 GPIO_ReadInputDataBit(X1_GPIO,X1_GPIO_PIN)  //输入高电平函数输出1
//#define KEY2_INPUT_LEVEL              GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_PIN)

#define X1_RCC_CLOCKCMD             RCC_APB2PeriphClockCmd
#define X1_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOA

#define X1_DOWN_LEVEL               0  /* 定义按键KEY2按下时是高电平为1，或者是低电平为0，具体值由原理图决定*/
#define X1_RCC_CLOCKAFIO            RCC_APB2Periph_AFIO
#define X1_GPIO_PORTSOURCE          GPIO_PortSourceGPIOA
#define X1_GPIO_PINSOURCE           GPIO_PinSource7
#define X1_EXITLINE                 EXTI_Line7
#define X1_IRQCHANNEL               EXTI9_5_IRQn
#define X1_IRQHANDLER               EXTI9_5_IRQHandler

#define X2_GPIO GPIOA//TIM3-CH1  光耦输入
#define X2_GPIO_PIN GPIO_Pin_6
#define X2 GPIO_ReadInputDataBit(X2_GPIO,X2_GPIO_PIN)  //输入高电平函数输出1

#define X3_GPIO GPIOA
#define X3_GPIO_PIN GPIO_Pin_5
#define X3 GPIO_ReadInputDataBit(X3_GPIO,X3_GPIO_PIN)  //输入高电平函数输出1

#define X4_GPIO GPIOA
#define X4_GPIO_PIN GPIO_Pin_4
#define X4 GPIO_ReadInputDataBit(X4_GPIO,X4_GPIO_PIN)  //输入高电平函数输出1

#define X5_GPIO GPIOA
#define X5_GPIO_PIN GPIO_Pin_3
#define X5 GPIO_ReadInputDataBit(X5_GPIO,X5_GPIO_PIN)  //输入高电平函数输出1

#define X6_GPIO GPIOA
#define X6_GPIO_PIN GPIO_Pin_2
#define X6 GPIO_ReadInputDataBit(X6_GPIO,X6_GPIO_PIN)  //输入高电平函数输出1

#define X7_GPIO GPIOA  //TIM2-CH2
#define X7_GPIO_PIN GPIO_Pin_1   //TIM2-CH1
#define X7 GPIO_ReadInputDataBit(FB1_GPIO,FB1_GPIO_PIN)  //输入高电平函数输出1

#define X8_GPIO GPIOA
#define X8_GPIO_PIN GPIO_Pin_0
#define X8 GPIO_ReadInputDataBit(FB1_GPIO,FB1_GPIO_PIN)  //输入高电平函数输出1

#define X9_GPIO GPIOA
#define X9_GPIO_PIN GPIO_Pin_6
#define X9 GPIO_ReadInputDataBit(FB1_GPIO,FB1_GPIO_PIN)  //输入高电平函数输出1

#define X10_GPIO GPIOE
#define X10_GPIO_PIN GPIO_Pin_5
#define X10 GPIO_ReadInputDataBit(FB1_GPIO,FB1_GPIO_PIN)  //输入高电平函数输出1

#define X11_GPIO GPIOE
#define X11_GPIO_PIN GPIO_Pin_4
#define X11 GPIO_ReadInputDataBit(FB1_GPIO,FB1_GPIO_PIN)  //输入高电平函数输出1

#define X12_GPIO GPIOE
#define X12_GPIO_PIN GPIO_Pin_3
#define X12 GPIO_ReadInputDataBit(FB1_GPIO,FB1_GPIO_PIN)  //输入高电平函数输出1

#define X13_GPIO GPIOE
#define X13_GPIO_PIN GPIO_Pin_2
#define X13 GPIO_ReadInputDataBit(FB1_GPIO,FB1_GPIO_PIN)  //输入高电平函数输出1

#define X14_GPIO GPIOE
#define X14_GPIO_PIN GPIO_Pin_1
#define X14 GPIO_ReadInputDataBit(FB1_GPIO,FB1_GPIO_PIN)  //输入高电平函数输出1

#define X15_GPIO GPIOE
#define X15_GPIO_PIN GPIO_Pin_0
#define X15 GPIO_ReadInputDataBit(FB1_GPIO,FB1_GPIO_PIN)  //输入高电平函数输出1

#define X16_GPIO GPIOD
#define X16_GPIO_PIN GPIO_Pin_7
#define X16 GPIO_ReadInputDataBit(FB1_GPIO,FB1_GPIO_PIN)  //输入高电平函数输出1

#define X17_GPIO GPIOD
#define X17_GPIO_PIN GPIO_Pin_6
#define X17 GPIO_ReadInputDataBit(FB1_GPIO,FB1_GPIO_PIN)  //输入高电平函数输出1

#define X18_GPIO GPIOD
#define X18_GPIO_PIN GPIO_Pin_5
#define X18 GPIO_ReadInputDataBit(FB1_GPIO,FB1_GPIO_PIN)  //输入高电平函数输出1


//定义输出端引脚
#define Y1_GPIO GPIOB
#define Y1_GPIO_PIN GPIO_Pin_0

#define Y2_GPIO GPIOB
#define Y2_GPIO_PIN GPIO_Pin_1

#define Y3_GPIO GPIOE
#define Y3_GPIO_PIN GPIO_Pin_8 

#define Y4_GPIO GPIOE
#define Y4_GPIO_PIN GPIO_Pin_9

#define Y5_GPIO GPIOB
#define Y5_GPIO_PIN GPIO_Pin_10

#define Y6_GPIO GPIOB
#define Y6_GPIO_PIN GPIO_Pin_5

#define Y7_GPIO GPIOE
#define Y7_GPIO_PIN GPIO_Pin_12

#define Y8_GPIO GPIOE
#define Y8_GPIO_PIN GPIO_Pin_13


#define LED1_GPIO_PIN                 GPIO_Pin_5
#define LED1_GPIO                     GPIOB

#define LED1_TOGGLE                   {LED1_GPIO->ODR ^=LED1_GPIO_PIN;}  //输出反转

#define LED2_TOGGLE                   {LED2_GPIO->ODR ^=LED2_GPIO_PIN;}  //输出反转

#define LED2_GPIO_PIN                 GPIO_Pin_6
#define LED2_GPIO                     GPIOD

#define LED3_GPIO_PIN                 GPIO_Pin_3
#define LED3_GPIO                     GPIOD



void RCC_Configuration(void);

void GPIO_Configuration(void);
void APP_init(void);

//void __irq PulseIn_0_Process(void);
void __irq PulseIn_1_Process(void);		//2016.1.2泉州，就一行
//void __irq PulseIn_2_3_Process(void);

void InPulse1_Start(void);
void InPulse1_Stop(void);
void InPulse2_Start(void);
void InPulse2_Stop(void);
void InPulse3_Start(void);
void InPulse3_Stop(void);
void InPulse4_Start(void);
void InPulse4_Stop(void);
void InPulse5_Start(void);
void InPulse5_Stop(void);
void InPulse6_Start(void);
void InPulse6_Stop(void);
void InPulse7_Start(void);
void InPulse7_Stop(void); 
void InPulse8_Start(void);
void InPulse8_Stop(void); 
void InPulse9_Start(void);
void InPulse9_Stop(void); 
void InPulse10_Start(void);
void InPulse10_Stop(void); 
void InPulse11_Start(void);
void InPulse11_Stop(void); 
void InPulse12_Start(void);
void InPulse12_Stop(void); 
void InPulse13_Start(void);
void InPulse13_Stop(void); 
void InPulse14_Start(void);
void InPulse14_Stop(void); 
void InPulse15_Start(void);
void InPulse15_Stop(void);

void Pulse_In_Init(void);

//void __irq PulseOut_0_Process(void) ;
//void PulseOut_0_Start(unsigned int frequence, unsigned int pulse_PreSet);
//void PulseOut_0_Stop(void);

//void __irq PulseOut_1_Process(void);	//2016.1.2	quanzhou

void Pulse_Out_Init(void);

void PulseOut_1_Start(unsigned int frequence, int pulse_PreSet);
void PulseOut_1_Stop(void);

void PulseOut_2_Start(unsigned int frequence, int pulse_PreSet);
void PulseOut_2_Stop(void);

void PulseOut_3_Start(unsigned int frequence, int pulse_PreSet);
void PulseOut_3_Stop(void);


void __irq DA0_10_Process(void);
void DA0_10_Start(float voltage);
void DA0_10_Stop(void);
void DA0_10_Init(void);



void X_In_Init(void);
void Y_Out_Init(void);		


void X1_EXIT_Config(void);

unsigned char Get_X_Value(unsigned char X_num);
void Set_Y_Value(unsigned char Y_num, unsigned char Value);
void Y_TOGGLE(int T_num);
//void X1_IRQHANDLER(void);
//	
////Timer0////
//int Delay_Tx_Start(unsigned long int , unsigned long int);
//void Delay_Tx_Stop(unsigned int);


#endif


