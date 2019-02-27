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
#ifndef __PLATFORM_H
#define __PLATFORM_H
#include "PCF8563.h"
#include "Valve.h"
#include "wireless.h"
#include "fragment.h"
#include "delay.h"
#include "stm32f10x.h"
#include "string.h"
#include "stdlib.h"
#include "adc.h"
#define CCR1_Val  40
//typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

#define M_RS485_LED_ON()			GPIO_ResetBits(GPIOC, GPIO_Pin_15)
#define M_RS485_LED_OFF()			GPIO_SetBits(GPIOC, GPIO_Pin_15)

#define S_RS485_LED_ON()			GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define S_RS485_LED_OFF()			GPIO_SetBits(GPIOC, GPIO_Pin_13)

#define GPRS_LED_OFF()				GPIO_ResetBits(GPIOC, GPIO_Pin_0)
#define GPRS_LED_ON()				GPIO_SetBits(GPIOC, GPIO_Pin_0)


#define TX_CTL_OFF()			GPIO_ResetBits(GPIOE, GPIO_Pin_3)
#define TX_CTL_ON()				GPIO_SetBits(GPIOE, GPIO_Pin_3)

#define RX_CTL_OFF()			GPIO_ResetBits(GPIOE, GPIO_Pin_4)
#define RX_CTL_ON()				GPIO_SetBits(GPIOE, GPIO_Pin_4)

/* System Clocks Configuration */
void RCC_Configuration(void);
/* NVIC configuration */
void NVIC_Configuration(void);
/* Configure the GPIO ports */
void GPIO_Configuration(void);
/* USART configuration */
void USART_Configuration(void);

void TIM_Configuration(void);
/* ADC configuration */
void ADC_Configuration(void);

void Send_UART(u8 ,u8 *,u16 );
u16 Get_Adc(u8 );
u16 Get_Temp(u8 );

void dog(void);
void RstDog(void);

void Auto_sent(u8 *,u16 );
void UART_1(u8 *,u16 );
void UART_2(u8 *,u16 );
void UART_3(u8 *,u16 );
void UART_4(u8 *,u16 );
void UART_5(u8 *buf,u16 sum);

u8 CRC8(u8 *ptr,u32 len);
u8 Add_Up (u8 *AddUpBuf,u16 AddUpLen);

/*****************通讯相关函数**********************/
u8   Computer_Concentrator_Protocol_Analysis(u8 *,u16);
void Err_Ack(u8 *Buf,u8 ErrCode);
void Right_Ack_Len0(u8 *Buf,u16 Len);
void Right_Ack_Change_CJQAddr(u8 *Buf,u16 Len);
void Right_Ack_Change_MeterAddr(u8 *Buf,u16 Len);
u8   CJQ_Addr_Check(u8*Buf);
void Read_Data_Command(u8*Buf);
void Read_All_Meter_Data_Command(u8*Buf);
void Read_One_Meter_Data(u8 *Buf,u16 Len);
void Read_All_Meter_Data(u8 *Buf,u16 Len);
u8   Read_CJQAddr(u8 *Buf,u16 Len);
u8 Composite_Frame_Format(u8*CmdBuf,u16 MeterLocation,u8 ExtOrInter);
void Composite_Read_All_Meter_Frame_Format(u8*CmdBuf,u16 MeterLocation,u8 ExtOrInter);
void Read_One_Meter_Realtime_Data(u8 *Buf,u16 Len);
void Read_All_Meter_Realtime_Data(u8 *Buf,u16 Len);
u8   Read_Meter_Installation_Information(u8 *Buf,u16 Len);
void Return_Real_Data(u8*MeterDataRecv,u16 MeterDataLen);

