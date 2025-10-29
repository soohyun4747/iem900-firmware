/**
  ******************************************************************************
  * @file    stm322xg_eval_lcd.c
  * @author  MCD Application Team
  * @version V5.0.3
  * @date    09-March-2012
  * @brief   This file includes the LCD driver for AM-240320L8TNQW00H (LCD_ILI9320)
  *          and AM240320D5TOQW01H (LCD_ILI9325) Liquid Crystal Display Modules
  *          of STM322xG-EVAL evaluation board(MB786) RevA and RevB.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreemen4t_liberty_v2
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
#include "fsmc_lcd.h"
#include "delay.h"


/** @defgroup STM322xG_EVAL_LCD_Private_Defines
  * @{
  */


/* Note: LCD /CS is NE1 - Bank 1 of NOR/SRAM Bank 1~4 */
#define LCD_REG              (*((volatile unsigned short *) 0x60000000))     /* A11 = 0 */
#define LCD_RAM              (*((volatile unsigned short *) 0x60001000))     /* A11 = 1 */

#define Back_color			0xd69a




/** @defgroup STM322xG_EVAL_LCD_Private_Functions
  * @{
  */ 


/****************************************************************************
  * @brief  Initializes the LCD.
  * @param  None
  * @retval None
****************************************************************************/
void LCD_Init(void)
{ 
	SSD1963_RESET = 1 ;
	Delay_ms(200);		

/* Configure the LCD Control pins --------------------------------------------*/
	LCD_CtrlLinesConfig();

/* Configure the FSMC Parallel interface -------------------------------------*/
	LCD_FSMCConfig();
/*------------------------------------------------*/
/* Start Initial Sequence */
	LCD_WriteReg(0x0001); 		//soft_reset
	Delay_us(5000);					 /* delay 5ms */
/*------------------------------------------------*/
	LCD_WriteReg(0x00E2); 		//set_pll_mn

	LCD_WriteData(0x0023);
	LCD_WriteData(0x0002);
	LCD_WriteData(0x0004);
/*------------------------------------------------*/
	LCD_WriteReg(0x00E0); 		//set_pll

	LCD_WriteData(0x0001);

	Delay_us(100);					 /* delay 100us */
/*------------------------------------------------*/
	LCD_WriteReg(0x00E0); 		//set_pll

	LCD_WriteData(0x0003);

	Delay_us(100);					 /* delay 100us */
/*------------------------------------------------*/
	LCD_WriteReg(0x0001); 		//soft_reset
	Delay_us(5000);					 /* delay 5ms */
/*------------------------------------------------*/
	LCD_WriteReg(0x00E6); 		//set_lshift_freq	PCLK = 120MHz*(LCDC_FPR+1)/2^20

	LCD_WriteData( LCD_PCLK >> 16 & 0x00FF );
	LCD_WriteData( LCD_PCLK >> 8  & 0x00FF );
	LCD_WriteData( LCD_PCLK       & 0x00FF );
/*------------------------------------------------*/
	LCD_WriteReg(0x00B0); 		//set_lcd_mode

	LCD_WriteData( 0x0020 );
	LCD_WriteData( 0x0000 );
	LCD_WriteData( (LCD_HOR_PIXEL-1) >> 8 & 0x00FF );
	LCD_WriteData( (LCD_HOR_PIXEL-1)      & 0x00FF );
	LCD_WriteData( (LCD_VER_PIXEL-1) >> 8 & 0x00FF );
	LCD_WriteData( (LCD_VER_PIXEL-1)      & 0x00FF );
	LCD_WriteData(0x0000);
/*------------------------------------------------*/
	LCD_WriteReg(0x00B4); 		//set_hori_period

	LCD_WriteData( LCD_HT >> 8  & 0x00FF );
	LCD_WriteData( LCD_HT  & 0x00FF );
	LCD_WriteData( LCD_HPS >> 8  & 0x00FF );
	LCD_WriteData( LCD_HPS & 0x00FF );
	LCD_WriteData( LCD_HPW );
	LCD_WriteData( LCD_LPS >> 8  & 0x00FF );
	LCD_WriteData( LCD_LPS & 0x00FF );
	LCD_WriteData(0x0000);
/*------------------------------------------------*/
	LCD_WriteReg(0x00B6); 		//set_vert_period

	LCD_WriteData( LCD_VT >> 8  & 0x00FF );
	LCD_WriteData( LCD_VT  & 0x00FF );
	LCD_WriteData( LCD_VPS  >> 8  & 0x00FF );
	LCD_WriteData( LCD_VPS & 0x00FF );
	LCD_WriteData( LCD_VPW );
	LCD_WriteData( LCD_FPS  >> 8  & 0x00FF );
	LCD_WriteData( LCD_FPS & 0x00FF );
/*------------------------------------------------*/
	LCD_WriteReg(0x00F0); 		//set_pixel_data_interface

	LCD_WriteData(0x0003);	//16-bit (565 format)
/*------------------------------------------------*/
	LCD_WriteReg(0x0036); 		//set_pixel_data_interface

	LCD_WriteData(0x0000);
/*------------------------------------------------*/
	LCD_WriteReg(0x00B8); 		//set_gpio_conf

	LCD_WriteData(0x000F);
	LCD_WriteData(0x0000);
/*------------------------------------------------*/
	LCD_WriteReg(0x00BA); 		//set_gpio_value

	LCD_WriteData(0x0002);		//ALL GPIO CLEAR
/*------------------------------------------------*/
	LCD_WriteReg(0x0029); 		//set_display_on
/*------------------------------------------------*/
	LCD_Clear(Back_color);

	Back_Light = 1;

}




