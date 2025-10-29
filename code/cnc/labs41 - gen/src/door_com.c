/**
  ******************************************************************************
  * @file    USART/USART_Printf/main.c 
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
#include "door_com.h"
#include "delay.h"
#include "buzzer.h"
#include <stdio.h>


/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern uint8_t cham_working_sta[8] ;	//작업상태 0= 빈상태 1=배양중
uint8_t door_status_data[8] ;		//문상태 0= 닫힘상태 1=열림상태
uint8_t door_open_error[8] ;		//문 열림상태 30초 이상

extern uint8_t Error_no ;			//ERROR 번호
extern uint8_t Sub_error;			//CHAMBER 번호
extern uint16_t main_log_flag ;
extern uint16_t sub_log_flag1 ;
extern uint16_t sub_log_flag2 ;

extern uint8_t	test_mode;

uint8_t test_door_led[8];		//문상태 0= x 1=red 2= green 3= orange

extern uint8_t	chamber_count;		//0 =8 chamber 1= 6 chamber


/* Private function prototypes -----------------------------------------------*/
void Door_input(void);
void Door_output(void);


  
/* Private functions ---------------------------------------------------------*/


void Door_work(void)
{
	Door_input();
	Door_output();
}



void Door_input(void)
{
uint16_t	Port_in,cnt;
static uint16_t	door_open_time[8] = {0,0,0,0,0,0,0,0};		//시간 단위로 열린 시간 체크

static uint8_t	door_check[8] = {0,0,0,0,0,0,0,0};		//시간 단위로 열린 시간 체크


GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	Delay_us(1);

	DOOR_E0 = 0 ;	
	Delay_us(1);
	Port_in = GPIO_ReadInputData(GPIOF) ;
	DOOR_E0 = 1 ;

	Port_in &= ( GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 );

	door_status_data[0] =  (Port_in & GPIO_Pin_7)>>7 ;		//문상태 0= 닫힘상태 1=열림상태
	door_status_data[1] =  (Port_in & GPIO_Pin_6)>>6 ;
	door_status_data[2] =  (Port_in & GPIO_Pin_5)>>5 ;
	door_status_data[3] =  (Port_in & GPIO_Pin_4)>>4 ;
	door_status_data[4] =  (Port_in & GPIO_Pin_3)>>3 ;
	door_status_data[5] =  (Port_in & GPIO_Pin_2)>>2 ;
	door_status_data[6] =  (Port_in & GPIO_Pin_1)>>1 ;
	door_status_data[7] =  (Port_in & GPIO_Pin_0)>>0 ;

	if (chamber_count ==1)	{ door_status_data[6] = 0;	door_status_data[7] = 0;}

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	for(cnt=0;cnt<8;cnt++)
	{	
		if(door_status_data[cnt])			//문상태 0= 닫힘상태 1=열림상태
		{
			if(door_open_time[cnt] == 300)	door_open_error[cnt] = 1;		//문 열린 시간 0.1*300 = 30초
			else door_open_time[cnt]++;	

			if(! door_check[cnt]) 	{door_check[cnt]=1; main_log_flag= 2; sub_log_flag1=cnt;	sub_log_flag2=0;	}
		}
		else 
		{
			door_open_time[cnt] = 0;
			door_open_error[cnt] = 0;

			if(door_check[cnt]) 	{door_check[cnt]=0; main_log_flag= 2; sub_log_flag1=cnt;	sub_log_flag2=1;}
		}
	}
}






void Door_LED(uint16_t LED_R, uint16_t LED_G)
{
uint16_t	Port_out;

	Port_out = GPIO_ReadOutputData(GPIOF); 
	Port_out &= ~( GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 );

	LED_R |= Port_out;
	LED_G |= Port_out;

	DOOR_E2 = 1;
	GPIO_Write(GPIOF, LED_R);	
	DOOR_E2 = 0;	

	DOOR_E1 = 1;
	GPIO_Write(GPIOF, LED_G);	
	DOOR_E1 = 0;

	return;
}




#define led_red    1
#define led_green  0x1000

void Door_output(void)
{

uint16_t	out_data_r,out_data_g,cnt;

static uint16_t	blink_count = 10;
static uint16_t	blink_led = 0;		//0이면 red, 1이면 green
///////////////////////////////////////////////////////
	out_data_r = 0;
	out_data_g = 0;

///////////////////////////////////////////////////////

	if (test_mode != 0)
	{
		for(cnt=0;cnt<8;cnt++)
		{
			out_data_r = out_data_r << 1;
			out_data_g = out_data_g << 1;

			switch (test_door_led[cnt])		//문상태 0= x 1=red 2= green 3= orange
			{
				case 0 :	out_data_r |= 0;		out_data_g |= 0;		break;
				case 1 :	out_data_r |= 1;		out_data_g |= 0;		break;
				case 2 :	out_data_r |= 0;		out_data_g |= 1;		break;
				case 3 :	out_data_r |= 1;		out_data_g |= 1;		break;
			}
		}

		Door_LED(out_data_r, out_data_g);
		return;
	}

///////////////////////////////////////////////////////
	if (blink_count-- == 0)
	{
		blink_count = 5;		//20회 * 0.1sec = 2sec
		blink_led ^= 0x01;		//led 점멸
	}
///////////////////////////////////////////////////////

	for(cnt=0;cnt<8;cnt++)
	{
		out_data_r = out_data_r << 1;
		out_data_g = out_data_g << 1;

		if(	door_status_data[cnt] )		//문상태 0= 닫힘상태->작업상태어 따라  1=열림상태 -> blink
		{
			if (blink_led)		out_data_g |= 1;		//0이면 red, 1이면 green
			else		        out_data_r |= 1;
		}
		else
		{
			if( cham_working_sta[cnt] != 0) out_data_g |= 1;			//작업상태 0= 빈상태->red  1=배양중->green
			else						    out_data_r |= 1;
		}
	}
///////////////////////////////////////////////////////
	Door_LED(out_data_r, out_data_g);
///////////////////////////////////////////////////////
}







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
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
