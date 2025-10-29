/**
  ******************************************************************************
  * @file    TIM/PWM_Output
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief     timer pwm output
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
#include "error.h"
#include "data_io.h"
#include "buzzer.h"

/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

extern uint8_t Error_no ;			//ERROR 번호
extern uint8_t Sub_error;			//CHAMBER 번호
extern uint8_t Error_EN;
extern uint8_t 	mode_value;				//세팅 모드 값 0=nomal / 1= co2 /2 = o2/3= temp
extern uint8_t	error_set_save[22];
extern uint8_t time_year, time_week, time_month,time_date,time_hour,time_min,time_sec;

extern uint8_t	test_mode;

uint8_t		last_error = 0;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/





void error_buzzer(void)
{
	if ( error_set_save[Error_no-1] == 1)	BUZZER_ON(10);

}


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
 
 
void error_check(void)
{
static uint8_t buzzer_cnt = 0 ;

static uint8_t error_clear = 0 ;
static	uint8_t		alarm_data1[2];		//error no, sub error
static 	uint8_t		alarm_data2[6];		//작동시간
static	uint8_t		alarm_data3[6];		//해제 시간

//===========================================================================

	if (test_mode != 0)
	{
		return;
	}

//===========================================================================

	if ( mode_value==0 )
	{
		if(Error_no != 0)
		{ 
			if(Error_EN == 0)
			{
				Error_EN=1;
				error_clear = 1;

				alarm_data1[0] = Error_no;
				alarm_data1[1] = Sub_error;

				alarm_data2[0] = time_year;
				alarm_data2[1] = time_month;
				alarm_data2[2] = time_date;
				alarm_data2[3] = time_hour;
				alarm_data2[4] = time_min;
				alarm_data2[5] = time_sec;

				buzzer_cnt = 60;
				error_buzzer();
			}

			else
			{
				if( ! buzzer_cnt--)	
				{
					buzzer_cnt = 60;
					error_buzzer();		
				}	
			}
		}
		else if ( error_clear == 1 )
		{
			error_clear = 0;
			alarm_data3[0] = time_year;
			alarm_data3[1] = time_month;
			alarm_data3[2] = time_date;
			alarm_data3[3] = time_hour;
			alarm_data3[4] = time_min;
			alarm_data3[5] = time_sec;

			Alarm_log_Write(alarm_data1, alarm_data2, alarm_data3);			
		}
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
