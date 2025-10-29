/**
  ******************************************************************************
  * @file    fsmc_nand.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   This file provides a set of functions needed to drive the
*                      NAND512W3A2 memory mounted on STM3210E-EVAL board.
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
#include "data_io.h"
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE      0x200


#define		data_all_clear	1
#define		mem_def0		0x12
#define		mem_def1		0x34
#define		mem_def2		0x56
#define		mem_def3		0x78







union TRANS_BYTE
{
  uint16_t dat16;
  uint8_t dat8[2];
} ;


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//u8 NAND_Buffer[BUFFER_SIZE];


uint8_t Data_Temp[3*512] = {0,};
uint8_t NAND_Buffer[3*512]; 

uint16_t main_log_flag = 0 ;
uint16_t sub_log_flag1 = 0 ;
uint16_t sub_log_flag2 = 0 ;




                                    //start_Addr  ,  active_no,    active_max     block_no     packet_no     value_max     page_no    value_no
                                    //const                        const          const                      const        const    
																		//메모리시작주소       활성화된 모듈번호    이동가능모듈수       유효데이타필요블럭수  패킷당바이트수      최대유효자료수     유효데이타 페이지수  유효자료수
Memory_Flash  Set_Data_Memory     = { S_start_Addr,  S_active_no,  S_active_max,  S_block_no,  S_packet_no,  S_value_max,  S_page_no  ,S_value_no };
Memory_Flash  Data_Temp_Memory    = { D_start_Addr,  D_active_no,  D_active_max,  D_block_no,  D_packet_no,  D_value_max,  D_page_no  ,D_value_no };
Memory_Flash  Voc_change_Memory   = { V_start_Addr,  V_active_no,  V_active_max,  V_block_no,  V_packet_no,  V_value_max,  V_page_no  ,V_value_no };
Memory_Flash  Alarm_log_Memory    = { A_start_Addr,  A_active_no,  A_active_max,  A_block_no,  A_packet_no,  A_value_max,  A_page_no  ,A_value_no };
Memory_Flash  Gas_change_Memory   = { G_start_Addr,  G_active_no,  G_active_max,  G_block_no,  G_packet_no,  G_value_max,  G_page_no  ,G_value_no };
Memory_Flash  Temp_set_Memory     = { T_start_Addr,  T_active_no,  T_active_max,  T_block_no,  T_packet_no,  T_value_max,  T_page_no  ,T_value_no };
Memory_Flash  Main_log_Memory     = { M_start_Addr,  M_active_no,  M_active_max,  M_block_no,  M_packet_no,  M_value_max,  M_page_no  ,M_value_no };
Memory_Flash  Cham_temp_Memory    = { C_start_Addr,  C_active_no,  C_active_max,  C_block_no,  C_packet_no,  C_value_max,  C_page_no  ,C_value_no };


extern uint8_t  data_save_flag;

extern uint8_t	test_mode;


extern uint8_t time_year, time_week, time_month, time_date, time_hour, time_min, time_sec;
extern uint8_t voc_last_year, voc_last_month, voc_last_date, voc_last_hour, voc_last_min  ;					//전체 챔버 세팅값
extern uint8_t last_valid_time;


extern uint16_t	gas_initial_time;
extern uint16_t	gas_period_time;
extern uint16_t	gas_work_time;

extern uint16_t Press_on_data;
extern uint16_t Press_off_data;
extern uint16_t Press_err_data;
extern uint16_t Press_err_cnt;

extern uint16_t Press2_on_data;
extern uint16_t Press2_off_data;
extern uint16_t Press2_err_data;
extern uint16_t Press2_err_cnt;

extern uint16_t	o2_set_data ;						//o2 세팅 데이타
extern uint16_t	co2_set_data;						//co2 세팅 데이타

extern uint16_t	gas2_start_time ;
extern uint16_t	gas2_initial_time ;
extern uint16_t	gas2_period_time  ;
extern uint16_t	gas2_work_time ;
extern uint16_t	gas1_gas2_dual_time;


extern uint16_t	o2_2nd_set_data;						//o2 세팅 데이타
extern uint16_t	co2_2nd_set_data;						//co2 세팅 데이타
extern uint8_t	gas_2nd_on_off;						//



extern uint8_t wet_mode;
extern uint8_t wet_mode_temp;
extern uint8_t chamber_count;		//0 =8 chamber 1= 6 chamber


extern uint16_t cham_all_set_temp ;					//전체 챔버 세팅값
extern uint8_t	error_set_save[22];

extern uint16_t cham_working_min[8];		//시간 단위 -> 분
extern uint8_t 	cham_working_sta[8];		//배양상태 0= 빈상태 1=배양중


extern int16_t	 	voc_filter_time;	
extern uint8_t	 	voc_valid_time;
extern uint8_t 		change_alarm_time;

extern uint8_t		voc_change_data[60]	;		


extern uint16_t cham_now_temp[8] ;		//챔버 0~6 의 현재온도
extern uint16_t cham_set_temp[8] ;		//챔버 0~6 의 세팅온도


extern uint16_t temp_set_data[8] ;		//챔버 0~7 의 온도 세팅 값
extern uint16_t temp_set_input[8] ;		//챔버 0~7 의 센서 세팅 값


extern uint16_t Tempsett_NO,Voc_change_NO,Main_log_NO,Alarmout_NO,Temp_log_NO;

extern uint8_t	save_pass_data[4];

extern uint8_t device_name[4];

extern uint16_t ext_set_data ;
extern uint16_t ext_set_input ;

extern uint16_t temp_top_set_data[8];
extern uint16_t temp_top_set_input[8];
extern uint16_t temp_bot_set_data[8];
extern uint16_t temp_bot_set_input[8];


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void NAND_Data_Move_block(Memory_Flash *mem, uint8_t *buffer);
void NAND_Read_block(Memory_Flash *mem, uint8_t *buffer, uint16_t addr_adj, uint8_t read_page);
void NAND_erase_block(Memory_Flash *mem);
ErrorStatus NAND_find_block(Memory_Flash *mem);




//uint32_t start_Addr : 저장된 데이타 메모리 주소 기본값
//uint8_t active_no : 현재 데이타가 저장된 메모리 위치수(0~32)
//uint8_t active_max : 최대 메모리 위치수
//uint8_t block_no : 현 데이타 유효 데이타 블럭 수
//uint16_t packet_no : 버퍼의 사이즈
//uint16_t value_max ;최대 유효 자료수
//uint16_t page_no ;유효 페이지 수
//uint8_t *buffer : 저장될 버퍼

void NAND_Data_Move_block(Memory_Flash *mem, uint8_t *buffer)
{
NAND_ADDRESS ReadAddr, WriteAddr, TempAddr;
uint8_t temp_data[3*512];
uint16_t i,j,offset_block;;

  offset_block =    mem->active_no * mem->block_no ;     //현재 저장된(액티브)인 블럭 번호
  ReadAddr.Zone  =  mem->start_Addr / (NAND_BLOCK_SIZE * NAND_ZONE_SIZE) ;
	ReadAddr.Block =  ( mem->start_Addr / NAND_BLOCK_SIZE ) % NAND_ZONE_SIZE  + offset_block ;
	ReadAddr.Page  =  mem->start_Addr % NAND_BLOCK_SIZE ;

  mem->active_no++;
  if( mem->active_no == mem->active_max)   mem->active_no = 0;  //저장할(액티브)인 블럭 번호
  offset_block =     mem->active_no * mem->block_no ;
	WriteAddr.Zone  =  mem->start_Addr / (NAND_BLOCK_SIZE * NAND_ZONE_SIZE) ;
	WriteAddr.Block = ( mem->start_Addr / NAND_BLOCK_SIZE) % NAND_ZONE_SIZE  + offset_block ;
	WriteAddr.Page  =  mem->start_Addr % NAND_BLOCK_SIZE ;

  //쓸 블럭 erase
  TempAddr = WriteAddr;
	for(i=0; i < mem->block_no; i++)
	{
		FSMC_NAND_EraseBlock(TempAddr);		// Erase the NAND first Block 
		TempAddr.Block++;
	}
  
  //첫번재 페이지에는 페이지 액티브 데이타 저장
  TempAddr = WriteAddr;
  temp_data[0] = mem_def0;
  temp_data[1] = mem_def1;
  temp_data[2] = mem_def2;
  temp_data[3] = mem_def3;
	FSMC_NAND_WriteSmallPage( temp_data, TempAddr, 1);	

  //두번째 페이지부터 데이타 리드
  TempAddr = ReadAddr;

	for(j=0; j < mem->page_no; j++)
	{
    for(i=0; i< mem->packet_no; i++)
		{
			temp_data[i] = buffer[i];
		}

    FSMC_NAND_AddressIncrement(&TempAddr);
		FSMC_NAND_ReadSmallPage ( temp_data + mem->packet_no, TempAddr, 1);

    for(i=0; i< mem->packet_no; i++)
		{
			buffer[i] = temp_data[ NAND_PAGE_SIZE+i ];
		}

    FSMC_NAND_AddressIncrement(&WriteAddr);
		FSMC_NAND_WriteSmallPage( temp_data, WriteAddr, 1);	
	}

  //읽은 블럭 erase
  TempAddr = ReadAddr;
	for(i=0; i < mem->block_no; i++)
	{
		FSMC_NAND_EraseBlock(TempAddr);		// Erase the NAND first Block 
		TempAddr.Block++;
	}
}



//원하는 자료를 읽어오기
void NAND_Save_block(Memory_Flash *mem, uint8_t *buffer, uint16_t addr_adj, uint8_t save_page)
{
NAND_ADDRESS WriteAddr;
uint16_t offset_block;
uint8_t temp_data[4];

  offset_block =    mem->active_no * mem->block_no ;     //현재 저장된(액티브)인 블럭 번호
  
  WriteAddr.Zone  =  (mem->start_Addr + addr_adj) / (NAND_BLOCK_SIZE * NAND_ZONE_SIZE) ;
	WriteAddr.Block =  ( (mem->start_Addr + addr_adj) / NAND_BLOCK_SIZE ) % NAND_ZONE_SIZE  + offset_block ;
	WriteAddr.Page  =  (mem->start_Addr + addr_adj) % NAND_BLOCK_SIZE;    //첫번째 페이지는 인식 코드가 들어가있으므로 읽는 페이지 번호는 +1해준다

  temp_data[0] = mem_def0;
  temp_data[1] = mem_def1;
  temp_data[2] = mem_def2;
  temp_data[3] = mem_def3;
	FSMC_NAND_WriteSmallPage( temp_data, WriteAddr, 1);	
  
  FSMC_NAND_AddressIncrement(&WriteAddr);

	// Read back the written data
	FSMC_NAND_WriteSmallPage (buffer, WriteAddr, save_page);
}


//원하는 자료를 읽어오기
void NAND_Read_block(Memory_Flash *mem, uint8_t *buffer, uint16_t addr_adj, uint8_t read_page)
{
NAND_ADDRESS ReadAddr;
uint16_t offset_block;;

  offset_block =    mem->active_no * mem->block_no ;     //현재 저장된(액티브)인 블럭 번호

  ReadAddr.Zone  =  (mem->start_Addr + addr_adj) / (NAND_BLOCK_SIZE * NAND_ZONE_SIZE) ;
	ReadAddr.Block =  ( (mem->start_Addr + addr_adj) / NAND_BLOCK_SIZE ) % NAND_ZONE_SIZE  + offset_block ;
	ReadAddr.Page  =  (mem->start_Addr + addr_adj) % NAND_BLOCK_SIZE;    //첫번째 페이지는 인식 코드가 들어가있으므로 읽는 페이지 번호는 +1해준다

  FSMC_NAND_AddressIncrement(&ReadAddr);

	// Read back the written data
	FSMC_NAND_ReadSmallPage (buffer, ReadAddr, read_page);
}


void NAND_erase_block(Memory_Flash *mem)
{
NAND_ADDRESS WriteAddr, TempAddr;
uint16_t i,offset_block;
uint8_t temp_data[512]; 

  offset_block =    mem->active_no * mem->block_no ;     //현재 저장된(액티브)인 블럭 번호

  WriteAddr.Zone  =  mem->start_Addr / (NAND_BLOCK_SIZE * NAND_ZONE_SIZE) ;
	WriteAddr.Block =  ( mem->start_Addr / NAND_BLOCK_SIZE ) % NAND_ZONE_SIZE  + offset_block ;
	WriteAddr.Page  =  mem->start_Addr % NAND_BLOCK_SIZE ;

  //현재 블럭 erase
  TempAddr = WriteAddr;
	for(i=0; i < mem->block_no; i++)
	{
		FSMC_NAND_EraseBlock(TempAddr);		// Erase the NAND first Block 
		TempAddr.Block++;
	}

  //첫번재 페이지에는 페이지 액티브 데이타 저장
  TempAddr = WriteAddr;
  temp_data[0] = mem_def0;
  temp_data[1] = mem_def1;
  temp_data[2] = mem_def2;
  temp_data[3] = mem_def3;
	FSMC_NAND_WriteSmallPage( temp_data, TempAddr, 1);	
}


ErrorStatus NAND_find_block(Memory_Flash *mem)
{
NAND_ADDRESS ReadAddr;
uint16_t i;
uint8_t temp_data[512]; 

  ReadAddr.Zone  =  mem->start_Addr / (NAND_BLOCK_SIZE * NAND_ZONE_SIZE) ;
	ReadAddr.Block =  ( mem->start_Addr / NAND_BLOCK_SIZE ) % NAND_ZONE_SIZE ;
	ReadAddr.Page  =  mem->start_Addr % NAND_BLOCK_SIZE ;

	for(i=0; i< mem->active_max ;i++)
	{
		FSMC_NAND_ReadSmallPage ( temp_data, ReadAddr, 1);

		if( temp_data[0] == mem_def0 && temp_data[1] == mem_def1 && temp_data[2] == mem_def2 && temp_data[3] == mem_def3 )
		{
      mem->active_no = i;
			return SUCCESS;
		}

    ReadAddr.Block = ReadAddr.Block + mem->block_no;
	}

  mem->active_no = mem->active_max - 1; //데이타가 존재하지 않는다
  return ERROR;
}



/*====================================================================

	데이타 저장/로드

====================================================================*/
void Data_Initial(void)
{
	nand_id_check();
   
  Set_Data_find();
  Data_Temp_find();
  Voc_change_find();
  Alarm_log_find();
  Gas_change_find();
  Temp_set_find();
  Main_log_find();
  Cham_temp_find();
}




