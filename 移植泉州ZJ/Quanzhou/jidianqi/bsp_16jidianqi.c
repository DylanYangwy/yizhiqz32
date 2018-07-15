#include "bsp_16jidianqi.h"

#define jidianqi_station_num	22

INTERALMEMORY g_InteralMemory;


U8 jdqComCount[8]={0};




/*************************************************
Function(��������): qz_Uart_Getch(void)
Description(�������ܡ����ܵȵ�����): ���ڻ���ַ�:�˺�����ͨ��Uart_Getch(void)��ֲ������
								 ����ͨѶЭ��read��
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:������
Commented:����ΰ
*************************************************/
unsigned char qz_Uart_Getch(void)
{
//	//INT8U temp=0;
//	INT8U temp=0xff;
//	INT16U Flag_Comm = 0;	
//	int i;
//	U32 ErrorLoop;

//	wdt_feed_dog();main_enter_flag = 1;
//	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;

////	while(!(rUTRSTAT3 & 0x1) && Flag_Comm<ErrorLoop) //Receive data read
//  //UART  TX/RX ״̬�Ĵ���  0 0 1 ���յ�����
//	while(Flag_Comm<ErrorLoop&&(USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == SET))
//	{
//		for(i=0;i<50;i++);	//������50��ʱ��Ļ�׼��
//		Flag_Comm++;    //  
//	}
//	if(Flag_Comm >= ErrorLoop)
//	{
//		temp=0xff;
//	}
//	else
//	{
//		/*temp=rUERSTAT3;
//		if (temp!=0)
//			return 0xff;
//		else */
////		temp=RdURXH3(); //����С�δ������룿��
//		
//		g_Timer2Uart.Timer_Cur = 0;//����
//	}
//	return temp;
  char temp=0;
	temp=USART_ReceiveChar(RS485_USARTx);
	
	return temp;
	
}
u8 CharToData(u8 Data)
{
	u8 DataVar;
	if(Data>=48 && Data<=57)
	{
		DataVar=Data-48;
	}
	else
	{
		DataVar=Data-55;
	}
	return DataVar;
}
	
/*************************************************
Function(��������): jidianqi_write_single(U8 which_port,U8 button_bit)
Description(�������ܡ����ܵȵ�����): �̵������ͨѶЭ��
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
void jidianqi_write_single(U8 which_port,U8 button_bit)
{
	U8 auchMsg[8],SendArray[8],RecArray[8];  
	U8 Count,jdqCheck,waitTime;
	//
	int i;
	U32 ErrorLoop;
	
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;
	
	    /* ���뷢��ģʽ */
    GPIO_SetBits(RS485_REDE_PORT,RS485_REDE_PIN); 
	
	//rUBRDIV3 = ( (int)(SYS_PCLK/16./57600+0.5) -1 );
	//Uart_Init(0,9600);
	
	
//			if(USART_RX_STA&0x8000)
//		{					   
//			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//			
//			for(t=0;t<len;t++)
//			{
//				RecArray[t] = (USART_RX_BUF[t]);
//			}
//.	if (rULCON3!=0x03) 
//.		rULCON3 =0x03; 								//0x03=00 000 0 011  ��ͨ ��У�� 1ֹͣλ 8����λ

 
	auchMsg[0]=jidianqi_station_num;
	auchMsg[1]=jidianqi_write_single_fun_num;		//3492~3491  2016.1.3 quanzhou
	auchMsg[2]=0x00;
	auchMsg[3]=which_port;
	if (g_InteralMemory.Bit[button_bit]==1)
	{
		auchMsg[4]=0xff;
		if (which_port!=dianci)
			g_InteralMemory.Bit[button_bit+9]=1;
	}
	else
	{
		auchMsg[4]=0x00;
		if (which_port!=dianci)
			g_InteralMemory.Bit[button_bit+9]=0;
	}
	auchMsg[5]=0x00;
	auchMsg[6]=(CRC1(auchMsg,6)) & 0xff;
	auchMsg[7]=(CRC1(auchMsg,6))>>8;
	for (i=0;i<8;i++)
	{
		SendArray[i]=auchMsg[i];
	}
