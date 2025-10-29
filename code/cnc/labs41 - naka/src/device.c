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
#include "device.h"
#include "delay.h"

#include <stdio.h>


/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */
 
#define co2KP = 1
#define co2KI = 0
#define co2KD = 0


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

  
/* Private functions ---------------------------------------------------------*/

uint16_t Press_on_data	= 0;
uint16_t Press_off_data = 0;
uint16_t Press_err_data	= 0;
uint16_t Press_err_cnt = 0;

uint16_t Press2_on_data	= 0;
uint16_t Press2_off_data = 0;
uint16_t Press2_err_data	= 0;
uint16_t Press2_err_cnt = 0;


uint8_t	test_out1_work[8];
uint8_t	test_out2_work[8];

extern uint8_t	test_mode;




extern uint8_t	output_work[20];

extern uint32_t CO2_data;
extern uint32_t O2_data;
extern uint16_t	ADC_Value[2];

extern uint16_t press_data1;
extern uint16_t press_data2;

extern uint8_t Error_no ;			//ERROR 번호
extern uint8_t Sub_error;

extern uint8_t door_status_data[8] ;	//문상태 0= 닫힘상태 1=열림상태
extern uint8_t door_open_error[8] ;		//문 열림상태 30초 이상
extern uint8_t cham_working_sta[8] ;	//배양상태 0= 빈상태 1=배양중

extern uint16_t	gas_initial_time;
extern uint16_t	gas_period_time;
extern uint16_t	gas_work_time;

extern uint16_t	gas2_start_time ;
extern uint16_t	gas2_initial_time ;
extern uint16_t	gas2_period_time  ;
extern uint16_t	gas2_work_time ;
extern uint16_t	gas1_gas2_dual_time;

extern uint16_t	o2_2nd_set_data;						//o2 세팅 데이타
extern uint16_t	co2_2nd_set_data;						//co2 세팅 데이타
extern uint8_t	gas_2nd_on_off;						//




extern uint8_t change_sta;

extern uint8_t	chamber_count;		//0 =8 chamber 1= 6 chamber











void ch_sol_out(uint8_t *DATA1, uint8_t *DATA2)
{
	SOL_CHAM0 = (*DATA1++)? 1 : 0  ;
	SOL_CHAM1 = (*DATA1++)? 1 : 0  ;
	SOL_CHAM2 = (*DATA1++)? 1 : 0  ;
	SOL_CHAM3 = (*DATA1++)? 1 : 0  ;
	SOL_CHAM4 = (*DATA1++)? 1 : 0  ;
	SOL_CHAM5 = (*DATA1++)? 1 : 0  ;
	SOL_CHAM6 = (*DATA1++)? 1 : 0  ;
	SOL_CHAM7 = (*DATA1++)? 1 : 0  ;

	SOL_SEL1 = 1 ;
	Delay_us(10);
	SOL_SEL1 = 0 ;

	SOL_CHAM0 = (*DATA2++)? 1 : 0  ;
	SOL_CHAM1 = (*DATA2++)? 1 : 0  ;
	SOL_CHAM2 = (*DATA2++)? 1 : 0  ;
	SOL_CHAM3 = (*DATA2++)? 1 : 0  ;
	SOL_CHAM4 = (*DATA2++)? 1 : 0  ;
	SOL_CHAM5 = (*DATA2++)? 1 : 0  ;
	SOL_CHAM6 = (*DATA2++)? 1 : 0  ;
	SOL_CHAM7 = (*DATA2++)? 1 : 0  ;

	SOL_SEL2 = 1 ;
	Delay_us(10);
	SOL_SEL2 = 0 ;
	
	SOL_CHAM0 = 0  ;
	SOL_CHAM1 = 0  ;
	SOL_CHAM2 = 0  ;
	SOL_CHAM3 = 0  ;
	SOL_CHAM4 = 0  ;
	SOL_CHAM5 = 0  ;
	SOL_CHAM6 = 0  ;
	SOL_CHAM7 = 0  ;
}




