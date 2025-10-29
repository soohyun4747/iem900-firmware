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
#include "control_uart.h"
#include "delay.h"
#include "data_io.h"
#include <stdio.h>


/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TX1BUFFERSIZE	0x200
#define RX1BUFFERSIZE	0x0A

#define RX1DATASIZE		26




union TRANS_BYTE
{
  uint16_t dat16;
  uint8_t dat8[2];
};

/* Private macro -------------------------------------------------------------*/
#define d2_move(k)		dat.dat16 = (k);	\
						Tx1Buffer[i++] =  dat.dat8[0]; \
						Tx1Buffer[i++] =  dat.dat8[1]; \
						check_sum += dat.dat8[0]; \
						check_sum += dat.dat8[1]

#define d1_move(k)		Tx1Buffer[i++] =  (k);	\
						check_sum += k

/* Private variables ---------------------------------------------------------*/
uint8_t Tx1Buffer[TX1BUFFERSIZE];
uint8_t Rx1Buffer[RX1BUFFERSIZE];

uint8_t Buffer[70];

uint8_t receive_data=0, receive_sta=0;

__IO uint8_t Rx1Counter = 0; 
__IO uint8_t Rx1Index = 0x00;
__IO uint8_t Rx1Flag = 0;
__IO uint8_t Rx1Mode = 0;

__IO uint8_t Tx1Flag = 0; 
__IO uint16_t Tx1Counter = 0; 
__IO uint16_t Tx1No = 0; 



uint8_t test_Tx_data = ' ';
uint8_t test_Rx_data = ' ';
uint8_t test_Tx_sta = 0;


extern uint16_t cham_now_temp[8] ;		//챔버 0~6 의 현재온도
extern uint16_t cham_set_temp[8] ;		//챔버 0~6 의 세팅온도

extern uint8_t 	cham_working_sta[8];		//배양상태 0= 빈상태 1=배양중
extern uint8_t  door_status_data[8] ;		//문상태 0= 닫힘상태 1=열림상태

extern uint16_t cham_working_min[8];		//시간 단위 -> 분

extern uint16_t	o2_set_data ;				//o2 세팅 데이타
extern uint16_t	co2_set_data;				//co2 세팅 데이타

extern uint16_t press_data1;
extern int16_t	voc_filter_time;

extern uint8_t Error_no ;			//ERROR 번호
extern uint8_t Sub_error;			//CHAMBER 번호
extern uint8_t	test_mode;


extern uint16_t Temp_set_cnt ;
extern uint16_t main_log_cnt ;
extern uint16_t voc_change_cnt ;
extern uint16_t alarm_log_cnt ;
extern uint16_t cham_temp_cnt ;
extern uint16_t gas_change_cnt ;


/* Private function prototypes -----------------------------------------------*/

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
  
/* Private functions ---------------------------------------------------------*/
void Control_Read(void);
void Control_Write(void);


  
  
 void tx_enable(void)
 {
	Tx1Flag = 1; 
	Tx1Counter = 0;
	USART_ITConfig(USART1, USART_IT_TC, DISABLE);
	USART_ClearITPendingBit(USART1, USART_IT_TC);
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);	// as you need to made TXE enable interrupt
 }

  
  
  
  
  
  
  

void Control_routin(void)
{
//===========================================================================
	if (test_mode != 0)
	{
		if (test_Tx_sta == 1)
		{		
			USART_SendData(USART1, test_Tx_data);
			test_Tx_sta = 0;
		}
		return;
	}
//===========================================================================
	Control_Read();
	Control_Write();
}	




