/**
  ******************************************************************************
  * @file    stm322xg_eval_lcd.c
  * @author  MCD Application Team
  * @version V5.0.3
  * @date    09-March-2012
  * @brief   This file includes the LCD driver for AM-240320L8TNQW00H (LCD_ILI9320)
  *          and AM240320D5TOQW01H (LCD_ILI9325) Liquid Crystal Display Modules
  *          of STM322xG-EVAL evaluation board(MB786) RevA and RevB.
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
#include "graphic.h"
#include "fsmc_lcd.h"
#include "data_io.h"
#include "fonts.c"


#define	revision_no		"20201210_A"





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




#define	settting_sel	0
#define	graph_sel		1
#define	history_sel		2
#define	calib_sel		3
#define	temp_log_sel	4






#define Back_color		0xd69a



/** @defgroup STM322xG_EVAL_LCD_Private_Defines
  * @{
  */
 
static uint16_t  bg_color;
static uint16_t  pn_color;


extern uint16_t x_12bit;
extern uint16_t y_12bit;
extern uint16_t Touch_x_data;
extern uint16_t Touch_y_data;

extern uint8_t cali_set;

extern uint8_t time_year, time_week, time_month, time_date, time_hour, time_min, time_sec;
extern uint8_t set_year, set_week, set_month, set_date, set_hour, set_min, set_sec;

extern uint32_t CO2_data;
extern uint32_t O2_data;
extern uint32_t N2_data;

uint8_t lcd_clear_flag = 0;

uint8_t lcd_popup_flag = 0;

uint8_t co2_work_val;
uint8_t co2_set_val;
uint8_t co2_now_val;

uint8_t o2_work_val;
uint8_t o2_set_val;
uint8_t o2_now_val;

extern uint16_t main_log_flag ;
extern uint16_t sub_log_flag1 ;
extern uint16_t sub_log_flag2 ;


extern uint16_t press_data1;
extern uint16_t press_data2;

extern uint16_t cham_working_min[8] ;	//½Ã°£ ´ÜÀ§ -> ºÐ
extern uint8_t cham_working_sta[8] ;	//¹è¾ç»óÅÂ 0= ºó»óÅÂ 1=¹è¾çÁß

extern uint16_t cham_set_temp[8] ;		//Ã¨¹ö 0~7 ÀÇ ¼¼ÆÃ¿Âµµ
extern uint16_t cham_top_temp[8] ;		//Ã¨¹ö 0~7 ÀÇ À­ ÆÇ³Ú ¿Âµµ
extern uint16_t cham_bot_temp[8] ;		//Ã¨¹ö 0~7 ÀÇ ¾Æ·¡ ÆÇ³Ú ¿Âµµ
extern uint16_t cham_now_temp[8] ;		//Ã¨¹ö 0~7 ÀÇ ·ÃÀç¿Âµµ

extern uint16_t ext_sensor_sta ;			//¿ÜºÎ Ã¨¹ö ¼¾¼­ ¿¬°á°ª
extern uint16_t ext_sensor_temp;			//¿ÜºÎ Ã¨¹ö ¿Âµµ°ª

extern uint8_t door_status_data[8] ;		//¹®»óÅÂ 0= ´ÝÈû»óÅÂ 1=¿­¸²»óÅÂ
extern uint8_t door_open_error[8] ;		//¹® ¿­¸²»óÅÂ 30ÃÊ ÀÌ»ó

extern uint8_t cham_temp_set_mode	;		//¿Âµµ ¼¼ÆÃ »óÅÂ 0= ºñÈ°»óÈ­ 1=¼¼ÆÃ¸ðµå 2= ¼¼ÆÃ¸ðµå(Áõ°¨Å°¸¦ ´©¸£°í ÀÖÀ½)

extern uint8_t cham_setting_sta[8] ;		//°³º° Ã¨¹ö ¿Âµµ ¼¼ÆÃ »óÅÂ
extern uint16_t cham_set_temp_dis[8] ;		//Ã¨¹ö 0~6 ÀÇ ¼¼ÆÃ¿Âµµ µð½ºÇÃ·¹ÀÌ°ª

extern uint16_t		ADC_Value[2];				//adc º¯È¯°ª

extern uint16_t Press_on_data;
extern uint16_t Press_off_data;
extern uint16_t Press_err_data;
extern uint16_t Press_err_cnt;

extern uint16_t Press2_on_data;
extern uint16_t Press2_off_data;
extern uint16_t Press2_err_data;
extern uint16_t Press2_err_cnt;


uint8_t data_loading_cham = 0 ;				//µ¥ÀÌÅ¸ ·Îµå Ã¨¹ö
uint8_t	temp_set_mode = 0; 					//¿Âµµ ¼¼ÆÃ ¸ðµå


extern int16_t 	voc_filter_time;					//ÀüÃ¼ Ã¨¹ö ¼¼ÆÃ°ª
extern uint8_t 	mode_value ;		//¼¼ÆÃ ¸ðµå °ª 0=nomal / 1= co2 /2 = o2/3= temp
extern uint8_t	sub_set_mode ;			// ¼­ºê ¼¼ÆÃ ¸ðµå

extern uint16_t cham_all_set_temp ;

extern uint16_t heat_top_pwm[8];
extern uint16_t heat_bot_pwm[8];


//extern uint16_t temp_set_data[8] ;	//Ã¨¹ö 0~7 ÀÇ ¿Âµµ ¼¼ÆÃ °ª
//extern uint16_t temp_set_input[8] ;	//Ã¨¹ö 0~7 ÀÇ ¼¾¼­ ¼¼ÆÃ °ª



extern uint8_t test_top_pwm[8];
extern uint8_t test_bot_pwm[8];
extern uint8_t test_door_led[8];		//¹®»óÅÂ 0= x 1=red 2= green 3= orange

extern uint8_t test_out1_work[8];
extern uint8_t test_out2_work[8];

extern uint8_t device_name[4];
uint8_t device_temp[4];


uint16_t	setting_data_page = 0;		//¼¼ÆÃ½Ã ±×·¡ÇÁ ÆäÀÌÁö ¹øÈ£


extern uint8_t	test_mode;

extern uint8_t	wet_change;

extern uint8_t test_Tx_data;
extern uint8_t test_Rx_data;
extern uint8_t test_Tx_sta;


extern uint16_t ext_set_data ;
extern uint16_t ext_set_input ;

extern uint8_t lock_data ;


uint8_t Error_no = 0;
uint8_t Sub_error = 0;
uint8_t Error_EN = 0;

uint16_t	gas_initial_time ;
uint16_t	gas_period_time  ;
uint16_t	gas_work_time ;

uint16_t	gas2_start_time ;
uint16_t	gas2_initial_time ;
uint16_t	gas2_period_time  ;
uint16_t	gas2_work_time ;
uint16_t	gas1_gas2_dual_time ;




uint16_t	o2_set_data;						//o2 ¼¼ÆÃ µ¥ÀÌÅ¸
uint16_t	co2_set_data;						//co2 ¼¼ÆÃ µ¥ÀÌÅ¸

uint16_t	o2_2nd_set_data;						//o2 ¼¼ÆÃ µ¥ÀÌÅ¸
uint16_t	co2_2nd_set_data;						//co2 ¼¼ÆÃ µ¥ÀÌÅ¸
uint8_t	gas_2nd_on_off;						//


uint8_t      change_sta	 = 0;



uint8_t		set_ch_no = 0;			//¼¼ÆÃ½Ã ±×·¡ÇÁ Ã¤³Î ¹øÈ£
uint8_t		set_posit = 0;
uint16_t	set_data_adr =0 ;	//µ¥ÀÌÅ¸ ·Îµå ±âº» ÁÖ¼Ò
uint8_t		set_dis_refresh = 0;	//¼¼ÆÃ µð½ºÃú·¹ÀÌ ¸®ÇÁ·¹½¬

uint16_t	set_data_temp = 0;		//ÀÓ½Ã ¼¼ÆÃ µ¥ÀÌÅ¸
uint16_t	set_data_temp2 = 0;		//ÀÓ½Ã ¼¼ÆÃ µ¥ÀÌÅ¸
uint16_t	set_data_temp3 = 0;		//ÀÓ½Ã ¼¼ÆÃ µ¥ÀÌÅ¸
uint16_t	set_data_temp4 = 0;		//ÀÓ½Ã ¼¼ÆÃ µ¥ÀÌÅ¸
uint16_t	set_data_temp5 = 0;		//ÀÓ½Ã ¼¼ÆÃ µ¥ÀÌÅ¸
uint16_t	set_data_temp6 = 0;		//ÀÓ½Ã ¼¼ÆÃ µ¥ÀÌÅ¸
uint16_t	set_data_temp7 = 0;		//ÀÓ½Ã ¼¼ÆÃ µ¥ÀÌÅ¸


uint8_t	pass_data[4] = {0,0,0,0};
uint8_t	save_pass_data[4];


uint8_t wet_mode;
uint8_t wet_mode_temp;
uint8_t	chamber_count;		//0 =8 chamber 1= 6 chamber



typedef struct DATA_SAVE_T1{
uint8_t data_year;
uint8_t data_month;
uint8_t data_date;
uint8_t data_hour;
uint8_t data_min;
uint16_t data_settemp;
}data_save_t1;


typedef struct DATA_SAVE_T2{
uint8_t data_cham0;
uint8_t data_cham1;
uint8_t data_cham2;
uint8_t data_cham3;
uint8_t data_cham4;
uint8_t data_cham5;
uint8_t data_cham6;
}data_save_t2;



uint8_t	error_set_save[22];
uint8_t	out_usb_mode[6];
extern uint8_t usb_out_status;


uint8_t voc_valid_time;
uint32_t valid_time_hour;
uint8_t change_alarm_time = 15;



uint8_t font_size = 0;					//0= 12*16, 1= 16*24 
uint8_t	 font_size_x, font_size_y;


/** @defgroup STM322xG_EVAL_LCD_Private_FunctionPrototypes
  * @{
  */ 



/** @defgroup STM322xG_EVAL_LCD_Private_Functions
  * @{
  */ 


void font_size_select(uint8_t size);
void test_mode_display(void);
void LCD_first(void);
void lcd_main(void);
void lcd_setting(void);
void lcd_setting2(void);
void lcd_setting3(void);
void lcd_setting4(void);
void lcd_setting5(void);
void lcd_setting6(void);
void LCD_left_data(void);
void lcd_left_data_display(void);

void LCD_alarm_load(uint8_t *string, uint16_t number, uint8_t len, uint8_t *string2);
void LCD_ShowString_buf(uint16_t x, uint16_t y, uint8_t *string, uint16_t number);




void LCD_Display(void)
{
///////////////////////////////////////////////////////
	if (test_mode != 0)
	{
		test_mode_display();
		return;
	}
///////////////////////////////////////////////////////

	if (mode_value==main_sel)	{lcd_main();
								lcd_clear_flag = 0;	}
	else if (mode_value<date_sel)	{	lcd_setting();
								lcd_clear_flag = 1;}
	else if (mode_value>=date_sel && mode_value<new_co2_sel )
							{	lcd_setting2();
								lcd_clear_flag = 2;}
	else if (mode_value>=new_co2_sel && mode_value<alarm_list)
							{	lcd_setting3();
								lcd_clear_flag = 3;}
	else if (mode_value>=alarm_list && mode_value<device_set)
							{	lcd_setting4();
								lcd_clear_flag = 4;}
	else if (mode_value>=device_set && mode_value<gas_1_setting)
							{	lcd_setting5();
								lcd_clear_flag = 5;}
	else if  (mode_value>=gas_1_setting)
							{	lcd_setting6();
									lcd_clear_flag = 6;}
							
	lcd_left_data_display();
}




/**********************************************************************************/

void LCD_time_updw_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (  x1+15,   y1+0,  x1+15+59, y1+35, IMG_time_up);
	LCD_DrawPicture (  x1+15,   y1+86,  x1+15+59, y1+86+35, IMG_time_dw);
	LCD_Box(x1, y1+38, x1+90, y1+38+44, LGRAY);
}


void LCD_time_updw_icon2(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (  x1,   y1+0,  x1+59, y1+35, IMG_time_up);
	LCD_DrawPicture (  x1,   y1+86,  x1+59, y1+86+35, IMG_time_dw);
	LCD_Box(x1, y1+38, x1+59, y1+38+44, LGRAY);

}

void LCD_set_tab_off_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+9,  y1+51, set_tab_off_0);
	LCD_DrawPicture2(   x1+9,   y1+0, x1+147, y1+51, set_tab_off_2);
	LCD_DrawPicture ( x1+148,   y1+0, x1+157, y1+51, set_tab_off_1);
}

void LCD_set_tab_on_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+9,  y1+51, set_tab_on_0);
	LCD_DrawPicture2(   x1+9,   y1+0, x1+147, y1+51, set_tab_on_2);
	LCD_DrawPicture ( x1+148,   y1+0, x1+157, y1+51, set_tab_on_1);
}


void LCD_select2_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+3,  y1+31, select0_0);
	LCD_DrawPicture2(   x1+4,   y1+0, x1+131, y1+31, select0_2);
	LCD_DrawPicture ( x1+132,   y1+0, x1+135, y1+31, select0_1);
}


void LCD_select3_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+3,  y1+31, select1_0);
	LCD_DrawPicture2(   x1+4,   y1+0, x1+131, y1+31, select1_2);
	LCD_DrawPicture ( x1+132,   y1+0, x1+135, y1+31, select1_1);
}


void LCD_small_on_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+7,  y1+59, small_on_0);
	LCD_DrawPicture2(   x1+8,   y1+0, x1+51,  y1+59, small_on_2);
	LCD_DrawPicture (  x1+52,   y1+0, x1+59,  y1+59, small_on_1);
}


void LCD_small_off_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+7, y1+59, small_off_0);
	LCD_DrawPicture2(   x1+8,   y1+0, x1+51, y1+59, small_off_2);
	LCD_DrawPicture (  x1+52,   y1+0, x1+59, y1+59, small_off_1);
}


void LCD_small_off_icon2(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+7, y1+59, small_off_0);
	LCD_DrawPicture2(   x1+8,   y1+0, x1+71, y1+59, small_off_2);
	LCD_DrawPicture (  x1+72,   y1+0, x1+79, y1+59, small_off_1);
}

void LCD_small_off_icon3(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+7, y1+59, small_off_0);
	LCD_DrawPicture2(   x1+8,   y1+0, x1+31, y1+59, small_off_2);
	LCD_DrawPicture (  x1+32,   y1+0, x1+39, y1+59, small_off_1);
}


/**********************************************************************************/
void LCD_air_icon_a(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+17, y1+111, air_icon_0);
	LCD_DrawPicture2(  x1+18,   y1+0,  x1+99, y1+111, air_icon_2);
	LCD_DrawPicture ( x1+100,   y1+0, x1+117, y1+111, air_icon_1);
}

void LCD_air_icon_b(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+19, y1+109, cham_total_0);
	LCD_DrawPicture2(  x1+20,   y1+0,  x1+99, y1+109, cham_total_2);
	LCD_DrawPicture ( x1+100,   y1+0, x1+119, y1+109, cham_total_1);
}


void LCD_air_icon_2gas(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+17, y1+111, air_icon_0);
	LCD_DrawPicture2(  x1+18,   y1+0,  x1+175, y1+111, air_icon_2);
	LCD_DrawPicture ( x1+176,   y1+0, x1+193, y1+111, air_icon_1);
}



void LCD_all_temp_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+19, y1+109, cham_total_0);
	LCD_DrawPicture2(  x1+20,   y1+0,  x1+99, y1+109, cham_total_2);
	LCD_DrawPicture ( x1+100,   y1+0, x1+119, y1+109, cham_total_1);

	LCD_DrawPicture (   x1+20,   y1+32,  x1+20+75, y1+32+17, cham_total_all);	//76,18
	LCD_DrawPicture (   x1+20,   y1+67,  x1+20+27, y1+67+13, cham_total_set);	//28,14

	font_size_select(0x00);
	LCD_ColorNumber2( x1+60,   y1+67,  YELLOW,   cham_total_2+67,  cham_all_set_temp, 3,  ZERO_DISABLE);
	LCD_Fill( x1+60+22,  y1+67+14,  x1+60+22+1,   y1+67+14+1,  YELLOW);
}






void LCD_data_dec_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+21, y1+51, data_dec_0);
	LCD_DrawPicture2(  x1+22,   y1+0,  x1+87, y1+51, data_dec_2);	//66
	LCD_DrawPicture (  x1+88,   y1+0, x1+109, y1+51, data_dec_1);
	LCD_DrawPicture (  x1+47,  y1+23, x1+62, y1+28, data_dec_3);
}


void LCD_data_inc_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+21, y1+51, data_inc_0);
	LCD_DrawPicture2(  x1+22,   y1+0,  x1+87, y1+51, data_inc_2);	//66
	LCD_DrawPicture (  x1+88,   y1+0, x1+109, y1+51, data_inc_1);
	LCD_DrawPicture (  x1+46,  y1+14,  x1+63, y1+31, data_inc_3);
}




void LCD_data_save_icon(uint16_t x1, uint16_t y1)
{
	LCD_air_icon_a(x1,y1);

	LCD_DrawPicture (  x1+28,  y1+14,  x1+89, y1+61, save_icon_0);	//62,48
	LCD_DrawPicture (  x1+27,  y1+73,  x1+90, y1+94, save_icon_1);	//64,22
}



void LCD_data_rt_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (  x1+0,   y1+0,   x1+3, y1+27, data_rl0);		//4*28
	LCD_DrawPicture2(  x1+4,   y1+0, x1+121, y1+27, data_rl2);		//118*28
	LCD_DrawPicture (  x1+122, y1+0, x1+125, y1+27, data_rl1);		//4*28
	LCD_DrawPicture (  x1+49,  y1+0,  x1+76, y1+27, data_rl_p0);	//28*28	
}



void LCD_data_lf_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (  x1+0,   y1+0,   x1+3, y1+27, data_rl0);		//4*28
	LCD_DrawPicture2(  x1+4,   y1+0, x1+121, y1+27, data_rl2);		//118*28
	LCD_DrawPicture (  x1+122, y1+0, x1+125, y1+27, data_rl1);		//4*28
	LCD_DrawPicture (  x1+49,  y1+0,  x1+76, y1+27, data_rl_p1);	//28*28	
}


void LCD_data_up_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (  x1+0,   y1+0,  x1+27, y1+3,   data_ud0);		//28*4
	LCD_DrawPicture3(  x1+0,   y1+4,  x1+27, y1+121, data_ud2);		//28*118
	LCD_DrawPicture (  x1+0, y1+122,  x1+27, y1+125, data_ud1);		//28*4
	LCD_DrawPicture (  x1+0,  y1+49,  x1+27, y1+76, data_ud_p1);	//28*28	
}



void LCD_data_dw_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (  x1+0,   y1+0,  x1+27, y1+3,   data_ud0);		//28*4
	LCD_DrawPicture3(  x1+0,   y1+4,  x1+27, y1+121, data_ud2);		//28*118
	LCD_DrawPicture (  x1+0, y1+122,  x1+27, y1+125, data_ud1);		//28*4
	LCD_DrawPicture (  x1+0,  y1+49,  x1+27, y1+76, data_ud_p0);	//28*28	
}


void LCD_arrow_dw_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (  x1+0,   y1+0,  x1+59, y1+35, IMG_time_dw);

}

void LCD_arrow_up_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (  x1+0,   y1+0,  x1+59, y1+35, IMG_time_up);
}






void LCD_temp_icon_a(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+19, y1+113, air_icon_0);
	LCD_DrawPicture2(  x1+20,   y1+0, x1+101, y1+113, air_icon_2);
	LCD_DrawPicture ( x1+102,   y1+0, x1+121, y1+113, air_icon_1);
}

void LCD_temp_icon_b(uint16_t x1, uint16_t y1)
{
//	LCD_DrawPicture (   x1+0,   y1+0,  x1+19, y1+113, air_icon_b0_0);
//	LCD_DrawPicture2(  x1+20,   y1+0, x1+101, y1+113, air_icon_b0_2);
//	LCD_DrawPicture ( x1+102,   y1+0, x1+121, y1+113, air_icon_b0_1);
}






void LCD_control_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+11, y1+49, alam_icon_0);
	LCD_DrawPicture2(  x1+12,   y1+0, x1+146, y1+49, alam_icon_2);
	LCD_DrawPicture ( x1+147,   y1+0, x1+158, y1+49, alam_icon_1);
}


void LCD_control_icon2(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+11, y1+49, alam_icon_0);
	LCD_DrawPicture2(  x1+12,   y1+0, x1+206, y1+49, alam_icon_2);
	LCD_DrawPicture ( x1+207,   y1+0, x1+218, y1+49, alam_icon_1);
}



void LCD_alam_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+11, y1+49, alam_icon_0);
	LCD_DrawPicture2(  x1+12,   y1+0, x1+156, y1+49, alam_icon_2);
	LCD_DrawPicture ( x1+157,   y1+0, x1+168, y1+49, alam_icon_1);
}




void LCD_cham_icon_off(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+19, y1+131, cham_off_0);
	LCD_DrawPicture2(  x1+20,   y1+0, x1+111, y1+131, cham_off_2);
	LCD_DrawPicture ( x1+112,   y1+0, x1+131, y1+131, cham_off_1);
	LCD_Fill( x1+11,  y1+62,  x1+119,  y1+63,  0xdefb);	
	LCD_Fill( x1+64,  y1+12,  x1+65,   y1+119, 0xdefb);	
}



void LCD_cham_icon_on(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+19, y1+131, cham_on_0);
	LCD_DrawPicture2(  x1+20,   y1+0, x1+111, y1+131, cham_on_2);
	LCD_DrawPicture ( x1+112,   y1+0, x1+131, y1+131, cham_on_1);
	LCD_Fill( x1+11,  y1+62,  x1+119,  y1+63,  0xa62d);	
	LCD_Fill( x1+64,  y1+12,  x1+65,   y1+119,  0xa62d);
}




void LCD_cham_icon_t(uint16_t x1, uint16_t y1)
{
//	LCD_DrawPicture (   x1+0,   y1+0,  x1+19, y1+131, cham_t0_0);
//	LCD_DrawPicture2(  x1+20,   y1+0, x1+111, y1+131, cham_t0_2);
//	LCD_DrawPicture ( x1+112,   y1+0, x1+131, y1+131, cham_t0_1);
}


void LCD_set_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+11, y1+83, set_icon_0);	//12
	LCD_DrawPicture2(  x1+12,   y1+0,  x1+77, y1+83, set_icon_2);	//66
	LCD_DrawPicture (  x1+78,   y1+0,  x1+89, y1+83, set_icon_1);
}


void LCD_set_icon2(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+11, y1+83, set_icon_0);
	LCD_DrawPicture2(  x1+12,   y1+0,  x1+139, y1+83, set_icon_2);
	LCD_DrawPicture (  x1+140,  y1+0,  x1+151, y1+83, set_icon_1);
}









void LCD_oil_icon(uint16_t x1, uint16_t y1,uint8_t mode_sta)
{
	if (mode_sta ==0)	
	{
		LCD_air_icon_a(x1,y1);
		LCD_DrawPicture (   x1+45,   y1+23,  x1+45+27, y1+23+19, IMG_oil_bl);
		LCD_DrawPicture (   x1+22,   y1+64,  x1+22+73, y1+64+17, IMG_culture_bl);
	}
	else
	{
		LCD_air_icon_b(x1,y1);
		LCD_DrawPicture (   x1+45,   y1+23,  x1+45+27, y1+23+19, IMG_oil_yl);
		LCD_DrawPicture (   x1+22,   y1+64,  x1+22+73, y1+64+17, IMG_culture_yl);
	}
}


void LCD_wet_icon(uint16_t x1, uint16_t y1,uint8_t mode_sta)
{
	if (mode_sta ==0)	
	{
		LCD_air_icon_a(x1,y1);
		LCD_DrawPicture (   x1+37,   y1+23,  x1+37+41, y1+23+17, IMG_wet_bl);
		LCD_DrawPicture (   x1+22,   y1+64,  x1+22+73, y1+64+17, IMG_culture_bl);
	}
	else
	{
		LCD_air_icon_b(x1,y1);
		LCD_DrawPicture (   x1+37,   y1+23,  x1+37+41, y1+23+17, IMG_wet_yl);
		LCD_DrawPicture (   x1+22,   y1+64,  x1+22+73, y1+64+17, IMG_culture_yl);
	}
}



void LCD_mode_icon(uint8_t mode_sta)
{
  LCD_oil_icon(150,350,!mode_sta);
  LCD_wet_icon(300,350,mode_sta);
}


void LCD_mode_icon2(uint8_t mode_sta)
{
  LCD_oil_icon(210,350,!mode_sta);
  LCD_wet_icon(335,350,mode_sta);
}






