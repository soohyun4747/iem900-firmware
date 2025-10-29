/**
  ******************************************************************************
  * @file    DATA_IO.H
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   Header for fsmc_nand.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DATA_IO_H
#define __DATA_IO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "fsmc_nand.h"



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


// 구조체 정의
typedef struct _Memory_Flash_struct {
    const uint32_t  start_Addr;     //메모리시작주소

    uint8_t   active_no;          //활성화된 모듈번호
    const uint8_t   active_max;   //이동가능모듈수
    const uint8_t   block_no;     //유효데이타필요블럭수

    uint16_t  packet_no;          //패킷당바이트수
    const uint16_t  value_max;     //최대유효자료수
    const uint16_t  page_no;      //유효데이타 페이지수

    uint16_t  value_no;          //유효자료수
} Memory_Flash;




/*
                    |packet_no  | value_max |           | page_no   | block_no  |active_max |           |           |start_Addr
메모리시작주소             패킷당바이트수    최대유효자료수    유효데이타바이트   유효데이타페이지수  유효데이타필요블럭수 이동가능모듈수    할당블럭수       블럭번호         메모리시작주소                                                               
            
Set_Data_Address      256	             1          256	       1            1	        32	      32        0x020~0x03F	  0x0400
Data_Temp_Address     772		          20        15440	      31            1	        32	      32        0x040~0x05F	  0x0800
Voc_change_Address	    8		          50          400	       1            1	        32	      32        0x060~0x07F	  0x0C00
Alarm_log_Address	     15		          60          900	       2            1	        32	      32        0x080~0x09F	  0x1000
Gas_change_Address	    9		          50          450	       1            1	        32	      32        0x0A0~0x0BF	  0x1400
Temp_set_Address	    10*	          1000 		    10000	      20            1	        32	      32        0x0C0~0x0DF	  0x1800
Main_log_Address	     10		        1000 		    10000	      20            1	        32	      32        0x0E0~0x0FF	  0x1C00
Cham_temp_Address	     40		        6000       240000	     469	         15         17	      255       0x100~0x1FF	  0x2000

*/


//메모리시작주소
#define S_start_Addr  0x0400
#define D_start_Addr	0x0800
#define V_start_Addr	0x0C00
#define A_start_Addr	0x1000
#define G_start_Addr	0x1400
#define T_start_Addr	0x1800
#define M_start_Addr	0x1C00
#define C_start_Addr	0x2000

//활성화된 모듈번호
#define S_active_no  0
#define D_active_no  0
#define V_active_no  0
#define A_active_no  0
#define G_active_no  0
#define T_active_no  0
#define M_active_no  0
#define C_active_no  0

//패킷당바이트수
#define S_packet_no	 256     //Setting
#define D_packet_no	 772     //Data_Temp
#define V_packet_no	 8       //voc_change
#define A_packet_no	 15      //alarm_log
#define G_packet_no	 9       //gas_change
#define T_packet_no	 10      //Temp_set
#define M_packet_no	 10       //main_log
#define C_packet_no	 40      //cham_temp

//유효자료수
#define S_value_no	0
#define D_value_no	0
#define V_value_no	0
#define A_value_no	0
#define G_value_no	0
#define T_value_no	0
#define M_value_no	0
#define C_value_no	0

//최대유효자료수
#define S_value_max	1
#define D_value_max	20
#define V_value_max	50
#define A_value_max	60
#define G_value_max	50
#define T_value_max	1000
#define M_value_max	1000
#define C_value_max	6000

//유효데이타 페이지수
#define S_page_no	  1
#define D_page_no	  31
#define V_page_no	  1
#define A_page_no	  2
#define G_page_no	  1
#define T_page_no	  20
#define M_page_no	  20
#define C_page_no	  469

//유효데이타 필요블럭수
#define S_block_no	  1
#define D_block_no	  1
#define V_block_no	  1
#define A_block_no	  1
#define G_block_no	  1
#define T_block_no	  1
#define M_block_no	  1
#define C_block_no	  15

//이동가능모듈수
#define S_active_max  32
#define D_active_max  32
#define V_active_max	32
#define A_active_max	32
#define G_active_max	32
#define T_active_max	32
#define M_active_max	32
#define C_active_max	17

//할당 블럭수
#define S_assign_no	  S_block_no * S_active_max
#define D_assign_no	  D_block_no * D_active_max
#define V_assign_no	  V_block_no * V_active_max
#define A_assign_no	  A_block_no * A_active_max
#define G_assign_no	  G_block_no * G_active_max
#define T_assign_no	  T_block_no * T_active_max
#define M_assign_no	  M_block_no * M_active_max
#define C_assign_no	  C_block_no * C_active_max

//데이타 불러올때 읽어야할 페이지 수
#define S_rd_page	1
#define D_rd_page	3
#define V_rd_page	1
#define A_rd_page	2
#define G_rd_page	1
#define T_rd_page	2
#define M_rd_page	2
#define C_rd_page	2



void Data_Initial(void);

void Set_Data_Write(void);
void Set_Data_Read(void);
void Set_Data_find(void);

void Data_Temp_erase(void);
void Data_Temp_find(void);
void Data_Temp_Write(void);
void Data_Temp_Read(uint16_t data_no, uint8_t ch_no_dat ,uint8_t *load_data1, uint16_t *load_data2, uint16_t *load_data3);

void Voc_change_erase(void);
void Voc_change_find(void);
void Voc_change_Write(void);
void Voc_change_Read(uint16_t data_no, uint8_t data_cnt, uint8_t *load1);

void Alarm_log_erase(void);
void Alarm_log_find(void);
void Alarm_log_Write(uint8_t *load1, uint8_t *load2, uint8_t *load3);
void Alarm_log_Read(uint16_t data_no, uint8_t data_cnt, uint8_t *load1, uint8_t *load2, uint8_t *load3);

void Gas_change_erase(void);
void Gas_change_find(void);
void Gas_change_Write(uint8_t gas_sel, uint8_t gas_val);
void Gas_change_Read(uint16_t data_no, uint8_t data_cnt, uint8_t *load1, uint8_t *load2);

void Temp_set_erase(void);
void Temp_set_find(void);
void Temp_set_Write(uint8_t *set_flag, uint8_t *set_data, uint16_t *setting_val);
void Temp_set_Read(uint16_t data_no, uint8_t data_cnt, uint8_t *mode_data, uint8_t *cham_data, uint16_t *set_data, uint8_t *time_data);

void Main_log_erase(void);
void Main_log_find(void);
void Main_log_Write(void);
void Main_log_Read(uint16_t data_no, uint8_t data_cnt, uint8_t *load1, uint8_t *load2);

void Cham_temp_erase(void);
void Cham_temp_find(void);
void Cham_temp_Write(void);
void Cham_temp_Read(uint16_t data_no, uint8_t data_cnt, uint8_t *time_data, uint16_t *now_data, uint16_t *set_data);


void nand_id_check(void);












#endif /* __DATA_IO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