void Command0(void)
{
uint16_t data_len,i;
uint8_t check_sum;
union TRANS_BYTE dat;

	check_sum = 0;

	Tx1Buffer[0] = 0x53;
	check_sum += 0x53;
	Tx1Buffer[1] = 0x01;
	check_sum += 0x01;

	i = 2;	
	data_len = 73 ;

	d2_move( data_len ) ;

	d2_move( cham_set_temp[0] ) ;
	d2_move( cham_set_temp[1] ) ;
	d2_move( cham_set_temp[2] ) ;
	d2_move( cham_set_temp[3] ) ;
	d2_move( cham_set_temp[4] ) ;
	d2_move( cham_set_temp[5] ) ;
	d2_move( cham_set_temp[6] ) ;
	d2_move( cham_set_temp[7] ) ;

	d2_move( cham_now_temp[0] ) ;
	d2_move( cham_now_temp[1] ) ;
	d2_move( cham_now_temp[2] ) ;
	d2_move( cham_now_temp[3] ) ;
	d2_move( cham_now_temp[4] ) ;
	d2_move( cham_now_temp[5] ) ;
	d2_move( cham_now_temp[6] ) ;
	d2_move( cham_now_temp[7] ) ;

	d1_move( cham_working_sta[0] ) ;
	d1_move( cham_working_sta[1] ) ;
	d1_move( cham_working_sta[2] ) ;
	d1_move( cham_working_sta[3] ) ;
	d1_move( cham_working_sta[4] ) ;
	d1_move( cham_working_sta[5] ) ;
	d1_move( cham_working_sta[6] ) ;
	d1_move( cham_working_sta[7] ) ;

	d1_move( door_status_data[0] ) ;
	d1_move( door_status_data[1] ) ;
	d1_move( door_status_data[2] ) ;
	d1_move( door_status_data[3] ) ;
	d1_move( door_status_data[4] ) ;
	d1_move( door_status_data[5] ) ;
	d1_move( door_status_data[6] ) ;
	d1_move( door_status_data[7] ) ;

	d2_move( cham_working_min[0]/60 );
	d2_move( cham_working_min[1]/60 );
	d2_move( cham_working_min[2]/60 );
	d2_move( cham_working_min[3]/60 );
	d2_move( cham_working_min[4]/60 );
	d2_move( cham_working_min[5]/60 );
	d2_move( cham_working_min[6]/60 );
	d2_move( cham_working_min[7]/60 );

	d1_move( co2_set_data ) ;
	d1_move( o2_set_data ) ;
	d1_move( co2_set_data ) ;
	d1_move( o2_set_data ) ;

	d1_move( press_data1 ) ;
	d2_move( voc_filter_time );

	d1_move( Error_no ) ;
	d1_move( Sub_error ) ;

	d1_move( check_sum );

	Tx1Buffer[i++] = 0xFE;

	Tx1No  = 6+73;
	receive_sta = 0 ;

	tx_enable();
}




#if 0
#define	T_max_tr		512 / T_packet_no

void Temp_set_log_Command1(void)
{
static uint8_t data_mode= 0;
static uint8_t check_sum;
static uint16_t data_pack_no;
static uint16_t remin_pack_no;
uint32_t adr_data,adr_point;
uint16_t i=0;
uint16_t trans_no=0;
union TRANS_BYTE dat;

	switch(data_mode)
	{
		case 0:
			check_sum = 0;

			data_pack_no = 0;
			remin_pack_no =	100;

			d1_move(0x53);
			d1_move(0x02);
			d2_move( Temp_set_cnt ) ;

			Tx1No  = 4;
			data_mode = 1;
			tx_enable();
			break;

		case 1:
			if 	(remin_pack_no > T_max_tr)	{ trans_no = T_max_tr;	remin_pack_no -= T_max_tr; }
			else if (remin_pack_no != 0)	{ trans_no = remin_pack_no;	remin_pack_no=0;		}
			else							{ data_mode = 2;		break;}		//  remin_pack_no==0 

			//check_sum = NAND_Read_data(data_pack_no, trans_no, Temp_set_Address, T_pack_no, T_rd_page, check_sum, Tx1Buffer);	//data packet = 10, 512/10 = 51

      
			data_pack_no += trans_no;

			Tx1No  = trans_no * T_packet_no;

			tx_enable();
			break;

		case 2:
			d1_move( check_sum );
			Tx1Buffer[i++] = 0xFE;

			Tx1No  = 2;
			data_mode = 0;
			receive_sta = 0 ;
			tx_enable();
			break;	
		}	
}



#define	M_max_tr		512 / M_pack_no

