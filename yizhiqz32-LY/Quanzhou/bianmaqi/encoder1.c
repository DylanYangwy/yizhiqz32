#include "includes.h"
U32 encoder1_speed_pulse=0;						//用于计算speed的脉冲数
U32	encoder1_speed=0;							//用于计算speed的脉冲数(大盘速度)的tmp值

//volatile unsigned int k_motor[7]={0};			//电机K值存放的数组(实际)


unsigned int dapan_round=0;						//大盘转了多少圈的监测值
U16	encoder1_pulse_number=0;					//编码器发出的脉冲数计算

const float k_factor[7][3]={{364,519,364},{249,309,249},{369,399,369},{264,291.6,264},{328.5,312.1,328.5},{1000,1000,1000},{1000,1000,1000}};
unsigned char jiansu_permite=1;					//大盘超速了之后减速
unsigned char signal;							//读取X2的状态值
unsigned char speedUpFlag[7]={0};
unsigned char speedDownFlag[7]={0};

unsigned char jianshu_ewaiduan_check=0;
unsigned char reset_timer_start=0;
unsigned int speedUpCnt[7]={1};
unsigned int speedDownCnt[7]={1};
unsigned int forceEqual = 1;		//作用：送纱电机变速的时候强制相等
									//不通过变速
unsigned int forceDownEqual = 0;
unsigned int forceUpEqual = 0;

unsigned int ewaiduan_fencen_status = 0;		//ewaiduan_fencen_status=1时，表示使用额外段的分层

#define	reset_time_100ms				4

/*************************************************
Function(函数名称): encoder1_data_process(void)
Description(函数功能、性能等的描述): 当大盘速度超过设定的最大速度，通过通讯调整变频器，使大盘降速
Calls (被本函数调用的函数清单): delay_qz(U8 delay_rank,U32 delay_t,U8 status);bianpingqi_set_speed(U32 speed)
Called By (调用本函数的函数清单): main()

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void encoder1_data_process(void){
	yuanpan_speed=encoder1_speed*600/encoder1_cal_factor;	//计算大盘速度
	if (Choose_bianpingqi_kb==CHOOSE_BIANPINGQI){
		if (yuanpan_speed>=max_speed){
			delay_qz(3,15,1);
			if (delay_3_count<6)
			{
				if (jiansu_permite==1)
				{
					bianpingqi_fullspeed_set-=20;
					bianpingqi_speed=bianpingqi_fullspeed_set;
					bianpingqi_set_speed(bianpingqi_speed);
					jiansu_permite=0;
				}
			}
			else if (jiansu_permite==0 && delay_3_count>7)
				jiansu_permite=1;
		}
		
	}
}
/*************************************************
Function(函数名称): getStage(void)
Description(函数功能、性能等的描述): 获得需要的stage，用于判断前后所需要的k值，
								  以消除前后速度变化过大问题
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): bianpingqi_speed_cal();at_check();getKMotor();

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): stage：所需要的阶段的基础阶段
													    direction：所需要的基础阶段的变化阶段
Output(对输出参数的说明):
Return: requestStage(所需要得到的stage)
Others: 本函数的思想是：制造一个二进制变量来表示设参数之后各个段的有无
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/

unsigned int getStage(const unsigned int stage,int direction){
	int requestStage = datouduan;
	unsigned char validRound = 0x00;
	
	//获取当前stage值
	if (direction == CURRENT){
		return stage;
	}
	
	/******************获取有效段数，判断哪些段是有效的(额外段较为特殊)****************/
	if (daduanquanshu != 0){
		validRound |= 1<<datouduan;
	}	
	if (middlequanshu != 0){
		validRound |= 1<<guoduduan;
	}	
	if (xiaoduanquanshu != 0){
		validRound |= 1<<xiaotouduan;
	}	
	if (caijiaoquanshu != 0){
		validRound |= 1<<fencenduan;
	}	
	if (langfeiquanshu != 0){
		validRound |= 1<<caijianduan;
	}
	
	//额外段较特殊，会有两种情况
	if(extra_part_quanshu != 0){
		if (direction == NEXTSTAGE){
			if (extra_part_quanshu!=0 && extra_part_jiansu!=0 && jianshu!=0 && (jianshu+1)%extra_part_jiansu==0){
				
				validRound |= 1<<ewaiduan;
			}
		}
		else{
			if (extra_part_quanshu!=0 && extra_part_jiansu!=0 && jianshu!=0 && (jianshu)%extra_part_jiansu==0){
				
				validRound |= 1<<ewaiduan;
			}
		}
	}

/*************************************************************************/