/*====================================================================

	세팅값 저장/로드

====================================================================*/
void Sett_data_initial(void)
{
uint8_t i;
  cham_set_temp[0] = 370;		//uint16
  cham_set_temp[1] = 370;
  cham_set_temp[2] = 370;
  cham_set_temp[3] = 370;
  cham_set_temp[4] = 370;
  cham_set_temp[5] = 370;
  cham_set_temp[6] = 370;
  cham_set_temp[7] = 370;
  cham_all_set_temp = 380;	//18

  cham_working_sta[0] = 0;	//uint8
  cham_working_sta[1] = 0;
  cham_working_sta[2] = 0;
  cham_working_sta[3] = 0;
  cham_working_sta[4] = 0;
  cham_working_sta[5] = 0;
  cham_working_sta[6] = 0;
  cham_working_sta[7] = 0;	//8

  cham_working_min[0] = 0;	//uint16
  cham_working_min[1] = 0;
  cham_working_min[2] = 0;
  cham_working_min[3] = 0;
  cham_working_min[4] = 0;
  cham_working_min[5] = 0;
  cham_working_min[6] = 0;
  cham_working_min[7] = 0;	//16

  temp_top_set_data[0] = 300;		//uint16_t
  temp_top_set_data[1] = 300;
  temp_top_set_data[2] = 300;
  temp_top_set_data[3] = 300;
  temp_top_set_data[4] = 300;
  temp_top_set_data[5] = 300;
  temp_top_set_data[6] = 300;
  temp_top_set_data[7] = 300;		//16

  temp_top_set_input[0] = 960;	//uint16_t
  temp_top_set_input[1] = 960;
  temp_top_set_input[2] = 960;
  temp_top_set_input[3] = 960;
  temp_top_set_input[4] = 960;
  temp_top_set_input[5] = 960;
  temp_top_set_input[6] = 960;
  temp_top_set_input[7] = 960;	//16

  temp_bot_set_data[0] = 300;		//uint16_t
  temp_bot_set_data[1] = 300;
  temp_bot_set_data[2] = 300;
  temp_bot_set_data[3] = 300;
  temp_bot_set_data[4] = 300;
  temp_bot_set_data[5] = 300;
  temp_bot_set_data[6] = 300;
  temp_bot_set_data[7] = 300;		//16

  temp_bot_set_input[0] = 960;	//uint16_t
  temp_bot_set_input[1] = 960;
  temp_bot_set_input[2] = 960;
  temp_bot_set_input[3] = 960;
  temp_bot_set_input[4] = 960;
  temp_bot_set_input[5] = 960;
  temp_bot_set_input[6] = 960;
  temp_bot_set_input[7] = 960;	//16

  gas_initial_time = 3;		//uint16_t
  gas_period_time = 20;
  gas_work_time = 5;
  Press_on_data = 70;
  Press_off_data = 90;
  Press_err_data = 2;
  Press_err_cnt = 30;

  Press2_on_data = 70;
  Press2_off_data = 90;
  Press2_err_data = 2;
  Press2_err_cnt = 30;

  o2_set_data = 50;
  co2_set_data = 50;			//14

  wet_mode = 0;				//uint8_t
  wet_mode_temp = 3;
  chamber_count= 0;

  voc_filter_time = 720;		//uint16_t
  voc_valid_time = 3;			//uint8_t
  change_alarm_time = 12;		//uint8_t

  voc_last_year	= 16;		//uint8_t
  voc_last_month  = 4;
  voc_last_date	= 30;
  voc_last_hour	= 00;
  voc_last_min	= 00;
  last_valid_time = 3;

  save_pass_data[0] = '0';		//uint8_t
  save_pass_data[1] = '0';
  save_pass_data[2] = '0';
  save_pass_data[3] = '0';

  device_name[0]	= 'C';		//uint8_t
  device_name[1]	= 'N';
  device_name[2]	= 'C';
  device_name[3]	= '0';

  ext_set_data = 300;
  ext_set_input = 960;

  o2_2nd_set_data= 50;					//uint16_t
  co2_2nd_set_data= 50;		
  gas_2nd_on_off = 0;

  gas2_start_time = 3 ;
  gas2_initial_time = 3;		//uint16_t
  gas2_period_time = 20;
  gas2_work_time = 5;
  gas1_gas2_dual_time = 0;

  for(i=0; i < 22 ; i++)		error_set_save[i] = 1 ;
}