u8	 Init_CJQ(u8 *Buf,u16 Len);
u8   Add_One_Meter(u8 *Buf,u16 Len);
void Change_CJQAddr(u8 *Buf,u16 Len);
u8   GetWorkingMode(void);
u8   Copy_Inter_To_EXT(void);
u8   Copy_CJQ_Parameter_From_Inter_To_EXT(void);
u8   Copy_MeterCheck_And_MeterCheckCheck_From_Inter_To_EXT(void);
u8   Copy_Meter_Num_From_Inter_To_EXT(void);
u8   Copy_Meter_Data_From_Inter_To_EXT(void);
u8   Copy_WenKong_Address_From_Inter_To_EXT(void);
u8   Check_Index_Equ(void);
u8   Save_Meter_Data(u8 *DataBuf,u16 DataLen);
u8   Auto_Save_Meter_Data(u8 SaveLocation ,u8 *DataBuf,u16 DataLen);
void Auto_Get_Meter_Data(void);


void Write_Meter_RTC(u8 *meteradd,u8 *data,u8 len);

u8  Change_One_Meter(u8 *Buf,u16 Len);
u8  Delete_One_Meter(u8 *Buf,u16 Len);
u8  Delete_All_Meter(u8 *Buf,u16 Len);
u8 Read_Electronic_Meter_Addr(u8 *Buf,u16 Len);
u8  Read_All_Installed_Meter_Number(u8 *Buf,u16 Len);
u8  Read_CJQ_Work_Mode(u8 *Buf,u16 Len);
u8  Read_CJQ_Time_Interval(u8 *Buf,u16 Len);
u8  Change_CJQ_Time_Interval(u8 *Buf,u16 Len);
u8  Change_CJQ_Work_Mode(u8 *Buf,u16 Len);

