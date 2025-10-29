/* CH376芯片 软件模拟SPI串行连接的硬件抽象层 V1.0 */
/* 提供I/O接口子程序 */

#include	"HAL.H"
#include 	"stm32f2xx.h"
#include	"delay.h"


#define CH376_SCS_0()		GPIO_ResetBits(GPIOE,  GPIO_Pin_2)
#define CH376_SCS_1()		GPIO_SetBits(GPIOE,  GPIO_Pin_2)

#define CH376_INT_HIGH()	(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0) != Bit_RESET)



void CH376_PORT_INIT( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

/*******************************************************/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 ;			//SCS
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;	//INT_WIRE, SPI_BZ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStruct);	

/*******************************************************/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1 , ENABLE);

	/* configure pins used by SPI1
	 * PB3 = SCK
	 * PB4 = MISO
	 * PB5 = MOSI
	 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	// connect SPI1 pins to SPI alternate function
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);

	/* configure SPI1 in Mode 0 
	 * CPOL = 1 --> clock is high when idle
	 * CPHA = 1 --> data is sampled at the first edge
	 */
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // set to receive only mode, seperate MISO lines
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;     // transmit in master mode, NSS pin has to be always high
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;        // clock is low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;      // data sampled at first edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // set the NSS management to internal and pull internal NSS high
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; // SPI frequency is APB1 frequency / 8
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
	SPI_Init(SPI1, &SPI_InitStruct); 

	SPI_Cmd(SPI1, ENABLE); // enable SPI1
}






UINT8	Spi376Exchange( UINT8 d )
{
	UINT8	i;
	i=50;
	while (i--);

	SPI1->DR = d; // write data to be transmitted to the SPI data register
	while( !(SPI1->SR & SPI_I2S_FLAG_TXE) ); // wait until transmit complete
	while( !(SPI1->SR & SPI_I2S_FLAG_RXNE) ); // wait until receive complete
	while( SPI1->SR & SPI_I2S_FLAG_BSY ); // wait until SPI is not busy anymore
	i=50;
	while (i--);
	return SPI1->DR; // return received data from SPI data register
}



void	xEndCH376Cmd(void)	
{
	CH376_SCS_1();
}


void	xWriteCH376Cmd( UINT8 mCmd )  /* ??376??? */
{
	CH376_SCS_1();
	Delay_us(1);
	CH376_SCS_0();
	Spi376Exchange( mCmd );
	Delay_us(2);
}

void	xWriteCH376Data( UINT8 mData )
{
	Spi376Exchange( mData );
}

UINT8	xReadCH376Data( void )
{
	return( Spi376Exchange( 0xFF ) );
}

UINT8	Query376Interrupt( void )
{
	return( CH376_INT_HIGH() ? FALSE : TRUE );
}



UINT8 mInitCH376Host( void )
{
	UINT8	res;
	
	CH376_PORT_INIT( );

	xWriteCH376Cmd( CMD11_CHECK_EXIST );
	xWriteCH376Data( 0x65 );
	res = xReadCH376Data( );
	xEndCH376Cmd( );
	if ( res != 0x9A ) return( ERR_USB_UNKNOWN );

	xWriteCH376Cmd( CMD11_SET_USB_MODE );
	xWriteCH376Data( 0x06 );
	Delay_us(50);
	res = xReadCH376Data( );
	xEndCH376Cmd( );

	if ( res == CMD_RET_SUCCESS ) return( USB_INT_SUCCESS );
	else return( ERR_USB_UNKNOWN );

}
