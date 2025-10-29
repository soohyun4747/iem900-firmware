/**
  ******************************************************************************
  * @file    SPI/SPI_TwoBoards/DataExchangeInterrupt/main.c
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
#include "temp_ic_spi.h"
#include "delay.h"


/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup SPI_DataExchangeInterrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/


#define		TEMP_ZERO		0			//X도값
#define		TEMP_SPAN		1500		//Y도값

#define		SENSOR_ZERO		0			//X도 일때 센서 값
#define		SENSOR_SPAN		4800		//Y도 일때 센서 값


#define		ERROR_SPAN		4800*500/1500		//50
#define		ERROR_ZERO		4800*50/1500		//5





/* Private define ------------------------------------------------------------*/







/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t cham_now_temp[8] ;		//챔버 0~7 의 현재온도

uint16_t cham_top_temp[8] ;		//챔버 0~7 의 윗 판넬 온도
uint16_t cham_bot_temp[8] ;		//챔버 0~7 의 아래 판넬 온도

uint32_t cham_sum_top_temp[8] ;		//챔버 0~7 의 합계온도
uint32_t cham_sum_bot_temp[8] ;		//챔버 0~7 의 합계온도

uint16_t temp_set_data[8] ;		//챔버 0~7 의 온도 세팅 값
uint16_t temp_set_input[8] ;	//챔버 0~7 의 센서 세팅 값

uint16_t temp_top_set_data[8] ;		//챔버 0~7 의 온도 세팅 값
uint16_t temp_top_set_input[8] ;	//챔버 0~7 의 센서 세팅 값
uint16_t temp_bot_set_data[8] ;		//챔버 0~7 의 온도 세팅 값
uint16_t temp_bot_set_input[8] ;	//챔버 0~7 의 센서 세팅 값



uint16_t sensor_top_in_data[8] ;			//챔버 0~7 의 온도 세팅 값
uint16_t sensor_bot_in_data[8] ;			//챔버 0~7 의 온도 세팅 값


uint16_t ext_sensor_sta ;			//외부 챔버 센서 연결값
uint16_t ext_sensor_temp;			//외부 챔버 온도값

uint16_t ext_set_data ;
uint16_t ext_set_input ;
uint16_t ext_sensor_input = 0;



extern uint8_t wet_mode;
extern uint8_t wet_mode_temp;
extern uint8_t	chamber_count;		//0 =8 chamber 1= 6 chamber



uint8_t	sensor_data_collect = 0;	// 0= 수집 안함 1= 수집 중 2= 수집 완료

/* Private function prototypes -----------------------------------------------*/

int16_t SPI3_read(uint16_t data);
int16_t TEMP_CH_READ(uint8_t Channel, uint8_t sub_no);
uint16_t SPI3_send(uint16_t data);
uint16_t TEMP_ext_READ(uint16_t *ex_temp);





extern 	uint8_t Error_no;
extern  uint8_t Sub_error;
extern uint8_t	test_mode;


/* Private functions ---------------------------------------------------------*/




void start_Calibraion(void)
{
	sensor_data_collect = 1;
}


uint8_t TEMP_DATA_calibration(uint8_t ch_no, uint8_t posit, uint16_t set_temp)	//0 = 미진입, 1 = 데이타 수집 중, 2 = 데이타 정상 3 = 데이타 오류
{
int16_t sensor_test;
uint16_t temp_data;

	if ( sensor_data_collect == 0)
	{
		return 1;
	}
	else if ( sensor_data_collect == 2)
	{
		sensor_data_collect = 3;

    if(posit==0) temp_data = sensor_top_in_data[ch_no];
    else         temp_data = sensor_bot_in_data[ch_no];

		sensor_test = (300) * temp_data / (960) ;

		sensor_test = sensor_test - set_temp ;

		if ( sensor_test > 30 || sensor_test < -30 )
		{
			return 3;
		}
		else
		{
      if(posit==0)
      {
        temp_top_set_data[ch_no] = set_temp;
        temp_top_set_input[ch_no] =  sensor_top_in_data[ch_no];
      }
      else
      {
        temp_bot_set_data[ch_no] = set_temp;
        temp_bot_set_input[ch_no] =  sensor_bot_in_data[ch_no];   
      }
			return 2;
		}
	}
	else 
	{
		return 1;
	}
}


