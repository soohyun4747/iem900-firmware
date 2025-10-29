/**
  ******************************************************************************
  * @file    ADC//delay.c 
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
#include "delay.h"



static  uint32_t  fac_us=0;      /*us延时倍乘数	*/
static  uint16_t  fac_ms=0;      /*ms延时倍乘数 */



/**
	*名称：void Delay_Init(void)
	*参数：无
	*返回：无
	*功能：基准延时初始化，使用SysTick
	*备注：SysTick时钟源由HCLK/8提供，当系统频率为72MHz时
           最小计数周期为1/9MHz,计满9次为1us，fac_us以9为单位  
**/

void Delay_Init(void)
{
	RCC_ClocksTypeDef  RCC_Clocks;

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	RCC_GetClocksFreq(&RCC_Clocks);

	fac_us =  (RCC_Clocks.HCLK_Frequency/8 / 1000000);

	fac_ms= (u16)fac_us*1000;
}


/**
	*名称：void Delay_ms(u16 nms)
	*参数：nms(ms number) 毫秒数 不能超过1864
	*返回：无
	*功能：毫秒级延时
	*备注：SysTick->LOAD为24位寄存器,所以最大延时小于或等于1864ms，
**/
void Delay_ms(u16 nms)
{	 		  	 	 
	SysTick->LOAD = (u32)nms * fac_ms - 1 ;		/*1us	*/  		 
	SysTick->VAL = 0x00;     			   /*檬扁拳 */
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk ;    	 

	while ( !(SysTick->CTRL &SysTick_CTRL_COUNTFLAG_Msk) );

	SysTick->CTRL=0x00;       /*关闭计数器	*/
	SysTick->VAL =0X00;       /*清空计数器	*/
} 



/**
	*名称：void Delay_us(u32 nus)
	*参数：us(number us) 延时微秒数 最大
	*返回：无
	*功能: 实现微秒级延时，最大延时1118ms
	*备注：这两个函数是通过寄存器配置的，没有对应的固件库函数，参考misc.h
**/		    								   
void Delay_us(u16 nus)
{
	SysTick->LOAD = nus * fac_us - 1 ;		/*1us	*/  		 
	SysTick->VAL = 0x00;     			   /*檬扁拳 */
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk ;    	 

	while ( !(SysTick->CTRL &SysTick_CTRL_COUNTFLAG_Msk) );

	SysTick->CTRL=0x00;       /*关闭计数器	*/
	SysTick->VAL =0X00;       /*清空计数器	*/
}


