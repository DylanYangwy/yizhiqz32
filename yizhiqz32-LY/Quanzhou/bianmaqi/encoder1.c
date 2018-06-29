#include "includes.h"
U32 encoder1_speed_pulse=0;						//���ڼ���speed��������
U32	encoder1_speed=0;							//���ڼ���speed��������(�����ٶ�)��tmpֵ

//volatile unsigned int k_motor[7]={0};			//���Kֵ��ŵ�����(ʵ��)


unsigned int dapan_round=0;						//����ת�˶���Ȧ�ļ��ֵ
U16	encoder1_pulse_number=0;					//����������������������

const float k_factor[7][3]={{364,519,364},{249,309,249},{369,399,369},{264,291.6,264},{328.5,312.1,328.5},{1000,1000,1000},{1000,1000,1000}};
unsigned char jiansu_permite=1;					//���̳�����֮�����
unsigned char signal;							//��ȡX2��״ֵ̬
unsigned char speedUpFlag[7]={0};
unsigned char speedDownFlag[7]={0};

unsigned char jianshu_ewaiduan_check=0;
unsigned char reset_timer_start=0;
unsigned int speedUpCnt[7]={1};
unsigned int speedDownCnt[7]={1};
unsigned int forceEqual = 1;		//���ã���ɴ������ٵ�ʱ��ǿ�����
									//��ͨ������
unsigned int forceDownEqual = 0;
unsigned int forceUpEqual = 0;

unsigned int ewaiduan_fencen_status = 0;		//ewaiduan_fencen_status=1ʱ����ʾʹ�ö���εķֲ�

#define	reset_time_100ms				4

/*************************************************
Function(��������): encoder1_data_process(void)
Description(�������ܡ����ܵȵ�����): �������ٶȳ����趨������ٶȣ�ͨ��ͨѶ������Ƶ����ʹ���̽���
Calls (�����������õĺ����嵥): delay_qz(U8 delay_rank,U32 delay_t,U8 status);bianpingqi_set_speed(U32 speed)
Called By (���ñ������ĺ����嵥): main()

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void encoder1_data_process(void){
	yuanpan_speed=encoder1_speed*600/encoder1_cal_factor;	//��������ٶ�
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
Function(��������): getStage(void)
Description(�������ܡ����ܵȵ�����): �����Ҫ��stage�������ж�ǰ������Ҫ��kֵ��
								  ������ǰ���ٶȱ仯��������
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): bianpingqi_speed_cal();at_check();getKMotor();

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): stage������Ҫ�Ľ׶εĻ����׶�
													    direction������Ҫ�Ļ����׶εı仯�׶�
Output(�����������˵��):
Return: requestStage(����Ҫ�õ���stage)
Others: ��������˼���ǣ�����һ�������Ʊ�������ʾ�����֮������ε�����
Author:������
Modified:
Commented:����ΰ
*************************************************/

