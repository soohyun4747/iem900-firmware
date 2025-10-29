/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/main.h 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   Header for main.c module
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
#ifndef __TEMP_IC_H
#define __TEMP_IC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "hw_config.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TEMP_Init(void);
void TEMP_READ(void);
uint8_t TEMP_DATA_calibration(uint8_t ch_no, uint8_t posit,uint16_t set_temp);
void TEMP_DATA_initial(uint8_t ch_no, uint8_t posit);
uint8_t TEMP_ext_calibration(uint16_t set_temp);
void TEMP_ext_initial(void);
void start_Calibraion(void);




#endif /* __TEMP_IC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