void TEMP_DATA_initial(uint8_t ch_no, uint8_t posit)
{
  if(posit == 0)
  {
    temp_top_set_data[ch_no] = 300;
    temp_top_set_input[ch_no] = 960;
  }
  else
  {
    temp_bot_set_data[ch_no] = 300;
    temp_bot_set_input[ch_no] = 960;
  }
}








void TEMP_READ(void)
{
static	uint32_t temp_count = 10 ;		//
static	uint8_t err_count[8] = {0,0,0,0,0,0,0,0} ;		//
static uint8_t collect_no;
uint16_t cn_no;
int16_t  sensor_in_top[8], sensor_in_bot[8], sensor_in, sensor_out ;
uint8_t minus_temp,error_cnt;

uint16_t ext_sensor_data ;			//외부 챔버 센서값
uint16_t ext_sensor_avr ;			//외부 챔버 평균값
static	uint16_t ext_sensor_sum = 0 ;			//외부 챔버 합계값
static	uint16_t ext_sensor_cnt = 4 ;			//외부 챔버 평균횟수
uint8_t i;

//////////////////////////////////////////////////////////////////////////////////////	
  if( temp_top_set_data[0] == 0xffff || temp_top_set_data[0] == 0x00)
  {
		for(cn_no=0; cn_no<8; cn_no++)
		{
			temp_top_set_data[cn_no] = 300;
			temp_top_set_input[cn_no] = 960;
			temp_bot_set_data[cn_no] = 300;
			temp_bot_set_input[cn_no] = 960;
		}
  }

//////////////////////////////////////////////////////////////////////////////////////	

	SET_EN = 0;

	Delay_us(100);
//////////////////////////////////////////////////////////////////////////////////////	

	if (test_mode != 0)
	{
		for(cn_no=0; cn_no<8; cn_no++)
		{
			temp_top_set_data[cn_no] = 300;
			temp_top_set_input[cn_no] = 960;
			temp_bot_set_data[cn_no] = 300;
			temp_bot_set_input[cn_no] = 960;
		}
	}

//////////////////////////////////////////////////////////////////////////////////////	
	for(cn_no=0; cn_no<8; cn_no++)
	{

    sensor_in_top[cn_no] = 0;
    for (i= 0; i<16; i++ )
    {
      sensor_in_top[cn_no]  +=  TEMP_CH_READ(cn_no,0);		//channel no ,  sub_no
      Delay_us(100);
    }
    sensor_in_top[cn_no] = sensor_in_top[cn_no]/16;
		sensor_in = sensor_in_top[cn_no];
		sensor_out = (temp_top_set_data[cn_no]-TEMP_ZERO) * (sensor_in  -  SENSOR_ZERO) / (temp_top_set_input[cn_no] - SENSOR_ZERO) + TEMP_ZERO ;
		if(sensor_out < 0)	sensor_out=0;		//0도 이하는 0도로
		cham_top_temp[cn_no] =  (uint16_t) sensor_out  ;	//channel no ,  sub_no


    sensor_in_bot[cn_no] = 0;
    for (i= 0; i<16; i++ )
    {
      sensor_in_bot[cn_no]  +=  TEMP_CH_READ(cn_no,1);		//channel no ,  sub_no
      Delay_us(100);
    }
    sensor_in_bot[cn_no] = sensor_in_bot[cn_no]/16;
		sensor_in = sensor_in_bot[cn_no];
		sensor_out = (temp_bot_set_data[cn_no]-TEMP_ZERO) * (sensor_in  -  SENSOR_ZERO) / (temp_bot_set_input[cn_no] - SENSOR_ZERO) + TEMP_ZERO ;
		if(sensor_out < 0)	sensor_out=0;		//0도 이하는 0도로
		cham_bot_temp[cn_no] =  (uint16_t) sensor_out  ;	//channel no ,  sub_no
	}
//////////////////////////////////////////////////////////////////////////////////////
	switch (sensor_data_collect)
	{
		case 0:
			collect_no = 0;
			for(cn_no=0; cn_no<8; cn_no++)		{	sensor_top_in_data[cn_no] = 0 ;	sensor_bot_in_data[cn_no] = 0 ;}
			break;
	
		case 1:
			collect_no++;

			for(cn_no=0; cn_no<8; cn_no++)		{		sensor_top_in_data[cn_no] += sensor_in_top[cn_no] ;			sensor_bot_in_data[cn_no] += sensor_in_bot[cn_no] ;			}

			if (collect_no == 10)
			{
				sensor_data_collect =2;
				for(cn_no=0; cn_no<8; cn_no++)	{		sensor_top_in_data[cn_no] /= 10 ;	sensor_bot_in_data[cn_no] /= 10 ; }
			}
			break;	

		case 2:
			break;

		case 3:
			sensor_data_collect =0;
			collect_no = 0;
			break;
	}
//////////////////////////////////////////////////////////////////////////////////////
	for(cn_no=0; cn_no<8; cn_no++)
	{
		cham_sum_top_temp[cn_no] += cham_top_temp[cn_no] ;
		cham_sum_bot_temp[cn_no] += cham_bot_temp[cn_no] ;
	}
//////////////////////////////////////////////////////////////////////////////////////
	if (wet_mode == 0)	minus_temp = 0;
	else 				minus_temp = wet_mode_temp;
//////////////////////////////////////////////////////////////////////////////////////
	if (--temp_count == 0)
	{
		temp_count= 10;	

		for(cn_no=0; cn_no<8; cn_no++)
		{
			cham_sum_top_temp[cn_no] /= 10 ;
			cham_sum_bot_temp[cn_no] /= 10 ;

			if ( cham_sum_top_temp[cn_no] != 0)		cham_sum_top_temp[cn_no] -= minus_temp ;

			cham_now_temp[cn_no] = (cham_sum_top_temp[cn_no]+cham_sum_bot_temp[cn_no]+1)/2 ;

			cham_sum_top_temp[cn_no] = 0;
			cham_sum_bot_temp[cn_no] = 0;
		}
	}
//////////////////////////////////////////////////////////////////////////////////////
	if (chamber_count == 0)		error_cnt=8;
	else						          error_cnt=6;
//////////////////////////////////////////////////////////////////////////////////////
	//if(Error_no == 0)
	{
		for(cn_no=0; cn_no<error_cnt; cn_no++)
		{
			if	 	(sensor_in_top[cn_no] == 0)			{	if(err_count[cn_no]++ == 30 ) 	{ Error_no =6; Sub_error = cn_no;	break;}		}

			else if	(sensor_in_bot[cn_no] == 0)			{ 	if(err_count[cn_no]++ == 30 ) 	{ Error_no =7; Sub_error = cn_no;	break;}		}

			else if	(sensor_in_top[cn_no] > ERROR_SPAN)	{ 	if(err_count[cn_no]++ == 30 ) 	{ Error_no =2; Sub_error = cn_no;	break;}		}

			else if	(sensor_in_bot[cn_no] > ERROR_SPAN)	{ 	if(err_count[cn_no]++ == 30 ) 	{ Error_no =3; Sub_error = cn_no;	break;}		}

			else if	(sensor_in_top[cn_no] < ERROR_ZERO)	{ 	if(err_count[cn_no]++ == 30 ) 	{ Error_no =4; Sub_error = cn_no;	break;}		}

			else if	(sensor_in_bot[cn_no] < ERROR_ZERO)	{ 	if(err_count[cn_no]++ == 30 ) 	{ Error_no =5; Sub_error = cn_no;	break;}		}

			else { err_count[cn_no] = 0; }
		}
	}
//////////////////////////////////////////////////////////////////////////////////////
	SET_EN = 1;

	Delay_us(100);

	ext_sensor_sta = TEMP_ext_READ(&ext_sensor_data);
	ext_sensor_sum += ext_sensor_data;

	if (--ext_sensor_cnt == 0)
	{
		ext_sensor_cnt= 4;	
		ext_sensor_avr = ext_sensor_sum >> 2 ;
		ext_sensor_sum = 0;

		ext_sensor_input = ext_sensor_avr;
		ext_sensor_temp = (ext_set_data-TEMP_ZERO) * (ext_sensor_avr -  SENSOR_ZERO) / (ext_set_input - SENSOR_ZERO) + TEMP_ZERO ;
	}
}





