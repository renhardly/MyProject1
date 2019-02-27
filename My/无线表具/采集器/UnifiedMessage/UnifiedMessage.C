#include "stm32f10x_lib.h"
#include "EXT_Flash.h"
#include "InterFlash.h"  
#include "UnifiedMessage.h"
void  CopyInterTOEXT(void)
{
	
	u8 i;
	u32 Start_Addr;
	u32 Write_Addr;
	for(i=0;i<132;i++) //内部flash总共256页，其中124页用于代码存储
	{
		Start_Addr=i*2048+0x0807F000;
		Write_Addr=i*4096+0x0807F000;
		ReadFlash_Byte(Start_Addr,G_PageTmp,PAGESIZE );
		SPI_Flash_Write(Write_Addr,G_PageTmp,PAGESIZE);
	}

}
u8 UnifiedMessage(void)
{
	u8 InterMeterNum=0;
	u8 EXTMeterNum=0;
	ReadFlash_Byte(0x0807F011,&InterMeterNum,1 );
	SPI_Flash_Read(0x00000011,&EXTMeterNum,1);
	if (InterMeterNum!=EXTMeterNum)
	{
		CopyInterTOEXT();
		return 0;
	}
	else
		return 1;
}