void Set_Data_Write(void)
{
uint8_t i;
uint16_t t=0;
union TRANS_BYTE data_mul;
//-------------------------------------------------
	data_mul.dat16 = cham_set_temp[0];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = cham_set_temp[1];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = cham_set_temp[2];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = cham_set_temp[3];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = cham_set_temp[4];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = cham_set_temp[5];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = cham_set_temp[6];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = cham_set_temp[7];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = cham_all_set_temp;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
//-------------------------------------------------
	NAND_Buffer[t++] =  cham_working_sta[0];
	NAND_Buffer[t++] =  cham_working_sta[1];
	NAND_Buffer[t++] =  cham_working_sta[2];
	NAND_Buffer[t++] =  cham_working_sta[3];
	NAND_Buffer[t++] =  cham_working_sta[4];
	NAND_Buffer[t++] =  cham_working_sta[5];
	NAND_Buffer[t++] =  cham_working_sta[6];
	NAND_Buffer[t++] =  cham_working_sta[7];
//-------------------------------------------------
	data_mul.dat16 = cham_working_min[0];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = cham_working_min[1];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = cham_working_min[2];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = cham_working_min[3];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	
	data_mul.dat16 = cham_working_min[4];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	
	data_mul.dat16 = cham_working_min[5];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	
	data_mul.dat16 = cham_working_min[6];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	
	data_mul.dat16 = cham_working_min[7];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
//-------------------------------------------------
	data_mul.dat16 = temp_top_set_data[0];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_top_set_data[1];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_top_set_data[2];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_top_set_data[3];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_top_set_data[4];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_top_set_data[5];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_top_set_data[6];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_top_set_data[7];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
//-------------------------------------------------
	data_mul.dat16 = temp_top_set_input[0];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_top_set_input[1];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_top_set_input[2];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_top_set_input[3];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_top_set_input[4];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_top_set_input[5];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_top_set_input[6];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_top_set_input[7];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
//-------------------------------------------------
  data_mul.dat16 = temp_bot_set_data[0];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_bot_set_data[1];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	
	data_mul.dat16 = temp_bot_set_data[2];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_bot_set_data[3];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_bot_set_data[4];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_bot_set_data[5];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_bot_set_data[6];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_bot_set_data[7];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
//-------------------------------------------------
	data_mul.dat16 = temp_bot_set_input[0];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_bot_set_input[1];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_bot_set_input[2];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_bot_set_input[3];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_bot_set_input[4];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_bot_set_input[5];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_bot_set_input[6];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = temp_bot_set_input[7];	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
//------------------------------------------------- 
	data_mul.dat16 = gas_initial_time;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = gas_period_time;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = gas_work_time;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
//-------------------------------------------------
	data_mul.dat16 = Press_on_data;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = Press_off_data;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = Press_err_data;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = Press_err_cnt;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = Press2_on_data;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = Press2_off_data;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = Press2_err_data;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = Press2_err_cnt;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = o2_set_data;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = co2_set_data;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
//-------------------------------------------------
	NAND_Buffer[t++] =  wet_mode;
	NAND_Buffer[t++] =  wet_mode_temp;	
	NAND_Buffer[t++] =  chamber_count;

	data_mul.dat16 = voc_filter_time;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	NAND_Buffer[t++] =  voc_valid_time;
	NAND_Buffer[t++] =  change_alarm_time;	
//-------------------------------------------------
	NAND_Buffer[t++] =  voc_last_year;
	NAND_Buffer[t++] =  voc_last_month;
	NAND_Buffer[t++] =  voc_last_date;
	NAND_Buffer[t++] =  voc_last_hour;
	NAND_Buffer[t++] =  voc_last_min;
	NAND_Buffer[t++] =  last_valid_time;
//-------------------------------------------------
	NAND_Buffer[t++] =  save_pass_data[0];
	NAND_Buffer[t++] =  save_pass_data[1];
	NAND_Buffer[t++] =  save_pass_data[2];
	NAND_Buffer[t++] =  save_pass_data[3];

	NAND_Buffer[t++] =  device_name[0];
	NAND_Buffer[t++] =  device_name[1];
	NAND_Buffer[t++] =  device_name[2];
	NAND_Buffer[t++] =  device_name[3];
//-------------------------------------------------
	data_mul.dat16 = ext_set_data;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];

	data_mul.dat16 = ext_set_input;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