/**********判断是否该段为有效段，若为有效段则返回该段，
		   若为无效段，则返回下一个段(取决于Previous/Next)************/
	for(requestStage=((int)stage + direction);;requestStage += direction){
		
		//两个边界
		if (requestStage < datouduan){
			requestStage = ewaiduan;
		}
		if (requestStage > ewaiduan){
			requestStage = datouduan;
		}
		
		if ( validRound & (1<<requestStage) ){
			return requestStage;
		}			
	}	
}


void parameter_read(void)
{}
void parameter_save(void)
{}
void songsha_fre_change(void)
{unsigned char bb;
	int i;
	wdt_feed_dog();main_enter_flag = 1;
	//所有圈数放在g_InteralMemory.Word[31]中
	g_InteralMemory.Word[31]=daduanquanshu+middlequanshu+xiaoduanquanshu+caijiaoquanshu+langfeiquanshu;
	
	//容错：将大头圈数一定要设置
	if (daduanquanshu == 0 && xiaoduanquanshu == 0 && caijiaoquanshu == 0)
		daduanquanshu=100;
	/***重要：额外段进入条件：额外段圈数不为0，额外段件数不为0，已织件数不为0，
			 已织件数%额外段需要件数==0，标志位为0
	***/
	
	
}	

/*************************************************
Function(函数名称): encoder1_data_reset(void)
Description(函数功能、性能等的描述):参数清零;清零主要是为了将编码器的记录值清零，
								并且将dapan_round清零，这样songsha_freq_change就可以重新工作
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
void encoder1_data_reset(void){
	if(encoder1_dangqianjian_reset==1)
	{
		dapan_round=dapan_round_save=0;
		encoder1_pulse_number=encoder1_pulse_number_save=0;
		
		extra_part_finish_flag=extra_part_unfinish;
		extra_part_flag=extra_part_stop;
		ewaiduan_fencen_status = 0;
		
		if (encoder1_speed == 0)
			forceEqual = 1;
		speed_status = 0;
		
//		if (tiaoxiankaiguan_kb == 1){
			#ifdef TIAOXIAN_YOUFENG_EN
			#elif defined TIAOSHA_NORMAL_EN
			#else			
			#endif
//		}
		#ifdef YAZHEN_NORMAL_EN
		#endif
		//lingwei_jiance_button = 1; //压针回零 by FJW
	}
	if (encoder1_jianshu_reset==1)
		jianshu=0;
	if (encoder1_zonggongquanshu_reset==1)
	{
		encoder1_pulse_number=encoder1_pulse_number_save=0;
		dapan_round=dapan_round_save=0;
		
		extra_part_finish_flag=extra_part_unfinish;
		extra_part_flag=extra_part_stop;
		ewaiduan_fencen_status = 0;
		if (encoder1_speed == 0)
			forceEqual = 1;
		speed_status = 0;
		//lingwei_jiance_button = 1;		//压针回零 by FJW
//		if (tiaoxiankaiguan_kb == 1){
//			#ifdef TIAOXIAN_YOUFENG_EN
//			#elif defined TIAOSHA_NORMAL_EN
//			#endif
//		}
		#ifdef YAZHEN_NORMAL_EN
		#endif
		jianshu=0;
	}
	if (songshaliang_data_reset==1)
	{
		banci_mima_shuru=KeyBoard(999999,0,0,0);
		if (banci_mima_shuru==-1)
			banci_mima_shuru=0;
		else if (banci_status_kw==s_ban || banci_mima_shuru==*banci_mima[banci_status_kw])
		{
			songsha1_num=songsha_num[0]=0;
			songsha2_num=songsha_num[1]=0;
			songsha3_num=songsha_num[2]=0;
			songsha4_num=songsha_num[3]=0;
			*banci[xiada_no]=0;
			*banci[shangda_no]=0;
			*banci[xiaxiao_no]=0;
			*banci[shangxiao_no]=0;
			*banci[jianshu_no]=0;
			banci_mima_shuru=0;
		}
		else if (banci_mima_shuru!=*banci_mima[banci_status_kw])
		{
			banci_mima_shuru=0;
			g_InteralMemory.Word[PING_CTRL]=30;
		}
	}
	if (total_qingling_b==1)
	{
		total_quanshu=0;
		xitong_total_num_upper_kw=0;
	}
}

/*************************************************
Function(函数名称): __irq	encoder1_process(void)
Description(函数功能、性能等的描述): 编码器中断函数(电机运动的关键，通过编码器实现速度环控制)
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 中断函数不应该过多，会导致中断函数无法执行完，而导致未知结果的重大错误。
	   所以应该避免任何中断函数的开销，具体表现为：浮点型运算的开销以及函数调用的开销；
        循环次数(时间复杂度);判断条件的开销也在考虑范围之内。
																		——by FJW
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
#define ABxiang
void 	encoder1_process(void)
{
	unsigned int jj;//,signal;,zushu //友峰去除组数
	static unsigned int error_times=0;
	static unsigned int reset_enter_times=0;	//未使用
	static unsigned int speedChangeCnt[2][7]={1};
		/**********此处为上升沿中断(((rGPFDAT >> 1) & 0x1)保证了其上升沿读出来的数值是1)
			  signal!=((rGPFDAT >> 2) & 0x1 此条件用来进行消抖(具体是停机的时候，
			  增量式编码器的性质，导致外部抖动会造成编码器信号一直存在，导致电机运动)
			  此种方法可以省去对中断信号的延时进入，因为通过编码器B相判断。
			  所以此方法只能用于双边沿的中断。
	**********/
