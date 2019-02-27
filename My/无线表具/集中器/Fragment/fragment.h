/******************** 山东三龙采集器项目 ********************
* File Name          : wireless.h
* Author             : liuyong
* Date First Issued  : 08/09/2013
* Description        : 万康自组网无线模块相关的操作函数
********************************************************************************/
#ifndef __FRAGMENT_H
#define __FRAGMENT_H

#include "string.h"
#include "stdlib.h"
#include "platform_config.h"

/* Private typedef -----------------------------------------------------------*/
/*------------------分片头部-------------------------------*/
typedef	 struct _FRAGMENT_HEADER_	//字节对齐
{
    u32		meshID;

    u8		reserved1;
    u8		reserved2;
    u8		reserved3;
    u8		serial_num;	//源数据报文编号

    u16		length:9;
    u8		index:7;
    u8		lengthID;
    u8		check;

} __attribute__((packed)) FRAGMENT_HEADER;

/*------------------分片重组状况结构体------------------------*/
typedef	 struct _COMBINE_STATUS_
{
    u8		package_num;	//源数据报文编号
    u8		fragment_num;	//总片数
    u8		received[8];	//已接收的片，相应索引置1

    u8		index;		   //当前片索引
    u8		faultcode;	   //当前片故障代码，见宏定义
    u8		completed;	   //重组完成标志。0为未完成，1为完成,由上层置0

} __attribute__((packed)) COMBINE_STATUS;

/* Private define ------------------------------------------------------------*/
#define		MAXPACKAGELEN		277	 //数据包最大长度
#define		MTULEN				80	 //最大传输单元
#define		MTUDATALEN			68	 //最大传输单元的数据长度

#define		ERROR_NON			0	 //分片正确
#define		ERROR_LEN			1	 //分片长度错误
#define		ERROR_CS			2	 //分片校验错误
#define		ERROR_PACKET_Num	3	 //报文编号错误

extern u8  G_Max_Package[MAXPACKAGELEN]; //采集器中需要分片的数据包
extern u8  G_CombinData[MAXPACKAGELEN]; //重组完成后的数据

extern	COMBINE_STATUS  sSTATUS; //分片重组状况

/* Function declaration -------------------------------------------------------------*/

void Send_Fragment(u8* pOriginal,u16 nL);
void Combine_Fragment(u8* pFragment,u8 nL);
void _delay_ms(u16 nMs);
#endif /* __WIRELESS_H */

