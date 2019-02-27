#include <string.h>
/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : platform_config.h
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : Evaluation board specific configuration file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
//typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
#define ADD_ONE 1
#define DEL_ONE 2
#define ALT_ONE 3
#define INI_ONE 3
#ifndef __PLATFORM_H
#define __PLATFORM_H
#include "PCF8563.h"
#include "wireless.h"
#include "Fragment.h"
#include "stm32f10x.h"
#include "adc.h"

#define CCR1_Val       40
#define CJQ_Num        20
#define SD_ALLMETERNUM_OFFSET  ((u16)126)
#define JZQINITNUM    ((u8)88)

#define TX_LED_ON()          GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define TX_LED_OFF()           GPIO_SetBits(GPIOC, GPIO_Pin_13)

#define RX_LED_ON()          GPIO_ResetBits(GPIOC, GPIO_Pin_15)
#define RX_LED_OFF()           GPIO_SetBits(GPIOC, GPIO_Pin_15)

#define UP_RS485_send()     	  GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define UP_RS485_Receive()	 	  GPIO_SetBits(GPIOC, GPIO_Pin_13)

#define Down_RS485_send()     	  GPIO_ResetBits(GPIOC, GPIO_Pin_15)
#define Down_RS485_Receive()	  GPIO_SetBits(GPIOC, GPIO_Pin_15)

#define USB_LED_OFF()		  GPIO_ResetBits(GPIOC, GPIO_Pin_15)
#define USB_LED_ON()     	  GPIO_SetBits(GPIOC, GPIO_Pin_15)


#define APC230_EN_L1()		  GPIO_ResetBits(GPIOD, GPIO_Pin_4)
#define APC230_EN_H1()     	  GPIO_SetBits(GPIOD, GPIO_Pin_4)
#define APC230_EN_L2()		  GPIO_ResetBits(GPIOE, GPIO_Pin_3)
#define APC230_EN_H2()     	  GPIO_SetBits(GPIOE, GPIO_Pin_3)

#define SLEEP_L1()		  GPIO_ResetBits(GPIOD, GPIO_Pin_5)
#define SLEEP_H1()     	  GPIO_SetBits(GPIOD, GPIO_Pin_5)
#define SLEEP_L2()		  GPIO_ResetBits(GPIOE, GPIO_Pin_4)
#define SLEEP_H2()     	  GPIO_SetBits(GPIOE, GPIO_Pin_4)

#define RESET_L1()		  GPIO_ResetBits(GPIOD, GPIO_Pin_6)
#define RESET_H1()     	  GPIO_SetBits(GPIOD, GPIO_Pin_6)
#define RESET_L2()		  GPIO_ResetBits(GPIOE, GPIO_Pin_5)
#define RESET_H2()     	  GPIO_SetBits(GPIOE, GPIO_Pin_5)
/* System Clocks Configuration */
void RCC_Configuration(void);
/* NVIC configuration */
void NVIC_Configuration(void);
/* Configure the GPIO ports */
void GPIO_Configuration(void);
/* USART configuration */
void USART_Configuration(void);
/* TIM configuration */
void TIM_Configuration(void);
/* I2C configuration */
void I2C_Configuration(void);
void Systick_config(void);
void ADC_Configuration(void);

void Send_UART(u8 ,u8 *,u16 );
void Compose_Up_Frame_Format(u8 *pMeterDataRecv,u16 G_RecvDataLen);

void dog(void);


void TestCH376(void);
u8 mInitCH376Host( void );
void mStopIfError(u8 );
void RstDog(void);
void Automatic_data_collection(void);
void EXTI_Configuration(void);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
void Analysis_user_profiles(void);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void Auto_Sent(u8 *,u16 );
void Upload_historical_data(char *);

void Add_Meter(char *);
void Delete_Meter(char *);


