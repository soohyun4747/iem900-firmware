/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/main.c 
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
#include "main.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t 	mode_value = 0;				//세팅 모드 값 0=nomal / 1= co2 /2 = o2/3= temp
uint8_t		sub_set_mode = 0;			// 서브 세팅 모드
uint8_t		test_mode = 0;
uint8_t		data_save_flag = 0;


extern uint8_t Error_no ;			//ERROR 번호
extern uint8_t Sub_error;			//CHAMBER 번호


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Nomal_GPIO_Setting(void);
void TIM6_config(uint16_t time_period);
void IWDG_enable_Setting(void);



/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{

	Delay_Init();
	Delay_ms(500);

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG , ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR , ENABLE);
	
	Nomal_GPIO_Setting();
	Delay_ms(500);

	HEAT_FAN = 1;
	HEAT_POW = 1;

	Delay_ms(2500);

	FSMC_NAND_Init();

	Data_Initial();

	I2C2_Init();		//touch sensor

	TEMP_Init();			//온도센서

	ADC_Config();

	Heater_PWM_setting();		//히터

	BUZZER_setting();

	CH376_Initial();	//usb 컨트롤러

	Heater_PID_Iniit();		//pid 기초

	RTC_Calender_Ini();		//rtc

	Control_Init();

	LCD_Init();

	RTC_TimeShow();

	Delay_ms(1000);

	IWDG_enable_Setting();

	Delay_ms(500);

	TIM6_config(100);		//100msec 주기

	while (1)		
	{
		data_save_flag = 0;
		
		TEMP_READ();

		PID_Control();

		Heater_Output();

		ADC_LOAD();

		Touch_Check();	

		Door_work();

		Device_Output();

		RTC_TimeShow();

		error_check();

    Data_Temp_Write();

		VOC_check();

		Chamber_work();

		Main_log_Write();

		Cham_temp_Write();

		Control_routin();

		usb_out_data();

		LCD_Display();

		IWDG_ReloadCounter();

		while( TIM_GetFlagStatus(TIM6,TIM_FLAG_Update) != SET)
		{			}
		TIM_ClearFlag (TIM6,TIM_FLAG_Update);

		}
}







//----------------------------// TIM6
void TIM6_config(uint16_t time_period)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM6, ENABLE);

//sysclk 120MHZ,  APB1 Prescaler =4, APB1 timer clock *2
//APB1 timer = 120MHZ/4*2 = 60MHZ
	TIM_TimeBaseStructure.TIM_Prescaler = 	60000-1; // PSC => 1mSEC
	TIM_TimeBaseStructure.TIM_Period = 		time_period-1; // ARR 100mSEC
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	TIM_Cmd(TIM6, ENABLE);
}





void Nomal_GPIO_Setting(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7  | GPIO_Pin_12 | GPIO_Pin_15 ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_12 ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 ;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4  | GPIO_Pin_5;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |
								   GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |
								   GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 ;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_Write(GPIOA, 0x8000);
	GPIO_Write(GPIOB, 0x0000);
	GPIO_Write(GPIOC, 0x1000);
	GPIO_Write(GPIOD, 0x0000);
	GPIO_Write(GPIOE, 0x0020);
	GPIO_Write(GPIOF, 0x0000);
	GPIO_Write(GPIOG, 0x0000);

	DOOR_E2 = 1;
	DOOR_E1 = 1;
	GPIO_Write(GPIOF, 0x0000);
	DOOR_E2 = 0;	
	DOOR_E1 = 0;
}








void IWDG_enable_Setting(void)
{	

	if ( RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET )		{	}
	else					BUZZER_ON(1);	

	RCC_ClearFlag();

	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	IWDG_SetPrescaler(IWDG_Prescaler_32);

	IWDG_SetReload(0x0fff);		//4096msec

	IWDG_ReloadCounter();

	IWDG_Enable();

}




#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
