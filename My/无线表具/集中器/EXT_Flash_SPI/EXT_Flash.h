#ifndef __FLASH_H
#define __FLASH_H
#include "stm32f10x.h"
/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
typedef struct
{
    u8 Data_String[1024];
}STRUCT_CHAR;
/* Private define ------------------------------------------------------------*/
#define PAGESIZE   ((u16)0x800)//stm32F103VET6每页2KByte，2048byte
extern  u8 G_PageTmp[PAGESIZE];
//W25X系列/Q系列芯片列表
#define W25Q80 	0XEF13
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16

#define JZQ_ADDR	      	((u32)0x00000000)//集中器地址
#define JZQ_RFADDR	      	((u32)0x00000008)//集中器无线模块主节点地址
#define CJQ_NUM	      		((u32)0x00000010)//采集器数量
#define JZQ_ALL_METER_NUM	((u32)0x00000011)//集中器中表具总数量
#define JZQ_WORKMODE	    ((u32)0x00000013)//集中器工作模式
#define JZQ_DATA_CARRY_TIME	((u32)0x00000014)//数据结转时间
#define JZQ_WIRELESS_NetType ((u32)0x00000017)//集中器无线模块网络类型
#define EXT_FLASH_CHECK 	((u32)0x00084000)//flash检查
#define JZQ_TIME_INTERVAL	((u32)0x00000017)//集中器定时抄表时间间隔


#define CJQ_INDEX        	((u32)0x00001000)//采集器索引
#define CJQ_INDEX_CHECK     ((u32)0x00001014)//采集器索引校验
#define CJQ_ADDR        	((u32)0x00001015)//采集器地址
#define CJQ_RFADDR        	((u32)0x000010B4)//采集器无线模块子节点地址
#define CJQ_METER_NUM        ((u32)0x00001154)//各个采集器表具数量
#define CJQ_METER_INDEX     ((u32)0x00002000)// 采集器1表具索引
#define CJQ_METER_ADDR		((u32)0x00003000)// 采集器1中表具地址
#define CJQ_METER_DATA		((u32)0x00004000)//采集器1中第1个表具数据

/*扇区地址*/
//#define JZQ_PARAMETER_Sector    	0;
//#define CJQ_NUM_Sector    			1;
//#define CJQ1_METER_CHECK_Sector     2;
//#define CJQ1_METER_ADDR_Sector      3;


#define WENKONG_SIZE			   1024
#define CJQ_ADDR_SIZE			   362
#define CJQ_NUM_SIZE			   20
#define JZQ_PARAMETER_SIZE		   100//23
#define METER_CHECK_SIZE		   256+1+1
#define METER_ADDR_SIZE			   256*8
#define METER_DATA_SIZE			   256
#define METERINDEX_TO_METERINDEX  (u32)130*4096


extern u16 SPI_FLASH_TYPE;//定义我们使用的flash芯片型号
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
s32 SPI_Flash_Write(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite);//写入flash
void SPI_Flash_Erase_Chip(void);    	  //整片擦除
void SPI_Flash_Erase_Sector(u32 Dst_Addr);//扇区擦除
void SPI_Flash_Wait_Busy(void);           //等待空闲
void SPI_Flash_PowerDown(void);           //进入掉电模式
void SPI_Flash_WAKEUP(void);			  //唤醒
void Delay(u16 i);
#endif
















