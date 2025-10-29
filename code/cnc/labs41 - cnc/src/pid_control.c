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
#include "pid_control.h"
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
typedef struct PID_ERROR{
int16_t error_sum;
int16_t error_pre;
}piderror_t;

/* Private define ------------------------------------------------------------*/

#define tempo_pid     0.10   	//´ÜÀ§½Ã°£ SEC

//! \xrefitem todo "Todo" "Todo list"   
#define K_P     300						//350.0   
//! \xrefitem todo "Todo" "Todo list"   
#define K_I   	25.0 * tempo_pid			//  25.00 * tempo_pid
//! \xrefitem todo "Todo" "Todo list"   
#define K_D     5.00 / tempo_pid

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint16_t cham_set_temp[8] ;				//Ã¨¹ö 0~7 ÀÇ ¼¼ÆÃ¿Âµµ
extern uint16_t cham_top_temp[8] ;		//Ã¨¹ö 0~7 ÀÇ À­ ÆÇ³Ú ¿Âµµ
extern uint16_t cham_bot_temp[8] ;		//Ã¨¹ö 0~7 ÀÇ ¾Æ·¡ ÆÇ³Ú ¿Âµµ

extern uint16_t heat_top_pwm[8];		//Ã¨¹ö 0~7 ÀÇ À­ ÆÇ³Ú pwm
extern uint16_t heat_bot_pwm[8];		//Ã¨¹ö 0~7 ÀÇ À­ ÆÇ³Ú pwm

extern 	uint8_t Error_no;
extern  uint8_t Sub_error;
extern uint8_t	test_mode;

extern uint8_t wet_mode;
extern uint8_t wet_mode_temp;

piderror_t  pid_up_error_data[8];
piderror_t  pid_down_error_data[8];


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
uint16_t PWM_PID(uint16_t now_temp, uint16_t set_temp, piderror_t * error_data);

  


 void PID_Control(void)
{
uint8_t i,plus_temp;
static uint8_t err_ch[8] ={0,};
//===========================================================================
	if (test_mode != 0)
	{
		return;
	}
//===========================================================================
	if (wet_mode == 0)	plus_temp = 0;
	else 				plus_temp = wet_mode_temp;
//===========================================================================
	if (  Error_no == 2 || Error_no == 3 || Error_no == 4|| Error_no == 5|| Error_no == 6|| Error_no == 7)
  {
     err_ch[Sub_error] = 1;
  }
  else
  {
		for(i=0;i<8;i++)
		{
			err_ch[i] = 0;
		}
  }


  for(i=0;i<8;i++)
  {
    if(err_ch[i] == 0)
    {
      heat_top_pwm[i] = PWM_PID( cham_top_temp[i], cham_set_temp[i]+plus_temp,	&pid_up_error_data[i] );
      heat_bot_pwm[i] = PWM_PID( cham_bot_temp[i], cham_set_temp[i], 				&pid_down_error_data[i] );
    }
    else
    {
      heat_top_pwm[i] = 0;
      heat_bot_pwm[i] = 0;

    }
  }
//===========================================================================

}
  

 
  
uint16_t PWM_PID(uint16_t now_temp, uint16_t set_temp, piderror_t *error_data)
{
int32_t error;
int32_t out_data;

	error =	set_temp - now_temp ;

	out_data = (K_P * error) + (K_I * error_data->error_sum) + (K_D * (error - error_data->error_pre)) ;

	error_data->error_pre = error; 

	error_data->error_sum += error;
	if (error_data->error_sum < -800) error_data->error_sum = -800;
	if (error_data->error_sum > 800) error_data->error_sum = 800;

	if (out_data < 0) out_data=0;
	if (out_data > 900) out_data=900;

	return out_data;
}
  




void Heater_PID_Iniit(void)
{
uint8_t i;
	for(i=0;i<8;i++)
	{
		pid_up_error_data[i].error_sum = 0;
		pid_up_error_data[i].error_pre = 0;
		pid_down_error_data[i].error_sum = 0;
		pid_down_error_data[i].error_pre = 0;
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