void LCD_cnc_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+79, y1+59, cnc_icon);

}





void LCD_check_on_off(uint16_t x1, uint16_t y1,uint8_t check_sta)
{
	if (check_sta == 0x00)
	{
		LCD_DrawPicture (   x1+0,   y1+0,  x1+23, y1+23, IMG_check_off);
	}
	else
	{
		LCD_DrawPicture (   x1+0,   y1+0,  x1+23, y1+23, IMG_check_on);
	}
}






void LCD_lock_on_icon(uint16_t x1, uint16_t y1)
{
	LCD_DrawPicture (   x1+0,   y1+0,  x1+17, y1+111, lock_on_0);
	LCD_DrawPicture2(  x1+18,   y1+0,  x1+99, y1+111, lock_on_2);
	LCD_DrawPicture ( x1+100,   y1+0, x1+117, y1+111, lock_on_1);
}


void LCD_lock_icon(uint16_t x1, uint16_t y1, uint8_t lock)
{
  if(lock == 0)
  {
    LCD_air_icon_a(x1,y1);
    LCD_DrawPicture (   x1+34,   y1+25,  x1+34+49, y1+25+71, lock_of_icon);
  }
  else
  {
    LCD_lock_on_icon(x1,y1);
    LCD_DrawPicture (   x1+34,   y1+25,  x1+34+49, y1+25+71, lock_on_icon);
  
  }
}











/**********************************************************************************/
void temp_display(uint16_t x1, uint16_t y1, uint16_t pencolor, const uint16_t *backcolor, uint16_t data)
{
	if (data != 0x00)
	{
		LCD_ColorNumber2( x1,   y1,  pencolor,   backcolor,  data, 3, ZERO_DISABLE);
		LCD_ShowString2( x1+16*3,   y1,  pencolor,   backcolor,  "#");
		LCD_Fill( x1+30,  y1+18,  x1+31,   y1+19,  pencolor);
	}
	else 
	{
		LCD_ShowString2( x1,   y1,  pencolor,   backcolor,  "---#");
	}
}



void percent_display(uint16_t x1, uint16_t y1, uint16_t pencolor, const uint16_t *backcolor, uint16_t data)
{
	if (data != 0x00)
	{
		LCD_ColorNumber2( x1,   y1,  pencolor,   backcolor,  data, 2, ZERO_ENABLE);
//		LCD_ShowString2( x1+16*3,   y1,  pencolor,   backcolor,  "%");
		LCD_Fill( x1+14,  y1+18,  x1+15,   y1+19,  pencolor);
	}
	else 
	{
//		LCD_ShowString2( x1,   y1,  pencolor,   backcolor,  "--% ");
	}
}




/**********************************************************************************/


void LCD_air_icon(uint16_t x1, uint16_t y1)
{
	LCD_air_icon_a(x1,y1);

	LCD_DrawPicture (   x1+12,   y1+24,  x1+12+37, y1+24+25, air_co2_0);	//38*26
	LCD_DrawPicture (   x1+12,   y1+63,  x1+12+37, y1+63+25, air_o2_0);		//38*26
	LCD_DrawPicture (   x1+90,   y1+27,  x1+90+17, y1+27+17, air_per_0);	//18*18
	LCD_DrawPicture (   x1+90,   y1+66,  x1+90+17, y1+66+17, air_per_0);
}

void LCD_air_data(uint16_t x1, uint16_t y1, uint8_t co2_data, uint16_t o2_data)
{
	font_size_select(0x01);

	LCD_ColorNumber2( x1+56,  y1+27,  BLACK,   air_icon_2+24, co2_data, 2, ZERO_ENABLE);	
	LCD_ColorNumber2( x1+56,  y1+66,  BLACK,   air_icon_2+24, o2_data, 2, ZERO_ENABLE);	

	LCD_Fill( x1+56+15,  y1+27+16,  x1+56+15+1,   y1+27+16+1,  BLACK);
	LCD_Fill( x1+56+15,  y1+66+16,  x1+56+15+1,   y1+66+16+1,  BLACK);

}


void LCD_air_icon2(uint16_t x1, uint16_t y1)
{
	LCD_air_icon_2gas(x1,y1);

	LCD_DrawPicture (   x1+78,    y1+12,  x1+78+37, y1+12+25, air_co2_0);
	LCD_DrawPicture (   x1+131,  y1+12,  x1+131+37, y1+12+25, air_o2_0);

	LCD_DrawPicture (   x1+112,   y1+43,  x1+112+17, y1+43+17, air_per_0);
	LCD_DrawPicture (   x1+112,   y1+74,  x1+112+17, y1+74+17, air_per_0);

	LCD_DrawPicture (   x1+170,   y1+43,  x1+170+17, y1+43+17, air_per_0);
	LCD_DrawPicture (   x1+170,   y1+74,  x1+170+17, y1+74+17, air_per_0);

	LCD_DrawPicture (   x1+6,   y1+43,  x1+6+57, y1+43+17, IMG_GAS1);
	LCD_DrawPicture (   x1+6,   y1+74,  x1+6+57, y1+74+17, IMG_GAS2);
}


void LCD_air_data2(uint16_t x1, uint16_t y1, uint8_t co2_data, uint16_t o2_data, uint8_t co2_data2, uint16_t o2_data2)
{
	font_size_select(0x01);

	LCD_ColorNumber2( x1+78,  y1+43,  BLACK,   air_icon_2+24, co2_data, 2, ZERO_ENABLE);	
	LCD_ColorNumber2( x1+78,  y1+74,  BLACK,   air_icon_2+24, co2_data2, 2, ZERO_ENABLE);	
	LCD_Fill( x1+78+15,  y1+43+16,  x1+78+15+1,   y1+43+16+1,  BLACK);
	LCD_Fill( x1+78+15,  y1+74+16,  x1+78+15+1,   y1+74+16+1,  BLACK);

	LCD_ColorNumber2( x1+136,  y1+43,  BLACK,   air_icon_2+24, o2_data, 2, ZERO_ENABLE);	
	LCD_ColorNumber2( x1+136,  y1+74,  BLACK,   air_icon_2+24, o2_data2, 2, ZERO_ENABLE);	
	LCD_Fill( x1+136+15,  y1+43+16,  x1+136+15+1,   y1+43+16+1,  BLACK);
	LCD_Fill( x1+136+15,  y1+74+16,  x1+136+15+1,   y1+74+16+1,  BLACK);
}












void LCD_cham_icon(uint16_t x1, uint16_t y1, uint8_t ch_no)
{
	if(cham_working_sta[ch_no] == 0x00)	
	{
		LCD_cham_icon_off (x1, y1);
	}
	else	
	{
		LCD_cham_icon_on (x1, y1);

	}
}






void LCD_small_icon(uint16_t x1, uint16_t y1, uint8_t ch_no, uint8_t set_sta)
{
	if(set_sta == 0)	
	{
		LCD_small_off_icon (x1, y1);
		LCD_ShowChar2( x1+25,  y1+23,   BLACK,  small_off_2+23,  ch_no);
	}
	else	
	{
		LCD_small_on_icon(x1, y1);
		LCD_ShowChar2( x1+25,  y1+23,   BLACK,  small_on_2+23,  ch_no);
	}
}


void LCD_small_icon2(uint16_t x1, uint16_t y1, uint8_t charter)
{
	LCD_small_off_icon (x1, y1);
	LCD_ShowChar2( x1+25,  y1+23,   BLUE,  small_off_2+23,  charter);
}



void LCD_small_icon3(uint16_t x1, uint16_t y1, uint8_t charter)
{
	LCD_small_off_icon3 (x1, y1);
	LCD_ShowChar2( x1+12,  y1+23,   BLUE,  small_off_2+23,  charter);
}




void LCD_small_off_icon_string (uint16_t x1, uint16_t y1, uint16_t pencolor, uint8_t * string)
{
	font_size_select(0x00);
	LCD_small_off_icon2 (x1, y1);
	LCD_ShowString2( x1+10,  y1+23,   BLUE,  small_off_2+23,  string);
}






void  set_sel_icon_m0(uint16_t x1, uint16_t y1, uint8_t status)
{
	if(status == 0)
	{
		LCD_set_tab_off_icon(x1,y1);
		LCD_DrawPicture (   x1+38,   y1+16,  x1+38+89, y1+16+19, set_tab_off_m0);	//90*20
	}
	else
	{
		LCD_set_tab_on_icon(x1,y1);
		LCD_DrawPicture (   x1+38,   y1+16,  x1+38+89, y1+16+19, set_tab_on_m0);	//90*20
	}
}

void  set_sel_icon_m1(uint16_t x1, uint16_t y1, uint8_t status)
{
	if(status == 0)
	{
		LCD_set_tab_off_icon(x1,y1);
		LCD_DrawPicture (   x1+38,   y1+16,  x1+38+89, y1+16+19, set_tab_off_m1);	//90*20
	}
	else
	{
		LCD_set_tab_on_icon(x1,y1);
		LCD_DrawPicture (   x1+38,   y1+16,  x1+38+89, y1+16+19, set_tab_on_m1);	//90*20
	}
}

void  set_sel_icon_m2(uint16_t x1, uint16_t y1, uint8_t status)
{
	if(status == 0)
	{
		LCD_set_tab_off_icon(x1,y1);
		LCD_DrawPicture (   x1+38,   y1+16,  x1+38+89, y1+16+19, set_tab_off_m2);	//90*20
	}
	else
	{
		LCD_set_tab_on_icon(x1,y1);
		LCD_DrawPicture (   x1+38,   y1+16,  x1+38+89, y1+16+19, set_tab_on_m2);	//90*20
	}
}



void  set_sel_icon_m3(uint16_t x1, uint16_t y1, uint8_t status)
{
	if(status == 0)
	{
		LCD_set_tab_off_icon(x1,y1);
		LCD_DrawPicture (   x1+23,   y1+16,  x1+23+119, y1+16+19, set_tab_off_m3);	//90*20
	}
	else
	{
		LCD_set_tab_on_icon(x1,y1);
		LCD_DrawPicture (   x1+23,   y1+16,  x1+23+119, y1+16+19, set_tab_on_m3);	//90*20
	}
}


void  set_sel_icon_m4(uint16_t x1, uint16_t y1, uint8_t status)
{
	if(status == 0)
	{
		LCD_set_tab_off_icon(x1,y1);
		LCD_DrawPicture (   x1+23,   y1+16,  x1+23+119, y1+16+19, set_tab_off_m4);	//90*20
	}
	else
	{
		LCD_set_tab_on_icon(x1,y1);
		LCD_DrawPicture (   x1+23,   y1+16,  x1+23+119, y1+16+19, set_tab_on_m4);	//90*20
	}
}


void  set_sel_icon_m5(uint16_t x1, uint16_t y1, uint8_t status)
{
	if(status == 0)
	{
		LCD_set_tab_off_icon(x1,y1);
		LCD_DrawPicture (   x1+23,   y1+16,  x1+23+119, y1+16+19, set_tab_off_m5);	//90*20
	}
	else
	{
		LCD_set_tab_on_icon(x1,y1);
		LCD_DrawPicture (   x1+23,   y1+16,  x1+23+119, y1+16+19, set_tab_on_m5);	//90*20
	}
}




void  set_alltemp_set_icon(uint16_t x1, uint16_t y1)
{
		LCD_set_tab_on_icon(x1,y1);
		LCD_DrawPicture (  x1+23,   y1+16,  x1+23+119, y1+16+19, set_tab_on_m6);	//120*20
}



void  set_calibration_on_icon(uint16_t x1, uint16_t y1)
{
		LCD_set_tab_on_icon(x1,y1);
		LCD_ShowString2( x1+23,  y1+16,   WHITE,  set_tab_on_2+16,  "Calibration");
}


void  set_calibration_off_icon(uint16_t x1, uint16_t y1)
{
		LCD_set_tab_off_icon(x1,y1);
		LCD_ShowString2( x1+23,  y1+16,   BLUE,  set_tab_off_2+16,  "Calibration");
}


void  set_initial_on_icon(uint16_t x1, uint16_t y1)
{
		LCD_set_tab_on_icon(x1,y1);
		LCD_ShowString2( x1+23,  y1+16,   WHITE,  set_tab_on_2+16,  "Initialize");
}

void  set_initial_off_icon(uint16_t x1, uint16_t y1)
{
		LCD_set_tab_off_icon(x1,y1);
		LCD_ShowString2( x1+23,  y1+16,   BLUE,  set_tab_off_2+16,  "Initialize");
}







void  LCD_select_icon(uint8_t mode_sel, uint8_t sub_mode)
{
uint8_t sel_mode_a=0, sel_mode_b=0, sel_mode_c=0;

	switch(sub_mode)
	{
		case 0:	 sel_mode_a=1;
				break;
		case 1:	 sel_mode_b=1;
				break;
		case 2:	 sel_mode_c=1;
				break;
	}
	
	switch(mode_sel)
	{
		case co2_sel :
			break;		
		case o2_sel :
			break;		
		case temp_sel :
			set_sel_icon_m3(  5,15, sel_mode_a);
			set_sel_icon_m4(190,15, sel_mode_b);
			set_sel_icon_m5(375,15, sel_mode_c);
			break;		
	}		
}	
	




void lcd_ch_off_no(uint16_t x1, uint16_t y1, uint8_t ch_no)
{
	switch(ch_no)
	{
		case 1: 	LCD_DrawPicture ( x1,   y1,  x1+23, y1+23, off_no1);
				break;
		case 2: 	LCD_DrawPicture ( x1,   y1,  x1+23, y1+23, off_no2);
				break;
		case 3: 	LCD_DrawPicture ( x1,   y1,  x1+23, y1+23, off_no3);
				break;
		case 4: 	LCD_DrawPicture ( x1,   y1,  x1+23, y1+23, off_no4);
				break;
		case 5: 	LCD_DrawPicture ( x1,   y1,  x1+23, y1+23, off_no5);
				break;
		case 6: 	LCD_DrawPicture ( x1,   y1,  x1+23, y1+23, off_no6);
				break;
		case 7: 	LCD_DrawPicture ( x1,   y1,  x1+23, y1+23, off_no7);
				break;
		case 8: 	LCD_DrawPicture ( x1,   y1,  x1+23, y1+23, off_no8);
				break;
	}

}


void lcd_ch_on_no(uint16_t x1, uint16_t y1, uint8_t ch_no)
{
	switch(ch_no)
	{
		case 1: 	LCD_DrawPicture ( x1,   y1,  x1+23, y1+23, on_no1);
				break;
		case 2: 	LCD_DrawPicture ( x1,   y1,  x1+23, y1+23, on_no2);
				break;
		case 3: 	LCD_DrawPicture ( x1,   y1,  x1+23, y1+23, on_no3);
				break;
		case 4: 	LCD_DrawPicture ( x1,   y1,  x1+23, y1+23, on_no4);
				break;
		case 5: 	LCD_DrawPicture ( x1,   y1,  x1+23, y1+23, on_no5);
				break;
		case 6: 	LCD_DrawPicture ( x1,   y1,  x1+23, y1+23, on_no6);
				break;
		case 7: 	LCD_DrawPicture ( x1,   y1,  x1+23, y1+23, on_no7);
				break;
		case 8: 	LCD_DrawPicture ( x1,   y1,  x1+23, y1+23, on_no8);
				break;
	}

}

//*****************************************************************//
//
//*****************************************************************//
void LCD_cham_data(uint16_t x1, uint16_t y1, uint8_t ch_no)
{
static uint8_t blink_cnt = 5, blink_sta=0;
	
////////////////////////////////////////////////////
	if (ch_no== 0)			//ch_no°¡ 0 ÀÏ¶§¸¸ ½ÇÇà
	{
		if (cham_temp_set_mode == 0)		//¼¼ÆÃ¸ðµå°¡ ¾Æ´Ï¸é Ç×»ó ÃÊ±âÈ­
		{
			blink_cnt = 5;
			blink_sta = 0;
		}
		else if (cham_temp_set_mode == 2)	//¼¼ÆÃ¸ðµå Å°¸¦ ¿¬¼Ó ´©¸§ 
		{
			blink_cnt = 5;
			blink_sta = 1;
		}
		else if( ! blink_cnt--)			//¼¼ÆÃ¸ðµå
		{
			blink_cnt = 5;
			blink_sta ^= 1;
		}
	}
////////////////////////////////////////////////////
//		½Ã°£µð½ºÇÃ·¹ÀÌ
////////////////////////////////////////////////////
	if(cham_working_sta[ch_no] == 0x00)			//Å¸ÀÓÇ¥½Ã
	{
//		LCD_ShowString2 ( x1+15,   y1+80,  BLACK,   cham_off_2+80,  "Emp");		
		LCD_DrawPicture ( x1+22,   y1+89,  x1+22+37, y1+89+17, set_mode_ch5);	//Emp

		temp_display( x1+66, y1+80,  BLACK, cham_off_2+80, cham_now_temp[ch_no]);
	}
	else	
	{
		LCD_ColorNumber2 ( x1+2,   y1+80,  BLACK,   cham_on_2+80,  cham_working_min[ch_no]/60 , 3, ZERO_DISABLE);	//´ÜÀ§°¡ ºÐÀ¸¹Ç·Î ½Ã°£À¸·Î º¯°æ
		LCD_ShowChar2 ( x1+2+46,   y1+80,  BLACK,   cham_on_2+80,  'H');
		temp_display( x1+66, y1+80,  BLACK, cham_on_2+80, cham_now_temp[ch_no]);
	}
///////////////////////////////////////////////////////////////
	if(cham_setting_sta[ch_no] == 0x00)				//ÀÏ¹Ý¸ðµå
	{
		if(cham_working_sta[ch_no] == 0x00)	
		{
		//	LCD_ShowChar2(x1+30, y1+20, BLUE, cham_off_2+20, ch_no+'1');
		//	LCD_ShowString2 ( x1+70,   y1+10,  BLUE,   cham_off_2+10,  "Set");

			lcd_ch_off_no(x1+20, y1+20, ch_no+1);
			LCD_DrawPicture ( x1+66,   y1+12,  x1+66+47, y1+12+19, set_mode_ch1);	//set	
			temp_display( x1+66, y1+38,  BLUE, cham_off_2+38, cham_set_temp[ch_no]);
		}
		else	
		{
//			LCD_ShowChar2(x1+30, y1+20, WHITE, cham_on_2+20, ch_no+'1');
//			LCD_ShowString2 ( x1+70,   y1+10,  WHITE,   cham_on_2+10,  "Set");

			lcd_ch_on_no(x1+20, y1+20, ch_no+1);
			LCD_DrawPicture ( x1+66,   y1+12,  x1+66+47, y1+12+19, set_mode_ch4);	//set
			temp_display( x1+66, y1+38,  WHITE, cham_on_2+38, cham_set_temp[ch_no]);
		}
	}
	else 									//¼¼ÆÃ¸ðµå
	{
		if(cham_working_sta[ch_no] == 0x00)	
		{
			if(blink_sta)
			{
		//		LCD_ShowChar2(x1+30, y1+20, RED, cham_off_2+20, ch_no+'1');
		//		LCD_ShowString2 ( x1+70,   y1+10,  RED,   cham_off_2+10,  "Set");

				lcd_ch_off_no(x1+20, y1+20, ch_no+1);
				LCD_DrawPicture ( x1+66,   y1+12,  x1+66+47, y1+12+19, set_mode_ch1);	//set
				temp_display( x1+66, y1+38,  RED, cham_off_2+38, cham_set_temp_dis[ch_no]);
			}
			else
			{	
				lcd_ch_off_no(x1+20, y1+20, ch_no+1);
				LCD_DrawPicture ( x1+66,   y1+12,  x1+66+47, y1+12+19, set_mode_ch1);	//set
		//		LCD_ShowChar2(x1+20, y1+20, RED, cham_off_2+20, ' ');
				LCD_ShowString2 ( x1+66,   y1+38,  RED,   cham_off_2+40,  "    ");
			}
		}
		else	
		{
			if(blink_sta)
			{
//				LCD_ShowChar2(x1+30, y1+20, RED, cham_on_2+20, ch_no+'1');
//				LCD_ShowString2 ( x1+70,   y1+10,  RED,   cham_on_2+10,  "Set");	

				lcd_ch_on_no(x1+20, y1+20, ch_no+1);
				LCD_DrawPicture ( x1+66,   y1+12,  x1+66+47, y1+12+19, set_mode_ch4);	//set


				temp_display( x1+66, y1+38,  RED, cham_on_2+38, cham_set_temp_dis[ch_no]);
			}
			else
			{

//				LCD_ShowChar2(x1+30, y1+20, RED, cham_on_2+20, ' ');

				lcd_ch_on_no(x1+20, y1+20, ch_no+1);
				LCD_DrawPicture ( x1+66,   y1+12,  x1+66+47, y1+12+19, set_mode_ch4);	//set
				LCD_ShowString2 ( x1+66,   y1+38,  RED,   cham_on_2+40,  "    ");
			}
		}
	}
}
///////////////////////////////////////////////////////////////








void LCD_total_icon(uint16_t x1, uint16_t y1)
{
	LCD_cham_icon_t(x1,y1);
//	LCD_ShowString2( x1+6,  y1+38,   YELLOW,  cham_t0_2+38,  "ALL");
//	LCD_ShowString2( x1+62,  y1+38,   YELLOW,  cham_t0_2+38,  "TEMP");
//	LCD_ShowString2( x1+6,  y1+80,   YELLOW,  cham_t0_2+80,  "Set");
}

void LCD_cham_total(uint16_t x1, uint16_t y1)
{
//	LCD_ColorNumber2( x1+66,   y1+80,  YELLOW,   cham_t0_2+80,  cham_all_set_temp, 3, ZERO_DISABLE);
	LCD_Fill( x1+96,  y1+98,  x1+97,   y1+99,  YELLOW);
}


void LCD_temp_set_icon(uint16_t x1, uint16_t y1)
{
//	LCD_temp_icon_a(x1,y1);
//	LCD_ShowString2( x1+30,  y1+50,   BLUE,  air_icon_a0_2+38,  "TEMP");

}





void LCD_alam_set_icon(uint16_t x1, uint16_t y1)
{
	LCD_alam_icon(x1,y1);
	LCD_ShowString2( x1+5,  y1+15,   BLUE,  alam_icon_2+15,  "Alarm List");

}



void LCD_set_mode_icon(uint16_t x1, uint16_t y1, uint8_t mode_sel)
{
	LCD_set_icon(x1,y1);

	switch(mode_sel)
	{
		case 0 :LCD_DrawPicture ( x1+24,   y1+33,  x1+24+47, y1+33+19, set_mode_ch0);	//48,20
		break;
		case 1 :LCD_DrawPicture ( x1+24,   y1+33,  x1+24+47, y1+33+19, set_mode_ch1);	//48,20
		break;
		case 2 :LCD_DrawPicture ( x1+24,   y1+33,  x1+24+47, y1+33+19, set_mode_ch2);	//48,20
		break;
		case 3 :LCD_DrawPicture ( x1+20,   y1+25,  x1+20+51, y1+25+40, set_mode_ch3);
		break;
	}
}









				

