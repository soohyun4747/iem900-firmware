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
#include "chamber_work.h"
#include "buzzer.h"
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


uint16_t 	cham_working_min[8] = {0,0,0,0,0,0,0,0};		//시간 단위 -> 분
uint8_t 	cham_working_sta[8] = {0,0,0,0,0,0,0,0};		//배양상태 0= 빈상태 1=배양중

extern uint8_t door_status_data[8] ;		//문상태 0= 닫힘상태 1=열림상태
extern uint8_t door_open_error[8] ;		//문 열림상태 30초 이상

extern uint8_t Error_no ;			//ERROR 번호
extern uint8_t Sub_error;			//CHAMBER 번호

extern uint8_t	test_mode;

extern uint8_t time_year, time_week, time_month, time_date, time_hour, time_min, time_sec;

uint8_t 	cham_temp_set_mode	 = 0;					//온도 세팅 상태 0= 비활상화 1=세팅모드 2= 세팅모드(증감키를 누르
uint8_t 	cham_setting_sta[8] = {0,0,0,0,0,0,0,0};		//개별 챔버 온도 세팅 상태
uint16_t 	cham_set_temp_dis[8] ;						//챔버 0~6 의 세팅온도 디스플레이값

uint16_t 	cham_all_set_temp = 380 ;					//전체 챔버 세팅값

extern uint16_t main_log_flag ;
extern uint16_t sub_log_flag1 ;
extern uint16_t sub_log_flag2 ;

extern uint8_t	chamber_count;		//0 =8 chamber 1= 6 chamber


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/







void Chamber_work(void)
{
static 	uint16_t 	cham_working_t[8] =  {0,0,0,0,0,0,0,0};	//시간 단위 0.1초 -> *600 = 60초(1분)
static 	uint8_t 	cham_work_h[8] =  {0,0,0,0,0,0,0,0};	//시간 단위 1분 -> *60 = 60분(1시간)
uint8_t cnt;
//===========================================================================
	if (test_mode != 0)
	{
		return;
	}
//===========================================================================
	for(cnt=0;cnt<8;cnt++)
	{
		if( cham_working_sta[cnt] && ! door_status_data[cnt] )	//배양중(1), 문닫힘(0) -> 시간 증가
		{
			cham_working_t[cnt]++;
			if(cham_working_t[cnt] == 600)	//1분 단위 600
			{
				cham_working_t[cnt] = 0;
				if(cham_working_min[cnt] < 999*60) cham_working_min[cnt]++;		//999시간 이산 측정 금지
	
				if(cham_work_h[cnt]++ == 60)	{cham_work_h[cnt] = 0;   Set_Data_Write(); }
			}
		}
		else if( cham_working_sta[cnt] && door_status_data[cnt] )	//배양중(1), 문열림(1) -> 시간 정지
		{
		}
		else 												//빈상태(0), 문 상관 없음 -> 시간 초기화
		{
				cham_working_t[cnt]=0;	
				cham_working_min[cnt]=0;
				cham_work_h[cnt]=0;
		}
	}
//===========================================================================
	if(Error_no == 0x00)
	{	
		if 		(door_open_error[0] == 1)	{Error_no =1; Sub_error= 0;	}
		else if (door_open_error[1] == 1)	{Error_no =1; Sub_error= 1;	}
		else if (door_open_error[2] == 1)	{Error_no =1; Sub_error= 2;	}
		else if (door_open_error[3] == 1)	{Error_no =1; Sub_error= 3;	}
		else if (door_open_error[4] == 1)	{Error_no =1; Sub_error= 4;	}
		else if (door_open_error[5] == 1)	{Error_no =1; Sub_error= 5;	}
		else if (door_open_error[6] == 1)	{Error_no =1; Sub_error= 6;	}
		else if (door_open_error[7] == 1)	{Error_no =1; Sub_error= 7;	}
	}


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
