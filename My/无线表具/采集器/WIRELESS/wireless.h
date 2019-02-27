 /******************** 山东三龙采集器项目 ********************
* File Name          : wireless.h
* Author             : liuyong
* Date First Issued  : 08/09/2013
* Description        : 万康自组网无线模块相关的操作函数
********************************************************************************/
#ifndef __WIRELESS_H
#define __WIRELESS_H


#include "platform_config.h"

/* Private typedef -----------------------------------------------------------*/
//typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


/* Private define ------------------------------------------------------------*/
//设置参数的类型
#define TYPE_ADDR				11
#define TYPE_CHANNEL_NUM		22
#define TYPE_TRANSMIT_POWER		33
#define TYPE_ALL_PARAMETER		44

#define CJQ_UART				1
#define JZQ_UART				0


extern u8  G_WireLess_Back;
extern u8  G_UpSendCmd[128];
extern u8  G_UART_Num; 

/* Function declaration -------------------------------------------------------------*/

//u8 Get_WireLess_Addr(u8* pAddr);
//u8 Set_WireLess_Addr(u8* pAddr);
//u8 Set_WireLess_Channel_Num(u8* pAddr);
//u8 Set_WireLess_Transmit_Power(u8* pAddr);
u8 Get_WireLess_Parameter(u8* pParameter);
u8 Set_WireLess_Parameter(u8 type,u8* pParameter);
u8 Get_WireLess_Routing(u8* pAddr,u8* pRouting);
#endif /* __WIRELESS_H */