#ifdef ABxiang
		
	
#endif	
	
		if ((0) && signal!=Get_X_Value(2)){
		  Get_X_Value(2);   	//Get_X_Value(2)，获得B相信号
	
//	if (((rGPFDAT >> 1) & 0x1) && signal!=((rGPFDAT >> 2) & 0x1)){//Get_X_Value(2)
//		signal=((rGPFDAT >> 2) & 0x1);//Get_X_Value(2)，获得B相信号
		encoder1_speed_pulse++;
		encoder1_pulse_number++;	//编码器脉冲数记录
#ifdef YAZHEN_NORMAL_EN
		/**普通压针功能**/
#endif	
			/**调线功能**/
//		if(tiaoxiankaiguan_kb == 1)	{//mode_choose == tiaoxian_mode
//#ifdef TIAOXIAN_YOUFENG_EN		
//#endif
//		}
    //友峰调线功能需要使用Y9/Y10作为压针电机，所以得去除两路电机
		/**将7组电机分为上下沿两次进行判断，以减小每次循环次数(上半部分)**/
//	for (jj=4;jj<8;jj++)
//		{
//			if (jj == 4){
//				motor_factor[jj] += k_motor[jj];
//				if (motor_factor[jj]>=1000000)
//				{
//					
//					Set_Y_Value(jj,0);
//	//				rGPEDAT &= ~(1<<jj);//
//					motor_factor[jj] -= 1000000;
//					songsha_num[jj]++;
//				}
//			}
//			/**/
//			if (motor_factor[jj - 4] >= 500000 || k_motor[jj - 4] >= 500000){
//				
//	    	Set_Y_Value(jj,1);
//	//			rGPEDAT |= (1<<(jj - 4));
//			}
//		}		
		/**************************************************************/
		//rGPEDAT |= (0xf);
	
	}
}

/*************************************************
Function(函数名称): getKMotor(void)
Description(函数功能、性能等的描述): 获取各个电机的跟踪系数(7路电机)
Calls (被本函数调用的函数清单): getStage(stage,direction)
Called By (调用本函数的函数清单): songsha_fre_change();
Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): bb:第bb个电机；
															stage：当前所处的段数；
															direction：可填参数CURRENT;PREVIOUS;NEXTSTAGE
															其中后两个参数是给调用的函数getStage(stage,direction)所用
Output(对输出参数的说明):
Return: k_factor[bb][x]*rate_different[bb][x]————即蔡老师所说的K值
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
int getKMotor(const unsigned char bb,const unsigned int stage,int direction){
	int requestStage = 0;
	requestStage = getStage(stage,direction);
	//如果需要的stage是额外段，那么返回大头段的速度，因为
	if (requestStage == ewaiduan){
		return (k_factor[bb][datou]*rate_different[bb][datou]);
	}
	//在各个段获得所需要的k值
	else if (requestStage != guoduduan && requestStage != caijianduan){
		if (requestStage == datouduan)
			return (k_factor[bb][datou]*rate_different[bb][datou]);
		else if (requestStage == xiaotouduan)
			return (k_factor[bb][xiaotou]*rate_different[bb][xiaotou]);
		else if (requestStage == fencenduan)
			return (k_factor[bb][fencen]*rate_different[bb][fencen]);
		else
			return (k_factor[bb][datou]*rate_different[bb][datou]);
	}
		/**此处为过渡段或者裁剪段的功能，由于此二段为变速运动，
	   所以必须知道其开始速度以及结束速度，此处for循环，
	   必须在过渡段和裁剪段使用PREVIOUS||NEXT，不然会死循环
	**/
		else{
			for (;requestStage == guoduduan || requestStage == caijianduan;){
			requestStage = getStage(requestStage,direction);
		}
		if (requestStage == datouduan)
			return (k_factor[bb][datou]*rate_different[bb][datou]);
		else if (requestStage == xiaotouduan)
			return (k_factor[bb][xiaotou]*rate_different[bb][xiaotou]);
		else if (requestStage == fencenduan)
			return (k_factor[bb][fencen]*rate_different[bb][fencen]);
		else
			return (k_factor[bb][datou]*rate_different[bb][datou]);
	}

}

int KeyBoard(int Max,int Min,U8 DigitPos,U8 DigitNumb)
{
}
