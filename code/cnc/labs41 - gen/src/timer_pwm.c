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
#include "timer_pwm.h"
#include "delay.h"

/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup TIM_PWM_Output
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t heat_top_pwm[8]	= {0,0,0,0,0,0,0,0};
uint16_t heat_bot_pwm[8]	= {0,0,0,0,0,0,0,0};


uint8_t test_top_pwm[8]	= {0,0,0,0,0,0,0,0};
uint8_t test_bot_pwm[8]	= {0,0,0,0,0,0,0,0};

extern uint8_t	test_mode;
extern uint8_t	chamber_count;		//0 =8 chamber 1= 6 chamber



/* Private function prototypes -----------------------------------------------*/
void TIM_Config(void);


/* Private functions ---------------------------------------------------------*/





/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
 
 
void Heater_Output(void)
{
//===========================================================================
	if (test_mode != 0)
	{
		heat_top_pwm[0] = test_top_pwm[0] *900;
		heat_bot_pwm[0] = test_bot_pwm[0] *900;
		heat_top_pwm[1] = test_top_pwm[1] *900;
		heat_bot_pwm[1] = test_bot_pwm[1] *900;
		heat_top_pwm[2] = test_top_pwm[2] *900;
		heat_bot_pwm[2] = test_bot_pwm[2] *900;
		heat_top_pwm[3] = test_top_pwm[3] *900;
		heat_bot_pwm[3] = test_bot_pwm[3] *900;
		heat_top_pwm[4] = test_top_pwm[4] *900;
		heat_bot_pwm[4] = test_bot_pwm[4] *900;
		heat_top_pwm[5] = test_top_pwm[5] *900;
		heat_bot_pwm[5] = test_bot_pwm[5] *900;
		heat_top_pwm[6] = test_top_pwm[6] *900;
		heat_bot_pwm[6] = test_bot_pwm[6] *900;
		heat_top_pwm[7] = test_top_pwm[7] *900;
		heat_bot_pwm[7] = test_bot_pwm[7] *900;
	}
//===========================================================================

	TIM4 -> CCR4  = heat_top_pwm[0];
	TIM4 -> CCR3  = heat_bot_pwm[0];
	TIM4 -> CCR2  = heat_top_pwm[1];
	TIM4 -> CCR1  = heat_bot_pwm[1];

	TIM1 -> CCR4  = heat_top_pwm[2];
	TIM1 -> CCR1  = heat_bot_pwm[2];
	TIM8 -> CCR4  = heat_top_pwm[3];
	TIM8 -> CCR3  = heat_bot_pwm[3];

	TIM2 -> CCR1  = heat_top_pwm[4];
	TIM2 -> CCR2  = heat_bot_pwm[4];
	TIM2 -> CCR3  = heat_top_pwm[5];
	TIM2 -> CCR4  = heat_bot_pwm[5];


	if (chamber_count == 0)		//0 =8 chamber 1= 6 chamber
	{
		TIM3 -> CCR1  = heat_top_pwm[6];
		TIM3 -> CCR2  = heat_bot_pwm[6];
		TIM3 -> CCR3  = heat_top_pwm[7];
		TIM3 -> CCR4  = heat_bot_pwm[7];
	}
	else 
	{
		TIM3 -> CCR1  = 0;
		TIM3 -> CCR2  = 0;
		TIM3 -> CCR3  = 0;
		TIM3 -> CCR4  = 0;
	}

//===========================================================================
}








void Heater_PWM_setting(void)
{
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure; 

uint16_t PrescalerValue = 0;

	/* TIM Configuration */
	TIM_Config();

  /* Compute the prescaler value */
	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 20000000) - 1;
//-----------------------------------------------------------
  /* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 4095;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);	
//-----------------------------------------------------------

	PrescalerValue = (uint16_t) ((SystemCoreClock) / 20000000) - 1;

	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_Period = 4095;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);	
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);	

//-----------------------------------------------------------
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;

	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
//-----------------------------------------------------------
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);

	TIM_OC3Init(TIM8, &TIM_OCInitStructure);
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  /* PWM1 Mode configuration: Channel1 */
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  /* PWM1 Mode configuration: Channel2 */
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  /* PWM1 Mode configuration: Channel3 */
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  /* PWM1 Mode configuration: Channel4 */

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  /* PWM1 Mode configuration: Channel1 */
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  /* PWM1 Mode configuration: Channel2 */
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  /* PWM1 Mode configuration: Channel3 */
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  /* PWM1 Mode configuration: Channel4 */

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  /* PWM1 Mode configuration: Channel1 */
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  /* PWM1 Mode configuration: Channel2 */
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  /* PWM1 Mode configuration: Channel3 */
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  /* PWM1 Mode configuration: Channel4 */

//-----------------------------------------------------------
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
//-----------------------------------------------------------
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
//-----------------------------------------------------------
	TIM_Cmd(TIM4, ENABLE);
  Delay_us(50);
  TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM8, ENABLE);
  Delay_us(50);
	TIM_Cmd(TIM2, ENABLE);
  Delay_us(50);
	TIM_Cmd(TIM3, ENABLE);

}











/**
  * @brief  Configure the TIM3 Ouput Channels.
  * @param  None
  * @retval None
  */
void TIM_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

//-----------------------------------------------------------
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 , ENABLE); 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1 | RCC_APB2Periph_TIM8, ENABLE); 
//-----------------------------------------------------------	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_11 ;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 ;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 ;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 


	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0,  GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1,  GPIO_AF_TIM2); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,  GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,  GPIO_AF_TIM2); 

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6,  GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7,  GPIO_AF_TIM3); 
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0,  GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1,  GPIO_AF_TIM3); 

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6,  GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7,  GPIO_AF_TIM4); 
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8,  GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9,  GPIO_AF_TIM4); 

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8,  GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11,  GPIO_AF_TIM1);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM8); 



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