int16_t TEMP_CH_READ(uint8_t Channel, uint8_t sub_no)
{
uint16_t temp =0 , Channel_no ;

	Channel_no = 2*Channel + sub_no;

	switch(Channel_no)
	{
		case(0):		//1T
			TS_ADR2=1;
			TS_ADR1=1;
			TS_ADR0=1;
			SET_CH0=0;
			SET_CH1=0;
			break ;
		case(1):		//1B
			TS_ADR2=1;
			TS_ADR1=1;
			TS_ADR0=1;
			SET_CH0=1;
			SET_CH1=1;
			break ;
		case(2):		//2T
			TS_ADR2=1;
			TS_ADR1=1;
			TS_ADR0=0;
			SET_CH0=0;
			SET_CH1=0;
			break ;
		case(3):		//2B
			TS_ADR2=1;
			TS_ADR1=1;
			TS_ADR0=0;
			SET_CH0=1;
			SET_CH1=1;
			break ;
		case(4):		//3T
			TS_ADR2=1;
			TS_ADR1=0;
			TS_ADR0=1;
			SET_CH0=0;
			SET_CH1=0;
			break ;
		case(5):		//3B
			TS_ADR2=1;
			TS_ADR1=0;
			TS_ADR0=1;
			SET_CH0=1;
			SET_CH1=1;
			break ;
		case(6):		//4T
			TS_ADR2=1;
			TS_ADR1=0;
			TS_ADR0=0;
			SET_CH0=0;
			SET_CH1=0;
			break ;
		case(7):		//4B
			TS_ADR2=1;
			TS_ADR1=0;
			TS_ADR0=0;
			SET_CH0=1;
			SET_CH1=1;
			break ;
		case(8):		//5T
			TS_ADR2=0;
			TS_ADR1=1;
			TS_ADR0=1;
			SET_CH0=1;
			SET_CH1=0;
			break ;
		case(9):		//5B
			TS_ADR2=0;
			TS_ADR1=1;
			TS_ADR0=1;
			SET_CH0=0;
			SET_CH1=1;
			break ;
		case(10):		//6T
			TS_ADR2=0;
			TS_ADR1=1;
			TS_ADR0=0;
			SET_CH0=1;
			SET_CH1=0;
			break ;
		case(11):		//6B
			TS_ADR2=0;
			TS_ADR1=1;
			TS_ADR0=0;
			SET_CH0=0;
			SET_CH1=1;
			break ;
		case(12):		//7T
			TS_ADR2=0;
			TS_ADR1=0;
			TS_ADR0=1;
			SET_CH0=1;
			SET_CH1=0;
			break ;
		case(13):		//7B
			TS_ADR2=0;
			TS_ADR1=0;
			TS_ADR0=1;
			SET_CH0=0;
			SET_CH1=1;
			break ;
		case(14):		//8T
			TS_ADR2=0;
			TS_ADR1=0;
			TS_ADR0=0;
			SET_CH0=1;
			SET_CH1=0;
			break ;
		case(15):		//8B
			TS_ADR2=0;
			TS_ADR1=0;
			TS_ADR0=0;
			SET_CH0=0;
			SET_CH1=1;
			break;
	}

	if (sub_no == 0x00)		TS_S0 = 1;
	else					TS_S1 = 1;

	Delay_us(50);

	temp = SPI3_read(0x0000);

	Delay_us(50);

	TS_S1 = 0;	
	TS_S0 = 0;	
	return temp;
}