//.	rGPHDAT |= 0x1000;	//GPH12	+Write
	GPIO_ResetBits(RS485_REDE_PORT,RS485_REDE_PIN);	
	Delay(DELAY_TIME_RTU);
	
	for (Count=0;Count<8;Count++)
	{
//		while((!(rUTRSTAT3 & 0x4)) && (waitTime<=ErrorLoop)){
		//UTRSTAT 100 ���ͻ���Ĵ�������Ч���ݷ��Ͳ��ҷ�����λ�Ĵ���Ϊ��ʱ
		//���������������û��ERRORLOOPʱ��
		while (waitTime<=ErrorLoop){
		for (i=0;i<50;i++){
				waitTime++;wdt_feed_dog();main_enter_flag = 1;
			}
		}
		waitTime=0;
		USART_SendChar(RS485_USARTx,SendArray[Count]);
		
//.		WrUTXH3(SendArray[Count]);wdt_feed_dog();main_enter_flag = 1;
//		while((!(rUTRSTAT3 & 0x4)) && (waitTime<=ErrorLoop)){
		
//		while((waitTime<=ErrorLoop)&&){//��
//		for (i=0;i<50;i++){
//				waitTime++;wdt_feed_dog();main_enter_flag = 1;
//			}
//		}
	}
	
	GPIO_ResetBits(RS485_REDE_PORT,RS485_REDE_PIN);//GPH12	-Read
//.	rGPHDAT &= 0xefff;	//GPH12	-Read
	Count = 0;//��ʼ��+��һ��
	
	/**20�ζ�ȡuart��ֵ������ֱֵ���˳�ѭ��
	���յĵ�һ����char��ջ�Ų�һ��
	**/
	while ((RecArray[0]=qz_Uart_Getch())!=jidianqi_station_num && Count<=20)
	{
		Count++;
	}
	
	/***ջ����ȷ֮��jdqcheck�ж϶�����֡�Ƿ���4������ȷ�ģ�
		�������ȷ����ôֱ�ӽ�status��1�����ⲿ������ͨѶ
		eg��if (hongdeng_button!=hongdeng_status)
	***/
	
	if (RecArray[0]==jidianqi_station_num){
		for (Count=1;Count<8;Count++)
		{
			RecArray[Count]=qz_Uart_Getch();
			if (RecArray[Count]==auchMsg[Count]){//����ж�֮ǰ�Լ���֪��ѽ
				jdqCheck++;
			}
		}
		if (jdqCheck>=4){//���4��
			if (g_InteralMemory.Bit[button_bit]==1)	//button == 1
				g_InteralMemory.Bit[button_bit+9]=1;//status == 1
			else
				g_InteralMemory.Bit[button_bit+9]=0;
			jdqComCount[which_port]=0;
		}
		else
			jdqComCount[which_port]++;
	}
	
	/***ջ��һֱ����ȷ��jdqComCount[which_port]���ж�5�Σ�����ͨѶЭ������5�Σ�
		ֱ�ӽ�status��Ϊ����Ҫ��λ���Ա�֤�ⲿ������ͨѶ
		eg��if (hongdeng_button!=hongdeng_status)
	***/
	else
		jdqComCount[which_port]++;
	if (jdqComCount[which_port]>=5){//
		if (g_InteralMemory.Bit[button_bit]==1)
			g_InteralMemory.Bit[button_bit+9]=1;
		else
			g_InteralMemory.Bit[button_bit+9]=0;
		jdqComCount[which_port]=0;
	}
	
	jdqComCount[which_port]=0;

}