void main_log_Command1(void)
{
static uint8_t data_mode= 0;
static uint8_t check_sum;
static uint16_t data_pack_no;
static uint16_t remin_pack_no;
uint16_t i=0;
uint16_t trans_no=0;
union TRANS_BYTE dat;

	switch(data_mode)
	{
		case 0:
			check_sum = 0;

			data_pack_no = 0;
			remin_pack_no =	main_log_cnt;

			d1_move(0x53);
			d1_move(0x03);
			d2_move( main_log_cnt ) ;

			Tx1No  = 4;
			data_mode = 1;
			tx_enable();
			break;

		case 1:
			if 	(remin_pack_no > M_max_tr)	{ trans_no = M_max_tr;	remin_pack_no -= M_max_tr; }
			else if (remin_pack_no != 0)	{ trans_no = remin_pack_no;	remin_pack_no=0;		}
			else							{ data_mode = 2;		break;}		//  remin_pack_no==0 

			check_sum = NAND_Read_data(data_pack_no, trans_no, main_log_Address, M_pack_no, M_rd_page, check_sum, Tx1Buffer);	//data packet = 9, 512/9 = 56

			data_pack_no += trans_no;

			Tx1No  = trans_no * M_pack_no;

			tx_enable();
			break;

		case 2:
			d1_move( check_sum );
			Tx1Buffer[i++] = 0xFE;

			Tx1No  = 2;
			data_mode = 0;
			receive_sta = 0 ;
			tx_enable();
			break;	
		}	
}















#define	V_max_tr		512 / V_pack_no

void voc_change_Command1(void)
{
static uint8_t data_mode= 0;
static uint8_t check_sum;
static uint16_t data_pack_no;
static uint16_t remin_pack_no;
uint16_t i=0;
uint16_t trans_no=0;
union TRANS_BYTE dat;

	switch(data_mode)
	{
		case 0:
			check_sum = 0;

			data_pack_no = 0;
			remin_pack_no =	voc_change_cnt;

			d1_move(0x53);
			d1_move(0x04);
			d2_move( voc_change_cnt ) ;

			Tx1No  = 4;
			data_mode = 1;
			tx_enable();
			break;

		case 1:
			if 	(remin_pack_no > V_max_tr)	{ trans_no = V_max_tr;	remin_pack_no -= V_max_tr; }
			else if (remin_pack_no != 0)	{ trans_no = remin_pack_no;	remin_pack_no=0;		}
			else							{ data_mode = 2;		break;}		//  remin_pack_no==0 

			check_sum = NAND_Read_data(data_pack_no, trans_no, voc_change_Address, V_pack_no, V_rd_page, check_sum, Tx1Buffer);	//data packet = 9, 512/9 = 56

			data_pack_no += trans_no;

			Tx1No  = trans_no * V_pack_no;

			tx_enable();
			break;

		case 2:
			d1_move( check_sum );
			Tx1Buffer[i++] = 0xFE;

			Tx1No  = 2;
			data_mode = 0;
			receive_sta = 0 ;
			tx_enable();
			break;	
		}	
}









#define	A_max_tr		512 / A_pack_no

void alarm_log_Command1(void)
{
static uint8_t data_mode= 0;
static uint8_t check_sum;
static uint16_t data_pack_no;
static uint16_t remin_pack_no;
uint16_t i=0;
uint16_t trans_no=0;
union TRANS_BYTE dat;

	switch(data_mode)
	{
		case 0:
			check_sum = 0;

			data_pack_no = 0;
			remin_pack_no =	alarm_log_cnt;

			d1_move(0x53);
			d1_move(0x05);
			d2_move( alarm_log_cnt ) ;

			Tx1No  = 4;
			data_mode = 1;
			tx_enable();
			break;

		case 1:
			if 	(remin_pack_no > A_max_tr)	{ trans_no = A_max_tr;	remin_pack_no -= A_max_tr; }
			else if (remin_pack_no != 0)	{ trans_no = remin_pack_no;	remin_pack_no=0;		}
			else							{ data_mode = 2;		break;}		//  remin_pack_no==0 

			check_sum = NAND_Read_data(data_pack_no, trans_no, alarm_log_Address, A_pack_no, A_rd_page, check_sum, Tx1Buffer);	//data packet = 9, 512/9 = 56

			data_pack_no += trans_no;

			Tx1No  = trans_no * A_pack_no;

			tx_enable();
			break;

		case 2:
			d1_move( check_sum );
			Tx1Buffer[i++] = 0xFE;

			Tx1No  = 2;
			data_mode = 0;
			receive_sta = 0 ;
			tx_enable();
			break;	
		}	
}