void LCD_Alam_Message(void)
{
static uint8_t err_blink = 0;
static uint8_t err_blink_cnt = 0;

	if(err_blink_cnt++ == 10)
	{
		err_blink_cnt = 0;
		err_blink ^= 1;

		font_size_select(0x00);

		if(err_blink)
		{
			switch(Error_no)
			{
				case 1:	LCD_ShowString_number(590,  310,   RED,  Back_color,	"    CHAMBER%      ", Sub_error+1,1);	//EEROR 1
						LCD_ShowString       (590,  340,   RED,  Back_color,	"    DOOR OPEN!!   ");
						break;

				case 2: LCD_ShowString_number(590,  310,   RED,  Back_color,  	"CHAMER% TOP HEATER", Sub_error+1,1);	//EEROR 2
						LCD_ShowString       (590,  340,   RED,  Back_color,  	"HIGH TEMP ERROR   ");
						break;

				case 3: LCD_ShowString_number(590,  310,   RED,  Back_color,  	"CHAMER% BOT HEATER", Sub_error+1,1);	//EEROR 3
						LCD_ShowString       (590,  340,   RED,  Back_color,  	"HIGH TEMP ERROR   ");
						break;

				case 4: LCD_ShowString_number(590,  310,   RED,  Back_color,  	"CHAMER% TOP HEATER", Sub_error+1,1);	//EEROR 4
						LCD_ShowString       (590,  340,   RED,  Back_color,  	"LOW TEMP ERROR    ");
						break;

				case 5: LCD_ShowString_number(590,  310,   RED,  Back_color,  	"CHAMER% BOT HEATER", Sub_error+1,1);	//EEROR 5
						LCD_ShowString       (590,  340,   RED,  Back_color,  	"LOW TEMP ERROR    ");
						break;

				case 6: LCD_ShowString_number(590,  310,   RED,  Back_color,  	"CHAMER% TOP TEMP  ", Sub_error+1,1);	//EEROR 6
						LCD_ShowString       (590,  340,   RED,  Back_color,  	"SENSOR ERROR      ");
						break;

				case 7: LCD_ShowString_number(590,  310,   RED,  Back_color,  	"CHAMER% BOT TEMP  ", Sub_error+1,1);	//EEROR 7
						LCD_ShowString       (590,  340,   RED,  Back_color,  	"SENSOR ERROR      ");
						break;

				case 8: LCD_ShowString_number(590,  310,   RED,  Back_color,  	"   GAS% SUPPLY    ", Sub_error+1,1);	//EEROR 8
						LCD_ShowString       (590,  340,   RED,  Back_color,  	"    PRESSS ERROR  ");
						break;

				case 9: LCD_ShowString_number(590,  310,   RED,  Back_color,  	"  GAS% SOL VALVE  ", Sub_error+1,1);	//EEROR 9
						LCD_ShowString       (590,  340,   RED,  Back_color,  	"   ERROR          ");
						break;

				case 10:LCD_ShowString_number(590,  310,   RED,  Back_color,  	"VOC FILTER TIME   ", 0,1);	//EEROR 10
						LCD_ShowString_number(590,  340,   RED,  Back_color,  	"REMAIN  %% DAY    ", Sub_error,2);
						break;

				case 11:LCD_ShowString       (590,  310,   RED,  Back_color,  	" POWER OFF STRANGE");	//EEROR 11
						LCD_ShowString       (590,  340,   RED,  Back_color,  	" ERROR            ");
						break;
			}
		}
	  else 
		{
		  LCD_ShowString(590,  310,   RED,  Back_color,  		"                  ");				//EEROR CLEAR
		  LCD_ShowString(590,  340,   RED,  Back_color,  		"                  ");				//EEROR CLEAR
		}

	}
	font_size_select(0x00);
}





void LCD_first_Alam_Message(void)
{
	font_size_select(0x01);

	LCD_ShowString(585,  280,   BLACK,  Back_color, "Alarm Message");
	LCD_ShowString(640,  370,   BLACK,  Back_color, "ENTER");

	LCD_Box( 640-5,  370-5,  720+5,  370+20+5,  GREEN);
}


void LCD_error_display(void)
{
static uint8_t lcd_sta = 0;

	if(Error_EN == 0x01)
	{
		if (lcd_sta != 0x01)
		{
			main_log_flag= 5;  sub_log_flag1=Sub_error;	sub_log_flag2=Error_no; 
		}	
		LCD_first_Alam_Message();
		LCD_Alam_Message();	
		lcd_sta = 0x01;
	}
	else
	{
		if (lcd_sta != 0x00)
		{
			LCD_Fill( 580,  280,  799,   395,  Back_color);
		}
		lcd_sta = 0x00;
	}
}

















void LCD_popup_icon(void)
{
	LCD_Fill( 150,  330,  570,  470,  Back_color);
	LCD_Box( 150,  330,  550,  470,  WHITE);

	LCD_ShowString(155,  350,   RED,  Back_color,  	" Setting is not Complete");
	LCD_ShowString(155,  370,   RED,  Back_color,  	" Save And Out?");	

	LCD_small_off_icon_string( 200, 400, BLACK, "YES");	//CLEAR BUTTON
	LCD_small_off_icon_string( 350, 400, BLACK, "NO");	//CLEAR BUTTON
}


void LCD_save_mode_icon(void)
{
static uint8_t lcd_sta = 2;

  if(wet_change == 0)
  {
     lcd_sta = 2;
     wet_change = 1;
  }

	if (lcd_popup_flag == 0x01)
	{
		if (lcd_sta != 0x02){
			LCD_popup_icon();
			lcd_sta = 2;
		}
	}
  else if(cham_temp_set_mode == 0)
	{
		if (lcd_sta != 0x00)
		{
			if (gas_2nd_on_off==0)
			{
				LCD_Fill( 150,  330,  570,   470,  Back_color);	
				LCD_air_icon( 10,350);
				LCD_air_data(10,350,co2_set_data,o2_set_data);	

        LCD_oil_icon(156,350,!wet_mode);
        LCD_wet_icon(300,350,wet_mode);
        LCD_lock_icon(450, 350, lock_data);
			}
			else
			{
				LCD_Fill( 150,  330,  570,  470,  Back_color);
				LCD_air_icon2( 10,350);
				LCD_air_data2(10,350,co2_set_data,o2_set_data,co2_2nd_set_data,o2_2nd_set_data);

        LCD_oil_icon(210,350,!wet_mode);
        LCD_wet_icon(330,350,wet_mode);
        LCD_lock_icon(450, 350, lock_data);
			}

			lcd_sta = 0x00;
		}
	}
	else
	{	
		if(lcd_sta != 0x01)
    {
      if ( gas_2nd_on_off==0 )	{
          LCD_Fill( 150,  330,  570,  470,  Back_color);
          LCD_all_temp_icon( 150,350);
          LCD_data_inc_icon( 300,350);
          LCD_data_dec_icon( 300,405);
          LCD_data_save_icon(450,350);
      }
      else	{			
        LCD_Fill( 210,  330,  570,  470,  Back_color);
        LCD_all_temp_icon( 210,350);
        LCD_data_inc_icon( 335,350);
        LCD_data_dec_icon( 335,405);
        LCD_data_save_icon(450,350);
      }

      lcd_sta = 0x01;
		}
	}  
}







void font_size_select(uint8_t size)
{
	font_size = size;

	if(size==0x00)		{font_size_x=11;	font_size_y=16;}
	else if(size==0x01)	{font_size_x=16;	font_size_y=24;}

}











void LCD_cham(void)
{
uint16_t temp_x,temp_y;
static uint8_t cham_dis_sta[8] ={1,1,1,1,1,1,1,1};
////////////////////////////////////////////////
	temp_x = (!chamber_count) ? 10 : 30;
	temp_y = (!chamber_count) ? 50 : 50;

	if(cham_dis_sta[0] != cham_working_sta[0])	
	{
		LCD_cham_icon(temp_x,temp_y,0);
		cham_dis_sta[0] = cham_working_sta[0];
	}
	LCD_cham_data(temp_x,temp_y,0);
////////////////////////////////////////////////
	temp_x = (!chamber_count) ? 150 : 210;
	temp_y = (!chamber_count) ? 50 : 50;

	if(cham_dis_sta[1] != cham_working_sta[1])	
	{
		LCD_cham_icon(temp_x,temp_y,1);
		cham_dis_sta[1] = cham_working_sta[1];
	}
	LCD_cham_data(temp_x,temp_y,1);
////////////////////////////////////////////////
	temp_x = (!chamber_count) ? 290 : 390;
	temp_y = (!chamber_count) ? 50 : 50;

	if(cham_dis_sta[2] != cham_working_sta[2])	
	{
		LCD_cham_icon(temp_x,temp_y,2);
		cham_dis_sta[2] = cham_working_sta[2];
	}
	LCD_cham_data(temp_x,temp_y,2);
////////////////////////////////////////////////
	temp_x = (!chamber_count) ? 430 : 30;
	temp_y = (!chamber_count) ? 50 : 190;
	
	if(cham_dis_sta[3] != cham_working_sta[3])	
	{
		LCD_cham_icon(temp_x,temp_y,3);
		cham_dis_sta[3] = cham_working_sta[3];
	}
	LCD_cham_data(temp_x,temp_y,3);
////////////////////////////////////////////////
	temp_x = (!chamber_count) ? 10 : 210;
	temp_y = (!chamber_count) ? 190 : 190;

	if(cham_dis_sta[4] != cham_working_sta[4])	
	{
		LCD_cham_icon(temp_x,temp_y,4);
		cham_dis_sta[4] = cham_working_sta[4];
	}
	LCD_cham_data(temp_x,temp_y,4);
////////////////////////////////////////////////
	temp_x = (!chamber_count) ? 150 : 390;
	temp_y = (!chamber_count) ? 190 : 190;

	if(cham_dis_sta[5] != cham_working_sta[5])	
	{
		LCD_cham_icon(temp_x,temp_y,5);
		cham_dis_sta[5] = cham_working_sta[5];
	}
	LCD_cham_data(temp_x,temp_y,5);
////////////////////////////////////////////////

	if (chamber_count ==1)		return;
	

////////////////////////////////////////////////
	temp_x = 290;
	temp_y = 190;

	if(cham_dis_sta[6] != cham_working_sta[6])	
	{
		LCD_cham_icon(temp_x,temp_y,6);
		cham_dis_sta[6]=cham_working_sta[6];
	}
	LCD_cham_data(temp_x,temp_y,6);
////////////////////////////////////////////////
	temp_x = 430;
	temp_y = 190;

	if(cham_dis_sta[7] != cham_working_sta[7])	
	{
		LCD_cham_icon(temp_x,temp_y,7);
		cham_dis_sta[7]=cham_working_sta[7];
	}
	LCD_cham_data(temp_x,temp_y,7);
////////////////////////////////////////////////
}


void LCD_test(void)
{
	font_size_select(0x00);
/*	LCD_ShowString_number(  50, 20,   BLACK,  Back_color, "ch1_top %%.%", cham_top_temp[0], 3);
	LCD_ShowString_number( 200, 20,   BLACK,  Back_color, "ch1_bot %%.%", cham_bot_temp[0], 3);
	LCD_ShowString_number(  50, 50,   BLACK,  Back_color, "ch2_top %%.%", cham_top_temp[1], 3);
	LCD_ShowString_number( 200, 50,   BLACK,  Back_color, "ch2_bot %%.%", cham_bot_temp[1], 3);
	LCD_ShowString_number(  50, 80,   BLACK,  Back_color, "ch3_top %%.%", cham_top_temp[2], 3);
	LCD_ShowString_number( 200, 80,   BLACK,  Back_color, "ch3_bot %%.%", cham_bot_temp[2], 3);
	LCD_ShowString_number(  50,110,   BLACK,  Back_color, "ch4_top %%.%", cham_top_temp[3], 3);
	LCD_ShowString_number( 200,110,   BLACK,  Back_color, "ch4_bot %%.%", cham_bot_temp[3], 3);
	LCD_ShowString_number(  50,140,   BLACK,  Back_color, "ch5_top %%.%", cham_top_temp[4], 3);
	LCD_ShowString_number( 200,140,   BLACK,  Back_color, "ch5_bot %%.%", cham_bot_temp[4], 3);
	LCD_ShowString_number(  50,170,   BLACK,  Back_color, "ch6_top %%.%", cham_top_temp[5], 3);
	LCD_ShowString_number( 200,170,   BLACK,  Back_color, "ch6_bot %%.%", cham_bot_temp[5], 3);
	LCD_ShowString_number(  50,200,   BLACK,  Back_color, "ch7_top %%.%", cham_top_temp[6], 3);
	LCD_ShowString_number( 200,200,   BLACK,  Back_color, "ch7_bot %%.%", cham_bot_temp[6], 3);
	LCD_ShowString_number(  50,230,   BLACK,  Back_color, "ch8_top %%.%", cham_top_temp[7], 3);
	LCD_ShowString_number( 200,230,   BLACK,  Back_color, "ch8_bot %%.%", cham_bot_temp[7], 3);

	LCD_ShowString_number( 350, 20,   BLACK,  Back_color, "top %%%%", heat_top_pwm[0], 4);
	LCD_ShowString_number( 450, 20,   BLACK,  Back_color, "bot %%%%", heat_bot_pwm[0], 4);
	LCD_ShowString_number( 350, 50,   BLACK,  Back_color, "top %%%%", heat_top_pwm[1], 4);
	LCD_ShowString_number( 450, 50,   BLACK,  Back_color, "bot %%%%", heat_bot_pwm[1], 4);
	LCD_ShowString_number( 350, 80,   BLACK,  Back_color, "top %%%%", heat_top_pwm[2], 4);
	LCD_ShowString_number( 450, 80,   BLACK,  Back_color, "bot %%%%", heat_bot_pwm[2], 4);
	LCD_ShowString_number( 350,110,   BLACK,  Back_color, "top %%%%", heat_top_pwm[3], 4);
	LCD_ShowString_number( 450,110,   BLACK,  Back_color, "bot %%%%", heat_bot_pwm[3], 4);
	LCD_ShowString_number( 350,140,   BLACK,  Back_color, "top %%%%", heat_top_pwm[4], 4);
	LCD_ShowString_number( 450,140,   BLACK,  Back_color, "bot %%%%", heat_bot_pwm[4], 4);
	LCD_ShowString_number( 350,170,   BLACK,  Back_color, "top %%%%", heat_top_pwm[5], 4);
	LCD_ShowString_number( 450,170,   BLACK,  Back_color, "bot %%%%", heat_bot_pwm[5], 4);
	LCD_ShowString_number( 350,200,   BLACK,  Back_color, "top %%%%", heat_top_pwm[6], 4);
	LCD_ShowString_number( 450,200,   BLACK,  Back_color, "bot %%%%", heat_bot_pwm[6], 4);
	LCD_ShowString_number( 350,230,   BLACK,  Back_color, "top %%%%", heat_top_pwm[7], 4);
	LCD_ShowString_number( 450,230,   BLACK,  Back_color, "bot %%%%", heat_bot_pwm[7], 4);
*/
}













/***********************************
	@brief£ºvoid byte_dot(uint16_t temp)
***********************************/
void lcd_main(void)
{
static uint8_t  first_flag =0;
static uint8_t  mode_value_old =0;
///////////////////////////////////////
	if (lcd_clear_flag !=0 )
	{
		LCD_Clear(Back_color);
		first_flag = 0 ;
	}

///////////////////////////////////////
	if (mode_value != mode_value_old)
	{
		LCD_Clear(Back_color);
		first_flag = 0 ;
		mode_value_old = mode_value;
	}
///////////////////////////////////////
	if ( !first_flag)
	{
		LCD_first();
		first_flag =1 ;
		wet_change = 0;
	}
///////////////////////////////////////
	font_size_select(0x01);

	LCD_cham();
	LCD_save_mode_icon();
	LCD_error_display();

///////////////////////////////////////
}






void lcd_left_data_display(void)
{
	font_size_select(0x00);

	LCD_ColorNumber( 575+11*2,  10,   BLACK,  Back_color,  time_year, 2, ZERO_ENABLE);
	LCD_ColorNumber( 575+11*5,  10,   BLACK,  Back_color,  time_month, 2, ZERO_ENABLE);
	LCD_ColorNumber( 575+11*8,  10,   BLACK,  Back_color,  time_date, 2, ZERO_ENABLE);
	LCD_ColorNumber( 575+11*12,  10,   BLACK,  Back_color,  time_hour, 2, ZERO_ENABLE);
	LCD_ColorNumber( 575+11*15,  10,   BLACK,  Back_color,  time_min, 2, ZERO_ENABLE);
	LCD_ColorNumber( 575+11*18,  10,   BLACK,  Back_color,  time_sec, 2, ZERO_ENABLE);
	LCD_ShowString_number( 664+11*4, 50,   BLACK,  Back_color, "%.%%", press_data1, 3);

	
	if ( gas_2nd_on_off==1)	{		
	
		if(	mode_value == gas_2_setting )
		{			LCD_ShowString( 664,  70,   BLACK,  Back_color, "Pr2:");
						LCD_ShowString( 763,  70,   BLACK,  Back_color, "bar"); 	
		}
		LCD_ShowString_number( 664+11*4, 70,   BLACK,  Back_color, "%.%%", press_data2, 3);		
	}

	
	if ( voc_filter_time > 240 ) 
	{
		LCD_ColorNumber( 664+11*3, 30,   BLACK,  Back_color, voc_filter_time, 4, ZERO_DISABLE);
	}
	else if ( voc_filter_time <= 240 && voc_filter_time >= 0 ) 
	{
		LCD_ColorNumber( 664+11*3, 30,   BLUE,  Back_color, voc_filter_time, 4, ZERO_DISABLE);
	}
	else
	{
		LCD_ShowString_number( 664+11*3, 30,   RED,  Back_color, "-", voc_filter_time * -1, 3);
		LCD_ColorNumber( 664+11*4, 30,   RED,  Back_color, voc_filter_time * -1, 3, ZERO_DISABLE);
	}

}








void Set_time_body(void)
{
	font_size_select(0x00);
	
	LCD_Fill(  85,  70,   86, 330,  BLACK);	
	LCD_Fill(  300, 70,  301, 330,  BLACK);	
	LCD_Fill(  400, 70,  401, 330,  BLACK);	

	LCD_Fill(  20, 90,   	 500, 90+24*0+1,  BLACK);	
	LCD_Fill(  20, 90+24*1,  500, 90+24*1+1,  BLACK);	
	LCD_Fill(  20, 90+24*2,  500, 90+24*2+1,  BLACK);	
	LCD_Fill(  20, 90+24*3,  500, 90+24*3+1,  BLACK);	
	LCD_Fill(  20, 90+24*4,  500, 90+24*4+1,  BLACK);	
	LCD_Fill(  20, 90+24*5,  500, 90+24*5+1,  BLACK);	
	LCD_Fill(  20, 90+24*6,  500, 90+24*6+1,  BLACK);	
	LCD_Fill(  20, 90+24*7,  500, 90+24*7+1,  BLACK);	
	LCD_Fill(  20, 90+24*8,  500, 90+24*8+1,  BLACK);	
	LCD_Fill(  20, 90+24*9,  500, 90+24*9+1,  BLACK);	
	LCD_Fill(  20, 90+24*10, 500, 90+24*10+1,  BLACK);	

	LCD_ShowString( 30,    72,   BLACK,  Back_color,  "No.");	
	LCD_ShowString( 120,   72,   BLACK,  Back_color,  "Date/Time");
	LCD_ShowString( 310,   72,   BLACK,  Back_color,  "Chamber");
	LCD_ShowString( 440,   72,   BLACK,  Back_color,  "Set");

	LCD_data_up_icon ( 530,  90);
	LCD_data_dw_icon ( 530, 220);
}



void Set_line_dis(uint16_t data_no, uint8_t *mode_data, uint8_t *cham_data, uint16_t *set_data, uint8_t *time_data)
{
uint16_t y_data,line;
uint8_t cham_dat[9],temp,temp2;

	for(line=0;line<10;line++)
	{
		y_data = 93+24*line ;

		LCD_ColorNumber( 25, y_data,   BLACK,  Back_color, ++data_no, 4, ZERO_DISABLE);
	
		if(*mode_data < 0x05)
		{
			temp = *cham_data++;
			cham_dat[0] = (temp & 0x01)?  '1' : ' ' ;
			cham_dat[1] = (temp & 0x02)?  '2' : ' ' ;
			cham_dat[2] = (temp & 0x04)?  '3' : ' ' ;
			cham_dat[3] = (temp & 0x08)?  '4' : ' ' ;
			cham_dat[4] = (temp & 0x10)?  '5' : ' ' ;
			cham_dat[5] = (temp & 0x20)?  '6' : ' ' ;
			cham_dat[6] = (temp & 0x40)?  '7' : ' ' ;
			cham_dat[7] = (temp & 0x80)?  '8' : ' ' ;
			cham_dat[8] = 0x00;	//null
			
			LCD_ShowString( 308, y_data,  BLACK,  Back_color, cham_dat );

			LCD_ShowString_number( 92,       y_data,  BLACK,  Back_color, "%%/",	*time_data++, 2);
			LCD_ShowString_number( 92+11*3,  y_data,  BLACK,  Back_color, "%%/",	*time_data++, 2);
			LCD_ShowString_number( 92+11*6,  y_data,  BLACK,  Back_color, "%% ",	*time_data++, 2);
			LCD_ShowString_number( 92+11*9,  y_data,  BLACK,  Back_color, "%%:",	*time_data++, 2);
			LCD_ShowString_number( 92+11*12, y_data,  BLACK,  Back_color, "%%:", 	*time_data++, 2);
			LCD_ShowString_number( 92+11*15, y_data,  BLACK,  Back_color, "%%", 	*time_data++, 2);

      temp2 = *mode_data++;
      switch(temp2)
      {
      case 0:
        LCD_ShowString_number( 410,      y_data,  BLACK,  Back_color, "%%.%      ", 	*set_data++, 3);
        break;
      case 1:
        LCD_ShowString_number( 410,      y_data,  BLACK,  Back_color, "%%.% T cal", 	*set_data++, 3);
        break;
      case 2:
        LCD_ShowString_number( 410,      y_data,  BLACK,  Back_color, "%%.% B cal", 	*set_data++, 3);
        break;
      case 3:
        LCD_ShowString_number( 410,      y_data,  BLACK,  Back_color, "T Init    ", 	*set_data++, 3);
        break; 
      case 4:
        LCD_ShowString_number( 410,      y_data,  BLACK,  Back_color, "B Init    ", 	*set_data++, 3);
        break; 
      }
		}

		else
		{
			cham_dat[0] = ' ' ;
			cham_dat[1] = ' ' ;
			cham_dat[2] = ' ' ;
			cham_dat[3] = ' ' ;
			cham_dat[4] = ' ' ;
			cham_dat[5] = ' ' ;
			cham_dat[6] = ' ' ;
			cham_dat[7] = ' ' ;
			cham_dat[8] = 0x00;	//null
	
			LCD_ShowString( 308, y_data,  BLACK,  Back_color, cham_dat );

			LCD_ShowString( 92,       y_data,  BLACK,  Back_color, "   ");
			LCD_ShowString( 92+11*3,  y_data,  BLACK,  Back_color, "   ");
			LCD_ShowString( 92+11*6,  y_data,  BLACK,  Back_color, "   ");
			LCD_ShowString( 92+11*9,  y_data,  BLACK,  Back_color, "   ");
			LCD_ShowString( 92+11*12, y_data,  BLACK,  Back_color, "   ");
			LCD_ShowString( 92+11*15, y_data,  BLACK,  Back_color, "  ");

			LCD_ShowString( 410,      y_data,  BLACK,  Back_color, "          ");	
		}
	}
}



void Set_time_data(uint16_t data_no)
{
//uint8_t i;
uint8_t time_data[60];
uint8_t cham_data[10];
uint16_t set_data[10];
uint8_t mode_data[10];

	font_size_select(0x00);

	Temp_set_Read(data_no*10, 10, mode_data, cham_data, set_data, time_data);
	Set_line_dis(data_no*10, mode_data, cham_data, set_data, time_data);	
}




void LCD_small_cham(uint8_t sel_ch)
{
uint8_t temp[8]={0,0,0,0,0,0,0,0};
	
	font_size_select(0x01);

	temp[sel_ch] = 1;

	LCD_small_icon( 10, 410, 0+'1', temp[0]);
	LCD_small_icon( 80, 410, 1+'1', temp[1]);
	LCD_small_icon(150, 410, 2+'1', temp[2]);
	LCD_small_icon(220, 410, 3+'1', temp[3]);
	LCD_small_icon(290, 410, 4+'1', temp[4]);
	LCD_small_icon(360, 410, 5+'1', temp[5]);

	if( chamber_count == 1)		return;

	LCD_small_icon(430, 410, 6+'1', temp[6]);
	LCD_small_icon(500, 410, 7+'1', temp[7]);
}










void LCD_Graph_Data_line(uint16_t low_value, uint16_t high_value, uint16_t *raw_data, uint16_t color, uint8_t dotline)
{
uint16_t	dis_data;
uint8_t		i, data[24];			//dotline = 0 ÀÏ¹Ý¼± 1= Á¡¼±
uint16_t	data_x1;
	
	for (i=0;i<24;i++)	
	{
		dis_data = raw_data[i];

    if(dis_data ==0xFFFF) dis_data = low_value;

		if ( dis_data < low_value ) dis_data = low_value;
		else if ( dis_data > high_value ) dis_data = high_value;
			
		data[i] = (uint8_t) 200 * ( dis_data - low_value) / (high_value-low_value) ;
	}

	data_x1 = 80;

	for (i=0;i<24;i++)	
	{
		LCD_Fill( data_x1-2,  (280-data[i])-2,  data_x1+2,  (280-data[i])+2,  color);

		if(i!=0) 
		{
			DrawLine( data_x1-20, 280-data[i-1] , data_x1, 280-data[i], color, dotline);
		}
		data_x1 += 20;
	}
}


