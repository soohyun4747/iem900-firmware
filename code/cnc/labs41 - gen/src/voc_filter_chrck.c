/**
  ******************************************************************************
  * @file    RTC/HW_Calendar/main.c 
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
#include "voc_filter_check.h"
#include "data_io.h"



/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup HW_Calendar
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

int16_t 	voc_filter_time;					//전체 챔버 세팅값

extern uint8_t Error_no ;			//ERROR 번호
extern uint8_t Sub_error;			//CHAMBER 번호
extern uint8_t valid_time;
extern uint8_t change_alarm_time;

extern uint8_t time_year, time_week, time_month, time_date, time_hour, time_min, time_sec;
uint8_t voc_last_year, voc_last_month, voc_last_date, voc_last_hour, voc_last_min  ;					//전체 챔버 세팅값
uint8_t last_valid_time;



typedef struct TIME_STAMP{
uint8_t d_year;
uint8_t d_month;
uint8_t d_date;
uint8_t d_hour;
uint8_t d_min;
}time_stamp;



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
int32_t min_check(time_stamp * time_x1, time_stamp * time_y1);

















void VOC_check(void)
{
time_stamp  now_time, set_time ;
	
	now_time.d_year = time_year;
	now_time.d_month = time_month;
	now_time.d_date = time_date;
	now_time.d_hour = time_hour;
	now_time.d_min	= time_min;

	set_time.d_year = voc_last_year;
	set_time.d_month = voc_last_month;
	set_time.d_date = voc_last_date;
	set_time.d_hour = voc_last_hour;
	set_time.d_min	= voc_last_min;

	voc_filter_time = ( (last_valid_time*30*24*60) -  min_check(&now_time, &set_time)  )/ 60 ;		//분단위

	if (voc_filter_time < -999) 	voc_filter_time = -999;

	if(Error_no == 0x00)
	{
		if(voc_filter_time <= 24*10 &&  time_hour == change_alarm_time && time_min == 0 && time_sec == 0)	{ Error_no =10; Sub_error= voc_filter_time/24;	}
	}
}





const unsigned char day_in_month[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};



//x1 - y1 의 시간 
int32_t min_check(time_stamp * time_x1, time_stamp * time_y1)
{
const unsigned char *cp = day_in_month;
uint32_t  total_x1;
uint32_t  total_y1;
uint8_t		i;
int32_t	 dat_period;
	
	total_x1 = ( (time_x1->d_year)*365 )+( (time_x1->d_year+3)/4 ) ;		//일자 구하기
	total_y1 = ( (time_y1->d_year)*365 )+( (time_y1->d_year+3)/4 ) ;		//일자 구하기

	for (i=0; i<time_x1->d_month;i++)
	{
		total_x1 +=	cp[i];

		if ( i == 2 && (time_x1->d_year & 0x03) == 0)	total_x1++;
	}

	for (i=0; i<time_y1->d_month;i++)
	{
		total_y1 +=	cp[i];

		if ( i == 2 && (time_y1->d_year & 0x03) == 0)	total_y1++;
	}

	total_x1 += (time_x1->d_date-1);
	total_y1 += (time_y1->d_date-1);

	total_x1 =  (total_x1 * 24*60) + (time_x1->d_hour * 60) + (time_x1->d_min);
	total_y1 =  (total_y1 * 24*60) + (time_y1->d_hour * 60) + (time_y1->d_min);

	dat_period = total_x1 - total_y1 ;

	return	dat_period;
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