void Device_Output(void)
{
static uint8_t cham_gas_check[8] ={0,0,0,0,0,0,0,0};		//IT,PT,WT 구별
static uint16_t cham_gas_cnt[8] ={0,0,0,0,0,0,0,0};		// 현 모드 동작 시간
static uint16_t gas1_work_time[8] ={0,0,0,0,0,0,0,0};		//gas1 동작 시간
static uint8_t gas2_working[8] ={0,0,0,0,0,0,0,0};				//GAS2 모로 넘어갔냐?

static uint16_t gas1_2_dual[8] ={0,0,0,0,0,0,0,0};				//GAS1,2 동시

static uint8_t gas1_min_cnt[8] ={0,0,0,0,0,0,0,0};				//


static uint8_t cham1_out[8] ={0,0,0,0,0,0,0,0};
static uint8_t cham2_out[8] ={0,0,0,0,0,0,0,0};

uint16_t Press1_max;
uint16_t Press1_min;

uint16_t Press2_max;
uint16_t Press2_min;

uint8_t i;
uint8_t press_work_sta = 0;
//uint8_t gas1_work_sta = 0;				//GAS1 동작이 존재하냐? 
//uint8_t gas2_work_sta = 0;				//GAS2 동작이 존재하냐? 

static uint16_t Perr1_cnt=0;
static uint8_t Press1_sta=0;
static uint16_t Perr2_cnt=0;
static uint8_t Press2_sta=0;

uint8_t Total_Out_off = 0;

static uint8_t valve1_err_count = 100;
static uint8_t valve2_err_count = 100;
uint8_t ch_max_no = 0;
///--------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------


///--------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------

	Press1_max = Press_off_data;
	Press1_min = Press_on_data;

	Press2_max = Press2_off_data;
	Press2_min = Press2_on_data;
///--------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------
	if (test_mode != 0)
	{
		ch_sol_out(test_out1_work,test_out2_work);
		return;
	}
///--------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------
	if (Error_no == 8 || Error_no == 9 )
	{	
			Total_Out_off = 1;					//전체 꺼짐
	}
///--------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------
//	gas1_work_sta = 0;				//GAS1 동작이 존재하냐? 
//	gas2_work_sta = 0;				//GAS2 동작이 존재하냐? 
	
	for(i=0; i<8; i++)
	{
		press_work_sta += cham_working_sta[i];		//배양상태 1=배양중
/*		
		if (cham_working_sta[i] == 1)
		{		
					if (gas2_working[i] ==0)		gas1_work_sta =1;
					else												gas2_work_sta =1;	
		}
*/
	}

///--------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------
	if (press_work_sta  == 0)
	{
			Total_Out_off = 1;						//전체 꺼짐
	}	
///--------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------
	if ( press_data1 < Press_err_data  && press_work_sta  != 0 )
	{
		if( Perr1_cnt++ > Press_err_cnt*10 )
		{
					Error_no = 8;
					Sub_error = 0;
					Perr1_cnt = 0 ;
		}
	}
	else
	{
		Perr1_cnt = 0 ;
	}
///--------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------
	if ( press_data2 < Press2_err_data  && press_work_sta  != 0 && gas_2nd_on_off ==1)
	{
		if( Perr2_cnt++ > Press2_err_cnt*10 )
		{
				Error_no = 8;
				Sub_error = 1;
				Perr2_cnt = 0 ;
		}
	}
	else
	{
		Perr2_cnt = 0 ;
	}

///--------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------
		if ( press_data1 >= Press1_max )
		{
				Press1_sta = 1;
				if (--valve1_err_count == 0)	{	Error_no = 9;	Sub_error = 0; }
		}
		else if ( press_data1 <= Press1_min )
		{
				Press1_sta = 0;	
				valve1_err_count = 100;
		}
		else
		{
				valve1_err_count = 100;
		}
//--------------------------------
		if ( press_data2 >= Press2_max  && gas_2nd_on_off ==1)
		{
				Press2_sta = 1;
				if (--valve2_err_count == 0)	{	Error_no = 9;	Sub_error = 1; }
		}
		else if ( press_data2 <= Press2_min )
		{
				Press2_sta = 0;	
				valve2_err_count = 100;
		}	
		else
		{
			valve2_err_count = 100;
		}
///--------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------
	
///--------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------
		
		if(Press1_sta==1)
		{
			ch_max_no = ( chamber_count == 0)	? 8 : 6;
				
				for(i =0; i<ch_max_no ; i++)
				{
						cham1_out[i] = 1;
						cham2_out[i] = 0;
				}
		}
		else if (Press2_sta==1)
		{
				ch_max_no = ( chamber_count == 0)	? 8 : 6;
				
				for(i =0; i<ch_max_no ; i++)
				{
						cham1_out[i] = 0;
						cham2_out[i] = 1;
				}
		}
		else if (Total_Out_off == 0)
		{
				for(i =0; i<8 ; i++)
				{
						if( cham_working_sta[i] == 1 )				//배양상태 1=배양중
						{
								if(gas2_working[i] == 0)		//gas1 working?
								{
											cham2_out[i] =0;
									
											if( door_status_data[i] == 1)			//문상태 0= 닫힘상태 1=열림상태
											{
													cham1_out[i] =0;								//문이 열림 상태
													cham_gas_check[i] = 0;
											}
											else
											{
													switch(cham_gas_check[i])			//cham_gas_check[] ,  0= initial , 1= period , 2 =work
													{
														case 0:		
															cham_gas_check[i] = 1;
															cham_gas_cnt[i] = gas_initial_time*10;
															cham1_out[i] = 1;
															break;
														case 1:		
															if (--cham_gas_cnt[i] == 0) 
															{
																cham_gas_check[i] = 2;
																cham_gas_cnt[i] = gas_period_time*10;
																cham1_out[i] = 0;
															}
															break;		
														case 2:		
															if (--cham_gas_cnt[i] == 0) 
															{
																cham_gas_check[i] = 1;
																cham_gas_cnt[i] = gas_work_time;
																cham1_out[i] =1;
															}
															break;
													}
											}

											if ( gas_2nd_on_off ==1 )
											{
													if (gas1_min_cnt[i]++ == 60)		//6초
													{
															gas1_min_cnt[i] = 0;
															
															if (gas1_work_time[i]++ ==  gas2_start_time*60*10)		
															{
																	cham1_out[i] =0;
																	cham_gas_check[i] = 0;
																	gas2_working[i] = 1;
																	gas1_2_dual[i] = 0;
															}	
													}	
											}

								}

								else if( gas2_working[i] == 1  )
								{
									
											if( door_status_data[i] == 1)			//문상태 0= 닫힘상태 1=열림상태
											{
													cham1_out[i] =0;								//문이 열림 상태
													cham2_out[i] =0;								//문이 열림 상태
													cham_gas_check[i] = 0;
											}
											else
											{
													switch(cham_gas_check[i])			//cham_gas_check[] ,  0= initial , 1= period , 2 =work
													{
														case 0:		
															cham_gas_check[i] = 1;
															cham_gas_cnt[i] = gas_initial_time*10;
															cham1_out[i] = 1;
															cham2_out[i] = 1;
															break;

														case 1:		
															if (--cham_gas_cnt[i] == 0) 
															{
																cham_gas_check[i] = 2;
																cham_gas_cnt[i] = gas_period_time*10;
																cham1_out[i] = 0;
																cham2_out[i] = 0;
															}
															break;		

														case 2:		
															if (--cham_gas_cnt[i] == 0) 
															{
																cham_gas_check[i] = 1;
																cham_gas_cnt[i] = gas_work_time;
																cham1_out[i] =1;
																cham2_out[i] =1;
															}
															break;
													}
											}

											if ( gas1_2_dual[i]++ ==  gas1_gas2_dual_time*60*10 || gas1_gas2_dual_time==0 )
											{
													cham1_out[i] =0;
													cham2_out[i] =0;
													cham_gas_check[i] = 0;
													gas2_working[i] = 2;
											}	
								}

								else	if (gas2_working[i] == 2)				//gas2 working?
								{
											cham1_out[i] =0;						
									
											if( door_status_data[i] == 1)			//문상태 0= 닫힘상태 1=열림상태
											{
													cham2_out[i] =0;								//문이 열림 상태
													cham_gas_check[i] = 0;
											}
											else
											{
													switch(cham_gas_check[i])			//cham_gas_check[] ,  0= initial , 1= period , 2 =work
													{
														case 0:		
															cham_gas_check[i] = 1;
															cham_gas_cnt[i] = gas2_initial_time*10;
															cham2_out[i] = 1;
															break;
														case 1:		
															if (--cham_gas_cnt[i] == 0) 
															{
																cham_gas_check[i] = 2;
																cham_gas_cnt[i] = gas2_period_time*10;
																cham2_out[i] = 0;
															}
															break;		
														case 2:		
															if (--cham_gas_cnt[i] == 0) 
															{
																cham_gas_check[i] = 1;
																cham_gas_cnt[i] = gas2_work_time;
																cham2_out[i] = 1;
															}
															break;
													}
											}
								}
						}
						else
						{
								cham1_out[i] =0;
								cham2_out[i] =0;
								cham_gas_check[i] = 0;	
								cham_gas_cnt[i] = 0;	
								gas1_work_time[i] = 0;
								gas2_working[i] = 0;
								gas1_min_cnt[i] = 0;

						}
				}
		}
		else
		{
				for(i =0; i<8 ; i++)
				{
						cham1_out[i] = 0;
						cham2_out[i] = 0;
						cham_gas_check[i] = 0;	
						cham_gas_cnt[i] = 0;	
						gas1_work_time[i] = 0;
						gas2_working[i] = 0;
						gas1_min_cnt[i] = 0;
				}
		}
///--------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------

	ch_sol_out(cham1_out, cham2_out);

///--------------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------
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
