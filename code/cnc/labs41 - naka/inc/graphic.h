/**
  ******************************************************************************
  * @file    fsmc_lcd.h
  * @author  MCD Application Team
  * @version V5.0.3
  * @date    09-March-2012
  * @brief   This file contains all the functions prototypes for the stm322xg_eval_lcd.c
  *          driver.
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
#ifndef __GRAPHIC_H
#define __GRAPHIC_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "hw_config.h"



typedef enum
{ 
  ZERO_ENABLE = 0,
  ZERO_DISABLE
}Zero_Bit;







/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LCD_ShowChar   (uint16_t x, uint16_t y, uint16_t pencolor, uint16_t backcolor, uint8_t charter);
void LCD_ShowString (uint16_t x, uint16_t y, uint16_t pencolor, uint16_t backcolor, uint8_t *string);
void LCD_ColorNumber(uint16_t x, uint16_t y, uint16_t pencolor, uint16_t backcolor, uint32_t number, uint8_t len, Zero_Bit zero_flag);
void LCD_ShowString_number(uint16_t x, uint16_t y, uint16_t pencolor, uint16_t backcolor, uint8_t *string, uint16_t number, uint8_t len);

void LCD_ShowChar2   (uint16_t x, uint16_t y, uint16_t pencolor, const uint16_t *backcolor, uint8_t charter);
void LCD_ShowString2 (uint16_t x, uint16_t y, uint16_t pencolor, const uint16_t *backcolor, uint8_t *string);
void LCD_ColorNumber2(uint16_t x, uint16_t y, uint16_t pencolor, const uint16_t *backcolor, uint32_t number, uint8_t len, Zero_Bit zero_flag);

void LCD_small_ShowChar(uint16_t x, uint16_t y, uint16_t pencolor, uint16_t backcolor, uint8_t charter);
void LCD_small_ShowString(uint16_t x, uint16_t y, uint16_t pencolor, uint16_t backcolor, uint8_t *string);

void LCD_Passstar(uint16_t x, uint16_t y, uint16_t pencolor, uint16_t backcolor);


void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color, uint8_t dotline) ;

void LCD_Display(void);


void LCD_first(void);

#endif /* __GRAPHIC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