//-------------------------------------------------
	data_mul.dat16 = o2_2nd_set_data;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = co2_2nd_set_data;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
//-------------------------------------------------
	NAND_Buffer[t++] =  gas_2nd_on_off;

	data_mul.dat16 = gas2_start_time;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = gas2_initial_time;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = gas2_period_time;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = gas2_work_time;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
	data_mul.dat16 = gas1_gas2_dual_time;	
	NAND_Buffer[t++] =  data_mul.dat8[0];
	NAND_Buffer[t++] =  data_mul.dat8[1];
//-------------------------------------------------
	for(i=0; i < 22 ; i++)
	{
		NAND_Buffer[t++] =  error_set_save[i];
	}
//-------------------------------------------------
//-------------------------------------------------
  NAND_Data_Move_block(&Set_Data_Memory, NAND_Buffer);

}


void Set_Data_Read(void)
{
uint8_t i;
uint16_t t=0;
union TRANS_BYTE data_mul;
  
  NAND_Read_block(&Set_Data_Memory, NAND_Buffer, 0, S_rd_page);

//-------------------------------------------------
	data_mul.dat8[0]= NAND_Buffer[t++]; 	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_set_temp[0] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++]; 	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_set_temp[1] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_set_temp[2] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_set_temp[3] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_set_temp[4] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_set_temp[5] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_set_temp[6] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_set_temp[7] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_all_set_temp = data_mul.dat16 ;
//-------------------------------------------------
	cham_working_sta[0] = NAND_Buffer[t++] ;
	cham_working_sta[1] = NAND_Buffer[t++] ;
	cham_working_sta[2] = NAND_Buffer[t++] ;
	cham_working_sta[3] = NAND_Buffer[t++] ;
	cham_working_sta[4] = NAND_Buffer[t++] ;
	cham_working_sta[5] = NAND_Buffer[t++] ;
	cham_working_sta[6] = NAND_Buffer[t++] ;
	cham_working_sta[7] = NAND_Buffer[t++] ;
//-------------------------------------------------
	data_mul.dat8[0]= NAND_Buffer[t++]; 	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_working_min[0] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_working_min[1] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_working_min[2] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_working_min[3] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_working_min[4] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_working_min[5] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_working_min[6] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_working_min[7] = data_mul.dat16 ;
//-------------------------------------------------
	data_mul.dat8[0]= NAND_Buffer[t++]; 	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_data[0] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_data[1] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_data[2] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_data[3] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_data[4] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_data[5] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_data[6] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_data[7] = data_mul.dat16 ;
//-------------------------------------------------
	data_mul.dat8[0]= NAND_Buffer[t++]; 	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_input[0] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_input[1] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_input[2] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_input[3] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_input[4] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_input[5] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_input[6] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_input[7] = data_mul.dat16 ;
//-------------------------------------------------
	data_mul.dat8[0]= NAND_Buffer[t++]; 	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_data[0] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_data[1] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_data[2] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_data[3] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_data[4] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_data[5] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_data[6] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_data[7] = data_mul.dat16 ;
//-------------------------------------------------
	data_mul.dat8[0]= NAND_Buffer[t++]; 	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_input[0] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_input[1] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_input[2] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_input[3] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_input[4] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_input[5] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_input[6] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_input[7] = data_mul.dat16 ;
  //-------------------------------------------------
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	gas_initial_time = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	gas_period_time = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	gas_work_time = data_mul.dat16 ;

	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	Press_on_data = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	Press_off_data = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	Press_err_data = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	Press_err_cnt = data_mul.dat16 ;

	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	Press2_on_data = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	Press2_off_data = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	Press2_err_data = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	Press2_err_cnt = data_mul.dat16 ;

	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	o2_set_data = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	co2_set_data = data_mul.dat16 ;
//-------------------------------------------------
	wet_mode = NAND_Buffer[t++] ;
	wet_mode_temp = NAND_Buffer[t++] ;
	chamber_count = NAND_Buffer[t++] ;
//-------------------------------------------------
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	voc_filter_time = data_mul.dat16 ;

	voc_valid_time = NAND_Buffer[t++] ;
	change_alarm_time = NAND_Buffer[t++] ;
//-------------------------------------------------
	voc_last_year =  NAND_Buffer[t++] ;
	voc_last_month = NAND_Buffer[t++] ;
	voc_last_date =  NAND_Buffer[t++] ;
	voc_last_hour =  NAND_Buffer[t++] ;
	voc_last_min = 	 NAND_Buffer[t++] ;
	last_valid_time = NAND_Buffer[t++] ;
//-------------------------------------------------
	save_pass_data[0] =  NAND_Buffer[t++] ;
	save_pass_data[1] =  NAND_Buffer[t++] ;
	save_pass_data[2] =  NAND_Buffer[t++] ;
	save_pass_data[3] =  NAND_Buffer[t++] ;

	device_name[0] =  NAND_Buffer[t++] ;
	device_name[1] =  NAND_Buffer[t++] ;
	device_name[2] =  NAND_Buffer[t++] ;
	device_name[3] =  NAND_Buffer[t++] ;
//-------------------------------------------------
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	ext_set_data = data_mul.dat16 ;

	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	ext_set_input = data_mul.dat16 ;
//-------------------------------------------------
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	o2_2nd_set_data = data_mul.dat16 ;

	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	co2_2nd_set_data = data_mul.dat16 ;

	gas_2nd_on_off =  NAND_Buffer[t++] ;
//-------------------------------------------------
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	gas2_start_time = data_mul.dat16 ;

	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	gas2_initial_time = data_mul.dat16 ;

	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	gas2_period_time = data_mul.dat16 ;

	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	gas2_work_time = data_mul.dat16 ;

	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	gas1_gas2_dual_time = data_mul.dat16 ;