/***********************************
	@brief：void LCD_WindowMax()
	@param：x1	pixel  0~239	시작지점x
			y1	pixel  0~319	시작지점y
			x2	pixel  0~239	종료지점x
			y2	pixel  0~319	종료지점y
	@retva：None
	@note：window set
***********************************/
void LCD_WindowMax (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) 
{
	LCD_WriteReg(0x002A); 		//set_column_address
	LCD_WriteData( x1 >>8 & 0x00FF);
	LCD_WriteData( x1 & 0x00FF);
	LCD_WriteData( x2 >>8 & 0x00FF);
	LCD_WriteData( x2 & 0x00FF);

	LCD_WriteReg(0x002B); 		//set_column_address
	LCD_WriteData( y1 >>8 & 0x00FF);
	LCD_WriteData( y1 & 0x00FF);
	LCD_WriteData( y2 >>8 & 0x00FF);
	LCD_WriteData( y2 & 0x00FF);

	LCD_WriteReg(0x002C); 		//set_column_address
}



/*******************************************************************************
  * @brief  Clears the hole LCD.
  * @param  Color: the color of the background.
  * @retval None
*******************************************************************************/
void LCD_Clear(uint16_t Color)
{
	LCD_Fill(0x0000,0x0000,LCD_HOR_PIXEL-1,LCD_VER_PIXEL-1,Color); 
}






/****************************************************************************
	@brief：void LCD_DrawPicture(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
	@param：x1	pixel  0~239	시작지점x
			y1	pixel  0~319	시작지점y
			x2	pixel  0~239	종료지점x
			y2	pixel  0~319	종료지점y
	@retva：None
	@note：fill display window 
****************************************************************************/
void LCD_DrawPicture3(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, const uint16_t *pic)
{
	uint16_t index ;
	uint16_t count ;
	uint16_t *bitmap = (u16 *)pic;
	uint16_t *bitemp = bitmap;

	LCD_WindowMax(x1,y1,x2,y2);

	count = (y2-y1+1) ;

	while(count--)
	{
		bitmap = bitemp;
		index = (x2-x1+1);

		while(index--)
		{
			LCD_RAM = *bitmap++;
		}
	}
}




/****************************************************************************
	@brief：void LCD_DrawPicture(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
	@param：x1	pixel  0~239	시작지점x
			y1	pixel  0~319	시작지점y
			x2	pixel  0~239	종료지점x
			y2	pixel  0~319	종료지점y
	@retva：None
	@note：fill display window 
****************************************************************************/
void LCD_DrawPicture2(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, const uint16_t *pic)
{
	uint16_t index = (y2-y1+1) ;
	uint16_t count ;
	uint16_t data ;
	uint16_t *bitmap = (u16 *)pic;

	LCD_WindowMax(x1,y1,x2,y2);

	while(index--)
	{
		data = *bitmap++;
		count = (x2-x1+1) ;
		while(count--)		LCD_RAM = data;
	}
}


/****************************************************************************
	@brief：void LCD_DrawPicture(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
	@param：x1	pixel  0~239	시작지점x
			y1	pixel  0~319	시작지점y
			x2	pixel  0~239	종료지점x
			y2	pixel  0~319	종료지점y
	@retva：None
	@note：fill display window 
****************************************************************************/
void LCD_DrawPicture(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, const uint16_t *pic)
{
	uint32_t index = 0;
	u16 *bitmap = (u16 *)pic;
	
	LCD_WindowMax(x1,y1,x2,y2);

	index = (u32)(y2-y1+1)*(x2-x1+1) ;		//(y2-y1)*(x2-x1)/16

	while(index--)
	{
		LCD_RAM = *bitmap++;
	}
}

