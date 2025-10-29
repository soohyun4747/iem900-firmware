/**
  ******************************************************************************
  * @file    ADC/VBAT_Measurement/main.c 
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
#include "adc_input.h"

/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup ADC_VBAT_Measurement
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define	BUFFERLENGHT	2
#define ARRAYSIZE		2*64		//ch수 3, 각각 32개씩

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t	ADC_Value[2];
uint16_t 	ADC3ConvertedValue[ARRAYSIZE] ;

uint16_t 	press_data1= 0;
uint16_t 	press_data2= 0;

/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
 
 



#define	adc_Default	725


void ADC_LOAD(void)
{
uint16_t i=0, j= 0;
uint32_t SUM_Value[2];
int16_t test_val;

	SUM_Value[0] = 0;
	SUM_Value[1] = 0;

	for(i=0;i<64;i++)
	 {
		SUM_Value[0] +=	ADC3ConvertedValue[j++];
		SUM_Value[1] += ADC3ConvertedValue[j++];
	 }

 	ADC_Value[0] = SUM_Value[0] / 64;	//FLOW_CO2
 	ADC_Value[1] = SUM_Value[1] / 64;	//FLOW_CO2


	test_val =(ADC_Value[0]-745)*4/119 ;
	if(test_val<0)	test_val=0;
	press_data1 = test_val;

	test_val =(ADC_Value[1]-745)*4/119 ;
	if(test_val<0)	test_val=0;
	press_data2 = test_val;
}
 
 
 
 
 

/**
  * @brief  ADC1 Channel Vbat configuration (DMA, ADC, CLK)
  * @param  None
  * @retval None
  */
void ADC_Config(void)
{
ADC_InitTypeDef       ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
GPIO_InitTypeDef  GPIO_initStructre; //Structure for analog input pin
DMA_InitTypeDef  DMA_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	GPIO_initStructre.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_initStructre.GPIO_Mode = GPIO_Mode_AN; 	//configured in analog mode
	GPIO_initStructre.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
	GPIO_Init(GPIOF,&GPIO_initStructre);			//Affecting the port with the initialization structure configuration

	/* Configure the DMA **********************************************************/
    /* DMA2 Stream0 channel0 configuration **************************************/
	DMA_DeInit(DMA2_Stream0);  //Set DMA registers to default values

	DMA_InitStructure.DMA_Channel = DMA_Channel_2;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC3->DR; //Source address
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC3ConvertedValue; //Destination address
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = ARRAYSIZE; //Buffer size
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //source size - 16bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; // destination size = 16b
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure); //Initialize the DMA
	DMA_Cmd(DMA2_Stream0, ENABLE); //Enable the DMA2 - Stream 4
	
	//ADC structure configuration
	ADC_DeInit();
    /* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC3 Init ****************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;				//continuous conversion
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = 0;  
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = BUFFERLENGHT;
	ADC_Init(ADC3, &ADC_InitStructure);

	 /* ADC3 regular channe configuration */ 
	ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 1, ADC_SampleTime_144Cycles);
	ADC_RegularChannelConfig(ADC3, ADC_Channel_8, 2, ADC_SampleTime_144Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

	ADC_DMACmd(ADC3, ENABLE);		 //Enable ADC3 DMA

	/* Enable ADC3 */
	ADC_Cmd(ADC3, ENABLE);

	ADC_SoftwareStartConv(ADC3);					//Start the conversion
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