void LCD_Graph_Body(void)
{
	LCD_Fill(  55,  80, 560,  80,  LGRAY);	
	LCD_Fill(  60, 100, 560, 100,  LGRAY);	
	LCD_Fill(  55, 120, 560, 120,  LGRAY);	
	LCD_Fill(  60, 140, 560, 140,  LGRAY);	
	LCD_Fill(  55, 160, 560, 160,  LGRAY);	
	LCD_Fill(  60, 180, 560, 180,  LGRAY);	
	LCD_Fill(  55, 200, 560, 200,  LGRAY);	
	LCD_Fill(  60, 220, 560, 220,  LGRAY);	
	LCD_Fill(  55, 240, 560, 240,  LGRAY);	
	LCD_Fill(  60, 260, 560, 260,  LGRAY);	

	LCD_Fill(  80,  80,  80, 285,  LGRAY);	
	LCD_Fill( 100,  80, 100, 285,  LGRAY);	
	LCD_Fill( 120,  80, 120, 285,  LGRAY);	
	LCD_Fill( 140,  80, 140, 285,  LGRAY);	
	LCD_Fill( 160,  80, 160, 285,  LGRAY);	
	LCD_Fill( 180,  80, 180, 285,  LGRAY);	
	LCD_Fill( 200,  80, 200, 285,  LGRAY);	
	LCD_Fill( 220,  80, 220, 285,  LGRAY);	
	LCD_Fill( 240,  80, 240, 285,  LGRAY);	
	LCD_Fill( 260,  80, 260, 285,  LGRAY);	
	LCD_Fill( 280,  80, 280, 285,  LGRAY);	
	LCD_Fill( 300,  80, 300, 285,  LGRAY);	
	LCD_Fill( 320,  80, 320, 285,  LGRAY);	
	LCD_Fill( 340,  80, 340, 285,  LGRAY);	
	LCD_Fill( 360,  80, 360, 285,  LGRAY);	
	LCD_Fill( 380,  80, 380, 285,  LGRAY);	
	LCD_Fill( 400,  80, 400, 285,  LGRAY);	
	LCD_Fill( 420,  80, 420, 285,  LGRAY);	
	LCD_Fill( 440,  80, 440, 285,  LGRAY);	
	LCD_Fill( 460,  80, 460, 285,  LGRAY);	
	LCD_Fill( 480,  80, 480, 285,  LGRAY);	
	LCD_Fill( 500,  80, 500, 285,  LGRAY);	
	LCD_Fill( 520,  80, 520, 285,  LGRAY);	
	LCD_Fill( 540,  80, 540, 285,  LGRAY);	

	LCD_Fill(  55, 280, 560, 280,  BLACK);	
	LCD_Fill(  60,  80,  60, 285,  BLACK);
}


void LCD_Graph_axis (void)
{
	font_size_select(0x00);

	LCD_ShowString( 0,   75,   BLACK,  Back_color,  "40.0");
	LCD_ShowString( 0,   95,   BLACK,  Back_color,  "39.0");
	LCD_ShowString( 0,  115,   BLACK,  Back_color,  "38.0");
	LCD_ShowString( 0,  135,   BLACK,  Back_color,  "37.0");
	LCD_ShowString( 0,  155,   BLACK,  Back_color,  "36.0");
	LCD_ShowString( 0,  175,   BLACK,  Back_color,  "35.0");
	LCD_ShowString( 0,  195,   BLACK,  Back_color,  "34.0");
	LCD_ShowString( 0,  215,   BLACK,  Back_color,  "33.0");
	LCD_ShowString( 0,  235,   BLACK,  Back_color,  "32.0");
	LCD_ShowString( 0,  255,   BLACK,  Back_color,  "31.0");
	LCD_ShowString( 0,  275,   BLACK,  Back_color,  "30.0");

	LCD_small_ShowString( 75, 290, WHITE, Back_color,  "0");
	LCD_small_ShowString( 95, 290, WHITE, Back_color,  "1");
	LCD_small_ShowString(115, 290, WHITE, Back_color,  "2");
	LCD_small_ShowString(135, 290, WHITE, Back_color,  "3");
	LCD_small_ShowString(155, 290, WHITE, Back_color,  "4");
	LCD_small_ShowString(175, 290, WHITE, Back_color,  "5");
	LCD_small_ShowString(195, 290, WHITE, Back_color,  "6");
	LCD_small_ShowString(215, 290, WHITE, Back_color,  "7");
	LCD_small_ShowString(235, 290, WHITE, Back_color,  "8");
	LCD_small_ShowString(255, 290, WHITE, Back_color,  "9");
	LCD_small_ShowString(275, 290, WHITE, Back_color,  "10");
	LCD_small_ShowString(295, 290, WHITE, Back_color,  "11");
	LCD_small_ShowString(315, 290, WHITE, Back_color,  "12");
	LCD_small_ShowString(335, 290, WHITE, Back_color,  "13");
	LCD_small_ShowString(355, 290, WHITE, Back_color,  "14");
	LCD_small_ShowString(375, 290, WHITE, Back_color,  "15");
	LCD_small_ShowString(395, 290, WHITE, Back_color,  "16");
	LCD_small_ShowString(415, 290, WHITE, Back_color,  "17");
	LCD_small_ShowString(435, 290, WHITE, Back_color,  "18");
	LCD_small_ShowString(455, 290, WHITE, Back_color,  "19");
	LCD_small_ShowString(475, 290, WHITE, Back_color,  "20");
	LCD_small_ShowString(495, 290, WHITE, Back_color,  "21");
	LCD_small_ShowString(515, 290, WHITE, Back_color,  "22");
	LCD_small_ShowString(535, 290, WHITE, Back_color,  "23");

}


void LCD_Graph_time( uint16_t x1, uint16_t y1, uint8_t *time_data)
{
	if ( *time_data++ == 0x01 )
	{
		LCD_ShowString_number( x1,       y1,  BLACK,  Back_color, "%%/",	*time_data++, 2);
		LCD_ShowString_number( x1+11*3,  y1,  BLACK,  Back_color, "%%/",	*time_data++, 2);
		LCD_ShowString_number( x1+11*6,  y1,  BLACK,  Back_color, "%%",		*time_data++, 2);
	}
	else
	{
		LCD_ShowString( x1, y1,   BLACK,  Back_color,  "        ");
	}
}



void LCD_Graph_display(void)
{
uint8_t  time_data[4];
uint16_t real_data[24];
uint16_t sett_data[24];

	LCD_Graph_axis();
	LCD_Graph_Body();

	Data_Temp_Read( setting_data_page, set_ch_no, time_data, real_data, sett_data);

	LCD_Graph_time( 260, 315, time_data);

	LCD_Graph_Data_line( 300, 400, real_data, BLUE, 0);	
	LCD_Graph_Data_line( 300, 400, sett_data, RED, 1);

}




//////////////////////////////////////////////////////
void lcd_graph_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();

	LCD_data_lf_icon ( 135, 340);
	LCD_data_rt_icon ( 330, 340);

	LCD_small_off_icon_string( 10, 320, BLACK, "CLEAR");	//CLEAR BUTTON

	set_ch_no = 0;
	setting_data_page = 0;

	LCD_select_icon(temp_sel,graph_sel);
	LCD_Graph_display();
	LCD_small_cham(set_ch_no);
}


void lcd_graph_data(void)
{
	LCD_Fill( 55,  70,  574,  300,  Back_color );	
	LCD_Graph_display();
	LCD_small_cham(set_ch_no);
}





//////////////////////////////////////////////////////

void lcd_history_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();	

	set_data_adr = 0;
	LCD_select_icon(temp_sel,history_sel);

	LCD_small_off_icon_string( 40, 340, BLACK, "CLEAR");	//CLEAR BUTTON
	LCD_small_off_icon_string( 400, 340, BLACK, " Log ");		//CLEAR BUTTON

	Set_time_body();	
	Set_time_data(set_data_adr);
}



void lcd_history_data(void)
{
	Set_time_data(set_data_adr);
}

//////////////////////////////////////////////////////

void lcd_ext_temp_first(void)
{
	LCD_ShowString( 420,  340,   MAGENTA,  Back_color,  "SENSOR");
	LCD_Fill( 430,  370,  510,     370+2,  0xf369 );
	LCD_Fill( 430,  430,  510,     430+2,  0xf369 );
	LCD_Fill( 430,  370,  430+2,   430,    0xf369 );
	LCD_Fill( 510,  370,  510+2,   430,    0xf369 );
}


void lcd_ext_temp_data(void)
{
	LCD_ColorNumber ( 450,   390,  RED,   Back_color,  ext_sensor_temp, 3, ZERO_DISABLE);
	LCD_Fill( 450+16+14,  390+18,  450+16+14+2,   390+18+2,  RED );
}


void lcd_valseting_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	

	LCD_left_data();

	set_alltemp_set_icon(50,220);

	LCD_data_inc_icon( 250,150);	
	LCD_data_dec_icon( 250,280);
	LCD_data_save_icon(420,200);

	set_calibration_off_icon(230,380);

	set_data_temp = cham_all_set_temp;
	LCD_select_icon(temp_sel,settting_sel);

	LCD_Fill( 260,  210,  340,   210+2,  0xf369 );
	LCD_Fill( 260,  270,  340,   270+2,  0xf369 );

	LCD_Fill( 260,  210,  260+2,   270,  0xf369 );
	LCD_Fill( 340,  210,  340+2,   270,  0xf369 );

	font_size_select(0x01);
	LCD_ColorNumber ( 280,   230,  RED,   Back_color,  set_data_temp, 3, ZERO_DISABLE);
	LCD_Fill( 280+16+14,  230+18,  280+16+14+2,   230+18+2,  RED );

	if(ext_sensor_sta ==1)	lcd_ext_temp_first();

}





void lcd_valseting_data(void)
{
	LCD_ColorNumber ( 280,   230,  RED,   Back_color,  set_data_temp, 3, ZERO_DISABLE);
	LCD_Fill( 280+16+14,  230+18,  280+16+14+2,   230+18+2,  RED );

	if(ext_sensor_sta ==1)	lcd_ext_temp_data();
}



//////////////////////////////////////////////////////

void Set_time_body2(void)
{
	font_size_select(0x00);
	
	LCD_Fill(  85,  70,   86, 330,  BLACK);	
	LCD_Fill(  300, 70,  301, 330,  BLACK);	
	LCD_Fill(  400, 70,  401, 330,  BLACK);	

	LCD_Fill(  20, 90,   	 500, 90+24*0+1,  BLACK);	
	LCD_Fill(  20, 90+24*1,  500, 90+24*1+1,  BLACK);	
	LCD_Fill(  20, 90+24*2,  500, 90+24*2+1,  BLACK);	
	LCD_Fill(  20, 90+24*3,  500, 90+24*3+1,  BLACK);	
	LCD_Fill(  20, 90+24*4,  500, 90+24*4+1,  BLACK);	
	LCD_Fill(  20, 90+24*5,  500, 90+24*5+1,  BLACK);	
	LCD_Fill(  20, 90+24*6,  500, 90+24*6+1,  BLACK);	
	LCD_Fill(  20, 90+24*7,  500, 90+24*7+1,  BLACK);	
	LCD_Fill(  20, 90+24*8,  500, 90+24*8+1,  BLACK);	
	LCD_Fill(  20, 90+24*9,  500, 90+24*9+1,  BLACK);	
	LCD_Fill(  20, 90+24*10, 500, 90+24*10+1,  BLACK);	

	LCD_ShowString( 30,    72,   BLACK,  Back_color,  "No.");	
	LCD_ShowString( 120,   72,   BLACK,  Back_color,  "Date/Time");
	LCD_ShowString( 320,   72,   BLACK,  Back_color,  "Temp.");
	LCD_ShowString( 420,   72,   BLACK,  Back_color,  "Sett.");

	LCD_data_up_icon ( 530,  90);
	LCD_data_dw_icon ( 530, 220);
}



void Set_line_dis2(uint16_t data_no, uint8_t *time_data, uint16_t *now_data, uint16_t *set_data)
{
uint16_t y_data,line;
uint8_t temp;

	for(line=0;line<10;line++)
	{
		y_data = 93+24*line ;
		temp = *time_data++;
		*time_data++;

		LCD_ColorNumber( 25, y_data,   BLACK,  Back_color, ++data_no, 4, ZERO_DISABLE);
	
		if(temp == 0x01)
		{			
			LCD_ShowString_number( 92,       y_data,  BLACK,  Back_color, "%%/",	*time_data++, 2);
			LCD_ShowString_number( 92+11*3,  y_data,  BLACK,  Back_color, "%%/",	*time_data++, 2);
			LCD_ShowString_number( 92+11*6,  y_data,  BLACK,  Back_color, "%% ",	*time_data++, 2);
			LCD_ShowString_number( 92+11*9,  y_data,  BLACK,  Back_color, "%%:",	*time_data++, 2);
			LCD_ShowString_number( 92+11*12, y_data,  BLACK,  Back_color, "%%:", 	*time_data++, 2);
			LCD_ShowString_number( 92+11*15, y_data,  BLACK,  Back_color, "%%", 	*time_data++, 2);

			LCD_ShowString_number( 320,      y_data,  BLACK,  Back_color, "%%.%", 	*now_data++, 3);
			LCD_ShowString_number( 420,      y_data,  BLACK,  Back_color, "%%.%", 	*set_data++, 3);

		}

		else
		{
			LCD_ShowString( 92,       y_data,  BLACK,  Back_color, "   ");
			LCD_ShowString( 92+11*3,  y_data,  BLACK,  Back_color, "   ");
			LCD_ShowString( 92+11*6,  y_data,  BLACK,  Back_color, "   ");
			LCD_ShowString( 92+11*9,  y_data,  BLACK,  Back_color, "   ");
			LCD_ShowString( 92+11*12, y_data,  BLACK,  Back_color, "   ");
			LCD_ShowString( 92+11*15, y_data,  BLACK,  Back_color, "  ");

			LCD_ShowString( 320,      y_data,  BLACK,  Back_color, "    ");	
			LCD_ShowString( 420,      y_data,  BLACK,  Back_color, "    ");	
		}
	}
}




void Set_time_data2(uint8_t set_ch_no, uint16_t data_no)
{
uint8_t time_data[80];
uint16_t now_data[80];
uint16_t set_data[80];
uint16_t now2_data[10];
uint16_t set2_data[10];
uint8_t i;

	font_size_select(0x00);

	Cham_temp_Read(data_no*10, 10,  time_data, now_data, set_data);

  for(i =0; i<10 ; i++)
  {
    now2_data[i] = now_data[8*i + set_ch_no];
    set2_data[i] = set_data[8*i + set_ch_no];
  }

	Set_line_dis2(data_no*10, time_data, now2_data, set2_data);	
}



void lcd_templog_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();	

	set_data_adr = 0;
	set_ch_no = 0;

	LCD_select_icon(temp_sel,history_sel);

	LCD_small_off_icon_string( 40, 340, BLACK, "CLEAR");	//CLEAR BUTTON
	LCD_small_off_icon_string( 400, 340, BLACK, " Set ");	//set BUTTON

	Set_time_body2();	

	Set_time_data2(set_ch_no,set_data_adr);
	LCD_small_cham(set_ch_no);
}



void lcd_templog_data(void)
{
	Set_time_data2(set_ch_no,set_data_adr);
	LCD_small_cham(set_ch_no);
}




void lcd_calib_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	

	LCD_left_data();
	LCD_select_icon(temp_sel,settting_sel);

	LCD_data_inc_icon( 250,130);	
	LCD_data_dec_icon( 250,260);
	LCD_data_save_icon(420,180);

	set_data_temp = 370;  //370
	set_ch_no = 0;
  set_posit =0;
	setting_data_page = 0;

	LCD_Fill( 260,  190,  340,   190+2,  0xf369 );
	LCD_Fill( 260,  250,  340,   250+2,  0xf369 );
	LCD_Fill( 260,  190,  260+2,   250,  0xf369 );
	LCD_Fill( 340,  190,  340+2,   250,  0xf369 );

	font_size_select(0x01);
	LCD_ShowString ( 20, 360,   BLUE,  Back_color, "Chamber");
}




void selsect_cal_mode(uint8_t set_p)
{
	font_size_select(0x00);

	if (set_p == 0)
	{
		set_calibration_on_icon(50,160);
		set_initial_off_icon(50,240);
	}
	else
	{
		set_calibration_off_icon(50,160);
		set_initial_on_icon(50,240);
	}
}


void select_cal_data(uint8_t set_p)
{
	if (set_p == 0)
	{
		LCD_ColorNumber ( 280,   210,  RED,   Back_color,  set_data_temp, 3, ZERO_DISABLE);
		LCD_Fill( 280+16+14,  210+18,  280+16+14+2,   210+18+2,  RED );
	}
	else
	{
		LCD_ShowString ( 280,   210,  RED,   Back_color,  "---");
	}
}



void lcd_calib_data(void)
{
	selsect_cal_mode(setting_data_page);

	font_size_select(0x01);
	switch (cali_set)
	{
		case 0 : select_cal_data(setting_data_page); 	break;						//0 = ¹ÌÁøÀÔ, 1 = µ¥ÀÌÅ¸ ¼öÁý Áß, 2 = µ¥ÀÌÅ¸ Á¤»ó 3 = µ¥ÀÌÅ¸ ¿À·ù
		case 1 : LCD_ShowString ( 280,   210,  RED,   Back_color,  "***");		break;	//
		case 2 : LCD_ShowString ( 280,   210,  RED,   Back_color,  "O K"); 	break;	//
		case 3 : LCD_ShowString ( 280,   210,  RED,   Back_color,  "ERR"); 	break;	//
	}

  LCD_small_icon( 220, 340, 'T', 1-set_posit);
  LCD_small_icon( 360, 340, 'B', set_posit);

	LCD_small_cham(set_ch_no);
}



/***********************************
	@brief£ºvoid byte_dot(uint16_t temp)
***********************************/
void lcd_setting(void)
{
static uint8_t graph_dis_sta = 0, old_mode_val = 0, old_set_sub = 0;
	
///////////////////////////////////////
	if (lcd_clear_flag !=1 )
	{
		LCD_Clear(Back_color);
		graph_dis_sta = 0x00;
	}
/////////////////////////////////////////
	if ( mode_value != old_mode_val || sub_set_mode != old_set_sub )
	{
		graph_dis_sta = 0x00;
		set_dis_refresh = 0;		
		old_mode_val  = mode_value;
		old_set_sub = sub_set_mode;
	}
/////////////////////////////////////////
	font_size_select(0x01);

	if ( graph_dis_sta == 0x00 )
	{
		set_data_adr = 0;
		graph_dis_sta = 1;

		switch(sub_set_mode)
		{
			case settting_sel :
				lcd_valseting_first();
				break;

			case graph_sel : 
				lcd_graph_first();
				break;

			case history_sel : 
				lcd_history_first();
				break;

			case calib_sel :
				lcd_calib_first();
				break;

			case temp_log_sel :
				lcd_templog_first();
				break;

		}
	}
/////////////////////////////////////////	
	if (set_dis_refresh ==0)
	{
		set_dis_refresh = 1;

		switch(sub_set_mode)
		{
			case settting_sel: 
				lcd_valseting_data();
				set_dis_refresh = 0;
				break;

			case graph_sel : 
				lcd_graph_data();
				break;

			case history_sel : 
				lcd_history_data();
				break;

			case calib_sel :
				lcd_calib_data();
				break;

			case temp_log_sel :
				lcd_templog_data();
				break;

		}
	}
/////////////////////////////////////////	
}



/*****************************************************************************************************************/
/*****************************************************************************************************************/
/*****************************************************************************************************************/

void  LCD_select_icon2(uint8_t mode_sel)
{
uint8_t sel_mode_a=0, sel_mode_b=0, sel_mode_c=0;

	font_size_select(0x00);

	switch(mode_sel)
	{
		case alarm_sel:	 sel_mode_a=1;
				break;
		case log_sel:	 sel_mode_b=1;
				break;
		case voc_sel:	 sel_mode_c=1;
				break;
	}

	set_sel_icon_m0(  5,15, sel_mode_a);
	set_sel_icon_m1(190,15, sel_mode_b);
	set_sel_icon_m2(375,15, sel_mode_c);

	font_size_select(0x01);

}	



/*
void  set_sel_icon2(uint16_t x1, uint16_t y1, uint8_t *string, uint8_t status)
{
	if (status == 0)
	{
		LCD_select2_icon(x1, y1);
		LCD_ShowString2 (x1+6,  y1+6,   BLACK,  select0_2+6, string);
	}
	else
	{
		LCD_select3_icon(x1, y1);
		LCD_ShowString2 (x1+6,  y1+6,   BLACK,  select1_2+6, string);
	}
}
*/






void Set_alarm_body(void)
{
	font_size_select(0x00);

	LCD_Fill(  85,  70,   86, 420,  BLACK);	
	LCD_Fill(  300, 70,  301, 420,  BLACK);	
	LCD_Fill(  410, 70,  411, 420,  BLACK);	

	LCD_Fill(  20, 90,   	 500, 90+30*0+1,  BLACK);	
	LCD_Fill(  20, 90+30*1,  500, 90+30*1+1,  BLACK);	
	LCD_Fill(  20, 90+30*2,  500, 90+30*2+1,  BLACK);	
	LCD_Fill(  20, 90+30*3,  500, 90+30*3+1,  BLACK);	
	LCD_Fill(  20, 90+30*4,  500, 90+30*4+1,  BLACK);	
	LCD_Fill(  20, 90+30*5,  500, 90+30*5+1,  BLACK);	
	LCD_Fill(  20, 90+30*6,  500, 90+30*6+1,  BLACK);	
	LCD_Fill(  20, 90+30*7,  500, 90+30*7+1,  BLACK);	
	LCD_Fill(  20, 90+30*8,  500, 90+30*8+1,  BLACK);	
	LCD_Fill(  20, 90+30*9,  500, 90+30*9+1,  BLACK);	
	LCD_Fill(  20, 90+30*10, 500, 90+30*10+1,  BLACK);	
	LCD_Fill(  20, 90+30*11, 500, 90+30*11+1,  BLACK);	

	LCD_ShowString( 30,    72,   BLACK,  Back_color,  "No.");	
	LCD_ShowString( 120,   72,   BLACK,  Back_color,  "Items");
	LCD_ShowString( 310,   72,   BLACK,  Back_color,  "Sound");
	LCD_ShowString( 420,   72,   BLACK,  Back_color,  "Display");

	LCD_ShowString( 30,  100,   BLACK,  Back_color, " 1");
	LCD_ShowString( 30,  130,   BLACK,  Back_color, " 2");
	LCD_ShowString( 30,  160,   BLACK,  Back_color, " 3");
	LCD_ShowString( 30,  190,   BLACK,  Back_color, " 4");
	LCD_ShowString( 30,  220,   BLACK,  Back_color, " 5");
	LCD_ShowString( 30,  250,   BLACK,  Back_color, " 6");
	LCD_ShowString( 30,  280,   BLACK,  Back_color, " 7");
	LCD_ShowString( 30,  310,   BLACK,  Back_color, " 8");
	LCD_ShowString( 30,  340,   BLACK,  Back_color, " 9");
	LCD_ShowString( 30,  370,   BLACK,  Back_color, "10");
	LCD_ShowString( 30,  400,   BLACK,  Back_color, "11");

	LCD_ShowString( 100,  100,   BLACK,  Back_color, "DOOR OPEN");
	LCD_ShowString( 100,  130,   BLACK,  Back_color, "TOP HIGH TEMP");
	LCD_ShowString( 100,  160,   BLACK,  Back_color, "BOT HIGH TEMP");
	LCD_ShowString( 100,  190,   BLACK,  Back_color, "TOP LOW TEMP");
	LCD_ShowString( 100,  220,   BLACK,  Back_color, "BOT LOW TEMP");
	LCD_ShowString( 100,  250,   BLACK,  Back_color, "TOP TEMP SENSOR");
	LCD_ShowString( 100,  280,   BLACK,  Back_color, "BOT TEMP SENSOR");
	LCD_ShowString( 100,  310,   BLACK,  Back_color, "GAS SUPPLY");
	LCD_ShowString( 100,  340,   BLACK,  Back_color, "SOL VALVE");
	LCD_ShowString( 100,  370,   BLACK,  Back_color, "VOC FILTER");
	LCD_ShowString( 100,  400,   BLACK,  Back_color, "TOUCH");
}




void voc_change_icon(uint8_t change)
{
	font_size_select(0x01);

	if (change == 0)
	{
		LCD_set_icon2(390,380);
		LCD_ShowString2( 390+20,  370+20,  BLACK,  set_icon_2+30,  "Start");
		LCD_ShowString2( 390+20,  370+50,  BLACK,  set_icon_2+50,  "replace");
	}
	else
	{
		LCD_set_icon2(390,380);
		LCD_ShowString2( 390+20,  370+35,  BLACK,  set_icon_2+35,  "END");
	}
}