#define	C_max_tr		512 / C_pack_no

void cham_temp_Command1(void)
{
static uint8_t data_mode= 0;
static uint8_t check_sum;
static uint16_t data_pack_no;
static uint16_t remin_pack_no;
uint16_t i=0;
uint16_t trans_no=0;
union TRANS_BYTE dat;

	switch(data_mode)
	{
		case 0:
			check_sum = 0;

			data_pack_no = 0;
			remin_pack_no =	cham_temp_cnt;

			d1_move(0x53);
			d1_move(0x06);
			d2_move( cham_temp_cnt ) ;

			Tx1No  = 4;
			data_mode = 1;
			tx_enable();
			break;

		case 1:
			if 	(remin_pack_no > C_max_tr)	{ trans_no = C_max_tr;	remin_pack_no -= C_max_tr; }
			else if (remin_pack_no != 0)	{ trans_no = remin_pack_no;	remin_pack_no=0;		}
			else							{ data_mode = 2;		break;}		//  remin_pack_no==0 

			check_sum = NAND_Read_data(data_pack_no, trans_no, cham_temp_Address, C_pack_no, C_rd_page, check_sum, Tx1Buffer);	//data packet = 9, 512/9 = 56

			data_pack_no += trans_no;

			Tx1No  = trans_no * C_pack_no;

			tx_enable();
			break;

		case 2:
			d1_move( check_sum );
			Tx1Buffer[i++] = 0xFE;

			Tx1No  = 2;
			data_mode = 0;
			receive_sta = 0 ;
			tx_enable();
			break;	
		}	
}






#define	G_max_tr		512 / G_pack_no

void gas_change_Command1(void)
{
static uint8_t data_mode= 0;
static uint8_t check_sum;
static uint16_t data_pack_no;
static uint16_t remin_pack_no;
uint16_t i=0;
uint16_t trans_no=0;
union TRANS_BYTE dat;

	switch(data_mode)
	{
		case 0:
			check_sum = 0;

			data_pack_no = 0;
			remin_pack_no =	gas_change_cnt;

			d1_move(0x53);
			d1_move(0x07);
			d2_move( gas_change_cnt ) ;

			Tx1No  = 4;
			data_mode = 1;
			tx_enable();
			break;

		case 1:
			if 	(remin_pack_no > G_max_tr)	{ trans_no = G_max_tr;	remin_pack_no -= G_max_tr; }
			else if (remin_pack_no != 0)	{ trans_no = remin_pack_no;	remin_pack_no=0;		}
			else							{ data_mode = 2;		break;}		//  remin_pack_no==0 

			check_sum = NAND_Read_data(data_pack_no, trans_no, gas_change_Address, G_pack_no, G_rd_page, check_sum, Tx1Buffer);	//data packet = 9, 512/9 = 56

			data_pack_no += trans_no;

			Tx1No  = trans_no * G_pack_no;

			tx_enable();
			break;

		case 2:
			d1_move( check_sum );
			Tx1Buffer[i++] = 0xFE;

			Tx1No  = 2;
			data_mode = 0;
			receive_sta = 0 ;
			tx_enable();
			break;	
		}	
}


#endif

void Control_Write(void)
{
	if ( receive_sta != 0 && Tx1Flag == 0 )
	{
		switch(receive_sta)
		{
			case 1:
				Command0();
				break;
/*
			case 2:
				//Temp_set_log_Command1();
				break;
			case 3:
				//main_log_Command1();
				break;
			case 4:
				//voc_change_Command1();
				break;
			case 5:
				//alarm_log_Command1();
				break;
			case 6:
				//cham_temp_Command1();
				break;
			case 7:
				//gas_change_Command1();
				break;
*/
			default:
				receive_sta = 0;
				break;
		}		
	}
}



void buffer_move(void)		//fifo software buffer
{
uint8_t num,num2,i;

	if (Rx1Index < 10)		// 10회 그 이상차면 버퍼 종료
	{
		num = 70;
		num2 = 63;

		for (i=0;i<63;i++)
		{
			Buffer[--num] = Buffer[--num2];
		}
		Buffer[0] = Rx1Buffer[0];
		Buffer[1] = Rx1Buffer[1];
		Buffer[2] = Rx1Buffer[2];
		Buffer[3] = Rx1Buffer[3];
		Buffer[4] = Rx1Buffer[4];
		Buffer[5] = Rx1Buffer[5];
		Buffer[6] = Rx1Buffer[6];

		Rx1Index++;	
	}
}