/***********************************
	@brief：void LCD_Draw_Pixel(uint16_t x1, uint16_t y1, uint16_t Color)
	@param：x1	pixel  0~799	시작지점x
			y1	pixel  0~479	시작지점y
	@retva：None
	@note：fill display window 
***********************************/
void LCD_Draw_Pixel(uint16_t x1, uint16_t y1, uint16_t Color)
{                    
	LCD_WindowMax(x1,y1,x1,y1);
	LCD_RAM = Color;
}  





/***********************************
	@brief：void LCD_Draw_Pixel(uint16_t x1, uint16_t y1, uint16_t Color)
	@param：x1	pixel  0~799	시작지점x
			y1	pixel  0~479	시작지점y
	@retva：None
	@note：fill display window 
***********************************/
void  LCD_Box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t Color)
{
	LCD_Fill(  x1, y1, x2, y1,  Color);	
	LCD_Fill(  x1, y1, x1, y2,  Color);	
	LCD_Fill(  x1, y2, x2, y2,  Color);	
	LCD_Fill(  x2, y1, x2, y2,  Color);	

}





/***********************************
	@brief：void LCD_Fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
	@param：x1	pixel  0~799	시작지점x
			y1	pixel  0~479	시작지점y
			x2	pixel  0~799	종료지점x
			y2	pixel  0~479	종료지점y
	@retva：None
	@note：fill display window 
***********************************/
void LCD_Fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t Color)
{                    
	uint32_t index = 0;

	LCD_WindowMax(x1,y1,x2,y2);

	index=(u32)(y2-y1+1)*(x2-x1+1);

	while(index--)
	{
		LCD_RAM = Color;
	}

}  


/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : 
* Input          : - index:
* Output         : None
* Return         : None
* Attention         : None
*******************************************************************************/
void LCD_WriteReg(uint16_t index)
{
	LCD_REG = index;
}

/*******************************************************************************
* Function Name  : LCD_WriteData
* Description    : 
* Input          : - index:
* Output         : None
* Return         : None
* Attention         : None
*******************************************************************************/
void LCD_WriteData(uint16_t data)
{
	LCD_RAM = data;
}






/**
  * @brief  Configures LCD Control lines (FSMC Pins) in alternate function mode.
  * @param  None
  * @retval None
  */
void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOD, GPIOE, GPIOg and AFIO clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG 
						, ENABLE);

/*-- GPIO Configuration ------------------------------------------------------*/
  /* SRAM Data lines,  NOE and NWE configuration */

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure.GPIO_Pin =	GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
									GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15 |
									GPIO_Pin_4 |GPIO_Pin_5;;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
									GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
									GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* SRAM Address lines configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	/*A11*/
	GPIO_Init(GPIOG, &GPIO_InitStructure);  

	/* NE1 configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

	GPIO_PinAFConfig(GPIOG, GPIO_PinSource1, GPIO_AF_FSMC);	   

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);

}




/**
  * @brief  Configures the Parallel interface (FSMC) for LCD(Parallel mode)
  * @param  None
  * @retval None
  */
void LCD_FSMCConfig(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;
   
  /* Enable FSMC clock */
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
  
/*-- FSMC Configuration ------------------------------------------------------*/
/*----------------------- SRAM Bank 1 ----------------------------------------*/
  /* FSMC_Bank1_NORSRAM4 configuration */
  p.FSMC_AddressSetupTime = 3;		//3
  p.FSMC_AddressHoldTime = 0;		//0
  p.FSMC_DataSetupTime = 7;			//7
  p.FSMC_BusTurnAroundDuration = 0;
  p.FSMC_CLKDivision = 0;
  p.FSMC_DataLatency = 0;
  p.FSMC_AccessMode = FSMC_AccessMode_A;
  /* Color LCD configuration ------------------------------------
     LCD configured as follow:
        - Data/Address MUX = Disable
        - Memory Type = SRAM
        - Data Width = 16bit
        - Write Operation = Enable
        - Extended Mode = Enable
        - Asynchronous Wait = Disable */

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);   

  /* Enable FSMC NOR/SRAM Bank1 */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}


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
  
/**
  * @}
  */  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