u16 HEX2BCD(u8 Data);
u8 BCD2HEX(u8 Data);
extern u8 G_ACK_kind,G_ACK_Accepted,G_ACK_COUNTER,G_METER_ADDR_TMP[7];
void Meter_Up_Protocol_Analysis(u8*pMeterDataRecv,u16 G_RecvDataLen);
void CJQ_Wireless_NetType_Read(u8* Buf,u16 Len);
void CJQ_Wireless_NetType_Set(u8* Buf,u16 Len);
void CJQ_NetAddr_Alter(u8* Buf,u16 Len);
void Back_Wireless_Addr(u8* Buf,u16 Len);
void Back_Wireless_Parameter(u8* Buf,u16 Len);
void CJQ_Addr_Alter(u8* Buf,u16 Len);
void Set_Wireless_Parameter(u8* Buf,u16 Len);
void Set_Wireless_Addr(u8* Buf,u16 Len);
void CJQ_Status_Read(u8* Buf,u16 Len);
void CJQ_RTC_Read(u8 *Buf,u16 Len);
void CJQ_RTC_Set(u8 *Buf,u16 Len);
void Write_Meter_RTC_FLAG(u8* Buf,u16 Len);
void CJQ_RTC_Set_ALL(u8 *Buf,u16 Len);
void Write_Meter_RTC_ACK(u8 *pMeterDataRecv,u16 G_RecvDataLen);
void CJQ_RTC_Set_ONE(u8 *Buf,u16 Len);
extern u8 G_RTC_Check_Flag;
#define METER_RTC_SET 1
#define METER_VALVE_SET 2
#define METER_ELCSYN_SET 3
#define METER_Settlement_Date_SET  4
#define METER_Meter_Reading_SET	5
#define METER_WR_Purchase_Amount_SET 6
#define METER_New_Key_SET 7
#define Meter_WR_Remaining_Amount_SET	 8
#define Meter_Read_Price_List_SET	  9
#define Meter_RD_Purchase_Amount_SET   10
#define Meter_RD_ADDR_SET		11
#define METER_Write_Price_SET	12
#define METER_HEAP_PSIZE 20
typedef struct _CJQ_Status_
{
    u8 ST00:1;//M-bus电压
    u8 ST01:1;//M-bus电流
    u8 ST02:1;//M-bus过载/短路
    u8 ST03:1;//电源电压
    u8 ST04:1;//备用电池电压
    u8 ST05:1;//MCU电压
    u8 ST06:1;//纽扣电池电压
    u8 ST07:1;//保留

    u8 ST10:1;//SD卡
    u8 ST11:1;//EEPROM
    u8 ST12:1;//FLASH	 //hu
    u8 ST13:1;//USB接口
    u8 ST14:1;//时钟信号 //li完成
    u8 ST15:1;//MCU内部温度
    u8 ST16:1;//蓝牙模块
    u8 ST17:1;//无线模块   //liu

    u8 ST20:1;//DTU模块
    u8 ST21:1;//环境湿度
    u8 ST22:1;//防盗
    u8 ST23:1;//存储空间  //hu
    u8 ST24:1;//校验异常  //hu
    u8 ST25:1;//网络冲突检测
    u8 ST26:1;//节点（无线模块）//liu
    u8 ST27:1;//无线模块信号强度

    u8 ST30:1;//时钟源	//li完成
    u8 ST31:1;//时钟差距//li完成
    u8 ST32:1;//参数初始化//hu
    u8 ST33:1;//表号	//liu
    u8 ST34:1;//表通讯	//li完成
    u8 ST35:1;//保留
    u8 ST36:1;//保留
    u8 ST37:1;//保留

    u8 ST40:1;//保留
    u8 ST41:1;//保留
    u8 ST42:1;//保留
    u8 ST43:1;//保留
    u8 ST44:1;//保留
    u8 ST45:1;//保留
    u8 ST46:1;//保留
    u8 ST47:1;//保留


} __attribute__((packed)) CJQ_Status;
extern CJQ_Status G_status;
typedef	 struct
{
	u8 *data_p;
	u8 type;
	u16 cmd_index[16]; 
    u8 counter_down;
} __attribute__((packed))CMD_CONTRL;
typedef struct
{
	u8 ADDR[8];
} __attribute__((packed)) CJQ;
extern CJQ G_CJQ;
extern u8 G_CMD_num;
void METER_Err_Ack(u8 *Buf,u8 ErrCode);
s32 Return_Meter_Index(u8 *meter_add);
void Clear_Meter_Data(u8* Buf,u16 Len);
u8  Add_One_Meter_t(u8 *Buf,u16 Len);//改
/***********hu**************/
void Meter_Up_FrameFormat_NO_Data_NO_ST(u8 *pMeterDataRecv,u16 G_RecvDataLen);
u8  Read_All_unstalled_Meter_Number(u8 *Buf,u16 Len);
u8  _Meter_Up_Protocol_Analysis(u8 *Buf,u16 Len);		//hu
u8  Meter_Up_FrameFormat(u8 *DataBuf,u16 DataLen);	//hu
u8  Meter_Up_Err_FrameFormat(u8 *DataBuf,u16 DataLen);	
void _Switch_Valve(u8 *meteradd,u8 *data,u8 len);
void _Write_Ele_and_Mechan_Syn_Data(u8 *meteradd,u8 *data,u8 len);
void Write_Price_List(u8 *Buf,u16 Len);
void _Write_Price_List(u8 *meteradd,u8 *data,u8 len);
void Settlement_Date(u8 *Buf,u16 Len);
void _Settlement_Date(u8 *meteradd,u8 *data,u8 len);
void Meter_Reading(u8 *Buf,u16 Len);
void _Meter_Reading(u8 *meteradd,u8 *data,u8 len);
void Meter_WR_Purchase_Amount(u8 *Buf,u16 Len);
void _Meter_WR_Purchase_Amount(u8 *meteradd,u8 *data,u8 len);
void Meter_New_Key(u8 *Buf,u16 Len);
void _Meter_New_Key(u8 *meteradd,u8 *data,u8 len);
void Meter_WR_Remaining_Amount(u8 *Buf,u16 Len);
void _Meter_WR_Remaining_Amount(u8 *meteradd,u8 *data,u8 len);
void Meter_Read_Price_List(u8 *Buf,u16 Len);
void _Meter_Read_Price_List(u8 *meteradd,u8 *data,u8 len);
void Meter_RD_Purchase_Amount(u8 *Buf,u16 Len);
void _Meter_RD_Purchase_Amount(u8 *meteradd,u8 *data,u8 len);
void Meter_RD_ADDR(u8 *Buf,u16 Len);
void _Meter_RD_ADDR(u8 *meteradd,u8 *data,u8 len);
#define G_Ative 0//hu
#endif
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