void Set_voc_body(void)
{
	font_size_select(0x00);

	LCD_Fill(  85,  70,   86, 330,  BLACK);	
	LCD_Fill(  300, 70,  301, 330,  BLACK);	

	LCD_Fill(  20, 90,   	 500, 90+24*0+1,  BLACK);	
	LCD_Fill(  20, 90+24*1,  500, 90+24*1+1,  BLACK);	
	LCD_Fill(  20, 90+24*2,  500, 90+24*2+1,  BLACK);	
	LCD_Fill(  20, 90+24*3,  500, 90+24*3+1,  BLACK);	
	LCD_Fill(  20, 90+24*4,  500, 90+24*4+1,  BLACK);	
	LCD_Fill(  20, 90+24*5,  500, 90+24*5+1,  BLACK);	
	LCD_Fill(  20, 90+24*6,  500, 90+24*6+1,  BLACK);	
	LCD_Fill(  20, 90+24*7,  500, 90+24*7+1,  BLACK);	
	LCD_Fill(  20, 90+24*8,  500, 90+24*8+1,  BLACK);	
	LCD_Fill(  20, 90+24*9,  500, 90+24*9+1,  BLACK);	
	LCD_Fill(  20, 90+24*10, 500, 90+24*10+1,  BLACK);

	LCD_ShowString( 30,    72,   BLACK,  Back_color,  "No.");	
	LCD_ShowString( 120,   72,   BLACK,  Back_color,  "Date/time");
	LCD_ShowString( 320,   72,   BLACK,  Back_color,  "Valid time");


	LCD_data_up_icon ( 530,  90);
	LCD_data_dw_icon ( 530, 220);

	LCD_small_off_icon_string( 10, 370, BLACK, "CLEAR");	//CLEAR BUTTON

	LCD_ShowString( 95,   340+48,   BLACK,  Back_color,  "Valid");
	LCD_ShowString( 95,   340+48+16,   BLACK,  Back_color,  " time");
	LCD_time_updw_icon2(165,340);
	LCD_ShowString( 235,   340+48,   BLACK,  Back_color,  "Alarm");
	LCD_time_updw_icon2(305,340);

	LCD_ShowString( 370,   360,   BLACK,  Back_color,  "Expiration:    H");

	LCD_ShowChar( 165+14+12,  340+48,   BLACK,  Back_color,  'M');
	LCD_ShowChar( 305+14+24,  340+48,   BLACK,  Back_color,  'H');

	voc_change_icon(change_sta);
}





/*
void LCD_time_updw_icon(uint16_t x1, uint16_t y1)
{
	LCD_data_inc_icon( x1,y1);	
	LCD_data_dec_icon( x1,y1+130);

	LCD_Fill( x1+10,  y1+60,  x1+100,   y1+60+2,  0xf369 );
	LCD_Fill( x1+10, y1+120,  x1+100,   y1+120+2,  0xf369 );
	LCD_Fill( x1+10,  y1+60,  x1+10+2,   y1+120,  0xf369 );
	LCD_Fill( x1+100,  y1+60,  x1+100+2,   y1+120,  0xf369 );

}
*/





void lcd_timedate_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();

	set_year = time_year;
	set_week = time_week;
	set_month = time_month;
	set_date = time_date;
	set_hour = time_hour;
	set_min = time_min;
	set_sec = time_sec;

	LCD_time_updw_icon(5,150);
	LCD_time_updw_icon2(125,150);
	LCD_time_updw_icon2(215,150);
	LCD_time_updw_icon2(305,150);
	LCD_time_updw_icon2(395,150);
	LCD_time_updw_icon2(485,150);

	LCD_data_save_icon(420,300);	

	font_size_select(0x01);

	LCD_ShowString( 100,  150+48,   BLACK,  Back_color, "Y");	
	LCD_ShowString( 190,  150+48,   BLACK,  Back_color, "M");	
	LCD_ShowString( 280,  150+48,   BLACK,  Back_color, "D");	
	LCD_ShowString( 370,  150+48,   BLACK,  Back_color, "H");	
	LCD_ShowString( 460,  150+48,   BLACK,  Back_color, "M");	
	LCD_ShowString( 550,  150+48,   BLACK,  Back_color, "S");	


	LCD_ShowString( 18,  198,   BLACK,  Back_color, "20");	

	LCD_ColorNumber( 18+32,   198,   BLACK,  Back_color,  set_year, 2, ZERO_ENABLE);
	LCD_ColorNumber( 125+14,  198,   BLACK,  Back_color,  set_month, 2, ZERO_ENABLE);
	LCD_ColorNumber( 215+14,  198,   BLACK,  Back_color,  set_date, 2, ZERO_ENABLE);
	LCD_ColorNumber( 305+14,  198,   BLACK,  Back_color,  set_hour, 2, ZERO_ENABLE);
	LCD_ColorNumber( 395+14,  198,   BLACK,  Back_color,  set_min, 2, ZERO_ENABLE);
	LCD_ColorNumber( 485+14,  198,   BLACK,  Back_color,  set_sec, 2, ZERO_ENABLE);




}


void lcd_alarm_set_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();

	LCD_select_icon2(alarm_sel);

	Set_alarm_body();

}






void Set_log_body(void)
{
	font_size_select(0x00);

	LCD_Fill(  52,  70,   53, 330,  BLACK);	
	LCD_Fill(  252, 70,  253, 330,  BLACK);	
	LCD_Fill(  382, 70,  383, 330,  BLACK);	

	LCD_Fill(  10, 90,   	 500, 90+24*0+1,  BLACK);	
	LCD_Fill(  10, 90+24*1,  500, 90+24*1+1,  BLACK);	
	LCD_Fill(  10, 90+24*2,  500, 90+24*2+1,  BLACK);	
	LCD_Fill(  10, 90+24*3,  500, 90+24*3+1,  BLACK);	
	LCD_Fill(  10, 90+24*4,  500, 90+24*4+1,  BLACK);	
	LCD_Fill(  10, 90+24*5,  500, 90+24*5+1,  BLACK);	
	LCD_Fill(  10, 90+24*6,  500, 90+24*6+1,  BLACK);	
	LCD_Fill(  10, 90+24*7,  500, 90+24*7+1,  BLACK);	
	LCD_Fill(  10, 90+24*8,  500, 90+24*8+1,  BLACK);	
	LCD_Fill(  10, 90+24*9,  500, 90+24*9+1,  BLACK);	
	LCD_Fill(  10, 90+24*10, 500, 90+24*10+1,  BLACK);

	LCD_ShowString( 15,    72,   BLACK,  Back_color,  "No.");	
	LCD_ShowString( 100,   72,   BLACK,  Back_color,  "Date/time");
	LCD_ShowString( 310,   72,   BLACK,  Back_color,  "Item");
	LCD_ShowString( 420,   72,   BLACK,  Back_color,  "Signal");

	LCD_data_up_icon ( 530,  90);
	LCD_data_dw_icon ( 530, 220);
}



void lcd_log_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();

	out_usb_mode[0] =1 ;
	out_usb_mode[1] =1 ;
	out_usb_mode[2] =1 ;
	out_usb_mode[3] =1 ;
	out_usb_mode[4] =1 ;
	out_usb_mode[5] =1 ;

	LCD_select_icon2(log_sel);
	
	Set_log_body();
	
	font_size_select(0x00);

	usb_out_status = 0;

	LCD_ShowString( 50,  375,   BLACK,  Back_color, "Temp");	
	LCD_ShowString( 150,  375,   BLACK,  Back_color, "Alarm");	
	LCD_ShowString( 250,  375,   BLACK,  Back_color, "Log");	
	LCD_ShowString( 350,  375,   BLACK,  Back_color, "VOC");	
	LCD_ShowString(  50,  415,   BLACK,  Back_color, "Temp Log");	
	LCD_ShowString( 250,  415,   RED,  Back_color, "All");	

	LCD_small_off_icon_string (390,380,BLUE, "Down");

	LCD_small_off_icon_string (490,380,BLUE, "Clear");

}



void main_log_sub_display(uint16_t line, uint8_t move_cnt, uint8_t sub_error, uint8_t sub_ch)
{
uint16_t y_data;
uint8_t	string_c[30];

  y_data = 93+24*line ;

  switch(sub_error)
  {
    case 1 : 
    LCD_alarm_load("CHAMBER% DOOR OPEN!         ", sub_ch+1,1, string_c);
    LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
    break;
    case 2 : 
    LCD_alarm_load("CHAMBER% TOP-HEATER HI ERR  ", sub_ch+1,1, string_c);	//EEROR 2
    LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
    break;
    case 3 : 
    LCD_alarm_load("CHAMBER% BOT-HEATER HI ERR  ", sub_ch+1,1, string_c);	//EEROR 3
    LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
    break;
    case 4 : 
    LCD_alarm_load("CHAMBER% TOP-HEATER LO ERR  ", sub_ch+1,1, string_c);	//EEROR 4
    LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
    break;
    case 5 : 
    LCD_alarm_load("CHAMBER% BOT-HEATER LO ERR  ", sub_ch+1,1, string_c);	//EEROR 5
    LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
    break;	
    case 6 : 
    LCD_alarm_load("CHAMBER% TOPTMP SENSOR ERR  ", sub_ch+1,1, string_c);	//EEROR 6
    LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
    break;
    case 7 : 
    LCD_alarm_load("CHAMBER% BOTTMP SENSOR ERR  ", sub_ch+1,1, string_c);	//EEROR 7
    LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
    break;
    case 8 : 
    LCD_alarm_load("GAS% SUPPLY PRESSURE ERROR  ", sub_ch+1,1, string_c);	//EEROR 8
    LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
    break;
    case 9 : 
    LCD_alarm_load("GAS% SOL VALVE ERROR        ", sub_ch+1,1, string_c);	//EEROR 9
    LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
    break;
    case 10: 
    LCD_alarm_load("VOC FILTER REMAIN DAY %%    ", sub_ch+1,2, string_c);	//EEROR 10
    LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
    break;
    case 11: 
    LCD_alarm_load("POWER OFF STRANGE ERROR     ", sub_ch+1,1, string_c);	//EEROR 11
    LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
    break;
  }

}



void main_log_display(uint16_t data_no, uint8_t *dis_data1, uint8_t *dis_data2)
{
uint16_t y_data,line;
uint8_t temp0,temp1,temp2,temp3;
static uint8_t move_cnt = 0, move_spd = 10;

	if (--move_spd == 0)	{ move_spd= 3;	 if (++move_cnt == 18)	{move_spd = 10; move_cnt = 0;}}

	for(line=0;line<10;line++)
	{
		y_data = 93+24*line ;

		LCD_ColorNumber( 4, y_data,   BLACK,  Back_color, ++data_no, 4, ZERO_DISABLE);

		temp0 = *dis_data1++;
		temp1 = *dis_data1++;
		temp2 = *dis_data1++;
		temp3 = *dis_data1++;

	if(temp0 == 1)
		{
			LCD_ShowString_number( 60,       y_data,  BLACK,  Back_color, "%%/",   *dis_data2++, 2);
			LCD_ShowString_number( 60+11*3,  y_data,  BLACK,  Back_color, "%%/",   *dis_data2++, 2);
			LCD_ShowString_number( 60+11*6,  y_data,  BLACK,  Back_color, "%% ",   *dis_data2++, 2);
			LCD_ShowString_number( 60+11*9,  y_data,  BLACK,  Back_color, "%%:",   *dis_data2++, 2);
			LCD_ShowString_number( 60+11*12, y_data,  BLACK,  Back_color, "%%:",   *dis_data2++, 2);
			LCD_ShowString_number( 60+11*15, y_data,  BLACK,  Back_color, "%%",    *dis_data2++, 2);
		}
		else
		{
			LCD_ShowString( 60,       y_data,   BLACK,  Back_color, "   ");
			LCD_ShowString( 60+11*3,  y_data,   BLACK,  Back_color, "   ");
			LCD_ShowString( 60+11*6,  y_data,   BLACK,  Back_color, "   ");
			LCD_ShowString( 60+11*9,  y_data,   BLACK,  Back_color, "   ");
			LCD_ShowString( 60+11*12, y_data,   BLACK,  Back_color, "   ");
			LCD_ShowString( 60+11*15, y_data,   BLACK,  Back_color, "  ");

      *dis_data2++;
      *dis_data2++;
      *dis_data2++;
      *dis_data2++;
      *dis_data2++;
      *dis_data2++;
      temp1 =0;
      temp2 =0;
      temp3 =0;
		}

		switch(temp1)
		{
			case 0: 
				LCD_ShowString( 260,  y_data,   BLACK,  Back_color, "           ");
				LCD_ShowString( 390,  y_data,   BLACK,  Back_color, "            ");
				break;
			case 1: 
				LCD_ShowString( 260,  y_data,   BLACK,  Back_color, "Temperature");
				LCD_ShowString( 390,  y_data,   BLACK,  Back_color, "Set Change  ");
				break;
			case 2: 
				LCD_ShowString_number( 260,  y_data,   BLACK,  Back_color, "Chamber No%", temp2+1, 1);
				if (temp3 ==0x00)
				{	LCD_ShowString( 390,  y_data,   BLACK,  Back_color, "Door Open   ");}
				else
				{	LCD_ShowString( 390,  y_data,   BLACK,  Back_color, "Door Close  ");}
				break;
			case 3: 
				LCD_ShowString_number( 260,  y_data,   BLACK,  Back_color, "Chamber No%", temp2+1, 1);
				if (temp3 ==0x00)
				{	LCD_ShowString( 390,  y_data,   BLACK,  Back_color, "CultureEmpty");}
				else
				{	LCD_ShowString( 390,  y_data,   BLACK,  Back_color, "CultureStart");}
				break;
			case 4: 
				LCD_ShowString_number( 260,  y_data,   BLACK,  Back_color, "VOC Filter ", temp2, 1);
				LCD_ShowString_number( 390,  y_data,   BLACK,  Back_color, "%Month Start", temp2, 1);
				break;
			case 5: 
				LCD_ShowString_number( 260,  y_data,   BLACK,  Back_color, "Alarm List ", temp2, 1);
        main_log_sub_display(line, move_cnt, temp3, temp2);
				break;
				
			case 6: 
				LCD_ShowString( 260,  y_data,   BLACK,  Back_color, "Mode Change");

				if (temp2 ==0x00)
				{	LCD_ShowString( 390,  y_data,   BLACK,  Back_color, "OIL Mode    ");}
				else
				{	LCD_ShowString( 390,  y_data,   BLACK,  Back_color, "WET Mode    ");}
				break;

			case 7: 
				LCD_ShowString( 260,  y_data,   BLACK,  Back_color, "CO2 Change ");
				LCD_ShowString_number( 390,  y_data,   BLACK,  Back_color, "%.% Percent ", temp2, 2);
				break;

			case 8: 
				LCD_ShowString( 260,  y_data,   BLACK,  Back_color, "O2 Change  ");
				LCD_ShowString_number( 390,  y_data,   BLACK,  Back_color, "%.% Percent ", temp2, 2);
				break;

			case 9: 
				LCD_ShowString( 260,  y_data,   BLACK,  Back_color, "Calibration");
      	LCD_ShowString_number( 390,  y_data,   BLACK,  Back_color, "Chamber No%", temp2+1, 1);
        break;

			case 10: 
				LCD_ShowString( 260,  y_data,   BLACK,  Back_color, "Initialize ");
      	LCD_ShowString_number( 390,  y_data,   BLACK,  Back_color, "Chamber No%", temp2+1, 1);
        break;

		}
	}
}








/*
void main_log_display(uint16_t data_no, uint8_t *dis_data1, uint8_t *dis_data2)
{
uint16_t y_data,line;
uint8_t temp0,temp1,temp2,temp3;
uint8_t	string_c[26];
static uint8_t move_cnt = 0, move_spd = 10;

	if (--move_spd == 0)	{ move_spd= 3;	 if (++move_cnt == 20)	{move_spd = 10; move_cnt = 0;}}

	for(line=0;line<10;line++)
	{
		y_data = 93+24*line ;

		LCD_ColorNumber( 4, y_data,   BLACK,  Back_color, ++data_no, 4, ZERO_DISABLE);

		temp0 = *dis_data1++;
		temp1 = *dis_data1++;
		temp2 = *dis_data1++;
		temp3 = *dis_data1++;

		if(temp0 == 1)
		{
			LCD_ShowString_number( 60,       y_data,  BLACK,  Back_color, "%%/",   *dis_data2++, 2);
			LCD_ShowString_number( 60+11*3,  y_data,  BLACK,  Back_color, "%%/",   *dis_data2++, 2);
			LCD_ShowString_number( 60+11*6,  y_data,  BLACK,  Back_color, "%% ",   *dis_data2++, 2);
			LCD_ShowString_number( 60+11*9,  y_data,  BLACK,  Back_color, "%%:",   *dis_data2++, 2);
			LCD_ShowString_number( 60+11*12, y_data,  BLACK,  Back_color, "%%:",   *dis_data2++, 2);
			LCD_ShowString_number( 60+11*15, y_data,  BLACK,  Back_color, "%%",    *dis_data2++, 2);
		}
		else
		{
			LCD_ShowString( 60,       y_data,   BLACK,  Back_color, "   ");
			LCD_ShowString( 60+11*3,  y_data,   BLACK,  Back_color, "   ");
			LCD_ShowString( 60+11*6,  y_data,   BLACK,  Back_color, "   ");
			LCD_ShowString( 60+11*9,  y_data,   BLACK,  Back_color, "   ");
			LCD_ShowString( 60+11*12, y_data,   BLACK,  Back_color, "   ");
			LCD_ShowString( 60+11*15, y_data,   BLACK,  Back_color, "  ");
      temp1 = 0;
		}
    
    switch(temp1)
    {
      case 0: 
        LCD_ShowString( 260,  y_data,   BLACK,  Back_color, "           ");
        LCD_ShowString( 390,  y_data,   BLACK,  Back_color, "            ");
        break;
      case 1: 
        LCD_ShowString( 260,  y_data,   BLACK,  Back_color, "Temperature");
        LCD_ShowString( 390,  y_data,   BLACK,  Back_color, "Set Change  ");
        break;
      case 2: 
        LCD_ShowString_number( 260,  y_data,   BLACK,  Back_color, "Chamber No%", temp2+1, 1);
        if (temp3 ==0x00)
        {	LCD_ShowString( 390,  y_data,   BLACK,  Back_color, "Door Open   ");}
        else
        {	LCD_ShowString( 390,  y_data,   BLACK,  Back_color, "Door Close  ");}
        break;
      case 3: 
        LCD_ShowString_number( 260,  y_data,   BLACK,  Back_color, "Chamber No%", temp2+1, 1);
        if (temp3 ==0x00)
        {	LCD_ShowString( 390,  y_data,   BLACK,  Back_color, "CultureEmpty");}
        else
        {	LCD_ShowString( 390,  y_data,   BLACK,  Back_color, "CultureStart");}
        break;
      case 4: 
        LCD_ShowString_number( 260,  y_data,   BLACK,  Back_color, "VOC Filter ", temp2, 1);
        LCD_ShowString_number( 390,  y_data,   BLACK,  Back_color, "%Month Start", temp2, 1);
        break;
      case 5: 
        LCD_ShowString_number( 260,  y_data,   BLACK,  Back_color, "Alarm List ", temp2, 1);

        switch(temp3)
        {
        case 1 : 
          LCD_alarm_load("CHAMBER% DOOR OPEN!         ", temp2+1,1, string_c);
          LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
          break;
        case 2 : 
          LCD_alarm_load("CHAMBER% TOP-HEATER HI ERR  ", temp2+1,1, string_c);	//EEROR 2
          LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
          break;
        case 3 : 
          LCD_alarm_load("CHAMBER% BOT-HEATER HI ERR  ", temp2+1,1, string_c);	//EEROR 3
          LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
          break;
        case 4 : 
          LCD_alarm_load("CHAMBER% TOP-HEATER LO ERR  ", temp2+1,1, string_c);	//EEROR 4
          LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
          break;
        case 5 : 
          LCD_alarm_load("CHAMBER% BOT-HEATER LO ERR  ", temp2+1,1, string_c);	//EEROR 5
          LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
          break;	
        case 6 : 
          LCD_alarm_load("CHAMBER% TOPTMP SENSOR ERR  ", temp2+1,1, string_c);	//EEROR 6
          LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
          break;
        case 7 : 
          LCD_alarm_load("CHAMBER% BOTTMP SENSOR ERR  ", temp2+1,1, string_c);	//EEROR 7
          LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
          break;
        case 8 : 
          LCD_alarm_load("GAS% SUPPLY PRESSURE ERROR   ", temp2+1,1, string_c);	//EEROR 8
          LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
          break;
        case 9 : 
          LCD_alarm_load("GAS% SOL VALVE ERROR         ", temp2+1,1, string_c);	//EEROR 9
          LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
          break;
        case 10: 
          LCD_alarm_load("VOC FILTER REMAIN DAY %%    ", temp2+1,2, string_c);	//EEROR 10
          LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
          break;
        case 11: 
          LCD_alarm_load("POWER OFF STRANGE ERROR     ", temp2+1,1, string_c);	//EEROR 11
          LCD_ShowString_buf(390,  y_data, string_c, move_cnt);
          break;
        }
        break;

      case 6: 
        LCD_ShowString( 260,  y_data,   BLACK,  Back_color, "Mode Change");

        if (temp2 ==0x00)
        {	LCD_ShowString( 390,  y_data,   BLACK,  Back_color, "OIL Mode    ");}
        else
        {	LCD_ShowString( 390,  y_data,   BLACK,  Back_color, "WET Mode    ");}
        break;

      case 7: 
        LCD_ShowString( 260,  y_data,   BLACK,  Back_color, "CO2 Change ");
        LCD_ShowString_number( 390,  y_data,   BLACK,  Back_color, "%.% Percent ", temp2, 2);
        break;

      case 8: 
        LCD_ShowString( 260,  y_data,   BLACK,  Back_color, "O2 Change  ");
        LCD_ShowString_number( 390,  y_data,   BLACK,  Back_color, "%.% Percent ", temp2, 2);
        break;

      case 9: 
        LCD_ShowString( 260,  y_data,   BLACK,  Back_color, "Calibration");
        LCD_ShowString_number( 390,  y_data,   BLACK,  Back_color, "Chamber No%", temp2+1, 1);
        break;

      case 10: 
        LCD_ShowString( 260,  y_data,   BLACK,  Back_color, "Initialize ");
        LCD_ShowString_number( 390,  y_data,   BLACK,  Back_color, "Chamber No%", temp2+1, 1);
        break;
    }
	}
}
*/








void lcd_voc_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();

	LCD_select_icon2(voc_sel);

	change_sta = 0;
	Set_voc_body();	
}




void lcd_timedate_data(void)
{
	LCD_ColorNumber( 18+32,   198,   BLACK,  Back_color,  set_year, 2, ZERO_ENABLE);
	LCD_ColorNumber( 125+14,  198,   BLACK,  Back_color,  set_month, 2, ZERO_ENABLE);
	LCD_ColorNumber( 215+14,  198,   BLACK,  Back_color,  set_date, 2, ZERO_ENABLE);
	LCD_ColorNumber( 305+14,  198,   BLACK,  Back_color,  set_hour, 2, ZERO_ENABLE);
	LCD_ColorNumber( 395+14,  198,   BLACK,  Back_color,  set_min, 2, ZERO_ENABLE);
	LCD_ColorNumber( 485+14,  198,   BLACK,  Back_color,  set_sec, 2, ZERO_ENABLE);
}


void lcd_alarm_set_data(void)
{
	LCD_check_on_off(330,94+30*0, error_set_save[0]);
	LCD_check_on_off(330,94+30*1, error_set_save[1]);
	LCD_check_on_off(330,94+30*2, error_set_save[2]);
	LCD_check_on_off(330,94+30*3, error_set_save[3]);
	LCD_check_on_off(330,94+30*4, error_set_save[4]);
	LCD_check_on_off(330,94+30*5, error_set_save[5]);
	LCD_check_on_off(330,94+30*6, error_set_save[6]);
	LCD_check_on_off(330,94+30*7, error_set_save[7]);
	LCD_check_on_off(330,94+30*8, error_set_save[8]);
	LCD_check_on_off(330,94+30*9, error_set_save[9]);
	LCD_check_on_off(330,94+30*10,error_set_save[10]);
	
	LCD_check_on_off(440,94+30*0, error_set_save[11]);
	LCD_check_on_off(440,94+30*1, error_set_save[12]);
	LCD_check_on_off(440,94+30*2, error_set_save[13]);
	LCD_check_on_off(440,94+30*3, error_set_save[14]);
	LCD_check_on_off(440,94+30*4, error_set_save[15]);
	LCD_check_on_off(440,94+30*5, error_set_save[16]);
	LCD_check_on_off(440,94+30*6, error_set_save[17]);
	LCD_check_on_off(440,94+30*7, error_set_save[18]);
	LCD_check_on_off(440,94+30*8, error_set_save[19]);
	LCD_check_on_off(440,94+30*9, error_set_save[20]);
	LCD_check_on_off(440,94+30*10,error_set_save[21]);
}







