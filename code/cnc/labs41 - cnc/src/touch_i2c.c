/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards/MasterTransmitterInterrupt/main.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "touch_i2c.h"
#include "data_io.h"
#include "buzzer.h"
#include "delay.h"
#include "temp_ic_spi.h"


/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup MasterTransmitterInterrupt
  * @{
  */ 


/* Private typedef -----------------------------------------------------------*/
# define Timeout_value   0x00FF


#define	TOUCH_TEST_ON	0



#define	main_sel	0

#define	co2_sel		1
#define	o2_sel		2
#define	temp_sel	3

#define date_sel    4
#define alarm_sel   5
#define log_sel     6
#define voc_sel     7

#define	new_co2_sel		8
#define	new_o2_sel		9
#define	work_time_sel	10
#define	pressure_set	11
#define	wet_mode_set	12
#define	cham_no_set		13
#define	ext_temp_cal		14
#define	pressure2_set	15

#define	password_set	16
#define alarm_list     		 17
#define device_set      		18
#define	gas_1_setting		19
#define	gas_2_setting		20






/* Private define ------------------------------------------------------------*/
#define SLAVE_ADDRESS 0x38 // the slave address (example)


#define Touch_set_mode(x, y)	( Touch_x_data>(x) ) && ( Touch_x_data<(x)+89 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+83 )
#define Touch_cham(x, y)		( Touch_x_data>(x) ) && ( Touch_x_data<(x)+132 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+132 )	

#define Touch_incdec2(x, y)		( Touch_x_data>(x) ) && ( Touch_x_data<(x)+109 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+51 )	
#define Touch_all_cham(x, y)	( Touch_x_data>(x) ) && ( Touch_x_data<(x)+119 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+109 )	
#define Touch_save(x, y)		( Touch_x_data>(x) ) && ( Touch_x_data<(x)+117 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+111 )	

#define	Touch_set_tab(x, y)		( Touch_x_data>(x) ) && ( Touch_x_data<(x)+157 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+51 )

#define Touch_set_small(x, y)	( Touch_x_data>(x) ) && ( Touch_x_data<(x)+59 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+59 )	
#define Touch_set_clear(x, y)	( Touch_x_data>(x) ) && ( Touch_x_data<(x)+79 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+59 )	

#define	Touch_move_rl(x, y)		( Touch_x_data>(x) ) && ( Touch_x_data<(x)+125 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+27 )
#define	Touch_move_ud(x, y)		( Touch_x_data>(x) ) && ( Touch_x_data<(x)+27 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+125 )

#define	Touch_set_time(x, y)	( Touch_x_data>(x) ) && ( Touch_x_data<(x)+219 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+49 )

#define Touch_set_chan(x, y)	( Touch_x_data>(x) ) && ( Touch_x_data<(x)+59 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+35 )	

#define Touch_pass_chan(x, y)	( Touch_x_data>(x) ) && ( Touch_x_data<(x)+79 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+59 )	






#define Touch_input_pad(x, y)	( Touch_x_data>(x) ) && ( Touch_x_data<(x)+39 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+59 )	
#define Touch_mark(x, y)	( Touch_x_data>(x) ) && ( Touch_x_data<(x)+101 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+45 )	


#define Touch_in(x, y)			( Touch_x_data>(x) ) && ( Touch_x_data<(x)+80 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+80 )	
//#define Touch_incdec(x, y)		( Touch_x_data>(x) ) && ( Touch_x_data<(x)+91 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+57 )	

#define Touch_set_move(x, y)	( Touch_x_data>(x) ) && ( Touch_x_data<(x)+100 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+24 )	
#define Touch_set_arr(x, y)	    ( Touch_x_data>(x) ) && ( Touch_x_data<(x)+24 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+100 )	

#define Touch_set_temp(x, y)	 ( Touch_x_data>(x) ) && ( Touch_x_data<(x)+121 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+113 )	
#define Touch_set_status(x, y)	 ( Touch_x_data>(x) ) && ( Touch_x_data<(x)+93 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+93 )	
#define Touch_set_alarm(x, y)		( Touch_x_data>(x) ) && ( Touch_x_data<(x)+168 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+50 )	

#define Touch_error(x, y)			( Touch_x_data>(x) ) && ( Touch_x_data<(x)+100 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+44 )	
#define Touch_set_sub(x, y)			( Touch_x_data>(x) ) && ( Touch_x_data<(x)+184 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+32 )	
#define Touch_set_sub2(x, y)			( Touch_x_data>(x) ) && ( Touch_x_data<(x)+136 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+32 )	

#define Touch_set_box(x, y)			( Touch_x_data>(x) ) && ( Touch_x_data<(x)+30 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+24 )	
#define Touch_set_replace(x, y)			( Touch_x_data>(x) ) && ( Touch_x_data<(x)+154 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+94 )	
#define Touch_check_box(x, y)			( Touch_x_data>(x) ) && ( Touch_x_data<(x)+50 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+50 )	

#define Touch_onoff_dot(x, y)	( Touch_x_data>(x) ) && ( Touch_x_data<(x)+50 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+25 )	



#define Touch_test(x, y)			( Touch_x_data>(x) ) && ( Touch_x_data<(x)+43 ) && ( Touch_y_data>(y) ) && ( Touch_y_data<(y)+30 )	



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t Touch_x_data = 0;
uint16_t Touch_y_data = 0;

uint16_t Touch_x_input = 0;
uint16_t Touch_y_input = 0;

uint16_t Touch_count = 0;
uint8_t buzzer_set= 0;

uint8_t touch_flag1= 0;
uint8_t touch_flag2= 0;

uint16_t	x_12bit, y_12bit	;

uint8_t temp_id_check	= 0;

uint8_t cali_set = 0;

uint8_t wet_change = 0;

uint8_t pass_val= 0;

uint8_t touch_version;

uint8_t lock_data = 0;

extern uint8_t  cham_working_sta[8] ;				//배양상태 0= 빈상태 1=배양중
extern uint8_t 	cham_temp_set_mode	 ;				//온도 세팅 상태 0= 비활상화 1=세팅모드 2= 세팅모드(증감키를 누르
extern uint8_t 	cham_setting_sta[8] ;				//개별 챔버 온도 세팅 상태
extern uint16_t  cham_set_temp_dis[8] ;				//챔버 0~6 의 세팅온도 디스플레이값
extern uint16_t  cham_set_temp[8] ;					//챔버 0~6 의 세팅온도
extern uint16_t cham_all_set_temp ;					//전체 챔버 세팅값
extern uint16_t	o2_set_data ;						//o2 세팅 데이타
extern uint16_t	co2_set_data ;						//co2 세팅 데이타

extern uint8_t 	mode_value ;		//세팅 모드 값 0=nomal / 1= co2 /2 = o2/3= temp
extern uint8_t	sub_set_mode ;			// 서브 세팅 모드

extern uint8_t Error_no ;			//ERROR 번호
extern uint8_t Sub_error;			//CHAMBER 번호
extern uint8_t Error_EN;
extern uint8_t	test_mode;

extern uint8_t change_sta;

extern int16_t 	voc_filter_time;					//전체 챔버 세팅값

extern uint8_t set_year, set_week, set_month, set_date, set_hour, set_min, set_sec;
extern uint8_t time_year, time_week, time_month, time_date, time_hour, time_min, time_sec;

extern uint8_t voc_last_year, voc_last_month, voc_last_date, voc_last_hour, voc_last_min  ;					//전체 챔버 세팅값
extern uint8_t last_valid_time;

extern uint16_t	set_data_temp;		//임시 세팅 데이타
extern uint16_t	set_data_temp2;		//임시 세팅 데이타
extern uint16_t	set_data_temp3;		//임시 세팅 데이타
extern uint16_t	set_data_temp4;		//임시 세팅 데이타
extern uint16_t	set_data_temp5;		//임시 세팅 데이타
extern uint16_t	set_data_temp6;		//임시 세팅 데이타
extern uint16_t	set_data_temp7;		//임시 세팅 데이타


extern uint8_t lcd_popup_flag;



extern uint16_t	setting_data_page;		//세팅시 그래프 페이지 번호
extern uint8_t	set_ch_no;			//세팅시 그래프 채널 번호
extern uint8_t	set_posit;

extern uint16_t	set_data_adr ;	//데이타 로드 기본 주소
extern uint8_t	set_dis_refresh ;	//세팅 디스츨레이 리프레쉬

extern uint8_t	error_set_save[22];
extern uint8_t	out_usb_mode[6];

extern uint8_t voc_valid_time;
extern uint8_t change_alarm_time;

extern uint16_t	gas_initial_time;
extern uint16_t	gas_period_time;
extern uint16_t	gas_work_time;
extern uint8_t	pass_data[4];
extern uint8_t	save_pass_data[4];

extern uint16_t	gas2_start_time ;
extern uint16_t	gas2_initial_time ;
extern uint16_t	gas2_period_time  ;
extern uint16_t	gas2_work_time ;
extern uint16_t	gas1_gas2_dual_time;

extern uint16_t	o2_2nd_set_data;						//o2 세팅 데이타
extern uint16_t	co2_2nd_set_data;						//co2 세팅 데이타
extern uint8_t	gas_2nd_on_off;						//

extern uint16_t Press_on_data;
extern uint16_t Press_off_data;
extern uint16_t Press_err_data;
extern uint16_t Press_err_cnt;

extern uint16_t Press2_on_data;
extern uint16_t Press2_off_data;
extern uint16_t Press2_err_data;
extern uint16_t Press2_err_cnt;

extern uint16_t main_log_flag ;
extern uint16_t sub_log_flag1 ;
extern uint16_t sub_log_flag2 ;

extern uint8_t	usb_out_flag;


extern uint8_t test_top_pwm[8];
extern uint8_t test_bot_pwm[8];
extern uint8_t test_door_led[8];		//문상태 0= x 1=red 2= green 3= orange

extern uint8_t test_out1_work[8];
extern uint8_t test_out2_work[8];

extern uint8_t device_name[4];
extern uint8_t device_temp[4];


extern uint8_t wet_mode;
extern uint8_t wet_mode_temp;
extern uint8_t	chamber_count;		//0 =8 chamber 1= 6 chamber

extern uint8_t test_Tx_data;
extern uint8_t test_Rx_data;
extern uint8_t test_Tx_sta;





/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void cham_touch_first(void);
void error_clear(void);
void cham_touch_continue(void);



void Touch_test_mode_Check(void);
void cham_temp_inc(void);
void cham_temp_dec(void);
void cham_temp_save(void);
void temp_inc_dec(void);
void cham_cali_save(uint8_t ch_dat, uint8_t ch_pos, uint16_t set_temp);


void Touch_Nomal_Check(void);
void Touch_Set_mode_Check(void);
void Touch_Set_mode_Check2(void);
void Touch_Set_mode_Check3(void);
void Touch_Set_mode_Check4(void);
void Touch_Set_mode_Check5(void);
void Touch_Set_mode_Check6(void);


void setting_touch_f0(void);
void setting_touch_f1(void);
void setting_touch_f2(void);
void setting_touch_f3(void);

void setting_touch_first3(void);
void setting_touch_first4(void);
void setting_touch_first5(void);
void setting_touch_first6(void);

void setting_touch_first7(void);
void setting_touch_first8(void);
void setting_touch_first9(void);
void setting_touch_first_press_set(void);
void setting_touch_first11(void);
void setting_touch_first12(void);

void Touch_set_left(void);



//extern uint8_t 	mode_value ;		//세팅 모드 값 0=nomal / 1= co2 /2 = o2/3= temp
//extern uint8_t	sub_set_mode ;			// 서브 세팅 모드






#if	TOUCH_TEST_ON == 0


void Touch_Check(void)
{
static uint16_t mode_out_cnt= 300;
static uint8_t	touch_time_cnt = 20 ; 
static uint8_t	touch_first = 0, first_cnt = 50 ; 
/*============================================================*/
	buzzer_set = 0;
	Touch_Read();

	if ( touch_first == 0 && (first_cnt--) )
	{
		return;
	}

	touch_first = 1;
/*============================================================*/
	if (test_mode != 0)
	{
		Touch_test_mode_Check();

		if ( buzzer_set == 1)
		{
			set_dis_refresh = 0x00;
			BUZZER_ON(1);
		}
		return;
	}
/*============================================================*/
	if (usb_out_flag != 0)
	{
		set_dis_refresh = 0;
		return;
	}
/*============================================================*/
	touch_flag2 = 0;

	if ( Touch_x_data == 0 && Touch_y_data == 0  )	
	{
		if ( Touch_x_input == 0 && Touch_y_input == 0 )
		{
			touch_flag1=0;
		}
		else if (touch_flag1 == 1 )
		{
			touch_flag1=4;

			Touch_x_data = Touch_x_input;
			Touch_y_data = Touch_y_input;
			Touch_x_input = 0;
			Touch_y_input = 0;		
		}
		else if ( touch_flag1 == 2 || touch_flag1 == 3 )
		{
			touch_flag1=0;
			Touch_x_input = 0;
			Touch_y_input = 0;	
		}
	}
	else 
	{
		Touch_x_input = Touch_x_data;
		Touch_y_input = Touch_y_data;	
		mode_out_cnt= 300;

		if ( touch_flag1 == 0 || touch_flag1 == 4 )
		{
			touch_flag1=1;
			touch_time_cnt = 20;
		}
		else if ( touch_flag1 == 1 )
		{
			if( ! touch_time_cnt--)		{touch_time_cnt = 1;	 touch_flag1=2;		touch_flag2 = 1;}
		}
		else if ( touch_flag1 == 2 || touch_flag1 == 3 )
		{
			touch_flag1=3;
			if( ! touch_time_cnt--)		{touch_time_cnt = 1;	touch_flag2 = 1;}
		}
	}


/*============================================================*/
	if(lock_data == 0)   Touch_set_left();
	
	if( mode_value ==main_sel) {Touch_Nomal_Check(); }
	else if (mode_value<date_sel) { Touch_Set_mode_Check();}
	else if (mode_value>=date_sel && mode_value<new_co2_sel )	{ Touch_Set_mode_Check2();	}
	else if (mode_value>=new_co2_sel && mode_value<alarm_list)	{ Touch_Set_mode_Check3();	}
	else if (mode_value>=alarm_list && mode_value<device_set)			{ Touch_Set_mode_Check4();	}
	else if (mode_value>=device_set&& mode_value<gas_1_setting)	{ Touch_Set_mode_Check5();	}
	else if (mode_value>=gas_1_setting)																						{ Touch_Set_mode_Check6();	}


/*============================================================*/
	if ( buzzer_set == 1)
	{
		set_dis_refresh = 0x00;
		if (error_set_save[10]==1)		BUZZER_ON(1);
	}
/*============================================================*/

	if (  (mode_value >= co2_sel && mode_value < new_co2_sel) || mode_value == alarm_list  )	
	{
		if (mode_out_cnt-- == 0 )	{mode_value = main_sel;	}
	}
}









void cham_set_clear(void)
{
	cham_setting_sta[0] =0;
	cham_setting_sta[1] =0;
	cham_setting_sta[2] =0;
	cham_setting_sta[3] =0;
	cham_setting_sta[4] =0;
	cham_setting_sta[5] =0;
	cham_setting_sta[6] =0;
	cham_setting_sta[7] =0;

	cham_temp_set_mode = 0	;
	lcd_popup_flag = 0;
	wet_change = 0;
}








void  Touch_set_left(void)
{
	if (cali_set != 0)
	{
		return;
	}

	if(cham_temp_set_mode != 0x00 && temp_id_check== 0x01)
	{
		if ( lcd_popup_flag == 0 )
		{
			 if( touch_flag1 == 4 )	//
			{	
				if		( Touch_set_mode(590,90) )		{ lcd_popup_flag=1;	buzzer_set = 1;	}
				else if ( Touch_set_mode(690,90) )		{ lcd_popup_flag=1;	buzzer_set = 1;	}
				else if ( Touch_set_temp(590,180) )		{ lcd_popup_flag=1;	buzzer_set = 1;	}
				else if ( Touch_set_status(690,180) )	{ lcd_popup_flag=1;	buzzer_set = 1;	}
			}
			else if ( touch_flag1 == 2 )
			{	 
				if ( Touch_set_mode(590,90) )			{ lcd_popup_flag=1;	buzzer_set = 1;	}
				else if ( Touch_set_mode(690,90) )		{ lcd_popup_flag=1;	buzzer_set = 1;	}
				else if ( Touch_set_temp(590,180) )		{ lcd_popup_flag=1;	buzzer_set = 1;	}
				else if ( Touch_set_status(690,180) )	{ lcd_popup_flag=1;	buzzer_set = 1;	}
			}
		}
		else
		{
			if( touch_flag1 == 4 )	//
			{	
				if		( Touch_set_clear(200, 400) )		{ cham_temp_save();	Set_Data_Write(); buzzer_set = 1;	}
				else if ( Touch_set_clear(350, 400) )		{ cham_set_clear();	buzzer_set = 1;	}
			}
			else if ( touch_flag1 == 2 )
			{	 
				if		( Touch_set_clear(200, 400) )		{ cham_temp_save();	Set_Data_Write(); buzzer_set = 1;	}
				else if ( Touch_set_clear(350, 400) )		{ cham_set_clear();	buzzer_set = 1;	}
			}
		}
	}
	else
	{
		if( touch_flag1 == 4 )	//
		{	
			if		( Touch_set_mode(590,90) )		{ mode_value = main_sel;	sub_set_mode = 0;	cham_set_clear();	buzzer_set = 1;	}
			else if ( Touch_set_mode(690,90) )		{ mode_value = alarm_sel;  	sub_set_mode = 0;	cham_set_clear();	buzzer_set = 1;	}
			else if ( Touch_set_temp(590,180) )		{ mode_value = temp_sel;  	sub_set_mode = 0;	cham_set_clear();	buzzer_set = 1; }
			else if ( Touch_set_status(690,180) )	{ mode_value = alarm_list;  sub_set_mode = 0;	cham_set_clear();	buzzer_set = 1; }
		}
		else if ( touch_flag1 == 2 )
		{	 
			if ( Touch_set_mode(590,90) )			{ mode_value = main_sel;		sub_set_mode = 0;	cham_set_clear();	buzzer_set = 1;	}
			else if ( Touch_set_mode(690,90) )		{ mode_value = password_set;  sub_set_mode = 0;	cham_set_clear();	buzzer_set = 1;	pass_val= 0; }
			else if ( Touch_set_temp(590,180) )		{ mode_value = temp_sel;  		sub_set_mode = 0;	cham_set_clear();	buzzer_set = 1; }
			else if ( Touch_set_status(690,180) )	{ mode_value = alarm_list;		sub_set_mode = 0;	cham_set_clear();	buzzer_set = 1; }
		}
	}
}







void set_val_inc_dec1(void)
{
	if 		( Touch_set_chan(240,180) )		{	if(set_data_temp != 99)		 set_data_temp++;	buzzer_set = 1;}
	else if ( Touch_set_chan(240,245) )		{	if(set_data_temp != 1)		 set_data_temp--;	buzzer_set = 1;}
}

void set_val_inc_dec2(void)
{
	if 		( Touch_set_chan(240,85) )		{	if(set_data_temp != 99)		set_data_temp++;	buzzer_set = 1;}
	else if ( Touch_set_chan(240,150) )		{	if(set_data_temp !=  1)	 	set_data_temp--;	buzzer_set = 1;}

	else if ( Touch_set_chan(240,225) )		{	if(set_data_temp2 !=999)	set_data_temp2++;	buzzer_set = 1;}
	else if ( Touch_set_chan(240,290) )		{	if(set_data_temp2 !=  1)	set_data_temp2--;	buzzer_set = 1;}

	else if ( Touch_set_chan(240,365) )		{	if(set_data_temp3 != 99)	set_data_temp3++;	buzzer_set = 1;}
	else if ( Touch_set_chan(240,430) )		{	if(set_data_temp3 != 1)		set_data_temp3--;	buzzer_set = 1;}
}


void set_val_press_inc_dec(void)
{
	if ( Touch_set_chan(80,120) )			{	if(set_data_temp != set_data_temp2-1)	set_data_temp++;	buzzer_set = 1;}
	else if ( Touch_set_chan(80,220) )		{	if(set_data_temp != 1)	 				set_data_temp--;	buzzer_set = 1;}

	else if ( Touch_set_chan(290,120) )		{	if(set_data_temp2 != 99)				set_data_temp2++;	buzzer_set = 1;}
	else if ( Touch_set_chan(290,220) )		{	if(set_data_temp2 != set_data_temp+1)	set_data_temp2--;	buzzer_set = 1;}

	else if ( Touch_set_chan(80,320) )		{	if(set_data_temp3 != 30)		set_data_temp3++;	buzzer_set = 1;}
	else if ( Touch_set_chan(80,420) )		{	if(set_data_temp3 != 1)	 		set_data_temp3--;	buzzer_set = 1;}

	else if ( Touch_set_chan(290,320) )		{	if(set_data_temp4 != 200)		set_data_temp4++;	buzzer_set = 1;}
	else if ( Touch_set_chan(290,420) )		{	if(set_data_temp4 != 10)		set_data_temp4--;	buzzer_set = 1;}
}




void mode_select_first(void)
{
	if 		( Touch_set_sub2(5,5) )			{ mode_value = new_co2_sel;  	buzzer_set = 1;}
	else if ( Touch_set_sub2(145,5) )		{ mode_value = new_o2_sel;		buzzer_set = 1;}
	else if ( Touch_set_sub2(285,5) )		{ mode_value = work_time_sel;  	buzzer_set = 1;}
	else if ( Touch_set_sub2(425,5) )		{ mode_value = pressure_set;  	buzzer_set = 1;}
	else if ( Touch_set_sub2(  5,40) )		{ mode_value = wet_mode_set;	buzzer_set = 1;}
	else if ( Touch_set_sub2(145,40) )		{ mode_value = cham_no_set;  	buzzer_set = 1;}
	else if ( Touch_set_sub2(285,40) )		{ mode_value = ext_temp_cal;  	buzzer_set = 1;}
	else if ( Touch_set_sub2(425,40) )		{ mode_value = pressure2_set;  	buzzer_set = 1;}	
	
}






void setting_touch_first7(void)
{
	mode_select_first();

	if ( Touch_set_small(50,400) )		{ test_mode= 1;  	buzzer_set = 1;}

	else if ( Touch_set_chan(240,180) )		{	if(set_data_temp != 99)		 set_data_temp++;	buzzer_set = 1;}
	else if ( Touch_set_chan(240,245) )		{	if(set_data_temp != 1)		 set_data_temp--;	buzzer_set = 1;}
	else if ( Touch_save(380,170) )			{  co2_set_data  = set_data_temp;	buzzer_set = 1; Set_Data_Write(); 
												main_log_flag= 7;  sub_log_flag1=set_data_temp;	sub_log_flag2=0;
												Gas_change_Write( 1, set_data_temp);
											}
}


void setting_touch_first8(void)
{
	mode_select_first();
	if ( Touch_set_small(50,400) )		{  test_mode= 1;  	buzzer_set = 1;}

	else if ( Touch_set_chan(240,180) )		{	if(set_data_temp != 99)		 set_data_temp++;	buzzer_set = 1;}
	else if ( Touch_set_chan(240,245) )		{	if(set_data_temp != 1)		 set_data_temp--;	buzzer_set = 1;}
	else if ( Touch_save(380,170) )			{  o2_set_data  = set_data_temp;	buzzer_set = 1;	Set_Data_Write();
												main_log_flag= 8;  sub_log_flag1=set_data_temp;	sub_log_flag2=0;
												Gas_change_Write( 2, set_data_temp);
											}
}

void setting_touch_first9(void)
{
	mode_select_first();

	if ( Touch_set_chan(240,85) )			{	if(set_data_temp != 99)		set_data_temp++;	buzzer_set = 1;}
	else if ( Touch_set_chan(240,150) )		{	if(set_data_temp !=  1)	 	set_data_temp--;	buzzer_set = 1;}

	else if ( Touch_set_chan(240,225) )		{	if(set_data_temp2 !=999)	set_data_temp2++;	buzzer_set = 1;}
	else if ( Touch_set_chan(240,290) )		{	if(set_data_temp2 !=  1)	set_data_temp2--;	buzzer_set = 1;}

	else if ( Touch_set_chan(240,365) )		{	if(set_data_temp3 != 99)	set_data_temp3++;	buzzer_set = 1;}
	else if ( Touch_set_chan(240,430) )		{	if(set_data_temp3 != 1)		set_data_temp3--;	buzzer_set = 1;}

	else if ( Touch_save(380,230) )			{   gas_initial_time =set_data_temp;	 gas_period_time =set_data_temp2;	 gas_work_time =set_data_temp3;
												buzzer_set = 1;		Set_Data_Write();}
}



void setting_touch_first_press_set(void)
{
	mode_select_first();

	if ( Touch_set_chan(80,120) )		{	if(set_data_temp != set_data_temp2-1)	set_data_temp++;	buzzer_set = 1;}
	else if ( Touch_set_chan(80,220) )		{	if(set_data_temp != 1)	 				set_data_temp--;	buzzer_set = 1;}

	else if ( Touch_set_chan(290,120) )		{	if(set_data_temp2 != 99)				set_data_temp2++;	buzzer_set = 1;}
	else if ( Touch_set_chan(290,220) )		{	if(set_data_temp2 != set_data_temp+1)	set_data_temp2--;	buzzer_set = 1;}

	else if ( Touch_set_chan(80,320) )		{	if(set_data_temp3 != 30)		set_data_temp3++;	buzzer_set = 1;}
	else if ( Touch_set_chan(80,420) )		{	if(set_data_temp3 != 0)	 		set_data_temp3--;	buzzer_set = 1;}

	else if ( Touch_set_chan(290,320) )		{	if(set_data_temp4 != 200)		set_data_temp4++;	buzzer_set = 1;}
	else if ( Touch_set_chan(290,420) )		{	if(set_data_temp4 != 10)		set_data_temp4--;	buzzer_set = 1;}

	else if ( Touch_save(420,220) )		
	{
		if (mode_value == pressure_set)		
		{
				Press_on_data =set_data_temp;	 Press_off_data =set_data_temp2;
				Press_err_data =set_data_temp3;	 Press_err_cnt =set_data_temp4;
				buzzer_set = 1;		Set_Data_Write();
		}
		else if (mode_value == pressure2_set)
		{
				Press2_on_data =set_data_temp;	 Press2_off_data =set_data_temp2;
				Press2_err_data =set_data_temp3;	 Press2_err_cnt =set_data_temp4;
				buzzer_set = 1;		Set_Data_Write();
		}
	}
}





void setting_touch_first_wet_mode(void)
{
	mode_select_first();

	if ( Touch_set_chan(240,180) )			{	if(set_data_temp != 10)		 set_data_temp++;	buzzer_set = 1;}
	else if ( Touch_set_chan(240,245) )		{	if(set_data_temp != 1)		 set_data_temp--;	buzzer_set = 1;}
	else if ( Touch_save(380,170) )			{  wet_mode_temp  = set_data_temp;	buzzer_set = 1; Set_Data_Write();  }

}


void set_val_wet_mode(void)
{
	if 		( Touch_set_chan(240,180) )		{	if(set_data_temp != 99)		 set_data_temp++;	buzzer_set = 1;}
	else if ( Touch_set_chan(240,245) )		{	if(set_data_temp != 1)		 set_data_temp--;	buzzer_set = 1;}
}




void cham_change_work(void)
{
	if ( chamber_count == 1)
	{
		cham_working_sta[6] = 0;	
		cham_working_sta[7] = 0;
	}
}


void setting_touch_first_cham_no(void)
{
	mode_select_first();

  if ( Touch_save(380,170) )			{  chamber_count  = set_data_temp;	cham_change_work();		buzzer_set = 1; Set_Data_Write(); }
}

void setting_touch_first_cham_no2(void)
{
	mode_select_first();

	if ( Touch_save(100,170) )				{  set_data_temp ^= 1;	buzzer_set = 1; }
}




void TEMP_ext_save(void)
{
	if (setting_data_page == 0)		//calibration
	{
		cali_set = TEMP_ext_calibration(set_data_temp);
		Set_Data_Write();
	}
	else		//initialize
	{
		TEMP_ext_initial();
		Set_Data_Write();
		cali_set = 2;
	}
}




void setting_touch_first_cal_temp(void)
{
	mode_select_first();

	if ( Touch_set_tab(50,180) )				{setting_data_page = 0;	buzzer_set = 1; }
	else if ( Touch_set_tab(50,260) )		{setting_data_page = 1;	buzzer_set = 1; }

	else if	( Touch_incdec2(250,150))	{	if(set_data_temp != 450 && setting_data_page==0)	 set_data_temp++;	buzzer_set = 1;}
	else if ( Touch_incdec2(250,280) )	{	if(set_data_temp != 200 && setting_data_page==0)	 set_data_temp--;	buzzer_set = 1;}
	else if ( Touch_save(420,200) )		{ 	TEMP_ext_save();	buzzer_set = 1;}

	
}


void setting_touch_ing_temp(void)
{
    if	( Touch_incdec2(250,150))	{	if(set_data_temp != 450 && setting_data_page==0)	 set_data_temp++;	}
	else if ( Touch_incdec2(250,280) )	{	if(set_data_temp != 200 && setting_data_page==0)	 set_data_temp--;	}
}






uint8_t pass_check(uint8_t in_data)
{
	if( set_data_temp!=3) 
	{
		pass_data[set_data_temp] = in_data;
		set_data_temp++;
	}
	else 
	{
		pass_data[set_data_temp] = in_data;

		if(pass_data[0] == '1' && pass_data[1] == '1' && pass_data[2] == '2' && pass_data[3] == '4' )
		{
			return 0;
		}
		else if	(pass_data[0] == save_pass_data[0] && pass_data[1] == save_pass_data[1] && pass_data[2] == save_pass_data[2] && pass_data[3] == save_pass_data[3] )
		{
			return 0;
		}	
		else
		{
			set_data_temp=0;
		}
	}
	return 1;
}



uint8_t pass_change(uint8_t in_data)
{
	if( set_data_temp!=3) 
	{
		pass_data[set_data_temp] = in_data;
		set_data_temp++;
		return 1;
	}
	else 
	{
		pass_data[set_data_temp] = in_data;
		set_data_temp++;

		save_pass_data[0]=pass_data[0];
		save_pass_data[1]=pass_data[1];
		save_pass_data[2]=pass_data[2];
		save_pass_data[3]=pass_data[3];

		Set_Data_Write();
		buzzer_set = 1;
		return 0;
	}
}





void set_data_inc(uint8_t in_data)
{
	switch(set_data_temp2)
	{
		case 0 : if( pass_check(in_data) == 0) {	mode_value =	(pass_val)? gas_1_setting : new_co2_sel	;  							}			break;		//pass 
		case 1 : if( pass_check(in_data) == 0) {	set_data_temp2 = 2;		set_data_temp = 0;	}		break;		//pass check
		case 2 : if( pass_change(in_data) == 0) {	set_data_temp2 = 3;		}		break;				//pass change
	}
}


void set_data_dec(void)
{
	if( set_data_temp!=0) 
	{
		set_data_temp--;
		pass_data[set_data_temp] = 0;
	}		
}


void set_data_clr(void)
{
	pass_data[0]=0;
	pass_data[1]=0; 
	pass_data[2]=0;
	pass_data[3]=0;
	set_data_temp=0;	
}


void pass_mode_change(void)
{
	switch(set_data_temp2)
	{
		case 0 : set_data_temp = 0; 	set_data_temp2 = 1;		break;		//pass change
		case 1 : set_data_temp = 0; 	set_data_temp2 = 0;		break;		//pass change
		case 2 : set_data_temp = 0; 	set_data_temp2 = 0;		break;		//pass change
	}
}





void setting_touch_first11(void)
{
uint8_t pass_char=0;

	if(set_data_temp2 != 3)
	{
		if		( Touch_set_small(160,160) )		{ pass_char = '1';	set_data_inc(pass_char); 	buzzer_set = 1;}	//1
		else if ( Touch_set_small(240,160) )		{ pass_char = '2';	set_data_inc(pass_char);	buzzer_set = 1;}	//2
		else if ( Touch_set_small(320,160) )		{ pass_char = '3';	set_data_inc(pass_char); 	buzzer_set = 1; }	//3

		else if ( Touch_set_small(160,240) )		{ pass_char = '4';	set_data_inc(pass_char);	buzzer_set = 1;}	//4
		else if ( Touch_set_small(240,240) )		{ pass_char = '5';	set_data_inc(pass_char);  	buzzer_set = 1;}	//5
		else if ( Touch_set_small(320,240) )		{ pass_char = '6';	set_data_inc(pass_char);	buzzer_set = 1;}	//6

		else if ( Touch_set_small(160,320) )		{ pass_char = '7';	set_data_inc(pass_char);  	buzzer_set = 1;}	//7
		else if ( Touch_set_small(240,320) )		{ pass_char = '8';	set_data_inc(pass_char);	buzzer_set = 1;}	//8
		else if ( Touch_set_small(320,320) )		{ pass_char = '9';	set_data_inc(pass_char);  	buzzer_set = 1;}	//9

		else if ( Touch_set_small(240,400) )		{ pass_char = '0';	set_data_inc(pass_char); 	buzzer_set = 1;}	//0

		else if ( Touch_set_small(160,400) )		{ set_data_clr();	buzzer_set = 1;}	//C
		else if ( Touch_set_small(320,400) )		{ set_data_dec();	buzzer_set = 1;}	//back

		else if ( Touch_pass_chan(420,400) )		{ pass_mode_change();		buzzer_set = 1;}	//back
	}
}



void change_complte(void)
{
static uint8_t dis_cnt = 10;
	
	if (mode_value == password_set && set_data_temp2 == 3)		
	{
		if(dis_cnt-- == 0)	{	set_dis_refresh = 0;	dis_cnt= 10;	set_data_temp2 = 0;		set_data_temp = 0;	}
	}
}




void Touch_Set_mode_Check3(void)
{
	if( touch_flag1 == 4 )	//
	{	
		if (mode_value == new_co2_sel)			setting_touch_first7();
		else if (mode_value == new_o2_sel)		setting_touch_first8();
		else if (mode_value == work_time_sel)	setting_touch_first9();
		else if (mode_value == pressure_set)	setting_touch_first_press_set();
		else if (mode_value == wet_mode_set)	setting_touch_first_wet_mode();
		else if (mode_value == cham_no_set)		setting_touch_first_cham_no();
		else if (mode_value == ext_temp_cal)		setting_touch_first_cal_temp();
	  else if (mode_value == pressure2_set)	setting_touch_first_press_set();
		else if (mode_value == password_set)	setting_touch_first11();

	}
	else if ( touch_flag1 == 3 && touch_flag2 == 1 )
	{	 
		if (mode_value == new_co2_sel)			set_val_inc_dec1();
		else if (mode_value == new_o2_sel)		set_val_inc_dec1();
		else if (mode_value == work_time_sel)	set_val_inc_dec2();
		else if (mode_value == pressure_set)	set_val_press_inc_dec();
		else if (mode_value == wet_mode_set)	set_val_wet_mode();
		else if (mode_value == ext_temp_cal)		setting_touch_ing_temp();
		else if (mode_value == pressure2_set)	set_val_press_inc_dec();
	}
  else if ( touch_flag1 == 2 )
	{	 
     if (mode_value == cham_no_set)		setting_touch_first_cham_no2();
  }
  

	change_complte();
}







void setting_touch_first12(void)
{
	if 		( Touch_move_rl(135, 400) )	    { if ( set_data_adr != 0)  	set_data_adr--; 	buzzer_set = 1;}	//Alarm_log_Read
	else if ( Touch_move_rl(330, 400) )		{ if ( set_data_adr != 4) 	set_data_adr++; 	buzzer_set = 1;}

}

void setting_touch_first12_1(void)
{
	if 		( Touch_move_rl(135, 400) )	    { if ( set_data_adr != 0)  	set_data_adr--; 	buzzer_set = 1;}	//Alarm_log_Read
	else if ( Touch_move_rl(330, 400) )		{ if ( set_data_adr != 4) 	set_data_adr++; 	buzzer_set = 1;}
	else if ( Touch_set_clear( 40, 400) )	{ Alarm_log_erase(); 		buzzer_set = 1;}
}


void Touch_Set_mode_Check4(void)
{
	if( touch_flag1 == 4 )
	{
		if (mode_value == alarm_list)			setting_touch_first12();
	}
	else if ( touch_flag1 == 2 )
	{
		if (mode_value == alarm_list)			setting_touch_first12_1();
	}

}



void name_all_clr(void)
{
	device_temp[0] = ' ';
	device_temp[1] = ' ';
	device_temp[2] = ' ';
	device_temp[3] = ' ';

}

void name_all_ret(void)
{
	device_temp[0] = device_name[0];
	device_temp[1] = device_name[1];
	device_temp[2] = device_name[2];
	device_temp[3] = device_name[3];

}


void name_all_save(void)
{
	device_name[0] = device_temp[0];
	device_name[1] = device_temp[1];
	device_name[2] = device_temp[2];
	device_name[3] = device_temp[3];
	Set_Data_Write();

}



void name_input(void)
{
	if 		( Touch_input_pad( 20, 160) )	    { device_temp[set_data_temp2++]= '0';		buzzer_set = 1;}
	else if ( Touch_input_pad( 70, 160) )	    { device_temp[set_data_temp2++]= '1';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(120, 160) )	    { device_temp[set_data_temp2++]= '2';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(170, 160) )	    { device_temp[set_data_temp2++]= '3';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(220, 160) )	    { device_temp[set_data_temp2++]= '4';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(270, 160) )	    { device_temp[set_data_temp2++]= '5';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(320, 160) )	    { device_temp[set_data_temp2++]= '6';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(370, 160) )	    { device_temp[set_data_temp2++]= '7';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(420, 160) )	    { device_temp[set_data_temp2++]= '8';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(470, 160) )	    { device_temp[set_data_temp2++]= '9';	 	buzzer_set = 1;}

	else if ( Touch_input_pad( 20, 240) )	    { device_temp[set_data_temp2++]= 'A';	 	buzzer_set = 1;}
	else if ( Touch_input_pad( 70, 240) )	    { device_temp[set_data_temp2++]= 'B';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(120, 240) )	    { device_temp[set_data_temp2++]= 'C';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(170, 240) )	    { device_temp[set_data_temp2++]= 'D';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(220, 240) )	    { device_temp[set_data_temp2++]= 'E';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(270, 240) )	    { device_temp[set_data_temp2++]= 'F';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(320, 240) )	    { device_temp[set_data_temp2++]= 'G';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(370, 240) )	    { device_temp[set_data_temp2++]= 'H';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(420, 240) )	    { device_temp[set_data_temp2++]= 'I';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(470, 240) )	    { device_temp[set_data_temp2++]= 'J';	 	buzzer_set = 1;}

	else if ( Touch_input_pad( 20, 320) )	    { device_temp[set_data_temp2++]= 'K';	 	buzzer_set = 1;}
	else if ( Touch_input_pad( 70, 320) )	    { device_temp[set_data_temp2++]= 'L';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(120, 320) )	    { device_temp[set_data_temp2++]= 'M';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(170, 320) )	    { device_temp[set_data_temp2++]= 'N';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(220, 320) )	    { device_temp[set_data_temp2++]= 'O';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(270, 320) )	    { device_temp[set_data_temp2++]= 'P';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(320, 320) )	    { device_temp[set_data_temp2++]= 'Q';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(370, 320) )	    { device_temp[set_data_temp2++]= 'R';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(420, 320) )	    { device_temp[set_data_temp2++]= 'S';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(470, 320) )	    { device_temp[set_data_temp2++]= 'T';	 	buzzer_set = 1;}

	else if ( Touch_input_pad( 20, 400) )	    { device_temp[set_data_temp2++]= 'U';	 	buzzer_set = 1;}
	else if ( Touch_input_pad( 70, 400) )	    { device_temp[set_data_temp2++]= 'V';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(120, 400) )	    { device_temp[set_data_temp2++]= 'W';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(170, 400) )	    { device_temp[set_data_temp2++]= 'X';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(220, 400) )	    { device_temp[set_data_temp2++]= 'Y';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(270, 400) )	    { device_temp[set_data_temp2++]= 'Z';	 	buzzer_set = 1;}
	else if ( Touch_input_pad(320, 400) )		{ if (set_data_temp2 != 0)		device_temp[--set_data_temp2]= ' ';	 	buzzer_set = 1;		}

	if (set_data_temp2 == 4)	{set_data_temp = 2;}
}




void setting_touch_first13(void)
{

	if (set_data_temp == 0 )
	{
		if 		( Touch_set_clear(350, 80) )	    { name_all_clr();	set_data_temp = 1; 	buzzer_set = 1;}
	}

	else if (set_data_temp == 1)
	{
		if 		( Touch_set_clear(350, 80) )	    { name_all_ret();	set_data_temp = 0; 	buzzer_set = 1;}
		name_input();
	}

	else if (set_data_temp == 2)
	{
		if 		( Touch_set_clear(350, 80) )	    {  name_all_ret();	set_data_temp = 0; 	buzzer_set = 1;}
		else if ( Touch_set_clear(450, 80) )	    { name_all_save();	set_data_temp = 0; 	buzzer_set = 1;}
	}

}






void Touch_Set_mode_Check5(void)
{
	if( touch_flag1 == 4 )
	{
		if (mode_value == device_set)			setting_touch_first13();
	}
}




void all_out_usb_mode(uint8_t data)
{
	out_usb_mode[0]=data;
	out_usb_mode[1]=data;
	out_usb_mode[2]=data;
	out_usb_mode[3]=data;
	out_usb_mode[4]=data;
}


void setting_touch_first5_1(void)
{
	if ( Touch_set_clear( 490, 380) )	{  Main_log_erase();	buzzer_set = 1;}

}



void setting_touch_first5(void)
{
	if 		( Touch_set_tab(  5,15) )		{ mode_value = alarm_sel;	buzzer_set = 1;}
	else if ( Touch_set_tab(190,15) )		{ mode_value = log_sel;		buzzer_set = 1;}
	else if ( Touch_set_tab(375,15) )		{ mode_value = voc_sel;		buzzer_set = 1;}

	else if ( Touch_move_ud(530,90) )	    { if ( set_data_adr != 0)  		set_data_adr--; 	buzzer_set = 1;}	//Main_log_Read
	else if ( Touch_move_ud(530,220) )		{ if ( set_data_adr != 99) 	set_data_adr++; 	buzzer_set = 1;}

	else if ( Touch_set_box( 20, 370) )		{ out_usb_mode[0] ^= 0x1; out_usb_mode[5] = 0x0;	buzzer_set = 1;}
	else if ( Touch_set_box(120, 370) )		{ out_usb_mode[1] ^= 0x1; out_usb_mode[5] = 0x0;	buzzer_set = 1;}
	else if ( Touch_set_box(220, 370) )		{ out_usb_mode[2] ^= 0x1; out_usb_mode[5] = 0x0;	buzzer_set = 1;}
	else if ( Touch_set_box(320, 370) )		{ out_usb_mode[3] ^= 0x1; out_usb_mode[5] = 0x0;	buzzer_set = 1;}
	else if ( Touch_set_box( 20, 410) )		{ out_usb_mode[4] ^= 0x1; out_usb_mode[5] = 0x0;	buzzer_set = 1;}
	else if ( Touch_set_box(220, 410) )		{ out_usb_mode[5] ^= 0x1;  all_out_usb_mode(out_usb_mode[5]); 	buzzer_set = 1;}

	else if ( Touch_set_clear( 390, 380) )	{ usb_out_flag = 1;		buzzer_set = 1;}

}










void changr_voc(void)
{
	if ( Touch_set_replace(390,370) )
	{
		if (change_sta ==0 ) 	{ change_sta =1;	buzzer_set = 1;	 }
		else					{ change_sta =0; 	Voc_change_Write();	Set_Data_Write(); 
									main_log_flag= 4;  sub_log_flag1=voc_valid_time;	sub_log_flag2=0; 
									buzzer_set = 1;
								}
	}
}


void setting_touch_first6(void)
{
	if 		( Touch_set_tab(  5,15) )		{ mode_value = alarm_sel;	buzzer_set = 1;}
	else if ( Touch_set_tab(190,15) )		{ mode_value = log_sel;		buzzer_set = 1;}
	else if ( Touch_set_tab(375,15) )		{ mode_value = voc_sel;		buzzer_set = 1;}

	else if ( Touch_move_ud(530,90) )	    { if ( set_data_adr != 0)  		set_data_adr--; 	buzzer_set = 1;}	//Voc_change_Read
	else if ( Touch_move_ud(530,220) )		{ if ( set_data_adr != 4)	 	set_data_adr++; 	buzzer_set = 1;}


	else if ( Touch_set_chan(165,340) )			{  if(voc_valid_time != 6) 		voc_valid_time++;		Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_chan(165,340+86) )		{  if(voc_valid_time != 1)		voc_valid_time--;		Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_chan(305,340) )			{  if(change_alarm_time != 23) 	change_alarm_time++;	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_chan(305,340+86) )		{  if(change_alarm_time != 0)	change_alarm_time--;	Set_Data_Write();	buzzer_set = 1;}


}


void setting_touch_first6_1(void)
{
	if ( Touch_set_clear( 10, 370) )	{  Voc_change_erase();	buzzer_set = 1;}
	changr_voc();
}



void Touch_Set_mode_Check2(void)
{
	if( touch_flag1 == 4 )
	{
		if (mode_value == date_sel)			setting_touch_first3();
		else if (mode_value == alarm_sel)	setting_touch_first4();
		else if (mode_value == log_sel)		setting_touch_first5();
		else if (mode_value == voc_sel)		setting_touch_first6();
	}

	if( touch_flag1 == 2 )
	{
		if (mode_value == date_sel)			setting_touch_first3();
		else if (mode_value == log_sel)		setting_touch_first5_1();
		else if (mode_value == voc_sel)		setting_touch_first6_1();
	}

	if( touch_flag1 == 3 && touch_flag2 == 1 )
	{
		if (mode_value == date_sel)			setting_touch_first3();
	}
}


void setting_touch_first3(void)
{
	if 		( Touch_set_chan(5+15,150) )		{  if(set_year != 99)	set_year++;		buzzer_set = 1;}
	else if ( Touch_set_chan(5+15,150+86) )		{  if(set_year != 15)	set_year--;		buzzer_set = 1;}
	else if ( Touch_set_chan(125,150) )			{  if(set_month != 12) 	set_month++;	buzzer_set = 1;}
	else if ( Touch_set_chan(125,150+86) )		{  if(set_month != 1)	set_month--;	buzzer_set = 1;}
	else if ( Touch_set_chan(215,150) )			{  if(set_date != 31)	set_date++;		buzzer_set = 1;}
	else if ( Touch_set_chan(215,150+86) )		{  if(set_date != 1)	set_date--;		buzzer_set = 1;}
	else if ( Touch_set_chan(305,150) )			{  if(set_hour != 23)	set_hour++;		buzzer_set = 1;}
	else if ( Touch_set_chan(305,150+86) )		{  if(set_hour != 0)	set_hour--;		buzzer_set = 1;}
	else if ( Touch_set_chan(395,150) )			{  if(set_min != 59)	set_min++;		buzzer_set = 1;}
	else if ( Touch_set_chan(395,150+86) )		{  if(set_min != 0)		set_min--;		buzzer_set = 1;}
	else if ( Touch_set_chan(485,150) )			{  if(set_sec != 59)	set_sec++;		buzzer_set = 1;}
	else if ( Touch_set_chan(485,150+86) )		{  if(set_sec != 0)		set_sec--;		buzzer_set = 1;}

	else if ( Touch_save(420,300) )				{  RTC_TimeRegulate();	buzzer_set = 1;}
}




void setting_touch_first4(void)
{
	if 		( Touch_set_tab(  5,15) )		{ mode_value = alarm_sel;	buzzer_set = 1;}
	else if ( Touch_set_tab(190,15) )		{ mode_value = log_sel;		buzzer_set = 1;}
	else if ( Touch_set_tab(375,15) )		{ mode_value = voc_sel;		buzzer_set = 1;}

	else if ( Touch_set_box(330,94+30*0) )		{ error_set_save[0] ^= 0x1; 	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(330,94+30*1) )		{ error_set_save[1] ^= 0x1; 	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(330,94+30*2) )		{ error_set_save[2] ^= 0x1; 	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(330,94+30*3) )		{ error_set_save[3] ^= 0x1;	  	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(330,94+30*4) )		{ error_set_save[4] ^= 0x1; 	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(330,94+30*5) )		{ error_set_save[5] ^= 0x1;  	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(330,94+30*6) )		{ error_set_save[6] ^= 0x1;  	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(330,94+30*7) )		{ error_set_save[7] ^= 0x1;  	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(330,94+30*8) )		{ error_set_save[8] ^= 0x1;  	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(330,94+30*9) )		{ error_set_save[9] ^= 0x1;  	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(330,94+30*10) )		{ error_set_save[10] ^= 0x1;  	Set_Data_Write();	buzzer_set = 1;}

	else if ( Touch_set_box(440,94+30*0) )		{ error_set_save[11] ^= 0x1; 	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(440,94+30*1) )		{ error_set_save[12] ^= 0x1; 	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(440,94+30*2) )		{ error_set_save[13] ^= 0x1; 	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(440,94+30*3) )		{ error_set_save[14] ^= 0x1;	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(440,94+30*4) )		{ error_set_save[15] ^= 0x1; 	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(440,94+30*5) )		{ error_set_save[16] ^= 0x1;  	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(440,94+30*6) )		{ error_set_save[17] ^= 0x1;  	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(440,94+30*7) )		{ error_set_save[18] ^= 0x1;  	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(440,94+30*8) )		{ error_set_save[19] ^= 0x1;  	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(440,94+30*9) )		{ error_set_save[20] ^= 0x1;  	Set_Data_Write();	buzzer_set = 1;}
	else if ( Touch_set_box(440,94+30*10) )		{ error_set_save[21] ^= 0x1; 	Set_Data_Write();	buzzer_set = 1;}
	
	


}












void setting_touch_f0_1(void)
{
	if( mode_value ==temp_sel)
	{
		if 		( Touch_incdec2(250,150))	{	if(set_data_temp != 450)	 set_data_temp++;	buzzer_set = 1;}
		else if ( Touch_incdec2(250,280) )	{	if(set_data_temp != 200)	 set_data_temp--;	buzzer_set = 1;}
	}
}


void setting_touch_f0(void)
{
	if( mode_value ==temp_sel)		//temp setting
	{
		if 		( Touch_set_tab(  5,15) )	{sub_set_mode = 0;	buzzer_set = 1; }
		else if ( Touch_set_tab(190,15) )	{sub_set_mode = 1;	buzzer_set = 1; }
		else if ( Touch_set_tab(375,15) )	{sub_set_mode = 2;	buzzer_set = 1; }
		else if ( Touch_set_tab(230,380) )	{sub_set_mode = 3;	buzzer_set = 1; }

		else if	( Touch_incdec2(250,150))	{	if(set_data_temp != 450)	 set_data_temp++;	buzzer_set = 1;}
		else if ( Touch_incdec2(250,280) )	{	if(set_data_temp != 200)	 set_data_temp--;	buzzer_set = 1;}
		else if ( Touch_save(420,200) )		{  cham_all_set_temp  = set_data_temp;		Set_Data_Write();	buzzer_set = 1;}
	}
}



void setting_touch_f1(void)
{
	if( mode_value ==temp_sel)		//temp setting
	{	
		if 		( Touch_set_tab(  5,15) )	{sub_set_mode = 0;	buzzer_set = 1; }
		else if ( Touch_set_tab(190,15) )	{sub_set_mode = 1;	buzzer_set = 1; }
		else if ( Touch_set_tab(375,15) )	{sub_set_mode = 2;	buzzer_set = 1; }

		else if ( Touch_move_rl(135,340) )	{ if ( setting_data_page != 20)  setting_data_page++; 	buzzer_set = 1;}	//Data_Temp_Read
		else if ( Touch_move_rl(330,340) )	{ if ( setting_data_page != 0)   setting_data_page--; 	buzzer_set = 1;}

		else if ( Touch_set_small( 10,410) )	{ set_ch_no=0 ;		setting_data_page = 0;	buzzer_set = 1;}
		else if ( Touch_set_small( 80,410) )	{ set_ch_no=1 ;		setting_data_page = 0;	buzzer_set = 1;}
		else if ( Touch_set_small(150,410) )	{ set_ch_no=2 ;		setting_data_page = 0;	buzzer_set = 1;}
		else if ( Touch_set_small(220,410) )	{ set_ch_no=3 ;		setting_data_page = 0;	buzzer_set = 1;}
		else if ( Touch_set_small(290,410) )	{ set_ch_no=4 ;		setting_data_page = 0;	buzzer_set = 1;}
		else if ( Touch_set_small(360,410) )	{ set_ch_no=5 ;		setting_data_page = 0;	buzzer_set = 1;}

		if( chamber_count == 1)		return;

		else if ( Touch_set_small(430,410) )	{ set_ch_no=6 ;		setting_data_page = 0;	buzzer_set = 1;}
		else if ( Touch_set_small(500,410) )	{ set_ch_no=7 ;		setting_data_page = 0;	buzzer_set = 1;}
	}
}


void setting_touch_f1_1(void)
{
	if( mode_value ==temp_sel)		//temp setting
	{	
		if ( Touch_set_clear( 10, 320) )	{	Data_Temp_erase(); 	buzzer_set = 1;}
	}
}



void setting_touch_f2(void)
{
	if( mode_value ==temp_sel)		//temp setting
	{	
		if 		( Touch_set_tab(  5,15) )	{sub_set_mode = 0;	buzzer_set = 1; }
		else if ( Touch_set_tab(190,15) )	{sub_set_mode = 1;	buzzer_set = 1; }
		else if ( Touch_set_tab(375,15) )	{sub_set_mode = 2;	buzzer_set = 1; }

		else if ( Touch_move_ud(530,90) )	{ if ( set_data_adr != 0)  		set_data_adr--;		buzzer_set = 1;}	//Temp_set_Read
		else if ( Touch_move_ud(530,220) )	{ if ( set_data_adr != 99)  	set_data_adr++;		buzzer_set = 1;}

		else if ( Touch_set_clear( 400, 340) )	{	sub_set_mode = 4; 	buzzer_set = 1;}


	}
}


void setting_touch_f2_1(void)
{
	if( mode_value ==temp_sel)		//temp setting
	{	
		if ( Touch_set_clear( 40, 340) )	{	Temp_set_erase(); 	buzzer_set = 1;}
	}
}

void setting_touch_f2_2(void)
{
	if( mode_value ==temp_sel)		//temp setting
	{	
		if 		( Touch_move_ud(530,90) )	{ if ( set_data_adr != 0)  		set_data_adr--;		buzzer_set = 1;}	//Temp_set_Read
		else if ( Touch_move_ud(530,220) )	{ if ( set_data_adr != 99)  	set_data_adr++;		buzzer_set = 1;}
	}
}




void TEMP_DATA_save(void)
{
	if (setting_data_page == 0)		//calibration
	{
		cali_set = 1;
		start_Calibraion();
	}
	else		//initialize
	{
		TEMP_DATA_initial(set_ch_no, set_posit );
		Set_Data_Write();
    
    cham_cali_save(set_ch_no, set_posit+2, set_data_temp);  //로그 데이타 저장 +2는 initial용
		cali_set= 2;
	}
}





void setting_touch_f3(void)
{
	if( mode_value ==temp_sel)		//temp setting
	{	
		if 		( Touch_set_tab(  5,15) )	{sub_set_mode = 0;	buzzer_set = 1; }
		else if ( Touch_set_tab(190,15) )	{sub_set_mode = 1;	buzzer_set = 1; }
		else if ( Touch_set_tab(375,15) )	{sub_set_mode = 2;	buzzer_set = 1; }

		else if ( Touch_set_tab(50,160) )	{setting_data_page = 0;	buzzer_set = 1; }
		else if ( Touch_set_tab(50,240) )	{setting_data_page = 1;	buzzer_set = 1; }
	
		else if	( Touch_incdec2(250,130))	{	if(set_data_temp != 450 && setting_data_page==0)	 set_data_temp++;	buzzer_set = 1;}
		else if ( Touch_incdec2(250,260) )	{	if(set_data_temp != 200 && setting_data_page==0)	 set_data_temp--;	buzzer_set = 1;}
		else if ( Touch_save(420,180) )		{ 	TEMP_DATA_save();	buzzer_set = 1;}
	
		else if ( Touch_set_small(220,340) )	{ set_posit=0 ;		setting_data_page = 0;	buzzer_set = 1;}
		else if ( Touch_set_small(360,340) )	{ set_posit=1 ;		setting_data_page = 0;	buzzer_set = 1;} 

		else if ( Touch_set_small( 10,410) )	{ set_ch_no=0 ;		setting_data_page = 0;	buzzer_set = 1;}
		else if ( Touch_set_small( 80,410) )	{ set_ch_no=1 ;		setting_data_page = 0;	buzzer_set = 1;}
		else if ( Touch_set_small(150,410) )	{ set_ch_no=2 ;		setting_data_page = 0;	buzzer_set = 1;}
		else if ( Touch_set_small(220,410) )	{ set_ch_no=3 ;		setting_data_page = 0;	buzzer_set = 1;}
		else if ( Touch_set_small(290,410) )	{ set_ch_no=4 ;		setting_data_page = 0;	buzzer_set = 1;}
		else if ( Touch_set_small(360,410) )	{ set_ch_no=5 ;		setting_data_page = 0;	buzzer_set = 1;}

		if( chamber_count == 1)		return;

		else if ( Touch_set_small(430,410) )	{ set_ch_no=6 ;		setting_data_page = 0;	buzzer_set = 1;}
		else if ( Touch_set_small(500,410) )	{ set_ch_no=7 ;		setting_data_page = 0;	buzzer_set = 1;}

	}

}


void setting_touch_f4(void)
{
	if( mode_value ==temp_sel)		//temp setting
	{	
		if 		( Touch_set_tab(  5,15) )	{sub_set_mode = 0;	buzzer_set = 1; }
		else if ( Touch_set_tab(190,15) )	{sub_set_mode = 1;	buzzer_set = 1; }
		else if ( Touch_set_tab(375,15) )	{sub_set_mode = 2;	buzzer_set = 1; }

		else if ( Touch_move_ud(530,90) )	{ if ( set_data_adr != 0)  		set_data_adr--;		buzzer_set = 1;}		//Cham_temp_Read
		else if ( Touch_move_ud(530,220) )	{ if ( set_data_adr != 599)  	set_data_adr++;		buzzer_set = 1;}

		else if ( Touch_set_clear( 400, 340) )	{	sub_set_mode = 2; 	buzzer_set = 1;}

		else if ( Touch_set_small( 10,410) )	{ set_ch_no=0 ;		set_data_adr = 0;	buzzer_set = 1;}
		else if ( Touch_set_small( 80,410) )	{ set_ch_no=1 ;		set_data_adr = 0;	buzzer_set = 1;}
		else if ( Touch_set_small(150,410) )	{ set_ch_no=2 ;		set_data_adr = 0;	buzzer_set = 1;}
		else if ( Touch_set_small(220,410) )	{ set_ch_no=3 ;		set_data_adr = 0;	buzzer_set = 1;}
		else if ( Touch_set_small(290,410) )	{ set_ch_no=4 ;		set_data_adr = 0;	buzzer_set = 1;}
		else if ( Touch_set_small(360,410) )	{ set_ch_no=5 ;		set_data_adr = 0;	buzzer_set = 1;}

		if( chamber_count == 1)		return;

		else if ( Touch_set_small(430,410) )	{ set_ch_no=6 ;		set_data_adr = 0;	buzzer_set = 1;}
		else if ( Touch_set_small(500,410) )	{ set_ch_no=7 ;		set_data_adr = 0;	buzzer_set = 1;}
	}

}

void setting_touch_f4_1(void)
{
	if( mode_value ==temp_sel)		//temp setting
	{	
		if ( Touch_set_clear( 40, 340) )	{	Cham_temp_erase(); 	buzzer_set = 1;}
	}
}

void setting_touch_f4_2(void)
{
	if( mode_value ==temp_sel)		//temp setting
	{	
		if		( Touch_move_ud(530,90) )	{ if ( set_data_adr != 0)  		set_data_adr--;		buzzer_set = 1;}
		else if ( Touch_move_ud(530,220) )	{ if ( set_data_adr != 599)  	set_data_adr++;		buzzer_set = 1;}		//Cham_temp_Read
	}
}




void Touch_Set_mode_Check(void)
{
static uint8_t cali_cnt = 5;

	if (cali_set == 1)
	{
		set_dis_refresh = 0;
		cali_cnt = 5;
		cali_set = TEMP_DATA_calibration(set_ch_no, set_posit, set_data_temp);			//0 = 미진입, 1 = 데이타 수집 중, 2 = 데이타 정상 3 = 데이타 오류

		if (cali_set == 2)
    {
      Set_Data_Write();
      cham_cali_save(set_ch_no, set_posit, set_data_temp);
    }
	}
	else if( cali_set ==2 || cali_set==3 )
	{
		set_dis_refresh = 0;

		if (cali_cnt-- == 0)	{	cali_cnt = 5;		cali_set = 0 ;	}
	}

	
	else if( touch_flag1 == 4 )
	{
		if (sub_set_mode == 0x00)		setting_touch_f0();
		else if (sub_set_mode == 0x01)	setting_touch_f1();
		else if (sub_set_mode == 0x02)	setting_touch_f2();
		else if (sub_set_mode == 0x03)	setting_touch_f3();
		else if (sub_set_mode == 0x04)	setting_touch_f4();

	}

	else if( touch_flag1 == 2 )
	{
		if (sub_set_mode == 0x00 || sub_set_mode == 0x03)	setting_touch_f0_1();
		else if (sub_set_mode == 0x01)						setting_touch_f1_1();	
		else if (sub_set_mode == 0x02)						setting_touch_f2_1();
		else if (sub_set_mode == 0x04)						setting_touch_f4_1();

	}
	else if( touch_flag1 == 3 && touch_flag2 == 1 )
	{
		if (sub_set_mode == 0x00 || sub_set_mode == 0x03 )		setting_touch_f0_1();
		else if (sub_set_mode == 0x02)							setting_touch_f2_2();
		else if (sub_set_mode == 0x04)							setting_touch_f4_2();
	}

}



void device_info(void )
{
	if	( Touch_mark(630,410) )			{ mode_value = device_set;	buzzer_set = 1;	}

}



void lcd_time_set_touch(void)
{
	if	( Touch_set_time(575,  15) )		{ mode_value = date_sel;	sub_set_mode = 0;	cham_set_clear();	buzzer_set = 1;	}

}



void wet_on_off(void)
{
uint16_t gas_touch_x1, gas_touch_x2;

	if ( gas_2nd_on_off==0 )	{		gas_touch_x1 = 156;		gas_touch_x2 = 300;	}
	else	{		gas_touch_x1 = 210;		gas_touch_x2 = 330;	}

	if ( ! cham_temp_set_mode)
	{
		if ( Touch_save(gas_touch_x1,350) )			{wet_mode = 0;	wet_change = 0;	Set_Data_Write(); buzzer_set = 1; main_log_flag= 6;  sub_log_flag1=0;	sub_log_flag2=0; }
		else if ( Touch_save(gas_touch_x2,350) )		{wet_mode = 1;	wet_change = 0;	Set_Data_Write(); buzzer_set = 1; main_log_flag= 6;  sub_log_flag1=1;	sub_log_flag2=0; }
	}
}

void lock_on_off(void)
{
	if (! cham_temp_set_mode)
	{
		if ( Touch_save(450,350) )			{ wet_change = 0;	 lock_data ^= 1;	buzzer_set = 1;  }
	}
}




void gas_selecot_set(void)
{
	if ( cham_temp_set_mode==0)
	{
		if ( Touch_save(10,350) )			{	mode_value = password_set;	 sub_set_mode = 0;	cham_set_clear();	buzzer_set = 1;		pass_val=1; }
	}
}



void Touch_Nomal_Check(void)
{
static uint8_t	touch_blink_cnt = 10 ;
static uint8_t	clear_cnt = 100;
static uint16_t	auto_lock_cnt = 200;
//-----------------------------------
	if( Error_EN == 1 )
	{
		if( touch_flag1 == 4 )
		{		
			error_clear();
			return;
		}
	}
//-----------------------------------
	if ( lcd_popup_flag == 1 )
	{
		return;
	}
//-----------------------------------
	if( touch_flag1 == 4 )
	{
    if(lock_data == 0)
    {
      cham_touch_first();
      temp_inc_dec();
      auto_lock_cnt = 200;

    }
		clear_cnt = 100;
	}
//-----------------------------------
	else if ( touch_flag1 == 2 )
	{
    lock_on_off();

    if(lock_data == 0)
    {
      cham_touch_continue();
      temp_inc_dec();
      lcd_time_set_touch();
      wet_on_off();
      gas_selecot_set();
      device_info();

      auto_lock_cnt = 200;
    }
		clear_cnt = 100;
	}
//-----------------------------------
	else if ( touch_flag1 == 3 && touch_flag2 == 1)
	{
    if(lock_data == 0)
    {
      temp_inc_dec();
    }
		clear_cnt = 100;
	}
//-----------------------------------	
	else if ( touch_flag1 == 0 )
	{
		if(cham_temp_set_mode == 2)
		{	
			if ( ! touch_blink_cnt--)	cham_temp_set_mode = 1;	//inc,dec키 눌러질 경우 블링크 해제 후 inc,dec키 띄면 블링크 다시
			clear_cnt = 100;
		}
		else if (cham_temp_set_mode == 1)	
		{
			touch_blink_cnt = 10;	
			if (! clear_cnt--)		cham_set_clear();
		}
    else if( lock_data == 0)
    {
      if ( ! auto_lock_cnt--) {   auto_lock_cnt = 200;	lock_data = 1; wet_change = 0;}
    } 
		else
		{
			touch_blink_cnt = 10;	
			clear_cnt = 100;			
		}
	}
//-----------------------------------	
}





void error_clear(void)
{
	if (  Touch_error(635,365) )
	{
		Error_EN = 0; Error_no = 0;	buzzer_set =1;
	}
}



void temp_inc_dec(void)
{
uint16_t gas_touch_x1, gas_touch_x2;

	if ( gas_2nd_on_off==0 )	{		gas_touch_x1 = 150;		gas_touch_x2 = 300;		}
	else	{		gas_touch_x1 = 210;		gas_touch_x2 = 335;	}

	if (cham_temp_set_mode)
	{
		if	    ( Touch_incdec2(gas_touch_x2,350) )		{cham_temp_inc();	temp_id_check= 1;	buzzer_set = 1; 	}
		else if ( Touch_incdec2(gas_touch_x2,405) )		{cham_temp_dec();	temp_id_check= 1;	buzzer_set = 1; 	}
		else if ( Touch_save(450,350) )			{cham_temp_save(); Set_Data_Write(); 	buzzer_set = 1; 	}
		else if ( Touch_all_cham(gas_touch_x1,350) )	
		{
			buzzer_set = 1; 
			cham_set_temp_dis[0]= cham_all_set_temp;
			cham_set_temp_dis[1]= cham_all_set_temp;
			cham_set_temp_dis[2]= cham_all_set_temp;
			cham_set_temp_dis[3]= cham_all_set_temp;
			cham_set_temp_dis[4]= cham_all_set_temp;
			cham_set_temp_dis[5]= cham_all_set_temp;
			cham_set_temp_dis[6]= cham_all_set_temp;
			cham_set_temp_dis[7]= cham_all_set_temp;
		}
	}

}







void cham_touch_continue(void)
{
	if (chamber_count == 0)
	{
		if (	  Touch_cham(10,50) )		{cham_working_sta[0] ^= 1; buzzer_set = 1; main_log_flag= 3;  sub_log_flag1=0;	sub_log_flag2=cham_working_sta[0]; Set_Data_Write();}
		else if ( Touch_cham(150,50) )		{cham_working_sta[1] ^= 1; buzzer_set = 1; main_log_flag= 3;  sub_log_flag1=1;	sub_log_flag2=cham_working_sta[1]; Set_Data_Write();}
		else if ( Touch_cham(290,50) )		{cham_working_sta[2] ^= 1; buzzer_set = 1; main_log_flag= 3;  sub_log_flag1=2;	sub_log_flag2=cham_working_sta[2]; Set_Data_Write();}
		else if ( Touch_cham(430,50) )		{cham_working_sta[3] ^= 1; buzzer_set = 1; main_log_flag= 3;  sub_log_flag1=3;	sub_log_flag2=cham_working_sta[3]; Set_Data_Write();}
		else if ( Touch_cham(10,190) )		{cham_working_sta[4] ^= 1; buzzer_set = 1; main_log_flag= 3;  sub_log_flag1=4;	sub_log_flag2=cham_working_sta[4]; Set_Data_Write();}
		else if ( Touch_cham(150,190) )		{cham_working_sta[5] ^= 1; buzzer_set = 1; main_log_flag= 3;  sub_log_flag1=5;	sub_log_flag2=cham_working_sta[5]; Set_Data_Write();}
		else if ( Touch_cham(290,190) )		{cham_working_sta[6] ^= 1; buzzer_set = 1; main_log_flag= 3;  sub_log_flag1=6;	sub_log_flag2=cham_working_sta[6]; Set_Data_Write();}
		else if ( Touch_cham(430,190) )		{cham_working_sta[7] ^= 1; buzzer_set = 1; main_log_flag= 3;  sub_log_flag1=7;	sub_log_flag2=cham_working_sta[7]; Set_Data_Write();}
	}
	else
	{
		if (	  Touch_cham(30,50) )		{cham_working_sta[0] ^= 1; buzzer_set = 1; main_log_flag= 3;  sub_log_flag1=0;	sub_log_flag2=cham_working_sta[0]; Set_Data_Write();}
		else if ( Touch_cham(210,50) )		{cham_working_sta[1] ^= 1; buzzer_set = 1; main_log_flag= 3;  sub_log_flag1=1;	sub_log_flag2=cham_working_sta[1]; Set_Data_Write();}
		else if ( Touch_cham(390,50) )		{cham_working_sta[2] ^= 1; buzzer_set = 1; main_log_flag= 3;  sub_log_flag1=2;	sub_log_flag2=cham_working_sta[2]; Set_Data_Write();}
		else if ( Touch_cham(30,190) )		{cham_working_sta[3] ^= 1; buzzer_set = 1; main_log_flag= 3;  sub_log_flag1=3;	sub_log_flag2=cham_working_sta[3]; Set_Data_Write();}
		else if ( Touch_cham(210,190) )		{cham_working_sta[4] ^= 1; buzzer_set = 1; main_log_flag= 3;  sub_log_flag1=4;	sub_log_flag2=cham_working_sta[4]; Set_Data_Write();}
		else if ( Touch_cham(390,190) )		{cham_working_sta[5] ^= 1; buzzer_set = 1; main_log_flag= 3;  sub_log_flag1=5;	sub_log_flag2=cham_working_sta[5]; Set_Data_Write();}
	}
}



void cham_touch_first(void)
{
uint8_t temp;
  
	if (chamber_count == 0)
	{
		if (  Touch_cham(10,50) )			{cham_setting_sta[0] ^= 1;  cham_set_temp_dis[0]= cham_set_temp[0]; buzzer_set = 1; }
		else if ( Touch_cham(150,50) )		{cham_setting_sta[1] ^= 1;	cham_set_temp_dis[1]= cham_set_temp[1]; buzzer_set = 1; }
		else if ( Touch_cham(290,50) )		{cham_setting_sta[2] ^= 1;	cham_set_temp_dis[2]= cham_set_temp[2]; buzzer_set = 1; }
		else if ( Touch_cham(430,50) )		{cham_setting_sta[3] ^= 1;	cham_set_temp_dis[3]= cham_set_temp[3]; buzzer_set = 1; }
		else if ( Touch_cham(10,190) )		{cham_setting_sta[4] ^= 1;	cham_set_temp_dis[4]= cham_set_temp[4]; buzzer_set = 1; }
		else if ( Touch_cham(150,190) )		{cham_setting_sta[5] ^= 1;	cham_set_temp_dis[5]= cham_set_temp[5]; buzzer_set = 1; }
		else if ( Touch_cham(290,190) )		{cham_setting_sta[6] ^= 1;	cham_set_temp_dis[6]= cham_set_temp[6]; buzzer_set = 1; }
		else if ( Touch_cham(430,190) )		{cham_setting_sta[7] ^= 1;	cham_set_temp_dis[7]= cham_set_temp[7]; buzzer_set = 1; }
	}
	else
	{
		if (  Touch_cham(30,50) )			{cham_setting_sta[0] ^= 1;  cham_set_temp_dis[0]= cham_set_temp[0]; buzzer_set = 1; }
		else if ( Touch_cham(210,50) )		{cham_setting_sta[1] ^= 1;	cham_set_temp_dis[1]= cham_set_temp[1]; buzzer_set = 1; }
		else if ( Touch_cham(390,50) )		{cham_setting_sta[2] ^= 1;	cham_set_temp_dis[2]= cham_set_temp[2]; buzzer_set = 1; }
		else if ( Touch_cham(30,190) )		{cham_setting_sta[3] ^= 1;	cham_set_temp_dis[3]= cham_set_temp[3]; buzzer_set = 1; }
		else if ( Touch_cham(210,190) )		{cham_setting_sta[4] ^= 1;	cham_set_temp_dis[4]= cham_set_temp[4]; buzzer_set = 1; }
		else if ( Touch_cham(390,190) )		{cham_setting_sta[5] ^= 1;	cham_set_temp_dis[5]= cham_set_temp[5]; buzzer_set = 1; }
	}

	temp = cham_setting_sta[0]+cham_setting_sta[1]+cham_setting_sta[2]+cham_setting_sta[3]+cham_setting_sta[4]+cham_setting_sta[5]+cham_setting_sta[6]+cham_setting_sta[7];
	if ( temp )	{cham_temp_set_mode = 1	;	}	//온도 세팅 상태 0= 비활상화 1=세팅모드 2= 세팅모드(증감키를 누르고 있음)
	else 		{cham_temp_set_mode = 0	;	temp_id_check= 0;	wet_change = 0;	}
}



void cham_temp_inc(void)
{
uint8_t ch_no;
	for(ch_no=0; ch_no<8 ; ch_no++)
	{
		if ( cham_setting_sta[ch_no] == 0x01)
		{
			if(cham_set_temp_dis[ch_no] != 450) 	cham_set_temp_dis[ch_no]++;
			cham_temp_set_mode = 2	;		//디스플레이 블링크 가 안되도록
		}
	}
}



void cham_temp_dec(void)
{
uint8_t ch_no;
	for(ch_no=0; ch_no<8 ; ch_no++)
	{
		if ( cham_setting_sta[ch_no] == 0x01)
		{
			if(cham_set_temp_dis[ch_no] != 200) 	cham_set_temp_dis[ch_no]--;
			cham_temp_set_mode = 2	;		//디스플레이 블링크 가 안되도록
			
		}
	}
}


void cham_cali_save(uint8_t ch_dat, uint8_t ch_pos, uint16_t set_temp)
{
uint8_t set_flag[2];
uint8_t set_data[2];

  set_flag[0] = ch_pos + 1;
  set_flag[1] = 1 << ch_dat;

	Temp_set_Write(set_flag, set_data, &set_temp); //set_flag

  if(ch_pos < 2)
  {
    main_log_flag= 9;  sub_log_flag1 = ch_dat;	sub_log_flag2=0; 
  }
  else
  {
    main_log_flag= 10;  sub_log_flag1 = ch_dat;	sub_log_flag2=0; 
  }
}


void cham_temp_save(void)
{
uint8_t ch_no;
uint8_t set_ch_no = 0;
uint8_t set_flag[2];
	for(ch_no=0; ch_no<8 ; ch_no++)
	{
		set_ch_no =  set_ch_no >> 1;
		if ( cham_setting_sta[ch_no] == 0x01)
		{
			cham_set_temp[ch_no] = cham_set_temp_dis[ch_no];		//시간 저장

			set_ch_no += 0x80;
		}
	}
	cham_temp_set_mode = 0;				//세팅 모드 해제
	lcd_popup_flag = 0;
	wet_change = 0;

  set_flag[0] = 0;
  set_flag[1] = 0;

	Temp_set_Write(set_flag, cham_setting_sta, cham_set_temp);

	main_log_flag= 1;  sub_log_flag1=set_ch_no;	sub_log_flag2=0; 

	for(ch_no=0; ch_no<8 ; ch_no++)
	{
		cham_setting_sta[ch_no] = 0;		//시간 저장
	}

}

void test_rs232(uint8_t char_sta)
{
	test_Tx_sta =1;

	if (char_sta == 0)		test_Tx_data ='a';
	else if(char_sta == 1)	test_Tx_data ='7';
	else if(char_sta == 2)	test_Tx_data ='C';
	else if(char_sta == 3)	test_Tx_data ='Q';
}


void Touch_test_mode_Check(void)
{
//-----------------------------------	
static uint8_t	touch_flag = 0;
static uint8_t tx_cnt = 0;

	if (Touch_x_data || Touch_y_data)
	{
		if( touch_flag == 0)
		{
		if 		(  Touch_test(215,  35+50*1) )		{test_top_pwm[0] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(215,  35+50*2) )		{test_top_pwm[1] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(215,  35+50*3) )		{test_top_pwm[2] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(215,  35+50*4) )		{test_top_pwm[3] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(215,  35+50*5) )		{test_top_pwm[4] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(215,  35+50*6) )		{test_top_pwm[5] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(215,  35+50*7) )		{test_top_pwm[6] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(215,  35+50*8) )		{test_top_pwm[7] ^= 1;  buzzer_set = 1; }

		else if	(  Touch_test(285,  35+50*1) )		{test_bot_pwm[0] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(285,  35+50*2) )		{test_bot_pwm[1] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(285,  35+50*3) )		{test_bot_pwm[2] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(285,  35+50*4) )		{test_bot_pwm[3] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(285,  35+50*5) )		{test_bot_pwm[4] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(285,  35+50*6) )		{test_bot_pwm[5] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(285,  35+50*7) )		{test_bot_pwm[6] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(285,  35+50*8) )		{test_bot_pwm[7] ^= 1;  buzzer_set = 1; }

		else if	(  Touch_test(425,  35+50*1) )		{	if(++test_door_led[0] == 4)	{test_door_led[0]= 0;}  buzzer_set = 1; }
		else if (  Touch_test(425,  35+50*2) )		{	if(++test_door_led[1] == 4)	{test_door_led[1]= 0;}  buzzer_set = 1; }
		else if (  Touch_test(425,  35+50*3) )		{	if(++test_door_led[2] == 4)	{test_door_led[2]= 0;}  buzzer_set = 1; }
		else if (  Touch_test(425,  35+50*4) )		{	if(++test_door_led[3] == 4)	{test_door_led[3]= 0;}  buzzer_set = 1; }
		else if (  Touch_test(425,  35+50*5) )		{	if(++test_door_led[4] == 4)	{test_door_led[4]= 0;}  buzzer_set = 1; }
		else if (  Touch_test(425,  35+50*6) )		{	if(++test_door_led[5] == 4)	{test_door_led[5]= 0;}  buzzer_set = 1; }
		else if (  Touch_test(425,  35+50*7) )		{	if(++test_door_led[6] == 4)	{test_door_led[6]= 0;}  buzzer_set = 1; }
		else if (  Touch_test(425,  35+50*8) )		{	if(++test_door_led[7] == 4)	{test_door_led[7]= 0;}  buzzer_set = 1; }

		else if	(  Touch_test(495,  35+50*1) )		{test_out1_work[0] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(495,  35+50*2) )		{test_out1_work[1] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(495,  35+50*3) )		{test_out1_work[2] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(495,  35+50*4) )		{test_out1_work[3] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(495,  35+50*5) )		{test_out1_work[4] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(495,  35+50*6) )		{test_out1_work[5] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(495,  35+50*7) )		{test_out1_work[6] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(495,  35+50*8) )		{test_out1_work[7] ^= 1;  buzzer_set = 1; }

		else if	(  Touch_test(565,  35+50*1) )		{test_out2_work[0] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(565,  35+50*2) )		{test_out2_work[1] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(565,  35+50*3) )		{test_out2_work[2] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(565,  35+50*4) )		{test_out2_work[3] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(565,  35+50*5) )		{test_out2_work[4] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(565,  35+50*6) )		{test_out2_work[5] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(565,  35+50*7) )		{test_out2_work[6] ^= 1;  buzzer_set = 1; }
		else if (  Touch_test(565,  35+50*8) )		{test_out2_work[7] ^= 1;  buzzer_set = 1; }

		else if (  Touch_test(640,  235) )			{ if(tx_cnt==4) tx_cnt=0;	test_rs232(tx_cnt++);	buzzer_set = 1; }


		}
		touch_flag =1 ;
	}
	else if (touch_flag)
	{
		touch_flag =0 ;
	}
//-----------------------------------	
}





void gas1_touch_cont(void)
{
	 if	( Touch_incdec2(35,80))	{	if(set_data_temp != 99)		set_data_temp++;	buzzer_set = 1;}		//co2
	else if ( Touch_incdec2(35,190) )	{	if(set_data_temp != 1 )		set_data_temp--;	buzzer_set = 1;}

	else if	( Touch_incdec2(185,80))	{	if(set_data_temp2 != 99)	 set_data_temp2++;	buzzer_set = 1;}		//o2
	else if ( Touch_incdec2(185,190) )	{	if(set_data_temp2 != 1 )	 set_data_temp2--;	buzzer_set = 1;}

	else if ( Touch_incdec2(35,280) )		{	if(set_data_temp3 != 99)		set_data_temp3++;	buzzer_set = 1;}	//initiial
	else if ( Touch_incdec2(35,390) )		{	if(set_data_temp3 !=  1)	 	set_data_temp3--;	buzzer_set = 1;}

	else if ( Touch_incdec2(185,280) )		{	if(set_data_temp4 !=999)	set_data_temp4++;	buzzer_set = 1;}		//period time
	else if ( Touch_incdec2(185,390) )		{	if(set_data_temp4 !=  1)	set_data_temp4--;	buzzer_set = 1;}

	else if ( Touch_incdec2(335,280) )		{	if(set_data_temp5 != 99)	set_data_temp5++;	buzzer_set = 1;}	//work time
	else if ( Touch_incdec2(335,390) )		{	if(set_data_temp5 != 1)		set_data_temp5--;	buzzer_set = 1;}

	else if ( Touch_incdec2(335,80) )		{	if(set_data_temp6 != 99)	set_data_temp6++;	buzzer_set = 1;}	//gas1_gas2_dual_time
	else if ( Touch_incdec2(335,190) )		{	if(set_data_temp6 != 0)		set_data_temp6--;	buzzer_set = 1;}

}



void gas1_touch_first(void)
{
	gas1_touch_cont();

	if 		( Touch_set_tab(  20,10) )			 	{ mode_value = gas_1_setting;		buzzer_set = 1;}
	else if ( Touch_set_tab(180,10) )		{ mode_value = gas_2_setting;		buzzer_set = 1;}
	else if ( Touch_save(450,210) )			{   
												co2_set_data =set_data_temp;	 o2_set_data =set_data_temp2;
												gas_initial_time =set_data_temp3;	 gas_period_time =set_data_temp4;	 gas_work_time =set_data_temp5;
												gas1_gas2_dual_time =	set_data_temp6;
												buzzer_set = 1;		Set_Data_Write();}
}




void gas2_touch_cont(void)
{
	 if	( Touch_incdec2(185,80))	{	if(set_data_temp != 99)		set_data_temp++;	buzzer_set = 1;}							//co2
	else if ( Touch_incdec2(185,190) )	{	if(set_data_temp != 1 )		set_data_temp--;	buzzer_set = 1;}

	else if	( Touch_incdec2(335,80))	{	if(set_data_temp2 != 99)	 set_data_temp2++;	buzzer_set = 1;}		//o2
	else if ( Touch_incdec2(335,190) )	{	if(set_data_temp2 != 1 )	 set_data_temp2--;	buzzer_set = 1;}

	else if ( Touch_incdec2(35,280) )		{	if(set_data_temp3 != 99)		set_data_temp3++;	buzzer_set = 1;}	//initiial
	else if ( Touch_incdec2(35,390) )		{	if(set_data_temp3 !=  1)	 	set_data_temp3--;	buzzer_set = 1;}

	else if ( Touch_incdec2(185,280) )		{	if(set_data_temp4 !=999)	set_data_temp4++;	buzzer_set = 1;}		//period time
	else if ( Touch_incdec2(185,390) )		{	if(set_data_temp4 !=  1)	set_data_temp4--;	buzzer_set = 1;}

	else if ( Touch_incdec2(335,280) )		{	if(set_data_temp5 != 99)	set_data_temp5++;	buzzer_set = 1;}	//work time
	else if ( Touch_incdec2(335,390) )		{	if(set_data_temp5 != 1)		set_data_temp5--;	buzzer_set = 1;}

	else if ( Touch_incdec2(35,80) )		{	if(set_data_temp6 != 99)	set_data_temp6++;	buzzer_set = 1;}	//start time
	else if ( Touch_incdec2(35,190) )		{	if(set_data_temp6 != 1)		set_data_temp6--;	buzzer_set = 1;}
	
}


void gas2_touch_first(void)
{
	gas2_touch_cont();

	if 		( Touch_set_tab(  20,10) )			 		{ mode_value = gas_1_setting;	buzzer_set = 1;}
	else if ( Touch_set_tab(180,10) )			{ mode_value = gas_2_setting;		buzzer_set = 1;}
	else if ( Touch_onoff_dot(380,30) )		{ set_data_temp7 = 1;		buzzer_set = 1;}	//on dot
	else if ( Touch_onoff_dot(490,30) )		{ set_data_temp7 = 0;		buzzer_set = 1;}	//off dot
	else if ( Touch_save(450,210) )			{   
												co2_2nd_set_data =set_data_temp;	 o2_2nd_set_data =set_data_temp2;
												gas2_initial_time =set_data_temp3;	 gas2_period_time =set_data_temp4;	 gas2_work_time =set_data_temp5;
												gas2_start_time =set_data_temp6;	 gas_2nd_on_off =set_data_temp7;
												buzzer_set = 1;		Set_Data_Write();}
	}




void Touch_Set_mode_Check6(void)
{
	if( touch_flag1 == 4 )
	{
		if (mode_value == gas_1_setting)			gas1_touch_first();
		else if (mode_value == gas_2_setting)	gas2_touch_first();
	}

	if( touch_flag1 == 2 )
	{
		if (mode_value == gas_1_setting)			gas1_touch_cont();
		else if (mode_value == gas_2_setting)		gas2_touch_cont();
	}

	if( touch_flag1 == 3 && touch_flag2 == 1 )
	{
		if (mode_value == gas_1_setting)			gas1_touch_cont();
		else if (mode_value == gas_2_setting)		gas2_touch_cont();
	}
}











#else


void Touch_Check(void)
{
static uint8_t	touch_flag = 0	;

	Touch_Read();

	if (Touch_x_data || Touch_y_data)
	{
		if( touch_flag == 0)
		{
			if ( Touch_in(0,0) )				output_work[0] ^= 1;
			else if ( Touch_in(100,  0) )		output_work[1] ^= 1;
			else if ( Touch_in(200,  0) )		output_work[2] ^= 1;
			else if ( Touch_in(300,  0) )		output_work[3] ^= 1;
			else if ( Touch_in(400,  0) )		output_work[4] ^= 1;
			else if ( Touch_in(500,  0) )		output_work[5] ^= 1;

			else if ( Touch_in(0,  100) )		output_work[6] ^= 1;
			else if ( Touch_in(100,100) )		output_work[7] ^= 1;
			else if ( Touch_in(200,100) )		output_work[8] ^= 1;
			else if ( Touch_in(300,100) )		output_work[9] ^= 1;
			else if ( Touch_in(400,100) )		output_work[10] ^= 1;
			else if ( Touch_in(500,100) )		output_work[11] ^= 1;
			else if ( Touch_in(600,100) )		output_work[12] ^= 1;
			else if ( Touch_in(700,100) )		output_work[13] ^= 1;
//******************************************
			else if ( Touch_in( 0, 200) )
			{
				output_work[14] = 1;
				if(pwm_d[0] < 40) pwm_d[0]++; 
			}
			else if ( Touch_in(100,200) )	
			{
				output_work[15] = 1;
				if(pwm_d[0] > 0) pwm_d[0]--; 
			}
//******************************************
			else if ( Touch_in( 0, 300) )
			{
				output_work[16] = 1;
				if(pwm_d[1] < 40) pwm_d[1]++; 
			}
			else if ( Touch_in(100,300) )	
			{
				output_work[17] = 1;
				if(pwm_d[1] > 0) pwm_d[1]--; 
			}
//******************************************
			else if ( Touch_in( 0, 400) )
			{
				output_work[18] = 1;
				if(pwm_d[2] < 40) pwm_d[2]++; 
			}
			else if ( Touch_in(100,400) )	
			{
				output_work[19] = 1;
				if(pwm_d[2] > 0) pwm_d[2]--; 
			}
//******************************************	
			
			
		}
		touch_flag =1 ;
	}

	else if (touch_flag)
	{
		touch_flag =0 ;
		output_work[14] = 0;
		output_work[15] = 0;
		output_work[16] = 0;
		output_work[17] = 0;
		output_work[18] = 0;
		output_work[19] = 0;

	}

}


#endif













Status I2C_BufferWriteRead(u8* pW_Buffer, u8* pR_Buffer,  u16 NumByteToWrite, u16 NumByteToRead, u8  SlaveAddress);


void touch_check(void)
{
u8 Tx_Buffer[1];
u8 Rx_Buffer[1];

  Tx_Buffer[0]= 0xA3;
	I2C_BufferWriteRead(Tx_Buffer, Rx_Buffer,  1, 1, SLAVE_ADDRESS<<1);
	I2C_GenerateSTOP(I2C2, ENABLE);
  if ( Rx_Buffer[0] == 85)  touch_version = 0;  //기존  FT5306 
  else                        touch_version = 1;  //신형 FT5436
}


void Touch_Read(void)
{
u8 Tx_Buffer[1];
u8 Rx_Buffer[5];

	uint16_t dat_x=0, dat_y=0;	
	uint8_t dat_high, dat_low;
	uint8_t touch_point;

  
	Tx_Buffer[0]= 0x02;
	I2C_BufferWriteRead(Tx_Buffer, Rx_Buffer,  1, 5, SLAVE_ADDRESS<<1);
	I2C_GenerateSTOP(I2C2, ENABLE);
	I2C2_Init();

	
	touch_point = Rx_Buffer[0];

	dat_high = Rx_Buffer[1] & 0x0F;	// read one byte and don't request another byte, stop transmission
	dat_low = Rx_Buffer[2];			// read one byte and don't request another byte, stop transmission
	dat_y = (uint16_t)dat_high<<8 | dat_low;

	dat_high = Rx_Buffer[3]; 	// read one byte and don't request another byte, stop transmission
	dat_low = Rx_Buffer[4];		 	// read one byte and don't request another byte, stop transmission
	dat_x = (uint16_t)dat_high<<8 | dat_low;


	if (touch_point == 0 | touch_point >= 5 )	//카운트가 0이면 터치 좌표가 0
	{
		dat_x =0;
		dat_y =0;
	}

	Touch_count = touch_point;
  
  if( touch_version == 0  )
  {
	Touch_x_data = dat_x;
	Touch_y_data = dat_y;
  }
  else
  {
    Touch_x_data = dat_y;
    Touch_y_data = dat_x;
  }

	return;
}











/*******************************************************************************
* Function Name  : I2C_BufferWriteRead
* Description    : I
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
Status I2C_BufferWriteRead(u8* pW_Buffer, u8* pR_Buffer,  u16 NumByteToWrite, u16 NumByteToRead, u8  SlaveAddress)
{  
	__IO uint32_t Timeout = 0;

	/* While the bus is busy */
//	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));

	// Clear EV6 by setting again the PE bit 
//	I2C_Cmd(I2C2, ENABLE);

	/* Send START condition */
	I2C_GenerateSTART(I2C2, ENABLE);

	/* Test on EV5 and clear it */
	Timeout = Timeout_value;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
	{	if (Timeout-- == 0)		return Error;		} 

	/* Send EEPROM address for write */
	I2C_Send7bitAddress(I2C2, SlaveAddress, I2C_Direction_Transmitter);

	/* Test on EV6 and clear it */
	Timeout = Timeout_value;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{	if (Timeout-- == 0)		return Error;		} 

	/* Clear EV6 by setting again the PE bit */
//	I2C_Cmd(I2C2, ENABLE);

	/* While there is data to be written */
	while(NumByteToWrite--)  
	{
		/* Send the current byte */
		I2C_SendData(I2C2, *pW_Buffer); 

		/* Point to the next byte to be written */
		pW_Buffer++; 

		/* Test on EV8 and clear it */
		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}

	/* Send STRAT condition a second time */  
	I2C_GenerateSTART(I2C2, ENABLE);

	/* Test on EV5 and clear it */
	Timeout = Timeout_value;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
	{	if (Timeout-- == 0)		return Error;		} 

	/* Send EEPROM address for read */
	I2C_Send7bitAddress(I2C2, SlaveAddress, I2C_Direction_Receiver);

	/* Test on EV6 and clear it */
	Timeout = Timeout_value;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{	if (Timeout-- == 0)		return Error;		} 

	/* While there is data to be read */
	while(NumByteToRead)  
	{
		if(NumByteToRead == 1)
		{
			/* Disable Acknowledgement */
			I2C_AcknowledgeConfig(I2C2, DISABLE);

			/* Send STOP Condition */
			I2C_GenerateSTOP(I2C2, ENABLE);
		}

		/* Test on EV7 and clear it */
		if(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))  
		{      
			/* Read a byte from the EEPROM */
			*pR_Buffer = I2C_ReceiveData(I2C2);

			/* Point to the next location where the byte read will be saved */
			pR_Buffer++; 

			/* Decrement the read bytes counter */
			NumByteToRead--;        
		}   
	}
	/* Enable Acknowledgement to be ready for another reception */
	I2C_AcknowledgeConfig(I2C2, ENABLE);

    return Success;
}




