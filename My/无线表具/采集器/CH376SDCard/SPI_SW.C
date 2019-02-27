/* CH376芯片 软件模拟SPI串行连接的硬件抽象层 V1.0 */
/* 提供I/O接口子程序 */

//#include "stm32f10x.h"
#include	"HAL.H"

/* 安富莱开发板硬件连接方式如下(实际应用电路可以参照修改下述定义及子程序) 
	PF10             CH376的SCS引脚
	PA5/SPI1_SCK     CH376的SCK引脚
	PA6/SPI1_MISO    CH376的SDO引脚
	PA7/SPI1_MOSI	 CH376的SDI引脚
	PE2              CH376输出的中断信号
*/
#define CH376_SCK_0()	GPIO_ResetBits(GPIOB,  GPIO_Pin_13)
#define CH376_SCK_1()	GPIO_SetBits(GPIOB,  GPIO_Pin_13)

#define CH376_SDI_0()	GPIO_ResetBits(GPIOB,  GPIO_Pin_14)
#define CH376_SDI_1()	GPIO_SetBits(GPIOB,  GPIO_Pin_14)

#define CH376_SCS_0()	GPIO_ResetBits(GPIOB,  GPIO_Pin_12)
#define CH376_SCS_1()	GPIO_SetBits(GPIOB,  GPIO_Pin_12)

#define CH376_SDO_HIGH()	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) != Bit_RESET)
#define CH376_INT_HIGH()	(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) != Bit_RESET)	 //中断脚

void mDelay0_5uS( void )  /* 至少延时0.5uS,根据单片机主频调整 */
{
	UINT8 i;

	for (i = 0; i < 30; i++);
}

/* SPI输出8个位数据 */
void Spi376OutByte(UINT8 d)  
{  
	/* 如果是硬件SPI接口,应该是先将数据写入SPI数据寄存器,然后查询SPI状态寄存器以等待SPI字节传输完成 */
	UINT8	i;
	for ( i = 0; i < 8; i ++ ) 
	{
		CH376_SCK_0();
		if ( d & 0x80 )
		{
			CH376_SDI_1();
		}
		else
		{
			CH376_SDI_0();
		}
		d <<= 1;  /* 数据位是高位在前 */
		CH376_SCK_1();  /* CH376在时钟上升沿采样输入 */
	}
}

/* SPI输入8个位数据 */
UINT8 Spi376InByte( void )
{  /* 如果是硬件SPI接口,应该是先查询SPI状态寄存器以等待SPI字节传输完成,然后从SPI数据寄存器读出数据 */
	UINT8	i, d;
	d = 0;
	for ( i = 0; i < 8; i ++ ) 
	{
		CH376_SCK_0();  /* CH376在时钟下降沿输出 */
		d <<= 1;  /* 数据位是高位在前 */
		if ( CH376_SDO_HIGH() ) d ++;
		CH376_SCK_1();
	}
	return( d );
}

void xEndCH376Cmd( )
{
	 CH376_SCS_1();
}

void	xWriteCH376Cmd( UINT8 mCmd )  /* 向CH376写命令 */
{
	CH376_SCS_1();  /* 防止之前未通过xEndCH376Cmd禁止SPI片选 */
	mDelay0_5uS( );
	
	CH376_SCS_0();  /* SPI片选有效 */
	Spi376OutByte( mCmd );  /* 发出命令码 */
	
	/* 延时1.5uS确保读写周期大于1.5uS,或者用上面一行的状态查询代替 */
	mDelay0_5uS( ); 
	mDelay0_5uS( ); 
	mDelay0_5uS( );  
}

void xWriteCH376Data( UINT8 mData )  /* 向CH376写数据 */
{
	Spi376OutByte( mData );
//	mDelay0_5uS( );  /* 确保读写周期大于0.6uS */
}

UINT8 xReadCH376Data( void )  /* 从CH376读数据 */
{
//	mDelay0_5uS( );  /* 确保读写周期大于0.6uS */
	return( Spi376InByte( ) );
}

/* 查询CH376中断(INT#低电平) */
UINT8	Query376Interrupt( void )
{
	return( CH376_INT_HIGH() ? FALSE : TRUE );
}
/* 初始化CH376 */
UINT8 mInitCH376Host( void )  
{
	UINT8	res = 0;

	xWriteCH376Cmd(CMD11_CHECK_EXIST);  /* 测试单片机与CH376之间的通讯接口 */
	xWriteCH376Data(0x55);
	res = xReadCH376Data();
	xEndCH376Cmd();
	if ( res != 0xAA ) 
		return( ERR_USB_UNKNOWN );  /* 通讯接口不正常,可能原因有:
								接口连接异常,其它设备影响(片选不唯一),串口波特率,一直在复位,晶振不工作 */
	xWriteCH376Cmd( CMD11_SET_USB_MODE );  /* 设备USB工作模式 */
//	xWriteCH376Data( 0x06 );  //06H=已启用的主机方式并且自动产生SOF包
	xWriteCH376Data( 0x03 );  //03H=SD卡主机模式/未启用的主机模式,用于管理和存取SD卡中的文件
	mDelayuS(50 );
	res = xReadCH376Data( );
	xEndCH376Cmd( );

	if ( res == CMD_RET_SUCCESS ) return( USB_INT_SUCCESS );
	else return( ERR_USB_UNKNOWN );  /* 设置模式错误 */
}