unsigned int getStage(const unsigned int stage,int direction){
	int requestStage = datouduan;
	unsigned char validRound = 0x00;
	
	//��ȡ��ǰstageֵ
	if (direction == CURRENT){
		return stage;
	}
	
	/******************��ȡ��Ч�������ж���Щ������Ч��(����ν�Ϊ����)****************/
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
	
	//����ν����⣬�����������
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

/**********�ж��Ƿ�ö�Ϊ��Ч�Σ���Ϊ��Ч���򷵻ظöΣ�
		   ��Ϊ��Ч�Σ��򷵻���һ����(ȡ����Previous/Next)************/
	for(requestStage=((int)stage + direction);;requestStage += direction){
		
		//�����߽�
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
	//����Ȧ������g_InteralMemory.Word[31]��
	g_InteralMemory.Word[31]=daduanquanshu+middlequanshu+xiaoduanquanshu+caijiaoquanshu+langfeiquanshu;
	
	//�ݴ�����ͷȦ��һ��Ҫ����
	if (daduanquanshu == 0 && xiaoduanquanshu == 0 && caijiaoquanshu == 0)
		daduanquanshu=100;
	/***��Ҫ������ν��������������Ȧ����Ϊ0������μ�����Ϊ0����֯������Ϊ0��
			 ��֯����%�������Ҫ����==0����־λΪ0
	***/
	
	
}	

/*************************************************
Function(��������): encoder1_data_reset(void)
Description(�������ܡ����ܵȵ�����):��������;������Ҫ��Ϊ�˽��������ļ�¼ֵ���㣬
								���ҽ�dapan_round���㣬����songsha_freq_change�Ϳ������¹���
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
		//lingwei_jiance_button = 1; //ѹ����� by FJW
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
		//lingwei_jiance_button = 1;		//ѹ����� by FJW
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
Function(��������): __irq	encoder1_process(void)
Description(�������ܡ����ܵȵ�����): �������жϺ���(����˶��Ĺؼ���ͨ��������ʵ���ٶȻ�����)
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: �жϺ�����Ӧ�ù��࣬�ᵼ���жϺ����޷�ִ���꣬������δ֪������ش����
	   ����Ӧ�ñ����κ��жϺ����Ŀ������������Ϊ������������Ŀ����Լ��������õĿ�����
        ѭ������(ʱ�临�Ӷ�);�ж������Ŀ���Ҳ�ڿ��Ƿ�Χ֮�ڡ�
																		����by FJW
Author:������
Modified:
Commented:����ΰ
*************************************************/
#define ABxiang
void 	encoder1_process(void)
{
	unsigned int jj;//,signal;,zushu //�ѷ�ȥ������
	static unsigned int error_times=0;
	static unsigned int reset_enter_times=0;	//δʹ��
	static unsigned int speedChangeCnt[2][7]={1};
		/**********�˴�Ϊ�������ж�(((rGPFDAT >> 1) & 0x1)��֤���������ض���������ֵ��1)
			  signal!=((rGPFDAT >> 2) & 0x1 ������������������(������ͣ����ʱ��
			  ����ʽ�����������ʣ������ⲿ��������ɱ������ź�һֱ���ڣ����µ���˶�)
			  ���ַ�������ʡȥ���ж��źŵ���ʱ���룬��Ϊͨ��������B���жϡ�
			  ���Դ˷���ֻ������˫���ص��жϡ�
	**********/
#ifdef ABxiang
		
	
#endif	
	
		if ((0) && signal!=Get_X_Value(2)){
		  Get_X_Value(2);   	//Get_X_Value(2)�����B���ź�
	
//	if (((rGPFDAT >> 1) & 0x1) && signal!=((rGPFDAT >> 2) & 0x1)){//Get_X_Value(2)
//		signal=((rGPFDAT >> 2) & 0x1);//Get_X_Value(2)�����B���ź�
		encoder1_speed_pulse++;
		encoder1_pulse_number++;	//��������������¼
#ifdef YAZHEN_NORMAL_EN
		/**��ͨѹ�빦��**/
#endif	
			/**���߹���**/
//		if(tiaoxiankaiguan_kb == 1)	{//mode_choose == tiaoxian_mode
//#ifdef TIAOXIAN_YOUFENG_EN		
//#endif
//		}
    //�ѷ���߹�����Ҫʹ��Y9/Y10��Ϊѹ���������Ե�ȥ����·���
		/**��7������Ϊ���������ν����жϣ��Լ�Сÿ��ѭ������(�ϰ벿��)**/
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
Function(��������): getKMotor(void)
Description(�������ܡ����ܵȵ�����): ��ȡ��������ĸ���ϵ��(7·���)
Calls (�����������õĺ����嵥): getStage(stage,direction)
Called By (���ñ������ĺ����嵥): songsha_fre_change();
Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): bb:��bb�������
															stage����ǰ�����Ķ�����
															direction���������CURRENT;PREVIOUS;NEXTSTAGE
															���к����������Ǹ����õĺ���getStage(stage,direction)����
Output(�����������˵��):
Return: k_factor[bb][x]*rate_different[bb][x]��������������ʦ��˵��Kֵ
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
int getKMotor(const unsigned char bb,const unsigned int stage,int direction){
	int requestStage = 0;
	requestStage = getStage(stage,direction);
	//�����Ҫ��stage�Ƕ���Σ���ô���ش�ͷ�ε��ٶȣ���Ϊ
	if (requestStage == ewaiduan){
		return (k_factor[bb][datou]*rate_different[bb][datou]);
	}
	//�ڸ����λ������Ҫ��kֵ
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
		/**�˴�Ϊ���ɶλ��߲ü��εĹ��ܣ����ڴ˶���Ϊ�����˶���
	   ���Ա���֪���俪ʼ�ٶ��Լ������ٶȣ��˴�forѭ����
	   �����ڹ��ɶκͲü���ʹ��PREVIOUS||NEXT����Ȼ����ѭ��
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