void I2C2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStruct;

	RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C2, ENABLE);

	I2C_DeInit(I2C2);
	/* setup SCL and SDA pins
	 * 1. SCL on PA8 and SDA on PC9
	 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// set pins to alternate function
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// set GPIO speed
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;			// set output to open drain --> the line has to be only pulled low, not driven high
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// enable pull up resistors

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					// init GPIOB,10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					// init GPIOB,11

	// Connect I2C2 pins to AF  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);	// SCL
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2); 	// SDA

	// configure I2C2
	I2C_InitStruct.I2C_ClockSpeed = 100000; 		// 100kHz
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;			// I2C mode
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;	// 50% duty cycle --> standard
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;			// own address, not relevant in master mode
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;		// I2C_Ack_Disable;		// disable acknowledge when reading (can be changed later on)
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
	I2C_Init(I2C2, &I2C_InitStruct);				// init I2C2
	
	// enable I2C2
	I2C_Cmd(I2C2, ENABLE);

  Delay_us(100);

  touch_check();
}



















/*
void Touch_Read(void)
{
	uint16_t dat_x=0, dat_y=0;	
	uint8_t dat_high, dat_low;
	uint8_t touch_point;

	I2C_start(I2C2, SLAVE_ADDRESS<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	I2C_write(I2C2, 0x02); // write one byte to the slave
	I2C_stop(I2C2); // stop the transmission

	I2C_start(I2C2, SLAVE_ADDRESS<<1, I2C_Direction_Receiver); // start a transmission in Master receiver mode

	touch_point = I2C_read_ack(I2C2);

	dat_high = I2C_read_ack(I2C2) & 0x0F;	// read one byte and don't request another byte, stop transmission
	dat_low = I2C_read_ack(I2C2);			// read one byte and don't request another byte, stop transmission
	dat_y = (uint16_t)dat_high<<8 | dat_low;

	dat_high = I2C_read_ack(I2C2) & 0x0F; 	// read one byte and don't request another byte, stop transmission
	dat_low = I2C_read_nack(I2C2);		 	// read one byte and don't request another byte, stop transmission
	dat_x = (uint16_t)dat_high<<8 | dat_low;

	if (touch_point == 0 | touch_point >= 5 )	//카운트가 0이면 터치 좌표가 0
	{
		dat_x =0;
		dat_y =0;
	}

	Touch_count = touch_point;
	Touch_x_data = dat_x;
	Touch_y_data = dat_y;

	return;
}

*/


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
