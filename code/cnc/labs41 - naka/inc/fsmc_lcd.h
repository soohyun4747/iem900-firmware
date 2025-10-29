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
#ifndef __FSMC_LCD_H
#define __FSMC_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "hw_config.h"


/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM322xG_EVAL
  * @{
  */
    
/** @addtogroup STM322xG_EVAL_LCD
  * @{
  */ 
  
  


/** @defgroup STM322xG_EVAL_LCD_Exported_Types
  * @{
  */
typedef struct 
{
  int16_t X;
  int16_t Y;
} Point, * pPoint;   
/**
  * @}
  */ 

/** @defgroup STM322xG_EVAL_LCD_Exported_Constants
  * @{
  */ 




#define LCD_PCLK		349524	//40MHz = 120MHz*(LCDC_FPR+1)/2^20

#define LCD_HOR_PIXEL	800
#define LCD_VER_PIXEL	480

#define LCD_HT			928		//HOR_PIXEL + HOR_PULSE_CLK + HOR_BACK_CLK + HOR_FRONT_CLK
#define LCD_HPS			88		//HOR_PULSE_CLK + HOR_BACK_CLK
#define LCD_HPW			48		//HOR_PULSE_CLK
#define LCD_LPS			0
#define LCD_LPSPP		0

#define LCD_VT			525		//VER_PIXEL + VER_PULSE_CLK + VER_BACK_CLK + VER_FRONT_CLK
#define LCD_VPS			32		//VER_PULSE_CLK + VER_BACK_CLK
#define LCD_VPW			3		//VER_PULSE_CLK
#define LCD_FPS			0




/** 
  * @brief  LCD color  
  */ 
#define WHITE		0xFFFF
#define BLACK		0x0000	  
#define BLUE		0x001F  
#define RED			0xF800
#define MAGENTA		0xF81F
#define GREEN		0x07E0
#define CYAN		0x7FFF
#define YELLOW		0xFFE0
#define BROWN		0XBC40
#define BRRED		0XFC07
#define GRAY		0X8430
#define LGRAY		0XC618


/** @defgroup STM322xG_EVAL_LCD_Exported_Functions
  * @{
  */ 
/** @defgroup  
  * @{
  */
void LCD_Init(void);
void LCD_Clear(uint16_t Color);
void LCD_Draw_Pixel(uint16_t x1, uint16_t y1, uint16_t Color);
void LCD_Fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t Color);
void LCD_Box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t Color);
void LCD_WindowMax (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawPicture(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, const uint16_t *pic); 
void LCD_DrawPicture2(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, const uint16_t *pic); 
void LCD_DrawPicture3(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, const uint16_t *pic); 

/** @defgroup  
  * @{
  */ 
void LCD_WriteReg(uint16_t index);
void LCD_WriteData(uint16_t data);


/**
  * @}
  */ 

/** @defgroup
  * @{
  */ 
void LCD_CtrlLinesConfig(void);
void LCD_FSMCConfig(void);
/**
  * @}
  */
/**
  * @}
  */    
#ifdef __cplusplus
}
#endif

#endif /* __STM322xG_EVAL_LCD_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