//-------------------------------------------------
	for(i=0; i < 22 ; i++)
	{
		error_set_save[i] =  NAND_Buffer[t++] ;
	}
//-------------------------------------------------
}


ErrorStatus Set_Data_Read2(void)
{
uint8_t i;
uint16_t t=0;
union TRANS_BYTE data_mul;
NAND_ADDRESS ReadAddr;

  ReadAddr.Zone  =  0 ;
	ReadAddr.Block =  0 ;
	ReadAddr.Page  =  0 ;
	FSMC_NAND_ReadSmallPage (NAND_Buffer, ReadAddr, 1);

  if(NAND_Buffer[0] == 0xff)
  {
    return ERROR; 
  }

	t++;
	data_mul.dat8[0]= NAND_Buffer[t++]; data_mul.dat8[1]= NAND_Buffer[t++];
	cham_set_temp[0] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_set_temp[1] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_set_temp[2] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_set_temp[3] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_set_temp[4] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_set_temp[5] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_set_temp[6] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_set_temp[7] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	cham_all_set_temp = data_mul.dat16 ;
//-------------------------------------------------
//-------------------------------------------------
  t = t+24+16+16+26+29+59;
//-------------------------------------------------

  if ( NAND_Buffer[t] == 0xff )    return ERROR; 

//-------------------------------------------------
	data_mul.dat8[0]= NAND_Buffer[t++]; data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_data[0] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_data[1] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_data[2] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_data[3] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_data[4] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_data[5] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_data[6] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_data[7] = data_mul.dat16 ;
//-------------------------------------------------
	data_mul.dat8[0]= NAND_Buffer[t++]; data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_input[0] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_input[1] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_input[2] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_input[3] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_input[4] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_input[5] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_input[6] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_top_set_input[7] = data_mul.dat16 ;
//-------------------------------------------------
	data_mul.dat8[0]= NAND_Buffer[t++]; 	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_data[0] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_data[1] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_data[2] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_data[3] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_data[4] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_data[5] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_data[6] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_data[7] = data_mul.dat16 ;
//-------------------------------------------------
	data_mul.dat8[0]= NAND_Buffer[t++]; 	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_input[0] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_input[1] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_input[2] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_input[3] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_input[4] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_input[5] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_input[6] = data_mul.dat16 ;
	data_mul.dat8[0]= NAND_Buffer[t++];	data_mul.dat8[1]= NAND_Buffer[t++];
	temp_bot_set_input[7] = data_mul.dat16 ;
//-------------------------------------------------

/*  cham_set_temp[0] = 370;		//uint16
  cham_set_temp[1] = 370;
  cham_set_temp[2] = 370;
  cham_set_temp[3] = 370;
  cham_set_temp[4] = 370;
  cham_set_temp[5] = 370;
  cham_set_temp[6] = 370;
  cham_set_temp[7] = 370;
  cham_all_set_temp = 380;	//18
*/
  cham_working_sta[0] = 0;	//uint8
  cham_working_sta[1] = 0;
  cham_working_sta[2] = 0;
  cham_working_sta[3] = 0;
  cham_working_sta[4] = 0;
  cham_working_sta[5] = 0;
  cham_working_sta[6] = 0;
  cham_working_sta[7] = 0;	//8

  cham_working_min[0] = 0;	//uint16
  cham_working_min[1] = 0;
  cham_working_min[2] = 0;
  cham_working_min[3] = 0;
  cham_working_min[4] = 0;
  cham_working_min[5] = 0;
  cham_working_min[6] = 0;
  cham_working_min[7] = 0;	//16

  gas_initial_time = 3;		//uint16_t
  gas_period_time = 20;
  gas_work_time = 5;
  Press_on_data = 70;
  Press_off_data = 90;
  Press_err_data = 2;
  Press_err_cnt = 30;

  Press2_on_data = 70;
  Press2_off_data = 90;
  Press2_err_data = 2;
  Press2_err_cnt = 30;

  o2_set_data = 50;
  co2_set_data = 50;			//14

  wet_mode = 0;				//uint8_t
  wet_mode_temp = 3;
  chamber_count= 0;

  voc_filter_time = 720;		//uint16_t
  voc_valid_time = 3;			//uint8_t
  change_alarm_time = 12;		//uint8_t

  voc_last_year	= 16;		//uint8_t
  voc_last_month  = 4;
  voc_last_date	= 30;
  voc_last_hour	= 00;
  voc_last_min	= 00;
  last_valid_time = 3;

  save_pass_data[0] = '0';		//uint8_t
  save_pass_data[1] = '0';
  save_pass_data[2] = '0';
  save_pass_data[3] = '0';

  device_name[0]	= 'C';		//uint8_t
  device_name[1]	= 'N';
  device_name[2]	= 'C';
  device_name[3]	= '0';

  ext_set_data = 300;
  ext_set_input = 960;

  o2_2nd_set_data= 50;					//uint16_t
  co2_2nd_set_data= 50;		
  gas_2nd_on_off = 0;

  gas2_start_time = 3 ;
  gas2_initial_time = 3;		//uint16_t
  gas2_period_time = 20;
  gas2_work_time = 5;
  gas1_gas2_dual_time = 0;

  for(i=0; i < 22 ; i++)		error_set_save[i] = 1 ;
//-------------------------------------------------
  return SUCCESS;
}










void Set_Data_find(void)
{
ErrorStatus flag;

  flag = NAND_find_block(&Set_Data_Memory);

  if ( flag == ERROR) 
  {
    if ( Set_Data_Read2() == ERROR)   Sett_data_initial();
    Set_Data_Write();
  }
  else
  {
    Set_Data_Read();
  }
}



/*====================================================================

	온도값 저장/로드

====================================================================*/
void make_Data_Temp(void)
{
uint16_t i;

	Data_Temp[0] = 1;
	Data_Temp[1] = time_year;
	Data_Temp[2] = time_month;
	Data_Temp[3] = time_date;

	for( i = 4; i < 3*512 ; i++)
	{
		Data_Temp[i] = 0xFF;
	}
}


void Data_Temp_erase(void)
{
  NAND_erase_block(&Data_Temp_Memory);
  make_Data_Temp();
}

void Data_Temp_find(void)
{
  NAND_find_block(&Data_Temp_Memory);
}


void Data_Temp_data_move(void)
{
  make_Data_Temp();
  NAND_Data_Move_block(&Data_Temp_Memory, Data_Temp);
}


void Data_Temp_data_small_save(void)
{
uint16_t i;
  NAND_Read_block(&Data_Temp_Memory, NAND_Buffer, 0, 2);

  Data_Temp[0] = 1;
  Data_Temp[1] = time_year;
  Data_Temp[2] = time_month;
  Data_Temp[3] = time_date;

  for (i=0; i<772; i++)
  {
      NAND_Buffer[i] = Data_Temp[i];
  }

  NAND_Save_block(&Data_Temp_Memory, NAND_Buffer, 0, D_rd_page);
}



