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
#include "buzzer.h"

/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/





/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
 
 
void BUZZER_ON(uint8_t cnt)
{
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	TIM_SetCounter(TIM7, 0x0000);
	TIM_SetAutoreload(TIM7, cnt*50);
	TIM_ITConfig(TIM7, TIM_FLAG_Update, ENABLE);
	TIM_Cmd(TIM7, ENABLE);
	BUZZER = 1 ;
}



void TIM7_IRQHandler(void)
{
//////////////////////////////////////////////////////////////////////
	if(TIM_GetFlagStatus(TIM7, TIM_FLAG_Update) != RESET)
	{
		//TIM_ITConfig(TIM7, TIM_FLAG_Update, DISABLE);
		TIM_ClearFlag(TIM7, TIM_FLAG_Update);
		TIM_Cmd(TIM7, DISABLE);
		BUZZER = 0 ;
	}
//////////////////////////////////////////////////////////////////////
}




void BUZZER_setting(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM7, ENABLE);

//sysclk 120MHZ,  APB1 Prescaler =4, APB1 timer clock *2
//APB1 timer = 120MHZ/4*2 = 60MHZ
	TIM_TimeBaseStructure.TIM_Prescaler = 	60000-1; // PSC => 1mSEC
	TIM_TimeBaseStructure.TIM_Period = 		50-1;		//100msec
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIM7, TIM_FLAG_Update);

	/* Enable the TIM3 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
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