uint8_t TEMP_ext_calibration(uint16_t set_temp)	//0 = 미진입, 1 = 데이타 수집 중, 2 = 데이타 정상 3 = 데이타 오류
{
int16_t sensor_test;

	sensor_test = 300 * ext_sensor_input / 960 ;

	sensor_test = sensor_test - set_temp ;

	if ( sensor_test > 30 || sensor_test < -30 )
	{
		return 3;
	}
	else
	{
		ext_set_data = set_temp;
		ext_set_input =  ext_sensor_input;
		return 2;
	}
}


void TEMP_ext_initial(void)
{
	ext_set_data = 300;
	ext_set_input = 960;
}



uint16_t TEMP_ext_READ(uint16_t *ex_temp)
{
uint16_t sta =0, raw_data ;
uint16_t i;
int32_t temp_data = 0;

  for(i = 0; i<16; i++)
  {
	SET_EN = 0;

	TS_ADR2=0;
	TS_ADR1=0;
	TS_ADR0=0;
	TS_S1 = 0;
	TS_S0 = 0;

	SET_CH0 = 0;
	SET_CH1 = 1;

	TS_EXT = 0;

	Delay_us(50);

	raw_data = SPI3_send(0x0000);
    temp_data += ( (raw_data & ~0x0003) >> 2 )& 0x3fff ;

	TS_EXT = 1;
	SET_EN = 1;

    Delay_us(100);
  }

  *ex_temp = temp_data/16;

	if (raw_data== 0x00)	sta =0;
	else	sta =1;

	return sta;
}