void Data_Temp_Write(void)
{
uint16_t i;
static uint16_t save_flag=0, save_en=0;
static uint16_t avr_now_temp[8] = {0,0,0,0,0,0,0,0} ;
static uint16_t save_now_temp[8] = {0,0,0,0,0,0,0,0} ;
static uint16_t avr_cnt = 0;
//===========================================================================
//===========================================================================
	if(save_en == 0)  //맨 처음 전원 켜질때
	{
		save_en = 1;

    NAND_Read_block(&Data_Temp_Memory, NAND_Buffer, 0, D_rd_page);

    if(NAND_Buffer[0] != 1)
    {
    make_Data_Temp();
	}
    else
    {
      if( NAND_Buffer[1] ==  time_year &&  NAND_Buffer[2] ==  time_month  &&  NAND_Buffer[3] ==  time_date )
      {
        for (i=0; i<772; i++)
        {
            Data_Temp[i] = NAND_Buffer[i];
        }
      }
      else
      {
        Data_Temp_data_move();
      }
    }
	}
//===========================================================================
	if( time_min==0 && save_flag == 0)
	{
		save_flag =1;

//------------------------------------------
		if ( time_hour==0 )		//0시 0분 0초 이면 기존 자료 이동
		{
			Data_Temp_data_move();
		}
//------------------------------------------
//		avr_cnt++;
//
//		for(i=0;i<8;i++)
//		{
//			avr_now_temp[i] += 	cham_now_temp[i];		//챔버 0~7 의 현재온도
//		}
//------------------------------------------
//			for(i=0;i<8;i++)
//			{
//				save_now_temp[i] = avr_now_temp[i] / avr_cnt;		//챔버 0~7 의 현재온도
//				avr_now_temp[i] = 0;
//			}	
//			avr_cnt= 0;
//------------------------------------------
			for(i=0;i<8;i++)
			{
				Data_Temp[4+(32*time_hour)+ 4*i+ 0] = 	cham_now_temp[i]&0xff;
				Data_Temp[4+(32*time_hour)+ 4*i+ 1] = 	cham_now_temp[i]>>8;
				Data_Temp[4+(32*time_hour)+ 4*i+ 2] = 	cham_set_temp[i]&0xff;
				Data_Temp[4+(32*time_hour)+ 4*i+ 3] = 	cham_set_temp[i]>>8;
			}

      Data_Temp_data_small_save();
//------------------------------------------

		}
	else if ( time_min != 0  ) { save_flag = 0; }
}



void Data_Temp_Read(uint16_t data_no, uint8_t ch_no_dat ,uint8_t *load_data1, uint16_t *load_data2, uint16_t *load_data3)
{
uint8_t i,j;
uint16_t  t;
uint32_t adr_data,adr_point;
union TRANS_BYTE temp1_d, temp2_d;
/*
  if(data_no == 0)  //0번 데이타의 경우 메모리에서 가져온다
  {
    t = 0;

    *load_data1++ = Data_Temp[t++];	//저장여부
    *load_data1++ = Data_Temp[t++];	//year
    *load_data1++ = Data_Temp[t++];	//month
    *load_data1++ = Data_Temp[t++];	//date

    for (i=0;i<24;i++)
    {
      for (j=0;j<8;j++)
      {
        temp1_d.dat8[0] = Data_Temp[t++] ;
        temp1_d.dat8[1] = Data_Temp[t++] ;
        temp2_d.dat8[0] = Data_Temp[t++] ;
        temp2_d.dat8[1] = Data_Temp[t++] ;

        if(j == ch_no_dat) 
        {
            *load_data2++ = temp1_d.dat16 ;
            *load_data3++ = temp2_d.dat16 ;
        }
      }
    }   
  }
  else
  {
    data_no--;    //flash에서는 1이상 데이타 가져온다
*/
    adr_data = data_no * D_packet_no / NAND_PAGE_SIZE  ;
    adr_point = data_no * D_packet_no % NAND_PAGE_SIZE;

    NAND_Read_block(&Data_Temp_Memory, NAND_Buffer, adr_data, D_rd_page);

    *load_data1++ = NAND_Buffer[adr_point++];	//저장여부
    *load_data1++ = NAND_Buffer[adr_point++];	//year
    *load_data1++ = NAND_Buffer[adr_point++];	//month
    *load_data1++ = NAND_Buffer[adr_point++];	//date

    for (i=0;i<24;i++)
    {
      for (j=0;j<8;j++)
      {
        temp1_d.dat8[0] = NAND_Buffer[adr_point++] ;
        temp1_d.dat8[1] = NAND_Buffer[adr_point++] ;
        temp2_d.dat8[0] = NAND_Buffer[adr_point++] ;
        temp2_d.dat8[1] = NAND_Buffer[adr_point++] ;

        if(j == ch_no_dat) 
        {
            *load_data2++ = temp1_d.dat16 ;
            *load_data3++ = temp2_d.dat16 ;
        }
      }
    }
//  }
}


/*====================================================================

	voc 교체 로그 저장/로드

====================================================================*/
void Voc_change_erase(void)
{
  NAND_erase_block(&Voc_change_Memory);
}

void Voc_change_find(void)
{
  NAND_find_block(&Voc_change_Memory);
}


void Voc_change_Write(void)
{
//-------------------------------------------------------
	voc_last_year = 	time_year;
	voc_last_month = 	time_month;
	voc_last_date = 	time_date;
	voc_last_hour = 	time_hour;
	voc_last_min = 		time_min;
	last_valid_time = 	voc_valid_time;
//-------------------------------------------------------
	NAND_Buffer[0] = 	1;
	NAND_Buffer[1] = 	time_year;
	NAND_Buffer[2] = 	time_month;
	NAND_Buffer[3] = 	time_date;
	NAND_Buffer[4] = 	time_hour;
	NAND_Buffer[5] = 	time_min;
	NAND_Buffer[6] = 	time_sec;
	NAND_Buffer[7] = 	voc_valid_time;

	NAND_Data_Move_block(&Voc_change_Memory, NAND_Buffer);
}


void Voc_change_Read(uint16_t data_no, uint8_t data_cnt, uint8_t *load1)
{
uint8_t i;
uint32_t adr_data,adr_point;

	adr_data = data_no * V_packet_no / NAND_PAGE_SIZE  ;
	adr_point = data_no * V_packet_no % NAND_PAGE_SIZE;
   
  NAND_Read_block(&Voc_change_Memory, NAND_Buffer, adr_data, V_rd_page);

	for (i=0;i<data_cnt;i++)
	{
		*load1++ = NAND_Buffer[adr_point++] ;
		*load1++ = NAND_Buffer[adr_point++] ;
		*load1++ = NAND_Buffer[adr_point++] ;
		*load1++ = NAND_Buffer[adr_point++] ;
		*load1++ = NAND_Buffer[adr_point++] ;
		*load1++ = NAND_Buffer[adr_point++] ;
		*load1++ = NAND_Buffer[adr_point++] ;
		*load1++ = NAND_Buffer[adr_point++] ;
	}
}

/*====================================================================

	ALARM 로그 저장/로드

====================================================================*/
void Alarm_log_erase(void)
{
  NAND_erase_block(&Alarm_log_Memory);
}

void Alarm_log_find(void)
{
  NAND_find_block(&Alarm_log_Memory);
}


