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
#define PAGESIZE   ((u16)0x800)//stm32F103VET6ÿҳ2KByte��2048byte
extern  u8 G_PageTmp[PAGESIZE];
//W25Xϵ��/Qϵ��оƬ�б�
#define W25Q80 	0XEF13
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16

#define JZQ_ADDR	      	((u32)0x00000000)//��������ַ
#define JZQ_RFADDR	      	((u32)0x00000008)//����������ģ�����ڵ��ַ
#define CJQ_NUM	      		((u32)0x00000010)//�ɼ�������
#define JZQ_ALL_METER_NUM	((u32)0x00000011)//�������б��������
#define JZQ_WORKMODE	    ((u32)0x00000013)//����������ģʽ
#define JZQ_DATA_CARRY_TIME	((u32)0x00000014)//���ݽ�תʱ��
#define JZQ_WIRELESS_NetType ((u32)0x00000017)//����������ģ����������
#define EXT_FLASH_CHECK 	((u32)0x00084000)//flash���
#define JZQ_TIME_INTERVAL	((u32)0x00000017)//��������ʱ����ʱ����


#define CJQ_INDEX        	((u32)0x00001000)//�ɼ�������
#define CJQ_INDEX_CHECK     ((u32)0x00001014)//�ɼ�������У��
#define CJQ_ADDR        	((u32)0x00001015)//�ɼ�����ַ
#define CJQ_RFADDR        	((u32)0x000010B4)//�ɼ�������ģ���ӽڵ��ַ
#define CJQ_METER_NUM        ((u32)0x00001154)//�����ɼ����������
#define CJQ_METER_INDEX     ((u32)0x00002000)// �ɼ���1�������
#define CJQ_METER_ADDR		((u32)0x00003000)// �ɼ���1�б�ߵ�ַ
#define CJQ_METER_DATA		((u32)0x00004000)//�ɼ���1�е�1���������

/*������ַ*/
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


extern u16 SPI_FLASH_TYPE;//��������ʹ�õ�flashоƬ�ͺ�
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
s32 SPI_Flash_Write(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite);//д��flash
void SPI_Flash_Erase_Chip(void);    	  //��Ƭ����
void SPI_Flash_Erase_Sector(u32 Dst_Addr);//��������
void SPI_Flash_Wait_Busy(void);           //�ȴ�����
void SPI_Flash_PowerDown(void);           //�������ģʽ
void SPI_Flash_WAKEUP(void);			  //����
void Delay(u16 i);
#endif
















