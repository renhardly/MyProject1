/******************** ɽ�������ɼ�����Ŀ ********************
* File Name          : flash.h
* Author             : liuyong
* Date First Issued  : 07/17/2013
* Description        : flash�洢��صĺ궨�塢ȫ�ֱ�������������
********************************************************************************/
#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f10x.h"
//#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define PAGESIZE   ((u16)0x800)//stm32F103VET6ÿҳ2KByte��2048byte
extern  u8 G_PageTmp[PAGESIZE];
#define WENKONG_PAGEADDR	      ((u32)0x0807F800)
#define CJQ_PARAMETER_PAGEADDR	  ((u32)0x0807F000)
#define CJQ_WORK_MODE	          ((u32)0x0807F010)//�ɼ�������ģʽ
#define METER_NUM_ADDR		      ((u32)0x0807F011)//���������ַ
#define TIME_INTERVAL			  ((u32)0x0807F012)//����ʱ����
#define WIRELESS_NETTYPE_ADDR     ((u32)0x0807F015)//����ģ����������
#define METER_CHECK_PAGEADDR	  ((u32)0x0807E800)
#define METER_NUM_PAGEADDR		  ((u32)0x0807E000)
#define METER_DATA_PAGEADDR		  ((u32)0x0807D800)	//ע�⣬�������ɸ�ҳ���ҳ��,ÿҳ�����
													//
#define CODE_END_PAGEADDR		  ((u32)0x0803DFFF)	//����ռ�Ľ�����ַ
#define FLASH_END_PAGEADDR		  ((u32)0x0807FFFF)	//����flash�Ľ�����ַ

#define WENKONG_SIZE			   1024
#define CJQ_PARAMETER_SIZE		   100
#define METER_CHECK_SIZE		   256+1
#define METER_NUM_SIZE			   256*8
#define METER_DATA_SIZE			   256

/* Private macro -------------------------------------------------------------*/



u8 WriteFlash_Page(u32 Page_Addr,u32 Page_Offset,u8 *Write_Buf,u16 Write_Len);
//void ReadFlash_Halfword(u32 Start_Addr,u16 *Read_Buf,u16 Read_Len ); 
void ReadFlash_Byte(u32 Start_Addr,u8 *Read_Buf,u16 Read_Len );

void FlashErase_Page(u32 Start_Addr );



 //W25Xϵ��/Qϵ��оƬ�б�	   
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16

#define EXT_CJQ_PARAMETER_PAGEADDR	  ((u32)0x00000000)//�ɼ�����ʼ��������ַ
#define EXT_CJQ_WORK_MODE	          ((u32)0x00000010)//�ɼ�������ģʽ��ַ
#define EXT_METER_NUM_ADDR		      ((u32)0x00000011)//���������ַ
#define EXT_TIME_INTERVAL			  ((u32)0x00000012)//����ʱ����
#define EXT_WENKONG_PAGEADDR	      ((u32)0x00000400)//�¿ص�ַ

#define EXT_METER_CHECK_PAGEADDR	  ((u32)0x00001000)// �������ҳ��ַ
#define EXT_METER_NUM_PAGEADDR		  ((u32)0x00002000)// ���ID��ַ
#define EXT_METER_DATA_PAGEADDR		  ((u32)0x00003000)//�������ɵ�ҳ���ҳ��

#define EXT_FLASH_CHECK		  		  ((u32)0x001FF000)//�����ж�FLASH�Ƿ���������

//#ifndef PAGESIZE
//#define PAGESIZE   ((u16)0x800)//stm32F103VET6ÿҳ2KByte��2048byte
extern u16 SPI_FLASH_TYPE;//��������ʹ�õ�flashоƬ�ͺ�
//extern u8 G_PageTmp[PAGESIZE];
//#endif		 
//ָ���
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
#define SPI_FLASH_CS_Low()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)//ʹ��Ƭѡ
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4) //ȡ��Ƭѡ


void SPI_Flash_Init(void);
u16  SPI_Flash_ReadID(void);  	    //��ȡFLASH ID
u8	 SPI_Flash_ReadSR(void);        //��ȡ״̬�Ĵ��� 
void SPI_FLASH_Write_SR(u8 sr);  	//д״̬�Ĵ���
void SPI_FLASH_Write_Enable(void);  //дʹ�� 
void SPI_FLASH_Write_Disable(void);	//д����
void SPI_Flash_Write_NoCheck(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite);
void SPI_Flash_Read(u32 ReadAddr,u8* pBuffer,u16 NumByteToRead);   //��ȡflash
u8 SPI_Flash_Write(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite);//д��flash
void SPI_Flash_Erase_Chip(void);    	  //��Ƭ����
void SPI_Flash_Erase_Sector(u32 Dst_Addr);//��������
void SPI_Flash_Wait_Busy(void);           //�ȴ�����
void SPI_Flash_PowerDown(void);           //�������ģʽ
void SPI_Flash_WAKEUP(void);			  //����
void Delay(u16 i);
#endif /* __FLASH_H */