void SD_USB(void);
void SPI_FLASH_BulkErase(void);
u8 CRC8(u8 *ptr,u32 len);
u8 Add_Up (u8 *AddUpBuf,u16 AddUpLen);
u8 GetWorkingMode(void);
u16 HEX2BCD(u8 Data);
u8 BCD2HEX(u8 Data);
void Err_Ack(u8 *Buf,u8 ErrCode);
s32 JZQ_Addr_Check(u8*Buf);
void Compose_Down_Frame_Format(u8* Buf, u16 Len);
void Compose_Up_Frame_Format(u8 *pMeterDataRecv,u16 G_RecvDataLen);
void CJQUp_Protocol_Analysis(u8*pMeterDataRecv,u16 G_RecvDataLen);
s32  Init_JZQ(u8 *Buf,u16 Len);
s32  Computer_Concentrator_Protocol_Analysis(u8 *Buf,u16 Len);
void UART_4(u8 *buf,u16 sum);
void JZQ_NetAddr_Alter(u8* Buf,u16 Len);
void JZQ_Status_Read(u8* Buf,u16 Len);
void JZQ_NetAddr_Read(u8* Buf,u16 Len);
void Back_Wireless_Addr(u8* Buf,u16 Len);
void JZQ_Wireless_NetType_Read(u8* Buf,u16 Len);
void Back_Wireless_Parameter(u8* Buf,u16 Len);
void Back_Wireless_Addr_ALL(u8* Buf,u16 Len);
void Right_Ack_Len0(u8 *Buf,u16 Len);
void Back_SubNot_Status(u8* Buf,u16 Len);
void JZQ_Wireless_NetType_Write(u8* Buf,u16 Len);
void JZQ_DEL_Subnot(u8* Buf,u16 Len);
void JZQ_ADD_Subnot(u8* Buf,u16 Len);
void JZQ_NetAddr_Write(u8* Buf,u16 Len);
void JZQ_MainNotAddr_Write(u8* Buf,u16 Len);
void Set_Wireless_Parameter(u8* Buf,u16 Len);
void JZQ_RTC_Set(u8 *Buf,u16 Len);
void JZQ_RTC_Read(u8 *Buf,u16 Len);
void JZQ_RTC_Set_1(u8 *Buf,u16 Len);
void JZQ_RTC_Set_noret(u8 *Buf,u16 Len);
void Read_JZQ_CJQ_Addr(u8 *Buf,u16 Len);
typedef struct _JZQ_Status_
{
    u8 ST00:1;//保留
    u8 ST01:1;//保留
    u8 ST02:1;//
    u8 ST03:1;//电源电压
    u8 ST04:1;//备用电池电压
    u8 ST05:1;//MCU电压
    u8 ST06:1;//纽扣电池电压
    u8 ST07:1;//保留

    u8 ST10:1;//SD卡
    u8 ST11:1;//EEPROM
    u8 ST12:1;//FLASH		 //hu
    u8 ST13:1;//USB接口
    u8 ST14:1;//时钟信号   //li完成
    u8 ST15:1;//MCU内部温度
    u8 ST16:1;//蓝牙模块
    u8 ST17:1;//无线模块  //liu

    u8 ST20:1;//DTU模块
    u8 ST21:1;//环境湿度
    u8 ST22:1;//防盗
    u8 ST23:1;//存储空间  //hu
    u8 ST24:1;//校验异常  //hu
    u8 ST25:1;//网络冲突检测//li
    u8 ST26:1;//节点（无线模块）//liu
    u8 ST27:1;//无线模块信号强度

    u8 ST30:1;//时钟源	//li完成
    u8 ST31:1;//时钟差距 //li完成
    u8 ST32:1;//参数初始化//hu
    u8 ST33:1;//表号   //liu
    u8 ST34:1;//表通讯 //li //归采集器
    u8 ST35:1;//采集器号 //liu
    u8 ST36:1;//采集器通讯 //li完成
    u8 ST37:1;//保留

    u8 ST40:1;//保留
    u8 ST41:1;//保留
    u8 ST42:1;//保留
    u8 ST43:1;//保留
    u8 ST44:1;//保留
    u8 ST45:1;//保留
    u8 ST46:1;//保留
    u8 ST47:1;//保留


} __attribute__((packed)) JZQ_Status;
typedef struct _CJQ_Status_
{
    u8	flag;
    u32 Counter;
} __attribute__((packed)) CJQ_status;
typedef struct
{
	u8 ADDR[8];
} __attribute__((packed)) JZQ;
extern CJQ_status G_CJQ_status[20];
extern JZQ_Status G_status;
extern JZQ G_JZQ;
extern u8 G_RTC_Check_Flag;
void DEL_ONE_CJQ(u8* Buf,u16 Len);
s32 Read_Installed_CJQAddr(u8 *Buf,u16 Len);

s32 Read_JZQAddr(u8 *Buf,u16 Len);
s32 JZQ_Flash_SD_Store(u8 *Buf,u16 Len);
s32 Read_JZQ_Flash_Data(u8 *DataBuf,u16 Len);
s32 Back_JZQ_Work_Mode(u8 *DataBuf,u16 Len);
s32 DEL_JZQ_Meter_Data(u8 *DataBuf,u16 Len);
s32 Add_Multi_Meter(u8 *Buf,u16 Len);

s32 DEL_CJQ_Meters(u8 *Buf,u16 Len);
s32 ALT_CJQ_ADDR(u8 *Buf,u16 Len);
s32 Deal_One_Meter(u8 *Buf,u16 Len,u8 cmd);
s32 Back_JZQ_JieZhuan_Time(u8 *Buf,u16 Len);
s32 Deal_One_CJQ(u8 *Buf,u16 Len,u8 cmd);
u8 Change_JZQAddr(u8 *Buf,u16 Len);
s32 Deal_One_CJQ(u8 *Buf,u16 Len,u8 cmd);
s32 Add_Multi_CJQ(u8 *Buf,u16 Len);
s32 ChangeWorkingMode(u8 *Buf,u16 Len);
s32 ChangeJieZhuan_Time(u8 *Buf,u16 Len);
s32 Return_Meter_History_Data(u8 *Buf,u16 Len);
#endif
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