//LOAD1 ; ERROER 상태
//LOAD2 ;발생시간
//LOAD3 ;해제시간
//
void Alarm_log_Write(uint8_t *load1, uint8_t *load2, uint8_t *load3)
{
//-------------------------------------------------------
	NAND_Buffer[0] = 	1;
	NAND_Buffer[1] = 	*load1++;
	NAND_Buffer[2] = 	*load1++;

	NAND_Buffer[3] = 	*load2++;
	NAND_Buffer[4] = 	*load2++;
	NAND_Buffer[5] = 	*load2++;
	NAND_Buffer[6] = 	*load2++;
	NAND_Buffer[7] = 	*load2++;
	NAND_Buffer[8] = 	*load2++;

	NAND_Buffer[9] = *load3++;
	NAND_Buffer[10] = *load3++;
	NAND_Buffer[11] = *load3++;
	NAND_Buffer[12] = *load3++;
	NAND_Buffer[13] = *load3++;
	NAND_Buffer[14] = *load3++;

	NAND_Data_Move_block(&Alarm_log_Memory, NAND_Buffer);
}


void Alarm_log_Read(uint16_t data_no, uint8_t data_cnt, uint8_t *load1, uint8_t *load2, uint8_t *load3)
{
uint8_t i;
uint32_t adr_data,adr_point;

	adr_data = data_no * A_packet_no / NAND_PAGE_SIZE  ;
	adr_point = data_no * A_packet_no % NAND_PAGE_SIZE;

  NAND_Read_block(&Alarm_log_Memory, NAND_Buffer, adr_data, A_rd_page);

	for (i=0;i<data_cnt;i++)
	{
		*load1++ = NAND_Buffer[adr_point++] ;
		*load1++ = NAND_Buffer[adr_point++] ;
		*load1++ = NAND_Buffer[adr_point++] ;

		*load2++ = NAND_Buffer[adr_point++] ;
		*load2++ = NAND_Buffer[adr_point++] ;
		*load2++ = NAND_Buffer[adr_point++] ;
		*load2++ = NAND_Buffer[adr_point++] ;
		*load2++ = NAND_Buffer[adr_point++] ;
		*load2++ = NAND_Buffer[adr_point++] ;

		*load3++ = NAND_Buffer[adr_point++] ;
		*load3++ = NAND_Buffer[adr_point++] ;
		*load3++ = NAND_Buffer[adr_point++] ;
		*load3++ = NAND_Buffer[adr_point++] ;
		*load3++ = NAND_Buffer[adr_point++] ;
		*load3++ = NAND_Buffer[adr_point++] ;
	}
}



/*====================================================================

	co2/o2 교체 로그 저장/로드

====================================================================*/
void Gas_change_erase(void)
{
  NAND_erase_block(&Gas_change_Memory);
}


void Gas_change_find(void)
{
  NAND_find_block(&Gas_change_Memory);
}


void Gas_change_Write(uint8_t gas_sel, uint8_t gas_val)
{
//-------------------------------------------------------
	NAND_Buffer[0] = 	1;
	NAND_Buffer[1] = 	gas_sel;	//1= co2_set_data , 2=o2_set_data
	NAND_Buffer[2] = 	gas_val;

	NAND_Buffer[3] = 	time_year;
	NAND_Buffer[4] = 	time_month;
	NAND_Buffer[5] = 	time_date;
	NAND_Buffer[6] = 	time_hour;
	NAND_Buffer[7] = 	time_min;
	NAND_Buffer[8] = 	time_sec;
  
	NAND_Data_Move_block(&Gas_change_Memory, NAND_Buffer);
}


void Gas_change_Read(uint16_t data_no, uint8_t data_cnt, uint8_t *load1, uint8_t *load2)
{
uint8_t i;
uint32_t adr_data,adr_point;

	adr_data = data_no * G_packet_no / NAND_PAGE_SIZE  ;
	adr_point = data_no * G_packet_no % NAND_PAGE_SIZE;

  NAND_Read_block(&Gas_change_Memory, NAND_Buffer, adr_data, G_rd_page);

	for (i=0;i<data_cnt;i++)
	{
		*load1++ = NAND_Buffer[adr_point++] ;
		*load1++ = NAND_Buffer[adr_point++] ;
		*load1++ = NAND_Buffer[adr_point++] ;

		*load2++ = NAND_Buffer[adr_point++] ;
		*load2++ = NAND_Buffer[adr_point++] ;
		*load2++ = NAND_Buffer[adr_point++] ;
		*load2++ = NAND_Buffer[adr_point++] ;
		*load2++ = NAND_Buffer[adr_point++] ;
		*load2++ = NAND_Buffer[adr_point++] ;
	}
}




/*====================================================================

	온도세팅 로그 저장/로드

====================================================================*/
void Temp_set_erase(void)
{
  NAND_erase_block(&Temp_set_Memory);
}


void Temp_set_find(void)
{
  NAND_find_block(&Temp_set_Memory);
}


void Temp_set_Write(uint8_t *set_flag, uint8_t *set_data, uint16_t *setting_val)
{
//-------------------------------------------------------
uint8_t i,j;
uint16_t temp_dat;
uint8_t	dat_no = 0;
uint8_t temp[8] = {0,0,0,0,0,0,0,0};
uint8_t out_no[8] = {0,0,0,0,0,0,0,0};
uint16_t out_data[8] = {0,0,0,0,0,0,0,0};
//-------------------------------------------------------
  if(set_flag[0] == 0)  //온도세팅
  {
    for(i=0;i<8;i++)
    {
      if (set_data[i]==0x01 && temp[i]==0x00 )
      {
        temp_dat = setting_val[i];

        for(j=i;j<8;j++)
        {
          if(setting_val[j] == temp_dat && set_data[j]==0x01 )
          {
            out_data[dat_no] = temp_dat;
            temp[j]=1;
            out_no[dat_no] += (1<<j) ;
          }
        }
        dat_no++;
      }
    }
    
    for(i=0;i<dat_no;i++)
    {
      NAND_Buffer[10*i+0] = 	set_flag[0];
      NAND_Buffer[10*i+1] = 	out_no[i];
      NAND_Buffer[10*i+2] = 	out_data[i]&0xff;
      NAND_Buffer[10*i+3] = 	out_data[i]>>8;
      NAND_Buffer[10*i+4] = 	time_year;
      NAND_Buffer[10*i+5] = 	time_month;
      NAND_Buffer[10*i+6] = 	time_date;
      NAND_Buffer[10*i+7] = 	time_hour;
      NAND_Buffer[10*i+8] = 	time_min;
      NAND_Buffer[10*i+9] = 	time_sec;
    }
    
  }
  else  //캘리브레이션
  {
    dat_no = 1;

    NAND_Buffer[0] = 	set_flag[0]; 
    NAND_Buffer[1] = 	set_flag[1];    //챔버
    NAND_Buffer[2] = 	setting_val[0]&0xff; //온도
    NAND_Buffer[3] = 	setting_val[0]>>8;
    NAND_Buffer[4] = 	time_year;
    NAND_Buffer[5] = 	time_month;
    NAND_Buffer[6] = 	time_date;
    NAND_Buffer[7] = 	time_hour;
    NAND_Buffer[8] = 	time_min;
    NAND_Buffer[9] = 	time_sec; 
  }
  
  Temp_set_Memory.packet_no = 10*dat_no;

	NAND_Data_Move_block(&Temp_set_Memory, NAND_Buffer);
}