void usb_out_sta_display(uint8_t usb_sta)			// 0= Á¤»ó, 1= 20%, 2= 40%, 3= 60%, 4= 80%, 5 = 100%, 6= error
{
	switch (usb_sta)
	{
		case 0: LCD_ShowString( 340,  350,   BLACK,  Back_color, "             ");	break;
		case 1: LCD_ShowString( 340,  350,   BLACK,  Back_color, " 20% Process ");	break;
		case 2: LCD_ShowString( 340,  350,   BLACK,  Back_color, " 40% Process ");	break;
		case 3: LCD_ShowString( 340,  350,   BLACK,  Back_color, " 60% Process ");	break;
		case 4: LCD_ShowString( 340,  350,   BLACK,  Back_color, " 80% Process ");	break;
		case 5: LCD_ShowString( 340,  350,   BLACK,  Back_color, "100% Complete");	break;
		case 6: LCD_ShowString( 340,  350,   RED, 	 Back_color, "   USB Error ");	break;
	}
}


void lcd_log_data(void)
{
uint8_t dis_data1[40];
uint8_t dis_data2[60];

	font_size_select(0x00);

  Main_log_Read(set_data_adr*10, 10, dis_data1, dis_data2);

	main_log_display(set_data_adr*10, dis_data1, dis_data2);
	
	LCD_check_on_off( 20,370,out_usb_mode[0]);
	LCD_check_on_off( 120,370,out_usb_mode[1]);
	LCD_check_on_off( 220,370,out_usb_mode[2]);
	LCD_check_on_off( 320,370,out_usb_mode[3]);
	LCD_check_on_off( 20, 410,out_usb_mode[4]);
	LCD_check_on_off( 220, 410,out_usb_mode[5]);

	usb_out_sta_display(usb_out_status);

}





void lcd_voc_dis(uint16_t dada_no, uint8_t *load1)
{
uint8_t i;

	font_size_select(0x00);

	for (i=0;i<10;i++)
	{
		LCD_ColorNumber		 (  40, 90+5+24*i,   BLACK,  Back_color,  ++dada_no, 2, ZERO_DISABLE);
	
		if ( load1[8*i+0] == 0x01)
		{
			LCD_ShowString_number( 100+11*0,  90+5+24*i,   BLACK,  Back_color, "%%/",    load1[8*i+1], 2);
			LCD_ShowString_number( 100+11*3,  90+5+24*i,   BLACK,  Back_color, "%%/",    load1[8*i+2], 2);
			LCD_ShowString_number( 100+11*6,  90+5+24*i,   BLACK,  Back_color, "%% ",    load1[8*i+3], 2);
			LCD_ShowString_number( 100+11*9,  90+5+24*i,   BLACK,  Back_color, "%%:",    load1[8*i+4], 2);
			LCD_ShowString_number( 100+11*12, 90+5+24*i,   BLACK,  Back_color, "%%:",    load1[8*i+5], 2);
			LCD_ShowString_number( 100+11*15, 90+5+24*i,   BLACK,  Back_color, "%%",     load1[8*i+6], 2);

			LCD_ShowString_number( 320, 90+5+24*i,   BLACK,  Back_color, "%Month", 	load1[8*i+7], 1);
		}
		else
		{
			LCD_ShowString( 100+11*0,  90+5+24*i,   BLACK,  Back_color, "                 ");
			LCD_ShowString( 320, 90+5+24*i,   BLACK,  Back_color, "      ");			
		}
	}

}


void lcd_voc_data(void)
{
uint8_t dis_data[80];
	
	font_size_select(0x00);
	
	LCD_ColorNumber( 165+14,  340+48,   BLACK,  Back_color,  voc_valid_time, 1, ZERO_ENABLE);
	LCD_ColorNumber( 305+14,  340+48,   BLACK,  Back_color,  change_alarm_time, 2, ZERO_DISABLE);
	LCD_ColorNumber( 490,  360,   BLACK,  Back_color,  24*30*voc_valid_time, 4, ZERO_DISABLE);
	voc_change_icon(change_sta);

	Voc_change_Read( set_data_adr*10, 10, dis_data);

	lcd_voc_dis( set_data_adr*10, dis_data);

}



/***********************************
	@brief£ºvoid byte_dot(uint16_t temp)
***********************************/
void lcd_setting2(void)
{
static uint8_t graph_dis_sta = 0, old_mode_val = 0, old_set_sub = 0;
/////////////////////////////////////////
	if (lcd_clear_flag !=2 )
	{
		LCD_Clear(Back_color);
		graph_dis_sta = 0x00;
	}
///////////////////////////////////////
	if ( mode_value != old_mode_val || sub_set_mode != old_set_sub )
	{
		graph_dis_sta = 0x00;
		set_dis_refresh = 0;		
		old_mode_val  = mode_value;
		old_set_sub = sub_set_mode;
	}

/////////////////////////////////////////
	font_size_select(0x01);
	
	if ( graph_dis_sta == 0x00 )
	{
		graph_dis_sta = 1;
		set_data_adr = 0;

		switch(mode_value)
		{
			case date_sel :
				lcd_timedate_first();
				break;

			case alarm_sel : 
				lcd_alarm_set_first();
				break;

			case log_sel : 
				lcd_log_first();
				break;
			case voc_sel : 
				lcd_voc_first();
				break;			
		}
	}
	
/////////////////////////////////////////
	if (set_dis_refresh ==0)
	{
		set_dis_refresh = 1;

		switch(mode_value)
		{
			case date_sel: 
				lcd_timedate_data();
				break;

			case alarm_sel : 
				lcd_alarm_set_data();
				break;

			case log_sel : 
				lcd_log_data();
				set_dis_refresh = 0;
				break;

			case voc_sel : 
				lcd_voc_data();
				break;
		}
	}
/////////////////////////////////////////				
}







/*****************************************************************************************************************/
/*****************************************************************************************************************/
/*****************************************************************************************************************/



void LCD_Passstar(uint16_t x, uint16_t y, uint16_t pencolor, uint16_t backcolor)
{
  u16 temp;
  u16 pos;
  u8 t;

	LCD_WindowMax (x, y, x+48-1, y+40-1);

	pn_color = pencolor ; 
	bg_color = backcolor;

	for(pos=0; pos<120; pos++)
	{
		temp=pass_star[pos];

		for(t=0; t<16; t++)
		{                 
			if(temp&0x8000)	LCD_WriteData(pn_color); 
			else 			LCD_WriteData(bg_color);
			temp<<=1; 
		}
	}
}


void pass_star_check(uint16_t x, uint16_t y, uint16_t pencolor, uint16_t backcolor,uint16_t star_sta)
{
	LCD_Box(x-4, y-12, x+48+4, y+40+12, LGRAY);
	
	if (star_sta == 0x00)
	{
		LCD_Fill( x, y, x+48-1, y+40-1, Back_color );
	}
	else 
	{
		LCD_Passstar(x,  y,  BLACK,   Back_color);
	}
}



void pass_display(uint16_t star_cnt)
{
uint16_t tta_0=0, tta_1=0, tta_2=0, tta_3=0;

	switch(star_cnt)
	{
	case 0: 
		tta_0 = 0; tta_1 = 0; tta_2 = 0; tta_3 = 0;
		break;
	case 1: 
		tta_0 = 1; tta_1 = 0; tta_2 = 0; tta_3 = 0;
		break;
	case 2: 
		tta_0 = 1; tta_1 = 1; tta_2 = 0; tta_3 = 0;
		break;
	case 3: 
		tta_0 = 1; tta_1 = 1; tta_2 = 1; tta_3 = 0;
		break;
	case 4: 
		tta_0 = 1; tta_1 = 1; tta_2 = 1; tta_3 = 1;
		break;
	}
	pass_star_check(160,  100,  BLACK,   Back_color, tta_0);
	pass_star_check(220,  100,  BLACK,   Back_color, tta_1);
	pass_star_check(280,  100,  BLACK,   Back_color, tta_2);
	pass_star_check(340,  100,  BLACK,   Back_color, tta_3);
}



void password_first(void)
{
	font_size_select(0x01);

	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();
	
	set_data_temp = 0;
	set_data_temp2 = 0;

	font_size_select(0x01);
	pass_display(set_data_temp);

	LCD_small_icon2(160, 160, '1');
	LCD_small_icon2(240, 160, '2');
	LCD_small_icon2(320, 160, '3');

	LCD_small_icon2(160, 240, '4');
	LCD_small_icon2(240, 240, '5');
	LCD_small_icon2(320, 240, '6');

	LCD_small_icon2(160, 320, '7');
	LCD_small_icon2(240, 320, '8');
	LCD_small_icon2(320, 320, '9');

	LCD_small_icon2(160, 400, 'C');
	LCD_small_icon2(240, 400, '0');
	LCD_small_icon2(320, 400, '$');


}


void password_data(void)
{
	pass_display(set_data_temp);
//--------------------------------------------------------------
	switch(set_data_temp2)
	{
		case 0 :
			LCD_ShowString( 120,  30,   BLACK,  Back_color, "                ");
			LCD_ShowString( 120,  60,   BLACK,  Back_color, "                ");
			LCD_small_off_icon_string( 420, 400, BLACK, "Change");			//CLEAR BUTTON
			break;

		case 1 :
			LCD_ShowString( 120,  30,   RED,    Back_color, "Passward Change ");
			LCD_ShowString( 120,  60,   BLACK,  Back_color, "Current Passward");
			LCD_small_off_icon_string( 420, 400, BLACK, "Cancle");			//CLEAR BUTTON
			break;

		case 2 :
			LCD_ShowString( 120,  30,   RED,    Back_color, "Passward Change ");
			LCD_ShowString( 120,  60,   BLACK,  Back_color, "  New Passward  ");
			LCD_small_off_icon_string( 420, 400, BLACK, "Cancle");			//CLEAR BUTTON
			break;

		case 3 :
			LCD_ShowString( 120,  30,   RED,    Back_color, "Passward Change ");
			LCD_ShowString( 120,  60,   BLACK,  Back_color, "   Complete     ");
			LCD_small_off_icon_string( 420, 400, BLACK, "Cancle");			//CLEAR BUTTON
			break;
	}
//--------------------------------------------------------------
}











void  set_sel_icon2(uint16_t x1, uint16_t y1, uint8_t *string, uint8_t status)
{
	if (status == 0)
	{
		LCD_select2_icon(x1, y1);
		LCD_ShowString2 (x1+6,  y1+6,   BLACK,  select0_2+6, string);
	}
	else
	{
		LCD_select3_icon(x1, y1);
		LCD_ShowString2 (x1+6,  y1+6,   BLACK,  select1_2+6, string);
	}
}



void  LCD_select_icon3(uint8_t mode_sel)
{
uint8_t sel_mode_a=0, sel_mode_b=0, sel_mode_c=0, sel_mode_d=0, sel_mode_e=0, sel_mode_f=0 ,sel_mode_g=0 ,sel_mode_h=0 ;

	font_size_select(0x00);

	switch(mode_sel)
	{
		case new_co2_sel:	 sel_mode_a=1;
				break;
		case new_o2_sel:	 sel_mode_b=1;
				break;
		case work_time_sel:	 sel_mode_c=1;
				break;
		case pressure_set:	 sel_mode_d=1;
				break;
		case wet_mode_set:	 sel_mode_e=1;
				break;
		case cham_no_set:	 sel_mode_f=1;
				break;
		case ext_temp_cal:	 sel_mode_g=1;
				break;
		case pressure2_set:	 sel_mode_h=1;
				break;
		
	}

	set_sel_icon2(  5, 5, " CO2 set ", sel_mode_a);
	set_sel_icon2(145, 5, "   O2 set", sel_mode_b);
	set_sel_icon2(285, 5, "WorkTime ", sel_mode_c);
	set_sel_icon2(425, 5, "Pressure ", sel_mode_d);
	set_sel_icon2(  5,40, "WET Mode ", sel_mode_e);
	set_sel_icon2(145,40, "Chamber no", sel_mode_f);
	set_sel_icon2(285,40, "Ex temp cal", sel_mode_g);
	set_sel_icon2(425, 40, "Pressure2", sel_mode_h);
	
	font_size_select(0x01);
}




void new_co2_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();

	LCD_arrow_up_icon(240, 180);	
	LCD_arrow_dw_icon(240, 245);

	LCD_data_save_icon(380,170);

	LCD_control_icon(30,205);
	LCD_small_off_icon_string (50,400,BLUE, " Test");

	set_data_temp = co2_set_data;
	LCD_select_icon3(new_co2_sel);
	LCD_ShowString2( 40+5,   205+15,  BLACK,   alam_icon_2+15,  "CO2    %" );
	font_size_select(0x01);

}






void new_o2_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();

	LCD_arrow_up_icon(240, 180);	
	LCD_arrow_dw_icon(240, 245);

	LCD_data_save_icon(380,170);

	LCD_control_icon(30,205);
	LCD_small_off_icon_string (50,400,BLUE, " Test");

	set_data_temp = o2_set_data;
	LCD_select_icon3(new_o2_sel);
	LCD_ShowString2( 40+5,   205+15,  BLACK,   alam_icon_2+15,  " O2    %" );
	font_size_select(0x01);


}



void work_time_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();	

	LCD_arrow_up_icon(240,  85);
	LCD_arrow_dw_icon(240, 150);

	LCD_arrow_up_icon(240, 225);	
	LCD_arrow_dw_icon(240, 290);

	LCD_arrow_up_icon(240, 365);	
	LCD_arrow_dw_icon(240, 430);

	LCD_data_save_icon(380,230);

	LCD_select_icon3(work_time_sel);

	font_size_select(0x01);

	LCD_control_icon(30,110);
	LCD_ShowString( 30,  80,   BLACK,  Back_color, "Initial Time");
	LCD_ShowString2( 30+5,   110+15,  BLACK,   alam_icon_2+15,  "    00S" );

	LCD_control_icon(30,250);
	LCD_ShowString( 30,  220,   BLACK,  Back_color, "Period Time");
	LCD_ShowString2( 30+5,   250+15,  BLACK,   alam_icon_2+15,  "   000S" );

	LCD_control_icon(30,390);
	LCD_ShowString( 30,  360,   BLACK,  Back_color, "Working Time");
	LCD_ShowString2( 30+5,   390+15,  BLACK,   alam_icon_2+15,  "    00S" );

	set_data_temp = gas_initial_time;
	set_data_temp2 = gas_period_time;
	set_data_temp3 = gas_work_time;


}


void pressure_set_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();	


	font_size_select(0x00);

		if(mode_value == pressure_set)
		{
			LCD_select_icon3(pressure_set);

			set_data_temp = Press_on_data;
			set_data_temp2 = Press_off_data;
			set_data_temp3 = Press_err_data;
			set_data_temp4 = Press_err_cnt;
				font_size_select(0x00);
			LCD_ShowString_number( 380, 90,   BLACK,  Back_color, "Press1  .   bar", press_data1, 3);
		}
		else
		{
			LCD_select_icon3(pressure2_set);

			set_data_temp = Press2_on_data;
			set_data_temp2 = Press2_off_data;
			set_data_temp3 = Press2_err_data;
			set_data_temp4 = Press2_err_cnt;
				font_size_select(0x00);
			LCD_ShowString_number( 380, 90,   BLACK,  Back_color, "Press2  .   bar", press_data2, 3);	
		
		}
	//---------------------------------------------------
	LCD_data_save_icon(420, 220);

	font_size_select(0x01);
//---------------------------------------------------
	LCD_ShowString( 30,  80,   BLACK,  Back_color, "Max Pressure");

	LCD_arrow_up_icon(80, 120);
	LCD_arrow_dw_icon(80, 220);	

	LCD_arrow_up_icon(290, 120);
	LCD_arrow_dw_icon(290, 220);

	LCD_control_icon(30,160);
	LCD_ShowString2( 30+5,   160+15,  BLACK,   alam_icon_2+15,  "  000bar" );
	LCD_control_icon(240,160);
	LCD_ShowString2( 240+5,   160+15,  BLACK,   alam_icon_2+15,  "  000bar" );
//---------------------------------------------------
	LCD_ShowString( 30,  280,   BLACK,  Back_color, "Min Pressure");

	LCD_arrow_up_icon(80, 320);
	LCD_arrow_dw_icon(80, 420);	

	LCD_arrow_up_icon(290, 320);
	LCD_arrow_dw_icon(290, 420);

	LCD_control_icon(30,360);
	LCD_ShowString2( 30+5,   360+15,  BLACK,   alam_icon_2+15,  "  000bar" );
	LCD_control_icon(240,360);
	LCD_ShowString2( 240+5,   360+15,  BLACK,   alam_icon_2+15,  "   00sec" );
//---------------------------------------------------
}









void wet_mode_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();

	LCD_arrow_up_icon(240, 180);
	LCD_arrow_dw_icon(240, 245);

	LCD_data_save_icon(380,170);

	LCD_control_icon(30,205);

	set_data_temp = wet_mode_temp;

	LCD_select_icon3(wet_mode_set);

	font_size_select(0x01);
	LCD_ShowString2( 45,   205+15,  BLACK,   alam_icon_2+15,  " WET --#" );
}





void chameber_no_set_dis(uint16_t x1, uint16_t y1, uint8_t set_no)
{
	LCD_air_icon_a(100,170);

	font_size_select(0x00);
	LCD_ShowString2( x1+20,   y1+65,  BLACK,   air_icon_2+65,  "Chamber" );
}




void chameber_no_set_data(uint16_t x1, uint16_t y1, uint8_t set_no)
{
	font_size_select(0x01);

	if ( set_no == 0)		LCD_ColorNumber2( x1+45,  y1+30,  BLACK,   air_icon_2+30, 8, 1, ZERO_DISABLE);	
	else	LCD_ColorNumber2( x1+45,  y1+30,  BLACK,   air_icon_2+30, 6, 1, ZERO_DISABLE);	

}


void cham_no_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();

	LCD_data_save_icon(380,170);

	set_data_temp = chamber_count;

	chameber_no_set_dis(100,170,set_data_temp);

	LCD_select_icon3(cham_no_set);
}



void wet_mode_data(void)
{
	LCD_ColorNumber2( 125,   205+15,  BLACK,   alam_icon_2+15,  set_data_temp, 2, ZERO_ENABLE);
	LCD_Fill( 125+14,  205+15+18,  125+14+1,   205+15+18+1,  BLACK );
}



void cham_no_data(void)
{
	chameber_no_set_data(100,170,set_data_temp);

}



void ext_temp_cal_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	

	LCD_left_data();
	LCD_select_icon3(ext_temp_cal);

	LCD_data_inc_icon( 250,150);	
	LCD_data_dec_icon( 250,280);
	LCD_data_save_icon(420,200);

	set_data_temp = ext_set_data;
	setting_data_page = 0;

	LCD_Fill( 260,  210,  340,   210+2,  0xf369 );
	LCD_Fill( 260,  270,  340,   270+2,  0xf369 );
	LCD_Fill( 260,  210,  260+2,   270,  0xf369 );
	LCD_Fill( 340,  210,  340+2,   270,  0xf369 );
}


void ext_temp_cal_data(void)
{
static uint8_t period = 20;

	set_dis_refresh = 0;

	selsect_cal_mode(setting_data_page);

	font_size_select(0x01);
	switch (cali_set)
	{
		case 0 : select_cal_data(setting_data_page); 	break;						//0 = ¹ÌÁøÀÔ, 1 = µ¥ÀÌÅ¸ ¼öÁý Áß, 2 = µ¥ÀÌÅ¸ Á¤»ó 3 = µ¥ÀÌÅ¸ ¿À·ù
		case 1 : LCD_ShowString ( 280,   230,  RED,   Back_color,  "***");		break;	//
		case 2 : LCD_ShowString ( 280,   230,  RED,   Back_color,  "O K"); 	break;	//
		case 3 : LCD_ShowString ( 280,   230,  RED,   Back_color,  "ERR"); 	break;	//
	}


	if (--period ==0)
	{
		period= 20;
		cali_set = 0;
	}
}





void new_co2_data(void)
{
	LCD_ColorNumber2( 104+5,   205+15,  BLACK,   alam_icon_2+15,  set_data_temp, 3, ZERO_DISABLE);
	LCD_Fill( 104+5+16+14,  205+15+18,  104+5+16+14+1,   205+15+18+1,  BLACK );
}


void new_o2_data(void)
{
	LCD_ColorNumber2( 104+5,   205+15,  BLACK,   alam_icon_2+15,  set_data_temp, 3, ZERO_DISABLE);
	LCD_Fill( 104+5+16+14,  205+15+18,  104+5+16+14+1,   205+15+18+1,  BLACK );
}


void work_time_data(void)
{
	LCD_ColorNumber2( 30+5+16*4,   110+15,  BLACK,   alam_icon_2+15,  set_data_temp, 2, ZERO_ENABLE);

	LCD_ColorNumber2( 30+5+16*3,   250+15,  BLACK,   alam_icon_2+15,  set_data_temp2, 3, ZERO_DISABLE);

	LCD_ColorNumber2( 30+5+16*4,   390+15,  BLACK,   alam_icon_2+15,  set_data_temp3, 2, ZERO_ENABLE);
	LCD_Fill( 30+5+16*4+14,  390+15+18,  30+5+16*4+14+1,   390+15+18+1,  BLACK );

}



void pressure_set_data(void)
{

	font_size_select(0x00);

		if(mode_value == pressure_set)
		{
				LCD_ShowString_number( 380+77, 90,   BLACK,  Back_color, "%.%%", press_data1, 3);
		}
		else
		{
				LCD_ShowString_number( 380+77, 90,   BLACK,  Back_color, "%.%%", press_data2, 3);
		}
	

	font_size_select(0x01);

	LCD_ColorNumber2( 30+5+16*3,   160+15,  BLACK,   alam_icon_2+15,  set_data_temp, 2, ZERO_ENABLE);
	LCD_Fill( 30+5+16*2+14, 160+15+18,  30+5+16*2+14+1,   160+15+18+1,  BLACK );

	LCD_ColorNumber2( 240+5+16*3,   160+15,  BLACK,   alam_icon_2+15,  set_data_temp2, 2, ZERO_ENABLE);
	LCD_Fill( 240+5+16*2+14,  160+15+18,  240+5+16*2+14+1,   160+15+18+1,  BLACK );

	LCD_ColorNumber2( 30+5+16*3,   360+15,  BLACK,   alam_icon_2+15,  set_data_temp3, 2, ZERO_ENABLE);
	LCD_Fill( 30+5+16*2+14, 360+15+18,  30+5+16*2+14+1,   360+15+18+1,  BLACK );

	LCD_ColorNumber2( 240+5+16*3,   360+15,  BLACK,   alam_icon_2+15,  set_data_temp4, 2, ZERO_DISABLE);

}




/***********************************
	@brief£ºvoid byte_dot(uint16_t temp)
***********************************/
void lcd_setting3(void)
{
static uint8_t graph_dis_sta = 0, old_mode_val = 0, old_set_sub = 0;
/////////////////////////////////////////
	if (lcd_clear_flag !=3 )
	{
		LCD_Clear(Back_color);
		graph_dis_sta = 0x00;
	}
///////////////////////////////////////
	if ( mode_value != old_mode_val || sub_set_mode != old_set_sub )
	{
		graph_dis_sta = 0x00;
		set_dis_refresh = 0;		
		old_mode_val  = mode_value;
		old_set_sub = sub_set_mode;
	}
/////////////////////////////////////////
	font_size_select(0x01);
	
	if ( graph_dis_sta == 0x00 )
	{
		graph_dis_sta = 1;
		set_data_adr = 0;

		switch(mode_value)
		{
			case new_co2_sel :
				new_co2_first();
				break;
			case new_o2_sel : 
				new_o2_first();
				break;
			case work_time_sel : 
				work_time_first();
				break;
			case pressure_set : 
				pressure_set_first();
				break;
			case wet_mode_set : 
				wet_mode_first();
				break;
			case cham_no_set : 
				cham_no_first();
				break;
			case ext_temp_cal : 
				ext_temp_cal_first();
				break;
			case pressure2_set : 
				pressure_set_first();
				break;
			case password_set :
				password_first();
				break;		
		}
	}
	
/////////////////////////////////////////
	if (set_dis_refresh ==0)
	{
		set_dis_refresh = 1;

		switch(mode_value)
		{
			case new_co2_sel: 
				new_co2_data();
				break;
			case new_o2_sel : 
				new_o2_data();
				break;
			case work_time_sel : 
				work_time_data();
				break;
			case pressure_set : 
				pressure_set_data();
				set_dis_refresh = 0;
				break;
			case wet_mode_set : 
				wet_mode_data();
				break;
			case cham_no_set : 
				cham_no_data();
				break;
			case ext_temp_cal : 
				ext_temp_cal_data();
				break;
			case pressure2_set : 
				pressure_set_data();
				set_dis_refresh = 0;
				break;
			case password_set :
				password_data();
				break;		
		}
	}
}





