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
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H


#define GPIO_IN_OFFSET	0x10
#define GPIO_OUT_OFFSET	0x14

#define PAin(n)		*((volatile unsigned long *)( 0x42000000 + ( ((GPIOA_BASE+GPIO_IN_OFFSET)-0x40000000) <<5)+ (n<<2) ))
#define PBin(n)		*((volatile unsigned long *)( 0x42000000 + ( ((GPIOB_BASE+GPIO_IN_OFFSET)-0x40000000) <<5)+ (n<<2) ))
#define PCin(n)		*((volatile unsigned long *)( 0x42000000 + ( ((GPIOC_BASE+GPIO_IN_OFFSET)-0x40000000) <<5)+ (n<<2) ))
#define PDin(n)		*((volatile unsigned long *)( 0x42000000 + ( ((GPIOD_BASE+GPIO_IN_OFFSET)-0x40000000) <<5)+ (n<<2) ))
#define PEin(n)		*((volatile unsigned long *)( 0x42000000 + ( ((GPIOE_BASE+GPIO_IN_OFFSET)-0x40000000) <<5)+ (n<<2) ))
#define PFin(n)		*((volatile unsigned long *)( 0x42000000 + ( ((GPIOF_BASE+GPIO_IN_OFFSET)-0x40000000) <<5)+ (n<<2) ))
#define PGin(n)		*((volatile unsigned long *)( 0x42000000 + ( ((GPIOG_BASE+GPIO_IN_OFFSET)-0x40000000) <<5)+ (n<<2) ))

#define PAout(n)	*((volatile unsigned long *)( 0x42000000 + ( ((GPIOA_BASE+GPIO_OUT_OFFSET)-0x40000000) <<5)+ (n<<2) ))
#define PBout(n)	*((volatile unsigned long *)( 0x42000000 + ( ((GPIOB_BASE+GPIO_OUT_OFFSET)-0x40000000) <<5)+ (n<<2) ))
#define PCout(n)	*((volatile unsigned long *)( 0x42000000 + ( ((GPIOC_BASE+GPIO_OUT_OFFSET)-0x40000000) <<5)+ (n<<2) ))
#define PDout(n)	*((volatile unsigned long *)( 0x42000000 + ( ((GPIOD_BASE+GPIO_OUT_OFFSET)-0x40000000) <<5)+ (n<<2) ))
#define PEout(n)	*((volatile unsigned long *)( 0x42000000 + ( ((GPIOE_BASE+GPIO_OUT_OFFSET)-0x40000000) <<5)+ (n<<2) )) 
#define PFout(n)	*((volatile unsigned long *)( 0x42000000 + ( ((GPIOF_BASE+GPIO_OUT_OFFSET)-0x40000000) <<5)+ (n<<2) ))
#define PGout(n)	*((volatile unsigned long *)( 0x42000000 + ( ((GPIOG_BASE+GPIO_OUT_OFFSET)-0x40000000) <<5)+ (n<<2) )) 


#define SSD1963_RESET	PBout(12)
#define Back_Light		PGout(2)

#define	DOOR_D0			PFout(7)
#define	DOOR_D1			PFout(6)
#define	DOOR_D2			PFout(5)
#define	DOOR_D3			PFout(4)
#define	DOOR_D4			PFout(3)
#define	DOOR_D5			PFout(2)
#define	DOOR_D6			PFout(1)
#define	DOOR_D7			PFout(0)

#define	DOOR_E0			PCout(12)
#define	DOOR_E1			PDout(2)
#define	DOOR_E2			PDout(3)

#define BUZZER			PAout(12)

#define	HEAT_POW		PGout(3)
#define	SOL_2			PGout(5)
#define	SOL_1			PGout(6)
#define	HEAT_FAN		PGout(7)


#define SOL_CHAM0		PCout(4)
#define SOL_CHAM1		PCout(5)
#define SOL_CHAM2		PBout(2)
#define SOL_CHAM3		PFout(11)
#define SOL_CHAM4		PFout(12)
#define SOL_CHAM5		PFout(13)
#define SOL_CHAM6		PFout(14)
#define SOL_CHAM7		PFout(15)

#define SOL_SEL1		PAout(4)
#define SOL_SEL2		PAout(5)



#define	TS_S0			PGout(10)
#define	TS_S1			PGout(14)
#define	TS_ADR2			PGout(11)
#define	TS_ADR1			PGout(12)
#define TS_ADR0			PGout(13)
#define	TS_EXT			PEout(5)

#define	SET_CH0			PEout(4)
#define	SET_CH1			PEout(3)
#define	SET_EN			PAout(15)

#define	SPI1_INT		PEin(0)
#define	SPI1_BUSY		PEin(1)
#define	SPI1_SCS		PEout(2)





/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#endif /* __HW_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
