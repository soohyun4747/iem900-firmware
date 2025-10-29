/* CH376芯片 硬件抽象层 V1.0 */
/* 提供I/O接口子程序 */

#include	"CH376INC.H"

#ifndef HAL_H__
#define HAL_H__

/* 附加的USB操作状态定义 */
#define		ERR_USB_UNKNOWN		0xFA	/* 未知错误,不应该发生的情况,需检查硬件或者程序错误 */

#define		EN_DIR_CREATE
#define		EN_LONG_NAME


void	CH376_PORT_INIT( void );  		/* CH376通讯接口初始化 */

void	xEndCH376Cmd( void );			/* 结束CH376命令,仅用于SPI接口方式 */

void	xWriteCH376Cmd( UINT8 mCmd );	/* 向CH376写命令 */

void	xWriteCH376Data( UINT8 mData );	/* 向CH376写数据 */

UINT8	xReadCH376Data( void );			/* 从CH376读数据 */

UINT8	Query376Interrupt( void );		/* 查询CH376中断(INT#引脚为低电平) */

UINT8	mInitCH376Host( void );			/* 初始化CH376 */

#endif