/*************************************************
Function(��������): jidianqi_write_fenshan(U8 which_port)
Description(�������ܡ����ܵȵ�����): ���ȵ�ͨѶЭ��(ûʲô��)������ע�Ͳο���һ������
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:������
Commented:����ΰ
*************************************************/
void jidianqi_write_fenshan(void)
{
		int i;
	U8 auchMsg[8],SendArray[8];
	U8 Count,waitTime;
		U32 ErrorLoop;
//	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;
	//3000  *19200 /115200 
	ErrorLoop = ERROR_NUM*19200/RS485_USARTx_BAUDRATE;
	
	auchMsg[0]=22;
	auchMsg[1]=0x05;		//3492~3491  2016.1.3 quanzhou
	auchMsg[2]=0x00;
	auchMsg[3]=0x01;
//	if (g_InteralMemory.Bit[button_bit]==1)
//	{
//		auchMsg[4]=0xff;
//		if (which_port!=dianci)
//			g_InteralMemory.Bit[button_bit+9]=1;
//	}
//	else
//	{
		auchMsg[4]=0x00;
//		if (which_port!=dianci)
//			g_InteralMemory.Bit[button_bit+9]=0;
//	}
	auchMsg[5]=0x00;
	auchMsg[6]=(CRC1(auchMsg,6)) & 0xff;
	auchMsg[7]=(CRC1(auchMsg,6))>>8;
			for (i=0;i<8;i++)
	{
		SendArray[i]=auchMsg[i];
	}
	GPIO_SetBits(RS485_REDE_PORT,RS485_REDE_PIN); 
  Delay(DELAY_TIME_RTU);
	for (Count=0;Count<8;Count++)
	{
    waitTime=0;
		USART_SendChar(RS485_USARTx,SendArray[Count]);
		while((USART_GetFlagStatus(RS485_USARTx,USART_FLAG_TC)!=SET)&&(waitTime<=ErrorLoop)){
			for (i=0;i<50;i++){
				waitTime++;wdt_feed_dog();main_enter_flag = 1;
			}	
		}

	}
}
/*************************************************
Function(��������): xxxx_fun(void)
Description(�������ܡ����ܵȵ�����): �����ⲿ�豸(���˵�ŷ���ͨ��ͨѶ���)��
								�жϽ��������if (hongdeng_button!=hongdeng_status)����ͨѶ�л���λ��
								������ɹ�����ͨѶ��������Ҫע����ǣ�hongdeng_buttonΪ�ڲ��ı��
								����(�������һ����������)�������ⲿ��ť��
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:jdqComCount[dianci]����ʵ�����壬����ȫ��ע��;by FJW 2018.1.10
Commented:����ΰ
*************************************************/
void hongdeng_fun(void)
{
	if (hongdeng_button!=hongdeng_status)// && jdqComCount[dianci]==0
		jidianqi_write_single(hongdeng,hongdeng_button_bit);
}

void huangdeng_fun(void)
{
	if (huangdeng_button!=huangdeng_status)// && jdqComCount[dianci]==0
		jidianqi_write_single(huangdeng,huangdeng_button_bit);
}

void lvdeng_fun(void)
{
	if(lvdeng_button!=lvdeng_status )//&& jdqComCount[dianci]==0
		jidianqi_write_single(lvdeng,lvdeng_button_bit);
}

void zhaobudeng_fun(void)
{
	if(zhaobudeng_button!=zhaobudeng_status )//&& jdqComCount[dianci]==0
		jidianqi_write_single(zhaobudeng,zhaobudeng_button_bit);
}

void zhaomingdeng_fun(void)
{
	if (zhaomingdeng_button!=zhaomingdeng_status )//&& jdqComCount[dianci]==0
		jidianqi_write_single(zhaomingdeng,zhaomingdeng_button_bit);
}

void dianci_fun(void)
{
	//ֻ��һ�ε�ŷ������ܹ�һֱ������Ȼ���ܻᷢ��(��ʵһֱSetҲ����ν)
	if(dianci_button!=dianci_status){
		Set_Y_Value(11,dianci_button);
		dianci_status=dianci_button;
		//jidianqi_write_single(dianci,dianci_button_bit);
	}
		
}

void youbeng_fun(void)
{
	if(youbeng_button!=youbeng_status)// && jdqComCount[dianci]==0
		jidianqi_write_single(youbeng,youbeng_button_bit);
}

void penyou24v_fun(void)
{
	if(penyou24v_button!=penyou24v_status)// && jdqComCount[dianci]==0
		jidianqi_write_single(penyou24v,penyou24v_button_bit);
}

void fenshan_fun(void)
{
	if(fenshan_button!=fenshan_status)// && jdqComCount[dianci]==0
//		jidianqi_write_fenshan();//�˺����Ķ���
	;
}



/*************************************************
Function(��������): jidianqi_fun(void)
Description(�������ܡ����ܵȵ�����): ͨѶд�룬ʹ�ü̵�������(���˵�ŷ�����)
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:������
Commented:����ΰ
*************************************************/
void jidianqi_fun(void)
{
	hongdeng_fun();
	huangdeng_fun();
	lvdeng_fun();
	zhaobudeng_fun();
	zhaomingdeng_fun();
	dianci_fun();
	//youbeng_fun();
	penyou24v_fun();
	//fenshan_fun();
}



