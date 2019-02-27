/******************** ɽ�������ɼ�����Ŀ ********************
* File Name          : wireless.h
* Author             : liuyong
* Date First Issued  : 08/09/2013
* Description        : ������������ģ����صĲ�������
********************************************************************************/
#ifndef __FRAGMENT_H
#define __FRAGMENT_H

#include "string.h"
#include "stdlib.h"
#include "platform_config.h"

/* Private typedef -----------------------------------------------------------*/
/*------------------��Ƭͷ��-------------------------------*/
typedef	 struct _FRAGMENT_HEADER_	//�ֽڶ���
{
    u32		meshID;

    u8		reserved1;
    u8		reserved2;
    u8		reserved3;
    u8		serial_num;	//Դ���ݱ��ı��

    u16		length:9;
    u8		index:7;
    u8		lengthID;
    u8		check;

} __attribute__((packed)) FRAGMENT_HEADER;

/*------------------��Ƭ����״���ṹ��------------------------*/
typedef	 struct _COMBINE_STATUS_
{
    u8		package_num;	//Դ���ݱ��ı��
    u8		fragment_num;	//��Ƭ��
    u8		received[8];	//�ѽ��յ�Ƭ����Ӧ������1

    u8		index;		   //��ǰƬ����
    u8		faultcode;	   //��ǰƬ���ϴ��룬���궨��
    u8		completed;	   //������ɱ�־��0Ϊδ��ɣ�1Ϊ���,���ϲ���0

} __attribute__((packed)) COMBINE_STATUS;

/* Private define ------------------------------------------------------------*/
#define		MAXPACKAGELEN		277	 //���ݰ���󳤶�
#define		MTULEN				80	 //����䵥Ԫ
#define		MTUDATALEN			68	 //����䵥Ԫ�����ݳ���

#define		ERROR_NON			0	 //��Ƭ��ȷ
#define		ERROR_LEN			1	 //��Ƭ���ȴ���
#define		ERROR_CS			2	 //��ƬУ�����
#define		ERROR_PACKET_Num	3	 //���ı�Ŵ���

extern u8  G_Max_Package[MAXPACKAGELEN]; //�ɼ�������Ҫ��Ƭ�����ݰ�
extern u8  G_CombinData[MAXPACKAGELEN]; //������ɺ������

extern	COMBINE_STATUS  sSTATUS; //��Ƭ����״��

/* Function declaration -------------------------------------------------------------*/

void Send_Fragment(u8* pOriginal,u16 nL);
void Combine_Fragment(u8* pFragment,u8 nL);
void _delay_ms(u16 nMs);
#endif /* __WIRELESS_H */