/* This funtion is used to transmit and receive data 
 * with SPI3
 * 			data --> data to be transmitted
 * 			returns received value
 */
uint16_t SPI3_send(uint16_t data)
{
	SPI3->DR = data; // write data to be transmitted to the SPI data register
//	while( !(SPI3->SR & SPI_I2S_FLAG_TXE) ); // wait until transmit complete
	while( !(SPI3->SR & SPI_I2S_FLAG_RXNE) ); // wait until receive complete
	while( SPI3->SR & SPI_I2S_FLAG_BSY ); // wait until SPI is not busy anymore
	return SPI3->DR; // return received data from SPI data register
}




/* This funtion is used to transmit and receive data 
 * with SPI3
 * 			data --> data to be transmitted
 * 			returns received value
 */
int16_t SPI3_read(uint16_t data)
{
int16_t input;

	input = SPI3_send(data) & ~0x0003;
	input = ( input >> 2 )& 0x3fff ;

	return (input);	
}



// this function initializes the SPI3 peripheral
void TEMP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI3 , ENABLE);
	
	/* configure pins used by SPI3
	 * PC10 = SCK
	 * PC11 = MISO
	 *
	 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	// connect SPI3 pins to SPI alternate function
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);


	/* configure SPI3 in Mode 0 
	 * CPOL = 0 --> clock is low when idle
	 * CPHA = 1 --> data is sampled at the first edge
	 */
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // set to receive only mode, seperate MISO lines
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;     // transmit in master mode, NSS pin has to be always high
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_16b; // one packet of data is 16 bits wide
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;        // clock is low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;      // data sampled at first edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // set the NSS management to internal and pull internal NSS high
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; // SPI frequency is APB1 frequency / 16
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
	SPI_Init(SPI3, &SPI_InitStruct); 

	SPI_Cmd(SPI3, ENABLE); // enable SPI3
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
