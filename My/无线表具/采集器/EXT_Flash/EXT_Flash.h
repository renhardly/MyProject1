#ifndef __FLASH_H
#define __FLASH_H	
	 
 //W25X系列/Q系列芯片列表	   
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16

#define EXT_CJQ_PARAMETER_PAGEADDR	  ((u32)0x00000000)//采集器初始化参数地址
#define EXT_METER_NUM_ADDR		      ((u32)0x00000011)//表具数量地址
#define EXT_METER_CHECK_PAGEADDR	  ((u32)0x00001000)// 表具索引页地址
#define EXT_METER_NUM_PAGEADDR		  ((u32)0x00002000)// 表具ID地址
#define EXT_METER_DATA_PAGEADDR		  ((u32)0x00003000)//表数据由高页向低页存

//#ifndef PAGESIZE
//#define PAGESIZE   ((u16)0x800)//stm32F103VET6每页2KByte，2048byte
extern u16 SPI_FLASH_TYPE;//定义我们使用的flash芯片型号
//extern u8 G_PageTmp[PAGESIZE];
//#endif		 
//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
#define SPI_FLASH_CS_Low()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)//使能片选
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4) //取消片选


void SPI_Flash_Init(void);
u16  SPI_Flash_ReadID(void);  	    //读取FLASH ID
u8	 SPI_Flash_ReadSR(void);        //读取状态寄存器 
void SPI_FLASH_Write_SR(u8 sr);  	//写状态寄存器
void SPI_FLASH_Write_Enable(void);  //写使能 
void SPI_FLASH_Write_Disable(void);	//写保护
void SPI_Flash_Write_NoCheck(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite);
void SPI_Flash_Read(u32 ReadAddr,u8* pBuffer,u16 NumByteToRead);   //读取flash
u8 SPI_Flash_Write(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite);//写入flash
void SPI_Flash_Erase_Chip(void);    	  //整片擦除
void SPI_Flash_Erase_Sector(u32 Dst_Addr);//扇区擦除
void SPI_Flash_Wait_Busy(void);           //等待空闲
void SPI_Flash_PowerDown(void);           //进入掉电模式
void SPI_Flash_WAKEUP(void);			  //唤醒
void Delay(u16 i);
#endif
