void buffer_out(uint8_t Tbuf[])		//fifo software buffer
{
uint8_t num;

	num = ( Rx1Index - 1) * 7;

	Tbuf[0] = Buffer[num++];
	Tbuf[1] = Buffer[num++];
	Tbuf[2] = Buffer[num++];
	Tbuf[3] = Buffer[num++];
	Tbuf[4] = Buffer[num++];
	Tbuf[5] = Buffer[num++];
	Tbuf[6] = Buffer[num++];

	Rx1Index--;	
}





void Control_Read(void)
{ 
uint8_t Temp_buf[7];

	if (Rx1Index != 0)
	{
		Rx1Counter = 0;
		Rx1Flag = 0;
		
		buffer_out(Temp_buf);

		if ( Temp_buf[0]==0x53 && Temp_buf[2]==0x01 && Temp_buf[3]==0x00 && Temp_buf[4]==0x00 && Temp_buf[6]==0xFE )
		{
			if ( Temp_buf[1]==0x01 && Temp_buf[5]==0x55)			receive_sta = 1;
			else if ( Temp_buf[1]==0x02 && Temp_buf[5]==0x56)		receive_sta = 2;
			else if ( Temp_buf[1]==0x03 && Temp_buf[5]==0x57)		receive_sta = 3;
			else if ( Temp_buf[1]==0x04 && Temp_buf[5]==0x58)		receive_sta = 4;
			else if ( Temp_buf[1]==0x05 && Temp_buf[5]==0x59)		receive_sta = 5;
			else if ( Temp_buf[1]==0x06 && Temp_buf[5]==0x5A)		receive_sta = 6;
			else if ( Temp_buf[1]==0x07 && Temp_buf[5]==0x5B)		receive_sta = 7;
			else receive_sta = 0;

		}
	}
}




/*
*********************************************************************************************************
*                                USART1 IRQ HANDLER FUNCTION
*
* Description: 시리얼 포트 1 인터럽트 처리 함수
*
* Arguments  : void
*
* Returns    : void
*********************************************************************************************************
*/
void USART1_IRQHandler(void)
{
uint8_t rec_data;
//////////////////////////////////////////////////////////////////////
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
//===========================================================================
		if (test_mode != 0)
		{
			test_Rx_data = USART_ReceiveData(USART1);
			return;
		}
//===========================================================================
		rec_data = USART_ReceiveData(USART1);

		if( Rx1Mode == 0 && rec_data == 0x53 )
		{
			Rx1Buffer[Rx1Counter++] = rec_data;
			Rx1Mode = 1;
		}
		else if ( Rx1Mode == 1)
		{
			Rx1Buffer[Rx1Counter++] = rec_data;
			if(	Rx1Counter == 7)
			{
				Rx1Mode = 0;	Rx1Counter = 0;	
				buffer_move();
			}
		}
		else
		{
			Rx1Mode = 0;	Rx1Counter = 0;

		}
	}
//////////////////////////////////////////////////////////////////////
	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{
		USART_SendData(USART1, Tx1Buffer[Tx1Counter++]);

		if(Tx1Counter == Tx1No)		/* Disable the USARTx Transmit interrupt */
		{
			USART_ClearITPendingBit(USART1, USART_IT_TXE);
			USART_ClearITPendingBit(USART1, USART_IT_TC);
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			USART_ITConfig(USART1, USART_IT_TC, ENABLE);
		}
	}
//////////////////////////////////////////////////////////////////////
	if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{
		USART_ITConfig(USART1, USART_IT_TC, DISABLE);
		Tx1Flag = 0; 
	}
}







/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void Control_Init(void)
{
  /* USARTx configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */

	
USART_InitTypeDef USART_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;


	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE);
//--------------------------------------------------------------
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

	// Configure USART Tx,Rx as alternate function  
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_10 ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10,  GPIO_AF_USART1); 
//--------------------------------------------------------------
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);

	// Enable the USARTx Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);

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