void Set_log_body2(void)
{
	font_size_select(0x00);

	LCD_Fill(  40,  70,   41, 378,  BLACK);	
	LCD_Fill(  240, 70,  241, 378,  BLACK);	
	LCD_Fill(  440, 70,  441, 378,  BLACK);	

	LCD_Fill(  20, 90,   	 550, 90+24*0+1,  BLACK);	
	LCD_Fill(  20, 90+24*1,  550, 90+24*1+1,  BLACK);	
	LCD_Fill(  20, 90+24*2,  550, 90+24*2+1,  BLACK);	
	LCD_Fill(  20, 90+24*3,  550, 90+24*3+1,  BLACK);	
	LCD_Fill(  20, 90+24*4,  550, 90+24*4+1,  BLACK);	
	LCD_Fill(  20, 90+24*5,  550, 90+24*5+1,  BLACK);	
	LCD_Fill(  20, 90+24*6,  550, 90+24*6+1,  BLACK);	
	LCD_Fill(  20, 90+24*7,  550, 90+24*7+1,  BLACK);	
	LCD_Fill(  20, 90+24*8,  550, 90+24*8+1,  BLACK);	
	LCD_Fill(  20, 90+24*9,  550, 90+24*9+1,  BLACK);	
	LCD_Fill(  20, 90+24*10, 550, 90+24*10+1,  BLACK);
	LCD_Fill(  20, 90+24*11, 550, 90+24*11+1,  BLACK);	
	LCD_Fill(  20, 90+24*12, 550, 90+24*12+1,  BLACK);	

	LCD_ShowString( 10,    72,   BLACK,  Back_color,  "No");	
	LCD_ShowString( 100,   72,   BLACK,  Back_color,  "Date/time");
	LCD_ShowString( 285,   72,   BLACK,  Back_color,  "Cancle");
	LCD_ShowString( 460,   72,   BLACK,  Back_color,  "Alarm");

	LCD_data_lf_icon ( 135, 400);
	LCD_data_rt_icon ( 330, 400);

	LCD_small_off_icon_string( 40, 400, BLACK, "CLEAR");	//CLEAR BUTTON
}



void alarm_list_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();

	Set_log_body2();
}









u32 mypow2(u8 m,u8 n)
{
u32 result=1;	 

	while(n--)result*=m;    
	return result;
}	


void LCD_alarm_load(uint8_t *string, uint16_t number, uint8_t len, uint8_t *string2)
{
uint8_t chat;
uint8_t t=0,temp;

	while(*string != '\0')
	{
		chat = *string;
		string++;

		if (chat == '%')
		{
			temp=(number/mypow2(10,len-t-1))%10;
			t++;
			*(string2++) = temp+'0';//temp+'0'
		}

	    else
		{
			*(string2++) = chat;	//chat
		}
	}
}


void LCD_ShowString_buf(uint16_t x, uint16_t y, uint8_t *string, uint16_t number)
{
uint8_t i;
uint8_t *buf;

	buf = string + number;

	for (i =0; i<11; i++)
	{
		LCD_ShowChar(x, y, BLACK,  Back_color, *(buf++) );		
		x += font_size_x;
	}
}


//	alarm_line_dis( set_data_adr*12, alarm_num, alarm_ont, alarm_off, list_d);
void alarm_line_dis(uint16_t data_no, uint8_t *load1, uint8_t *load2, uint8_t *load3, uint8_t first_dat)
{
uint16_t line_d;
uint16_t err_d;
uint16_t flag_d;
uint16_t sub_err;
uint16_t line;
uint8_t	string_c[30];
static uint8_t move_cnt = 0, move_spd = 10;

	font_size_select(0x00);

	if (--move_spd == 0)	{ move_spd= 3;	 if (++move_cnt == 18)	{move_spd = 10; move_cnt = 0;}}
	
	for(line = 0; line<12; line++)
	{
		line_d = 96+24*line;

		flag_d =  *load1++;
		err_d =   *load1++;
		sub_err = *load1++;

		LCD_ColorNumber( 15, line_d,    BLACK,  Back_color,  ++data_no, 2, ZERO_DISABLE);

		if ( flag_d == 0x01 )
		{
			LCD_ShowString_number(  45+11*0,  line_d,    BLACK,  Back_color,	"%%/",  	*load2++, 2);
			LCD_ShowString_number(  45+11*3,  line_d,    BLACK,  Back_color, 	"%%/",  	*load2++, 2);
			LCD_ShowString_number(  45+11*6,  line_d,    BLACK,  Back_color,	"%% ",  	*load2++, 2);
			LCD_ShowString_number(  45+11*9,  line_d,    BLACK,  Back_color,	"%%:",  	*load2++, 2);
			LCD_ShowString_number(  45+11*12, line_d,    BLACK,  Back_color,	"%%:",  	*load2++, 2);
			LCD_ShowString_number(  45+11*15, line_d,    BLACK,  Back_color,	"%%",  	 	*load2++, 2);

			LCD_ShowString_number( 245+11*0,  line_d,    BLACK,  Back_color,	"%%/",  	*load3++, 2);
			LCD_ShowString_number( 245+11*3,  line_d,    BLACK,  Back_color, 	"%%/",  	*load3++, 2);
			LCD_ShowString_number( 245+11*6,  line_d,    BLACK,  Back_color,	"%% ",  	*load3++, 2);
			LCD_ShowString_number( 245+11*9,  line_d,    BLACK,  Back_color,	"%%:",  	*load3++, 2);
			LCD_ShowString_number( 245+11*12, line_d,    BLACK,  Back_color,	"%%:",  	*load3++, 2);
			LCD_ShowString_number( 245+11*15, line_d,    BLACK,  Back_color,	"%%", 	 	*load3++, 2);

			switch( err_d )
			{
				case 1 : 
						LCD_alarm_load("CHAMBER% DOOR OPEN!         ", sub_err+1,1, string_c);
						LCD_ShowString_buf(443,  line_d, string_c, move_cnt);
						break;
				case 2 : 
						LCD_alarm_load("CHAMBER% TOP-HEATER HI ERR  ", sub_err+1,1, string_c);	//EEROR 2
						LCD_ShowString_buf(443,  line_d, string_c, move_cnt);
						break;
				case 3 : 
						LCD_alarm_load("CHAMBER% BOT-HEATER HI ERR  ", sub_err+1,1, string_c);	//EEROR 3
						LCD_ShowString_buf(443,  line_d, string_c, move_cnt);
						break;
				case 4 : 
						LCD_alarm_load("CHAMBER% TOP-HEATER LO ERR  ", sub_err+1,1, string_c);	//EEROR 4
						LCD_ShowString_buf(443,  line_d, string_c, move_cnt);
						break;
				case 5 : 
						LCD_alarm_load("CHAMBER% BOT-HEATER LO ERR  ", sub_err+1,1, string_c);	//EEROR 5
						LCD_ShowString_buf(443,  line_d, string_c, move_cnt);
						break;	
				case 6 : 
						LCD_alarm_load("CHAMBER% TOPTMP SENSOR ERR  ", sub_err+1,1, string_c);	//EEROR 6
						LCD_ShowString_buf(443,  line_d, string_c, move_cnt);
						break;
				case 7 : 
						LCD_alarm_load("CHAMBER% BOTTMP SENSOR ERR  ", sub_err+1,1, string_c);	//EEROR 7
						LCD_ShowString_buf(443,  line_d, string_c, move_cnt);
						break;
				case 8 : 
						LCD_alarm_load("GAS% SUPPLY PRESSURE ERROR  ", sub_err+1,1, string_c);	//EEROR 8
						LCD_ShowString_buf(443,  line_d, string_c, move_cnt);
						break;
				case 9 : 
						LCD_alarm_load("GAS% SOL VALVE ERROR        ", sub_err+1,1, string_c);	//EEROR 9
						LCD_ShowString_buf(443,  line_d, string_c, move_cnt);
						break;
				case 10: 
						LCD_alarm_load("VOC FILTER REMAIN DAY %%    ", sub_err+1,2, string_c);	//EEROR 10
						LCD_ShowString_buf(443,  line_d, string_c, move_cnt);
						break;
				case 11: 
						LCD_alarm_load("POWER OFF STRANGE ERROR     ", sub_err+1,1, string_c);	//EEROR 11
						LCD_ShowString_buf(443,  line_d, string_c, move_cnt);
						break;
			}
		}
		else
		{
			LCD_ShowString(  45+11*0,  line_d,    BLACK,  Back_color,	"                 " );
			LCD_ShowString( 245+11*0,  line_d,    BLACK,  Back_color,	"                 " );
			LCD_ShowString( 443,  line_d,    BLACK,  Back_color,	"           " );		
		}
	}
}


void alarm_list_data(uint8_t list_d)
{
uint8_t alarm_num[36];
uint8_t alarm_ont[72];
uint8_t alarm_off[72];

  Alarm_log_Read(set_data_adr*12, 12,  alarm_num, alarm_ont, alarm_off); 
	alarm_line_dis( set_data_adr*12, alarm_num, alarm_ont, alarm_off, list_d);

}





/***********************************
	@brief£ºvoid byte_dot(uint16_t temp)
***********************************/
void lcd_setting4(void)
{
static uint8_t graph_dis_sta = 0, old_mode_val = 0, old_set_sub = 0, alarm_sta = 0;
/////////////////////////////////////////
	if (lcd_clear_flag !=4 )
	{
		LCD_Clear(Back_color);
		graph_dis_sta = 0x00;
	}
///////////////////////////////////////
	if ( mode_value != old_mode_val || sub_set_mode != old_set_sub )
	{
		graph_dis_sta = 0x00;
		set_dis_refresh = 0;		
		old_mode_val  = mode_value;
		old_set_sub = sub_set_mode;
	}
/////////////////////////////////////////
	font_size_select(0x01);
	
	if ( graph_dis_sta == 0x00 )
	{
		graph_dis_sta = 1;
		set_data_adr = 0;
		alarm_sta = 0;

		switch(mode_value)
		{
			case alarm_list :
				alarm_list_first();
				break;	
		}
	}
	
/////////////////////////////////////////
	if (set_dis_refresh ==0)
	{
		set_dis_refresh = 1;

		switch(mode_value)
		{
			case alarm_list: 
				alarm_sta = 0;
				alarm_list_data(alarm_sta);
				break;
		}
	}
	else
	{
		switch(mode_value)
		{
			case alarm_list: 
				alarm_sta = 1;
				alarm_list_data(alarm_sta);
				break;
		}
	}
/////////////////////////////////////////				
}










void LCD_KEYPAD(void)
{
	font_size_select(0x01);

	LCD_small_icon3( 20, 160, '0');
	LCD_small_icon3( 70, 160, '1');
	LCD_small_icon3(120, 160, '2');
	LCD_small_icon3(170, 160, '3');
	LCD_small_icon3(220, 160, '4');
	LCD_small_icon3(270, 160, '5');
	LCD_small_icon3(320, 160, '6');
	LCD_small_icon3(370, 160, '7');
	LCD_small_icon3(420, 160, '8');
	LCD_small_icon3(470, 160, '9');

	LCD_small_icon3( 20, 240, 'A');
	LCD_small_icon3( 70, 240, 'B');
	LCD_small_icon3(120, 240, 'C');
	LCD_small_icon3(170, 240, 'D');
	LCD_small_icon3(220, 240, 'E');
	LCD_small_icon3(270, 240, 'F');
	LCD_small_icon3(320, 240, 'G');
	LCD_small_icon3(370, 240, 'H');
	LCD_small_icon3(420, 240, 'I');
	LCD_small_icon3(470, 240, 'J');

	LCD_small_icon3( 20, 320, 'K');
	LCD_small_icon3( 70, 320, 'L');
	LCD_small_icon3(120, 320, 'M');
	LCD_small_icon3(170, 320, 'N');
	LCD_small_icon3(220, 320, 'O');
	LCD_small_icon3(270, 320, 'P');
	LCD_small_icon3(320, 320, 'Q');
	LCD_small_icon3(370, 320, 'R');
	LCD_small_icon3(420, 320, 'S');
	LCD_small_icon3(470, 320, 'T');

	LCD_small_icon3( 20, 400, 'U');
	LCD_small_icon3( 70, 400, 'V');
	LCD_small_icon3(120, 400, 'W');
	LCD_small_icon3(170, 400, 'X');
	LCD_small_icon3(220, 400, 'Y');
	LCD_small_icon3(270, 400, 'Z');
	LCD_small_icon3(320, 400, '$');
}





void device_set_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();

	font_size_select(0x00);

	LCD_ShowString(  50,  50,   BLACK,  Back_color,	"DEVICE INFOMATION" );
	LCD_ShowString(  50,  80,   BLACK,  Back_color,	"MODEL NAME :         " );
	LCD_ShowString(  50,  110,  BLACK,  Back_color,	"REVISION NO :" );
	LCD_ShowString(  50+11*14,  110,  BLACK,  Back_color,	revision_no );

	set_data_temp = 0;
	set_data_temp2 = 0;

	device_temp[0] = device_name[0];
	device_temp[1] = device_name[1];
	device_temp[2] = device_name[2];
	device_temp[3] = device_name[3];
}




void device_name_display(void)
{
	font_size_select(0x00);

	LCD_ShowChar(  50+11*13,  80,   BLACK,  Back_color,	device_temp[0] );
	LCD_ShowChar(  50+11*14,  80,   BLACK,  Back_color,	device_temp[1] );
	LCD_ShowChar(  50+11*15,  80,   BLACK,  Back_color,	device_temp[2] );
	LCD_ShowChar(  50+11*16,  80,   BLACK,  Back_color,	device_temp[3] );

}



void device_set_data(void)
{
	device_name_display();

	if ( set_data_temp == 0)
	{
		LCD_small_off_icon_string( 350, 80, BLACK, "Change");			//CLEAR BUTTON
		LCD_Fill( 450,  80,  530,  140,  Back_color );	
		LCD_Fill( 20,  160,  510,  460,  Back_color );	
		set_data_temp2 = 0;
	}
	else if ( set_data_temp == 1)
	{
		LCD_small_off_icon_string( 350, 80, BLACK, "Cancle");			//CLEAR BUTTON
		LCD_Fill( 450,  80,  530,  140,  Back_color );	
		LCD_KEYPAD();
	}

	else if ( set_data_temp == 2)
	{
		LCD_small_off_icon_string( 450, 80, BLACK, " Save ");
	}

}



/***********************************
	@brief£ºvoid byte_dot(uint16_t temp)
***********************************/
void lcd_setting5(void)
{
static uint8_t graph_dis_sta = 0, old_mode_val = 0, old_set_sub = 0;
/////////////////////////////////////////
	if (lcd_clear_flag !=5 )
	{
		LCD_Clear(Back_color);
		graph_dis_sta = 0x00;
	}
///////////////////////////////////////
	if ( mode_value != old_mode_val || sub_set_mode != old_set_sub )
	{
		graph_dis_sta = 0x00;
		set_dis_refresh = 0;		
		old_mode_val  = mode_value;
		old_set_sub = sub_set_mode;
	}
/////////////////////////////////////////
	font_size_select(0x01);

	if ( graph_dis_sta == 0x00 )
	{
		graph_dis_sta = 1;
		set_data_adr = 0;

		switch(mode_value)
		{
			case device_set :
				device_set_first();
				break;	
		}
	}
	
/////////////////////////////////////////
	if (set_dis_refresh ==0)
	{
		set_dis_refresh = 1;

		switch(mode_value)
		{
			case device_set: 
				device_set_data();
				break;
		}
	}
/////////////////////////////////////////				
}





void  gas1_sel_menu(uint16_t x1, uint16_t y1, uint8_t status)
{
	if(status == 0)
	{
		LCD_set_tab_off_icon(x1,y1);
		LCD_DrawPicture (   x1+38,   y1+18,  x1+38+51, y1+18+15, menu_1gas_off);	//90*20
	}
	else
	{
		LCD_set_tab_on_icon(x1,y1);
		LCD_DrawPicture (   x1+38,   y1+18,  x1+38+51, y1+18+15, menu_1gas_on);	//90*20
	}
}

void  gas2_sel_menu(uint16_t x1, uint16_t y1, uint8_t status)
{
	if(status == 0)
	{
		LCD_set_tab_off_icon(x1,y1);
		LCD_DrawPicture (   x1+38,   y1+18,  x1+38+51, y1+18+15, menu_2gas_off);	//90*20
	}
	else
	{
		LCD_set_tab_on_icon(x1,y1);
		LCD_DrawPicture (   x1+38,   y1+18,  x1+38+51, y1+18+15, menu_2gas_on);	//90*20
	}
}


void gas_sel_selector(uint8_t mode_sel)
{
	if (mode_sel==0)
	{
		gas1_sel_menu(20,10,1);
		gas2_sel_menu(180,10,0);
	}
	else
	{
		gas1_sel_menu(20,10,0);
		gas2_sel_menu(180,10,1);
	}
}


void gas2_sett_selector(uint8_t gas2_set)
{
	if (gas2_set==0)
	{
			LCD_ShowString(  350, 30,   BLACK,  Back_color,  " ON");	
			LCD_DrawPicture (   405,   28,  405+25, 28+25, dot_off);
			LCD_ShowString(     460, 30,   BLACK,  Back_color,  "OFF");	
			LCD_DrawPicture (   515, 28,  515+25, 28+25, dot_on);
	}
	else 
	{
		LCD_ShowString(  350, 30,   BLACK,  Back_color,  " ON");	
		LCD_DrawPicture (   405,   28,  405+25, 28+25, dot_on);
		LCD_ShowString(     460, 30,   BLACK,  Back_color,  "OFF");	
		LCD_DrawPicture (   515, 28,  515+25, 28+25, dot_off);
	}
}


void data_sett_num_display(uint16_t x1, uint16_t y1,uint16_t data, uint8_t dot_posit)
{
	switch(dot_posit)
	{
		case 0:
			LCD_ColorNumber ( x1+16+20,   y1+50+20,  RED,   Back_color,  data, 2, ZERO_ENABLE);
			LCD_Fill( x1+20+16+14,  y1+50+20+18,  x1+20+16+14+2,   y1+50+20+18+2,  RED );	
			break;

		case 1:
				LCD_ColorNumber ( x1+16+20,   y1+50+20,  RED,   Back_color,  data, 2, ZERO_DISABLE);
				break;
	
		case 2:
				LCD_ColorNumber ( x1+8+20,   y1+50+20,  RED,   Back_color,  data, 3, ZERO_ENABLE);
				LCD_Fill( x1+8+20+16+14,  y1+50+20+18,  x1+8+20+16+14+2,   y1+50+20+18+2,  RED );	
				break;
	
		case 3:
				LCD_ColorNumber ( x1+8+20,   y1+50+20,  RED,   Back_color,  data, 3, ZERO_DISABLE);
				break;
	}
}



void box_data(uint16_t x1, uint16_t y1)
{
	LCD_Fill( x1,  			y1,  		x1+60,   y1+2,  	0xf369 );
	LCD_Fill( x1,  			y1+50,  	x1+60,   y1+52,  	0xf369 );
	LCD_Fill( x1,  			y1,  		x1+2,   	y1+50,  	0xf369 );
	LCD_Fill( x1+60, 		y1, 		x1+62,  	y1+50, 	0xf369 );
}

void box_module(uint16_t x1, uint16_t y1,uint8_t *string1,uint8_t *string2)
{
	LCD_data_inc_icon( x1,y1);	
	LCD_data_dec_icon( x1,y1+110);
	box_data( x1+20,  y1+55);
	LCD_ShowString( x1-32, y1+70,   BLACK,  Back_color,  string1);	
	LCD_ShowString( x1+90, y1+70,   BLACK,  Back_color,  string2);	

}




void gas1_set_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();

	set_data_temp = co2_set_data;
	set_data_temp2 = o2_set_data;
	set_data_temp3 = gas_initial_time;
	set_data_temp4 = gas_period_time;
	set_data_temp5 = gas_work_time;
	set_data_temp6 = gas1_gas2_dual_time;
	set_data_temp7 = 0;

	font_size_select(0x01);

	box_module( 35,80, "CO2","%");
	box_module( 185,80, " O2","%");
	box_module( 335,80, "DL","M");

	box_module( 35,280, " IT","S");	
	box_module( 185,280, " PT","S");	
	box_module( 335,280, " WT","S");		

	gas_sel_selector(0);
	LCD_data_save_icon(450,210);

}



void gas1_set_data(void)
{
	font_size_select(0x01);
	data_sett_num_display(35, 80, set_data_temp, 0);			//co2
	data_sett_num_display(185, 80, set_data_temp2, 0);		//o2
	data_sett_num_display(335, 80, set_data_temp6, 1);		//

	data_sett_num_display(35, 280, set_data_temp3, 1);				//initiial
	data_sett_num_display(185, 280, set_data_temp4, 3);			//period time
	data_sett_num_display(335, 280, set_data_temp5, 0);			//work time
}



void gas2_set_first(void)
{
	LCD_Fill( 0,  0,  574,  479,  Back_color );	
	LCD_left_data();

	set_data_temp = co2_2nd_set_data;
	set_data_temp2 = o2_2nd_set_data;
	set_data_temp3 = gas2_initial_time;
	set_data_temp4 = gas2_period_time;
	set_data_temp5 = gas2_work_time;
	set_data_temp6 = gas2_start_time;
	set_data_temp7 = gas_2nd_on_off;
	
	font_size_select(0x01);

	box_module( 35,80, " ST","H");
	box_module( 185,80, "CO2","%");	
	box_module( 335,80, " O2","%");

	box_module( 35,280, " IT","S");	
	box_module( 185,280, " PT","S");	
	box_module( 335,280, " WT","S");		

	gas_sel_selector(1);
	gas2_sett_selector(set_data_temp7);	

	LCD_data_save_icon(450,210);

}


void gas2_set_data(void)
{
	font_size_select(0x01);
	data_sett_num_display(185, 80, set_data_temp, 0);			//co2
	data_sett_num_display(335, 80, set_data_temp2, 0);		//o2

	data_sett_num_display(35, 280, set_data_temp3, 1);				//initiial
	data_sett_num_display(185, 280, set_data_temp4, 3);			//period time
	data_sett_num_display(335, 280, set_data_temp5, 0);			//work time

	data_sett_num_display(35, 80, set_data_temp6, 1);		//start time
	gas2_sett_selector(set_data_temp7);	
}




/***********************************
	@brief£ºvoid byte_dot(uint16_t temp)
***********************************/
void lcd_setting6(void)
{
static uint8_t graph_dis_sta = 0, old_mode_val = 0, old_set_sub = 0;
/////////////////////////////////////////
	if (lcd_clear_flag !=6 )
	{
		LCD_Clear(Back_color);
		graph_dis_sta = 0x00;
	}
///////////////////////////////////////
	if ( mode_value != old_mode_val || sub_set_mode != old_set_sub )
	{
		graph_dis_sta = 0x00;
		set_dis_refresh = 0;		
		old_mode_val  = mode_value;
		old_set_sub = sub_set_mode;
	}
/////////////////////////////////////////
	font_size_select(0x01);

	if ( graph_dis_sta == 0x00 )
	{
		graph_dis_sta = 1;
		set_data_adr = 0;

		switch(mode_value)
		{
			case gas_1_setting :
				gas1_set_first();
				break;	
			case gas_2_setting :
				gas2_set_first();
				break;	
		}
	}
	
/////////////////////////////////////////
	if (set_dis_refresh ==0)
	{
		set_dis_refresh = 1;

		switch(mode_value)
		{
			case gas_1_setting :
				gas1_set_data();
				break;	
			case gas_2_setting :
				gas2_set_data();
				break;	
		}
	}
/////////////////////////////////////////				
}








/***********************************
	LCD_left_data
***********************************/
void LCD_left_data(void)
{
	font_size_select(0x01);

	LCD_set_mode_icon(590,90,0);
	LCD_set_mode_icon(690,90,1);
	LCD_set_mode_icon(590,180,2);
	LCD_set_mode_icon(690,180,3);

	LCD_cnc_icon(640,400);

	LCD_Fill( 575,  43,  576,  450,  0xc618);	

	font_size_select(0x00);
	LCD_ShowString( 575,  10,   BLACK,  Back_color, "20  .  .  .   :  :  ");
	LCD_ShowString( 664,  30,   BLACK,  Back_color, "VF:     Hr");
	LCD_ShowString( 664,  50,   BLACK,  Back_color, "Pr1:     bar");
	
	if ( gas_2nd_on_off==1 )	{			LCD_ShowString( 664,  70,   BLACK,  Back_color, "Pr2:     bar");	}
		
}