void Temp_set_Read(uint16_t data_no, uint8_t data_cnt, uint8_t *mode_data, uint8_t *cham_data, uint16_t *set_data, uint8_t *time_data)
{
uint8_t i;
uint32_t adr_data,adr_point;
union TRANS_BYTE sett_load;

	adr_data = data_no * T_packet_no / NAND_PAGE_SIZE ;
	adr_point = data_no * T_packet_no % NAND_PAGE_SIZE ;

  NAND_Read_block(&Temp_set_Memory, NAND_Buffer, adr_data, G_rd_page);

	for (i=0;i<data_cnt;i++)
	{
    *mode_data++ = NAND_Buffer[adr_point++] ;	
		*cham_data++ = NAND_Buffer[adr_point++] ;	

		sett_load.dat8[0] = NAND_Buffer[adr_point++] ;
		sett_load.dat8[1] = NAND_Buffer[adr_point++] ;
		*set_data++ = sett_load.dat16;

		*time_data++ = NAND_Buffer[adr_point++] ;
		*time_data++ = NAND_Buffer[adr_point++] ;
		*time_data++ = NAND_Buffer[adr_point++] ;
		*time_data++ = NAND_Buffer[adr_point++] ;
		*time_data++ = NAND_Buffer[adr_point++] ;
		*time_data++ = NAND_Buffer[adr_point++] ;
	}
}


/*====================================================================

	전체 로그 저장/로드

====================================================================*/
void Main_log_erase(void)
{
  NAND_erase_block(&Main_log_Memory);
}


void Main_log_find(void)
{
  NAND_find_block(&Main_log_Memory);
}



void Main_log_Write(void)
{
//-------------------------------------------------------
	if(main_log_flag != 0x00)
	{
		NAND_Buffer[0] = 	1;		          //out_data[i]
		NAND_Buffer[1] = 	main_log_flag;		//out_data[i]
		NAND_Buffer[2] =	sub_log_flag1;
		NAND_Buffer[3] = 	sub_log_flag2;

		NAND_Buffer[4] = 	time_year;
		NAND_Buffer[5] = 	time_month;
		NAND_Buffer[6] = 	time_date;
		NAND_Buffer[7] = 	time_hour;
		NAND_Buffer[8] = 	time_min;
		NAND_Buffer[9] = 	time_sec;

    NAND_Data_Move_block(&Main_log_Memory, NAND_Buffer);

		main_log_flag = 0x00;
		sub_log_flag1 = 0x00;
		sub_log_flag2 = 0x00;
	}
}



void Main_log_Read(uint16_t data_no, uint8_t data_cnt, uint8_t *load1, uint8_t *load2)
{
uint8_t i;
uint32_t adr_data,adr_point;

	adr_data = data_no * M_packet_no / NAND_PAGE_SIZE  ;
	adr_point = data_no * M_packet_no % NAND_PAGE_SIZE;

  NAND_Read_block(&Main_log_Memory, NAND_Buffer, adr_data, M_rd_page);

	for (i=0;i<data_cnt;i++)
	{
		*load1++ = NAND_Buffer[adr_point++] ;
		*load1++ = NAND_Buffer[adr_point++] ;
		*load1++ = NAND_Buffer[adr_point++] ;
		*load1++ = NAND_Buffer[adr_point++] ;

		*load2++ = NAND_Buffer[adr_point++] ;
		*load2++ = NAND_Buffer[adr_point++] ;
		*load2++ = NAND_Buffer[adr_point++] ;
		*load2++ = NAND_Buffer[adr_point++] ;
		*load2++ = NAND_Buffer[adr_point++] ;
		*load2++ = NAND_Buffer[adr_point++] ;
	}
}


/*====================================================================

	세팅 현재 온도 변화 로그 저장/로드

====================================================================*/
void Cham_temp_erase(void)
{
  NAND_erase_block(&Cham_temp_Memory);
}


void Cham_temp_find(void)
{
  NAND_find_block(&Cham_temp_Memory);
}


void Cham_temp_Write(void)
{
uint8_t i;
uint16_t t=0;
union TRANS_BYTE data_mul;

static uint16_t save_flag=0;

//===========================================================================
	if( time_sec==1 && time_min%5 == 0 && save_flag == 0)
	{
		save_flag = 1;

    NAND_Buffer[0] = 	1;
    NAND_Buffer[1] = 	0;

    NAND_Buffer[2] = 	time_year;
    NAND_Buffer[3] = 	time_month;
    NAND_Buffer[4] = 	time_date;
    NAND_Buffer[5] = 	time_hour;
    NAND_Buffer[6] = 	time_min;
    NAND_Buffer[7] = 	0;

    t= 8;

    for(i=0;i<8;i++)
    {
      data_mul.dat16 = cham_now_temp[i];	
      NAND_Buffer[t++] =  data_mul.dat8[0];
      NAND_Buffer[t++] =  data_mul.dat8[1];
      data_mul.dat16 = cham_set_temp[i];	
      NAND_Buffer[t++] =  data_mul.dat8[0];
      NAND_Buffer[t++] =  data_mul.dat8[1];
    }

    NAND_Data_Move_block(&Cham_temp_Memory, NAND_Buffer);
	}
	else if ( time_sec != 1  )
  {
    save_flag = 0; 
  }
}


void Cham_temp_Read(uint16_t data_no, uint8_t data_cnt, uint8_t *time_data, uint16_t *now_data, uint16_t *set_data)
{
uint8_t i,j;
uint32_t adr_data,adr_point;
union TRANS_BYTE data_trans1,data_trans2;
  
	adr_data = data_no * C_packet_no / NAND_PAGE_SIZE  ;
	adr_point = data_no * C_packet_no % NAND_PAGE_SIZE;
  
  NAND_Read_block(&Cham_temp_Memory, NAND_Buffer, adr_data, C_rd_page);

	for (i=0;i<data_cnt;i++)
	{
		*time_data++ = NAND_Buffer[adr_point++] ;		//저장여부
		*time_data++ = NAND_Buffer[adr_point++] ;		//저장여부
		*time_data++ = NAND_Buffer[adr_point++] ;		//년
		*time_data++ = NAND_Buffer[adr_point++] ;		//월
		*time_data++ = NAND_Buffer[adr_point++] ;		//일
		*time_data++ = NAND_Buffer[adr_point++] ;		//시
		*time_data++ = NAND_Buffer[adr_point++] ;		//분
		*time_data++ = NAND_Buffer[adr_point++] ;		//초

		for(j=0;j<8;j++)
		{
			data_trans1.dat8[0] = NAND_Buffer[adr_point++] ;
			data_trans1.dat8[1] = NAND_Buffer[adr_point++] ;
			data_trans2.dat8[0] = NAND_Buffer[adr_point++] ;
			data_trans2.dat8[1] = NAND_Buffer[adr_point++] ;	

      *now_data++ = data_trans1.dat16;
			*set_data++ = data_trans2.dat16;
		}
	}
}




/*====================================================================

	메모리 체크

====================================================================*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

void nand_id_check(void)
{
	/* Add your application code here
	*/
NAND_IDTypeDef NAND_ID;		
	
	FSMC_NAND_ReadID(&NAND_ID);

	if((NAND_ID.Maker_ID == 0xAD) && (NAND_ID.Device_ID == 0x76))
	{
//		NAND_Test();
	}
	else
	{
//		while(1);
	}


}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