/***********************************
	@brief£ºvoid byte_dot(uint16_t temp)
***********************************/
void LCD_first(void)
{
	font_size_select(0x01);

	co2_work_val= 1;
	o2_work_val	=1;	
	
	if( chamber_count == 0)
	{
		LCD_cham_icon(10,50,0);
		LCD_cham_icon(150,50,1);
		LCD_cham_icon(290,50,2);
		LCD_cham_icon(430,50,3);
		LCD_cham_icon(10,190,4);
		LCD_cham_icon(150,190,5);
		LCD_cham_icon(290,190,6);
		LCD_cham_icon(430,190,7);
	}
	else
	{
		LCD_cham_icon(30,50,0);
		LCD_cham_icon(210,50,1);
		LCD_cham_icon(390,50,2);
		LCD_cham_icon(30,190,3);
		LCD_cham_icon(210,190,4);
		LCD_cham_icon(390,190,5);
	}

	LCD_left_data();
}








void Color_box(uint16_t x_axis, uint16_t y_axis, uint8_t color_sel )
{
uint16_t boxcolor = 0;

	switch(color_sel)
	{
		case 0:		boxcolor = WHITE;	break;
		case 1:		boxcolor = RED;		break;
		case 2:		boxcolor = GREEN;	break;
		case 3:		boxcolor = YELLOW;	break;
	}

	LCD_Fill(  x_axis,  y_axis,   x_axis+43,  y_axis+30,  boxcolor);	

}




void test_mode_nomal(void)
{
uint16_t i = 0;
	font_size_select(0x00);

	LCD_ShowString( 350,  5,   BLACK,  Back_color, "Test Mode");

	i = 24 ;

	LCD_Fill(  0,  i+50*0,   799, i+50*0,  BLACK);	
	LCD_Fill(  0,  i+50*1,   799, i+50*1,  BLACK);	
	LCD_Fill(  0,  i+50*2,   799, i+50*2,  BLACK);	
	LCD_Fill(  0,  i+50*3,   799, i+50*3,  BLACK);	
	LCD_Fill(  0,  i+50*4,   799, i+50*4,  BLACK);	
	LCD_Fill(  0,  i+50*5,   799, i+50*5,  BLACK);	
	LCD_Fill(  0,  i+50*6,   799, i+50*6,  BLACK);	
	LCD_Fill(  0,  i+50*7,   799, i+50*7,  BLACK);	
	LCD_Fill(  0,  i+50*8,   799, i+50*8,  BLACK);	
	LCD_Fill(  0,  i+50*9,   799, i+50*9,  BLACK);	
//----------------------------------------------------------------------

	LCD_ShowString( 0,  40+50*0,   GREEN,  Back_color, "ch n");
	LCD_ShowString( 0,  40+50*1,   BLACK,  Back_color, "ch 1");
	LCD_ShowString( 0,  40+50*2,   BLACK,  Back_color, "ch 2");
	LCD_ShowString( 0,  40+50*3,   BLACK,  Back_color, "ch 3");
	LCD_ShowString( 0,  40+50*4,   BLACK,  Back_color, "ch 4");
	LCD_ShowString( 0,  40+50*5,   BLACK,  Back_color, "ch 5");
	LCD_ShowString( 0,  40+50*6,   BLACK,  Back_color, "ch 6");
	LCD_ShowString( 0,  40+50*7,   BLACK,  Back_color, "ch 7");
	LCD_ShowString( 0,  40+50*8,   BLACK,  Back_color, "ch 8");
	LCD_Fill(  60,  24,   60,  474,  BLACK);	

//----------------------------------------------------------------------

	LCD_ShowString( 80,  35+50*0,   BROWN,  Back_color, "Top");
	LCD_ShowString( 80,  55+50*0,   BROWN,  Back_color, "Temp");

	LCD_ColorNumber( 80,  40+50*1,   BLACK,  Back_color, cham_top_temp[0], 3, ZERO_ENABLE);
	LCD_ColorNumber( 80,  40+50*2,   BLACK,  Back_color, cham_top_temp[1], 3, ZERO_ENABLE);
	LCD_ColorNumber( 80,  40+50*3,   BLACK,  Back_color, cham_top_temp[2], 3, ZERO_ENABLE);
	LCD_ColorNumber( 80,  40+50*4,   BLACK,  Back_color, cham_top_temp[3], 3, ZERO_ENABLE);
	LCD_ColorNumber( 80,  40+50*5,   BLACK,  Back_color, cham_top_temp[4], 3, ZERO_ENABLE);
	LCD_ColorNumber( 80,  40+50*6,   BLACK,  Back_color, cham_top_temp[5], 3, ZERO_ENABLE);
	LCD_ColorNumber( 80,  40+50*7,   BLACK,  Back_color, cham_top_temp[6], 3, ZERO_ENABLE);
	LCD_ColorNumber( 80,  40+50*8,   BLACK,  Back_color, cham_top_temp[7], 3, ZERO_ENABLE);

	LCD_Fill(  130,  24,   130,  474,  BLACK);	

//----------------------------------------------------------------------

	LCD_ShowString( 150,  35+50*0,   BROWN,  Back_color, "Bot");
	LCD_ShowString( 150,  55+50*0,   BROWN,  Back_color, "Temp");

	LCD_ColorNumber( 150,  40+50*1,   BLACK,  Back_color, cham_bot_temp[0], 3, ZERO_ENABLE);
	LCD_ColorNumber( 150,  40+50*2,   BLACK,  Back_color, cham_bot_temp[1], 3, ZERO_ENABLE);
	LCD_ColorNumber( 150,  40+50*3,   BLACK,  Back_color, cham_bot_temp[2], 3, ZERO_ENABLE);
	LCD_ColorNumber( 150,  40+50*4,   BLACK,  Back_color, cham_bot_temp[3], 3, ZERO_ENABLE);
	LCD_ColorNumber( 150,  40+50*5,   BLACK,  Back_color, cham_bot_temp[4], 3, ZERO_ENABLE);
	LCD_ColorNumber( 150,  40+50*6,   BLACK,  Back_color, cham_bot_temp[5], 3, ZERO_ENABLE);
	LCD_ColorNumber( 150,  40+50*7,   BLACK,  Back_color, cham_bot_temp[6], 3, ZERO_ENABLE);
	LCD_ColorNumber( 150,  40+50*8,   BLACK,  Back_color, cham_bot_temp[7], 3, ZERO_ENABLE);

	LCD_Fill(  200,  24,   200,  474,  BLACK);	

//----------------------------------------------------------------------

	LCD_ShowString( 220,  35+50*0,   BROWN,  Back_color, "Top");
	LCD_ShowString( 220,  55+50*0,   BROWN,  Back_color, "Heat");

	Color_box(  215,  35+50*1,  test_top_pwm[0]+1);	
	Color_box(  215,  35+50*2,  test_top_pwm[1]+1);	
	Color_box(  215,  35+50*3,  test_top_pwm[2]+1);	
	Color_box(  215,  35+50*4,  test_top_pwm[3]+1);	
	Color_box(  215,  35+50*5,  test_top_pwm[4]+1);	
	Color_box(  215,  35+50*6,  test_top_pwm[5]+1);	
	Color_box(  215,  35+50*7,  test_top_pwm[6]+1);	
	Color_box(  215,  35+50*8,  test_top_pwm[7]+1);	

	LCD_Fill(  270,  24,   270,  474,  BLACK);	

//----------------------------------------------------------------------

	LCD_ShowString( 290,  35+50*0,   BROWN,  Back_color, "Bot");
	LCD_ShowString( 290,  55+50*0,   BROWN,  Back_color, "Heat");

	Color_box(  285,  35+50*1,  test_bot_pwm[0]+1);	
	Color_box(  285,  35+50*2,  test_bot_pwm[1]+1);	
	Color_box(  285,  35+50*3,  test_bot_pwm[2]+1);	
	Color_box(  285,  35+50*4,  test_bot_pwm[3]+1);	
	Color_box(  285,  35+50*5,  test_bot_pwm[4]+1);	
	Color_box(  285,  35+50*6,  test_bot_pwm[5]+1);	
	Color_box(  285,  35+50*7,  test_bot_pwm[6]+1);	
	Color_box(  285,  35+50*8,  test_bot_pwm[7]+1);	

	LCD_Fill(  340,  24,   340,  474,  BLACK);	

//----------------------------------------------------------------------

	LCD_ShowString( 360,  35+50*0,   BROWN,  Back_color, "Door");
	LCD_ShowString( 360,  55+50*0,   BROWN,  Back_color, "Stat");

	Color_box(  355,  35+50*1,  2-door_status_data[0]);	
	Color_box(  355,  35+50*2,  2-door_status_data[1]);	
	Color_box(  355,  35+50*3,  2-door_status_data[2]);	
	Color_box(  355,  35+50*4,  2-door_status_data[3]);	
	Color_box(  355,  35+50*5,  2-door_status_data[4]);	
	Color_box(  355,  35+50*6,  2-door_status_data[5]);	
	Color_box(  355,  35+50*7,  2-door_status_data[6]);	
	Color_box(  355,  35+50*8,  2-door_status_data[7]);	

	LCD_Fill(  410,  24,   410,  474,  BLACK);	

//----------------------------------------------------------------------

	LCD_ShowString( 430,  35+50*0,   BROWN,  Back_color, "Door");
	LCD_ShowString( 430,  55+50*0,   BROWN,  Back_color, "LED");

	Color_box(  425,  35+50*1,  test_door_led[0]);	
	Color_box(  425,  35+50*2,  test_door_led[1]);	
	Color_box(  425,  35+50*3,  test_door_led[2]);	
	Color_box(  425,  35+50*4,  test_door_led[3]);	
	Color_box(  425,  35+50*5,  test_door_led[4]);	
	Color_box(  425,  35+50*6,  test_door_led[5]);	
	Color_box(  425,  35+50*7,  test_door_led[6]);	
	Color_box(  425,  35+50*8,  test_door_led[7]);	

	LCD_Fill(  480,  24,   480,  474,  BLACK);	

//----------------------------------------------------------------------

	LCD_ShowString( 500,  35+50*0,   BROWN,  Back_color, "Out1");
	LCD_ShowString( 500,  55+50*0,   BROWN,  Back_color, "Rely");

	Color_box(  495,  35+50*1,  test_out1_work[0]+1);	
	Color_box(  495,  35+50*2,  test_out1_work[1]+1);	
	Color_box(  495,  35+50*3,  test_out1_work[2]+1);	
	Color_box(  495,  35+50*4,  test_out1_work[3]+1);	
	Color_box(  495,  35+50*5,  test_out1_work[4]+1);	
	Color_box(  495,  35+50*6,  test_out1_work[5]+1);	
	Color_box(  495,  35+50*7,  test_out1_work[6]+1);	
	Color_box(  495,  35+50*8,  test_out1_work[7]+1);	

	LCD_Fill(  550,  24,   550,  474,  BLACK);	

//----------------------------------------------------------------------

	LCD_ShowString( 570,  35+50*0,   BROWN,  Back_color, "Out2");
	LCD_ShowString( 570,  55+50*0,   BROWN,  Back_color, "Rely");

	Color_box(  565,  35+50*1,  test_out2_work[0]+1);	
	Color_box(  565,  35+50*2,  test_out2_work[1]+1);	
	Color_box(  565,  35+50*3,  test_out2_work[2]+1);	
	Color_box(  565,  35+50*4,  test_out2_work[3]+1);	
	Color_box(  565,  35+50*5,  test_out2_work[4]+1);	
	Color_box(  565,  35+50*6,  test_out2_work[5]+1);	
	Color_box(  565,  35+50*7,  test_out2_work[6]+1);	
	Color_box(  565,  35+50*8,  test_out2_work[7]+1);	

	LCD_Fill(  620,  24,   620,  474,  BLACK);	

//----------------------------------------------------------------------
	LCD_ShowString_number(  640,  35,   BLACK,  Back_color,	"EXT1: %%%",  ext_sensor_temp, 3);

	LCD_ShowString_number(  640,  85,   BLACK,  Back_color,	"ADC1: %%%",  press_data1, 3);
	LCD_ShowString_number(  640, 135,   BLACK,  Back_color,	"ADC2: %%%",  press_data2, 3);

	LCD_ShowString_number(  640+11*0, 185,   BLACK,  Back_color,	"%%/",  time_year, 2);
	LCD_ShowString_number(  640+11*3, 185,   BLACK,  Back_color,	"%%/",  time_month, 2);
	LCD_ShowString_number(  640+11*6, 185,   BLACK,  Back_color,	"%% ",  time_date, 2);
	LCD_ShowString_number(  640+11*0, 205,   BLACK,  Back_color,	"%%:",  time_hour, 2);
	LCD_ShowString_number(  640+11*3, 205,   BLACK,  Back_color,	"%%:",  time_min, 2);
	LCD_ShowString_number(  640+11*6, 205,   BLACK,  Back_color,	"%%",   time_sec, 2);

//----------------------------------------------------------------------
	Color_box(  640,  235,  0);	

	LCD_ShowString( 690+11*0,  235,   BROWN,  Back_color, "TX:");
	LCD_ShowChar(   690+11*3,  235,   BLACK,  Back_color,  (char)test_Tx_data );
	LCD_ShowString( 690+11*5,  235,   BROWN,  Back_color, "RX:");
	LCD_ShowChar(   690+11*8,  235,   BLACK,  Back_color,  (char)test_Rx_data );

//----------------------------------------------------------------------


}






void test_mode_display(void)
{
static uint8_t  first_flag =0;

	if (first_flag ==0 )
	{
		LCD_Clear(Back_color);
		first_flag = 1 ;
		return;
	}

	test_mode_nomal();

}





void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color, uint8_t dotline) 
{
	int16_t dx, dy, sx, sy, err, e2, cnt; 	//dotline =0 ÀÏ¹Ý¼­ 1 =Á¡¼± 
	
	/* Check correction */
	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1) ; 
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 

	cnt= 0;
	sx = (x0 < x1) ? 1 : -1; 
	sy = (y0 < y1) ? 1 : -1; 
	err = ((dx > dy) ? dx : -dy) / 2; 

	while (1) {
		cnt++;
		if(cnt==6) cnt=0;
		if(cnt<3 && dotline == 1){
			LCD_Draw_Pixel(x0, y0, color); 
		}
		else if (dotline==0) {
			LCD_Draw_Pixel(x0, y0, color);
		}

		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err; 
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		} 
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		} 
	}
}

/***********************************
	@brief£ºvoid byte_dot(uint16_t temp)
***********************************/
void byte_dot2 (uint16_t temp)
{
	u8 t;

	for(t=0; t<8; t++)
	{                 
		if(temp&0x80)	LCD_WriteData(pn_color); 
		else 			LCD_WriteData(bg_color);
		temp<<=1; 
	}
}


/***********************************
	@brief£ºvoid LCD_ShowChar3(uint16_t x, uint16_t y, uint8_t num)
	@param£ºx,y	pixel  ½ÃÀÛÁöÁ¡
			num
	@retva£ºNone
	@note£ºfill display window 
***********************************/
void LCD_small_ShowChar(uint16_t x, uint16_t y, uint16_t pencolor, uint16_t backcolor, uint8_t charter)
{
  u16 temp;
  u16 pos;

	pn_color = pencolor ; 
	bg_color = backcolor;

	LCD_WindowMax (x, y, x+8-1, y+12-1);

	charter=charter-' ';
	
	for(pos=0; pos<12; pos++)
	{
		temp=ASCII8x12[charter][pos];

		byte_dot2(temp);
	} 

}

/***********************************
	@brief£ºvoid LCD_ShowChar3(uint16_t x, uint16_t y, uint8_t num)
	@param£ºx,y	pixel  ½ÃÀÛÁöÁ¡
			num
	@retva£ºNone
	@note£ºfill display window 
***********************************/
void LCD_small_ShowString(uint16_t x, uint16_t y, uint16_t pencolor, uint16_t backcolor, uint8_t *string)
{
uint8_t chat;

	while(*string != '\0')
	{
		chat = *string;
		string++;
		LCD_small_ShowChar(x, y,pencolor,backcolor, chat);
		x += 8;
	}
}


/***********************************
	@brief£ºvoid byte_dot(uint16_t temp)
***********************************/
void byte_dot (uint16_t temp)
{
	u8 t;
	if (font_size==0x00)
	{
			temp<<=1; 
			for(t=0; t<font_size_x; t++)
			{                 
			if(temp&0x8000)	LCD_WriteData(pn_color); 
			else 			LCD_WriteData(bg_color);
			temp<<=1; 
			}
	}
	else
	{
		for(t=0; t<font_size_x; t++)
			{                 
			if(temp&0x0001)	LCD_WriteData(pn_color); 
			else 			LCD_WriteData(bg_color);
			temp>>=1; 
			}
	}

}

/***********************************
	@brief£ºvoid LCD_ShowChar3(uint16_t x, uint16_t y, uint8_t num)
	@param£ºx,y	pixel  ½ÃÀÛÁöÁ¡
			num
	@retva£ºNone
	@note£ºfill display window 
***********************************/
void LCD_ShowChar2(uint16_t x, uint16_t y, uint16_t pencolor, const uint16_t *backcolor, uint8_t charter)
{
  u16 temp;
  u16 pos;

	pn_color = pencolor;

	LCD_WindowMax (x, y, x+font_size_x-1, y+font_size_y-1);

	charter=charter-' ';
	
	for(pos=0; pos<font_size_y; pos++)
	{
		if (font_size==0x00)		temp=ASCII_12x16[charter][pos];
		else if	(font_size==0x01)	temp=ASCII_16x24[charter][pos];

		bg_color = *backcolor;
		byte_dot(temp);
		backcolor++;
	} 
}


/***********************************
	@brief£ºvoid LCD_ShowChar3(uint16_t x, uint16_t y, uint8_t num)
	@param£ºx,y	pixel  ½ÃÀÛÁöÁ¡
			num
	@retva£ºNone
	@note£ºfill display window 
***********************************/
void LCD_ShowString2(uint16_t x, uint16_t y, uint16_t pencolor, const uint16_t *backcolor, uint8_t *string)
{
uint8_t chat;
uint16_t *ba_cl = (u16 *)backcolor;

	while(*string != '\0')
	{
		chat = *string;
		string++;
		LCD_ShowChar2(x, y, pencolor, ba_cl, chat);
		x += font_size_x;
	}
}



/***********************************
	*Ãû³Æ£ºvoid LCD_ShowString(u16 x0, u16 y0, u8 *pcstr, u16 PenColor, u16 BackColor)
	*²ÎÊý£ºx0 y0     ÆðÊ¼×ø±ê
		   pcstr     ×Ö·û´®Ö¸Õë
		   PenColor  ×ÖÌåÑÕ ?
		   BackColor ×ÖÌå±³¾° ?
	*¹¦ÄÜ£ºµ÷ÓÃ×Ö·ûºÍºº×ÖÏÔÊ¾º¯Êý£¬ÊµÏÖ×Ö·û´®ÏÔÊ¾
	*±¸×¢£º	
***********************************/
u32 mypow(u8 m,u8 n)
{
u32 result=1;	 

	while(n--)result*=m;    
	return result;
}			 

/***********************************
	*Ãû³Æ£ºvoid LCD_ShowString(u16 x0, u16 y0, u8 *pcstr, u16 PenColor, u16 BackColor)
	*²ÎÊý£ºx0 y0     ÆðÊ¼×ø±ê
		   pcstr     ×Ö·û´®Ö¸Õë
		   PenColor  ×ÖÌåÑÕ ?
		   BackColor ×ÖÌå±³¾° ?
	*¹¦ÄÜ£ºµ÷ÓÃ×Ö·ûºÍºº×ÖÏÔÊ¾º¯Êý£¬ÊµÏÖ×Ö·û´®ÏÔÊ¾
	*±¸×¢£º	
***********************************/
void LCD_ColorNumber2(uint16_t x, uint16_t y, uint16_t pencolor,  const uint16_t *backcolor, uint32_t number, uint8_t len, Zero_Bit zero_flag)
{
uint8_t t,temp;
uint8_t enshow=0;	
uint16_t *ba_cl = (u16 *)backcolor;

	for(t=0;t<len;t++)
	{
		temp=(number/mypow(10,len-t-1))%10;

		if(zero_flag == ZERO_ENABLE )
		{
			LCD_ShowChar2(x, y, pencolor, ba_cl, temp+'0');
			x += font_size_x;
		}
		else
		{
			if(enshow==0&&t<(len-1))
			{
				if(temp==0)
				{
					LCD_ShowChar2(x, y, pencolor, ba_cl, ' ');
					x += font_size_x;
					continue;
				}else enshow=1; 	 	 
			}
			LCD_ShowChar2(x, y, pencolor, ba_cl, temp+'0');
			x += font_size_x;
		}
	}
} 


/***********************************
	@brief£ºvoid LCD_ShowChar3(uint16_t x, uint16_t y, uint8_t num)
	@param£ºx,y	pixel  ½ÃÀÛÁöÁ¡
			num
	@retva£ºNone
	@note£ºfill display window 
***********************************/
void LCD_ShowChar(uint16_t x, uint16_t y, uint16_t pencolor, uint16_t backcolor, uint8_t charter)
{
  u16 temp;
  u16 pos;

	LCD_WindowMax (x, y, x+font_size_x-1, y+font_size_y-1);

	charter=charter-' ';

	pn_color = pencolor ; 
	bg_color = backcolor;

	for(pos=0; pos<font_size_y; pos++)
	{
		if (font_size==0x00)		temp=ASCII_12x16[charter][pos];
		else if	(font_size==0x01)	temp=ASCII_16x24[charter][pos];

		byte_dot(temp);
	}
}

/***********************************
	@brief£ºvoid LCD_ShowChar3(uint16_t x, uint16_t y, uint8_t num)
	@param£ºx,y	pixel  ½ÃÀÛÁöÁ¡
			num
	@retva£ºNone
	@note£ºfill display window 
***********************************/
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t pencolor, uint16_t backcolor, uint8_t *string)
{
uint8_t chat;

	while(*string != '\0')
	{
		chat = *string;
		string++;
		LCD_ShowChar(x, y,pencolor,backcolor, chat);
		x += font_size_x;
	}
}


/***********************************
	@brief£ºvoid LCD_ShowString_number(
	@param£ºx,y	pixel  ½ÃÀÛÁöÁ¡
			num
	@retva£ºNone
	@note£º¸¸¾à string Áß¿¡ %¸¦ ¸¸³ª¸é % ¼ýÀÚ¸¦ Ç¥½Ã
***********************************/
void LCD_ShowString_number(uint16_t x, uint16_t y, uint16_t pencolor, uint16_t backcolor, uint8_t *string, uint16_t number, uint8_t len)
{
uint8_t chat;
uint8_t t=0,temp;

	while(*string != '\0')
	{
		chat = *string;
		string++;

		if (chat == '%')
		{
			temp=(number/mypow(10,len-t-1))%10;
			t++;
			LCD_ShowChar(x, y,pencolor,backcolor, temp+'0');
		}

	    else
		{
		   LCD_ShowChar(x, y,pencolor,backcolor, chat);
		}
		x += font_size_x;
	}
}





/***********************************
	*Ãû³Æ£ºvoid LCD_ShowString(u16 x0, u16 y0, u8 *pcstr, u16 PenColor, u16 BackColor)
	*²ÎÊý£ºx0 y0     ÆðÊ¼×ø±ê
		   pcstr     ×Ö·û´®Ö¸Õë
		   PenColor  ×ÖÌåÑÕ ?
		   BackColor ×ÖÌå±³¾° ?
	*¹¦ÄÜ£ºµ÷ÓÃ×Ö·ûºÍºº×ÖÏÔÊ¾º¯Êý£¬ÊµÏÖ×Ö·û´®ÏÔÊ¾
	*±¸×¢£º	
***********************************/
void LCD_ColorNumber(uint16_t x, uint16_t y, uint16_t pencolor, uint16_t backcolor, uint32_t number, uint8_t len, Zero_Bit zero_flag)
{
	uint8_t t,temp;
	uint8_t enshow=0;	

	for(t=0;t<len;t++)
	{
		temp=(number/mypow(10,len-t-1))%10;

		if(zero_flag == ZERO_ENABLE )
		{
			LCD_ShowChar(x, y, pencolor, backcolor, temp+'0');
			x += font_size_x;
		}

		else
		{
			if(enshow==0&&t<(len-1))
			{
				if(temp==0)
				{
					LCD_ShowChar(x, y, pencolor, backcolor, ' ');
					x += font_size_x;
					continue;
				}else enshow=1; 	 	 

			}
			LCD_ShowChar(x, y, pencolor, backcolor, temp+'0');
			x += font_size_x;
		}
	}
}






/**
  * @}
  */ 

/**
  * @}
  */ 
  
/**
  * @}
  */ 

/**
  * @}
  */ 
  
/**
  * @}
  */  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
