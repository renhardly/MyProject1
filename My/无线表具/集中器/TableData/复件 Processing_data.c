#include <stdio.h>
#include <string.h>
#include "stm32f10x_lib.h"
#include "platform_config.h"

/*兰吉尔
char data_buf1[]={0x68,0xC8,0xC8,0x68,0x08,0x00,0x72,0x53,0x87,0x57,0x66,0xA7,0x32,0x03,0x04,0x16,0x10,0x00,0x00,0x09,
				0x74,0x08,0x09,0x70,0x08,0x0C,0x06,0x78,0x56,0x34,0x12,0x0C,0x14,0x12,0x34,0x56,0x78,0x0B,0x2D,0x56,
				0x34,0x12,0x0B,0x3B,0x56,0x34,0x12,0x0A,0x5B,0x00,0x00,0x0A,0x5F,0x00,0x00,0x0A,0x62,0x00,0x00,0x4C,
				0x14,0x00,0x00,0x00,0x00,0x4C,0x06,0x00,0x00,0x00,0x00,0x0C,0x78,0x53,0x87,0x57,0x66,0x0C,0x22,0x51,
				0x28,0x00,0x00,0x3C,0x22,0x51,0x28,0x00,0x00,0x7C,0x22,0x00,0x00,0x00,0x00,0x42,0x6C,0x01,0x01,0x8C,
				0x01,0x06,0x00,0x00,0x00,0x00,0xCC,0x01,0x06,0x00,0x00,0x00,0x00,0x8C,0x02,0x06,0x00,0x00,0x00,0x00,
				0xCC,0x02,0x06,0x00,0x00,0x00,0x00,0x8C,0x03,0x06,0x00,0x00,0x00,0x00,0xCC,0x03,0x06,0x00,0x00,0x00,
				0x00,0x8C,0x04,0x06,0x00,0x00,0x00,0x00,0xCC,0x04,0x06,0x00,0x00,0x00,0x00,0x8C,0x05,0x06,0x00,0x00,
				0x00,0x00,0xCC,0x05,0x06,0x00,0x00,0x00,0x00,0x8C,0x06,0x06,0x00,0x00,0x00,0x00,0xCC,0x06,0x06,0x00,
				0x00,0x00,0x00,0x8C,0x07,0x06,0x00,0x00,0x00,0x00,0xCC,0x07,0x06,0x00,0x00,0x00,0x00,0x8C,0x08,0x06,
				0x00,0x00,0x00,0x00,0x2E,0x16};*/
		//卡姆鲁普401		
/*u8 data_buf1[]={0x68,0xBE,0xBE,0x68,0x08,0x51,0x72,0x81,0x16,0x71,0x08,0x2D,0x2C,0x02,0x0C,0x01,0x00,0x00,0x00,0x0C,
				0x78,0x81,0x16,0x71,0x08,0x04,0x0F,0x00,0x00,0x00,0x00,0x04,0x14,0x00,0x00,0x00,0x00,0x04,0x22,0x99,
				0x20,0x00,0x00,0x04,0x59,0x2D,0x0A,0x00,0x00,0x04,0x5D,0x1E,0x09,0x00,0x00,0x04,0x61,0x0F,0x01,0x00,
				0x00,0x04,0x2D,0x00,0x00,0x00,0x00,0x14,0x2D,0x12,0x34,0x56,0x78,0x04,0x3B,0x00,0x00,0x00,0x00,0x14,
				0x3B,0x00,0x00,0x00,0x00,0x84,0x40,0x14,0x00,0x00,0x00,0x00,0x84,0x80,0x40,0x14,0x00,0x00,0x00,0x00,
				0x04,0x6D,0x20,0x23,0x69,0x1A,0x44,0x0F,0x00,0x00,0x00,0x00,0x44,0x14,0x00,0x00,0x00,0x00,0x54,0x2D,
				0x00,0x00,0x00,0x00,0x54,0x3B,0x00,0x00,0x00,0x00,0xC4,0x40,0x14,0x00,0x00,0x00,0x00,0xC4,0x80,0x40,
				0x14,0x00,0x00,0x00,0x00,0x42,0x6C,0x61,0x16,0x0F,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x77,0x7D,
				0x00,0x00,0x60,0x58,0x29,0x00,0x01,0x41,0x0C,0x01,0x00,0x70,0x01,0x0B,0x34,0x16};*/
		//卡姆鲁普402
/*u8 data_buf1[]={0x68,0xF7,0xF7,0x68,0x08,0x13,0x72,0x19,0x25,0x20,0x60,0x2D,0x2C,0x0B,0x0C,0x01,0x10,0x00,0x00,0x0C,
				0x78,0x19,0x25,0x20,0x60,0x04,0x06,0x78,0x56,0x34,0x12,0x04,0x14,0x5F,0x00,0x00,0x00,0x04,0x22,0x86,
				0x06,0x00,0x00,0x04,0x59,0xC5,0x08,0x00,0x00,0x04,0x5D,0xAA,0x08,0x00,0x00,0x04,0x61,0x1B,0x00,0x00,
				0x00,0x04,0x2D,0x00,0x00,0x00,0x00,0x14,0x2D,0x3E,0x00,0x00,0x00,0x04,0x3B,0x00,0x00,0x00,0x00,0x14,
				0x3B,0xB6,0x03,0x00,0x00,0x84,0x10,0x06,0x00,0x00,0x00,0x00,0x84,0x20,0x06,0x00,0x00,0x00,0x00,0x84,
				0x40,0x14,0xB1,0xEB,0x01,0x00,0x84,0x80,0x40,0x14,0x4E,0x1E,0x01,0x00,0x84,0xC0,0x40,0x06,0x00,0x00,
				0x00,0x00,0x04,0x6D,0x18,0x26,0x83,0x09,0x44,0x06,0x00,0x00,0x00,0x00,0x44,0x14,0x00,0x00,0x00,0x00,
				0x54,0x2D,0x00,0x00,0x00,0x00,0x54,0x3B,0x00,0x00,0x00,0x00,0xC4,0x10,0x06,0x00,0x00,0x00,0x00,0xC4,
				0x20,0x06,0x00,0x00,0x00,0x00,0xC4,0x40,0x14,0x00,0x00,0x00,0x00,0xC4,0x80,0x40,0x14,0x00,0x00,0x00,
				0x00,0xC4,0xC0,0x40,0x06,0x00,0x00,0x00,0x00,0x42,0x6C,0x00,0x00,0x0F,0x00,0x10,0x00,0x00,0x20,0x00,
				0x00,0x00,0x1B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x8B,0x82,
				0x00,0x00,0x8C,0x55,0x00,0x00,0x1F,0xFD,0x24,0x00,0x17,0x9E,0x96,0x03,0x00,0x00,0x00,0x00,0x01,0x11,
				0x01,0x04,0x03,0x70,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x40,0x16};*/
struct A Table_data;

/*******************************************************************************
* Function Name  : Agreement_13757
* Description    : 解析13757协议。
* Input          : buf、data_len
* Output         : None
* Return         : -1、0、1
*******************************************************************************/
char Agreement_13757(u8 *buf,u16 data_len)
	{
	float shuju1=0.0;
	u32 asd;
	u16 bk2;
	u8 DIF,VIF,DIFE,DIV;
	u8 *p;
	u8 L1,L2,L3,L4,i;
	static char biao[10];
	
//	p=data_buf1;
	p=buf;
	while(*p++!=0x68);
	p=&buf[25];
	while(1)
	{
	bk2=p-&buf[0];
	if((data_len-bk2)<=2)
		 return 0; 
	for(i=0;i<8;i++) biao[i]=0;//清空缓冲器
	DIF=*p++;
	switch(DIF)
		{
		case 0x04 : 
			VIF=*p++;
			switch(VIF)
				{
				case 0x06 : //kwh、累计热量 二进制
					L1=*p++;
					L2=*p++;
					L3=*p++;
					L4=*p++;
					asd=L1|(L2<<8)|(L3<<16)|(L4<<24);
					sprintf(biao,"%ld",asd);
					strcat(Table_data.BCRL,biao);
					//strcat(Table_data.BCDW,"6");
					strcat(Table_data.BCDW,"kwh");
					break;
				case 0x0f : //GJ、累计热量  二进制
					L1=*p++;
					L2=*p++;
					L3=*p++;
					L4=*p++;
					shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/100.0;
					sprintf(biao,"%f",shuju1);
					strcat(Table_data.BCRL,biao);
				//	strcat(Table_data.BCDW,"f");
					strcat(Table_data.BCDW,"GJ");
					break;
				case 0x10 : //体积、累积流量 单位：升
					L1=*p++;
					L2=*p++;
					L3=*p++;
					L4=*p++;
					asd=L1|(L2<<8)|(L3<<16)|(L4<<24);
					sprintf(biao,"%ld",asd);
					break;
				case 0x14 : //体积、累积流量 单位：立方
					L1=*p++;
					L2=*p++;
					L3=*p++;
					L4=*p++;
					shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/100.0;
					sprintf(biao,"%.2f",shuju1);
					strcat(Table_data.LJLL,biao);
					break;
				case 0x22 : //小时计数、运行时间
					L1=*p++;
					L2=*p++;
					L3=*p++;
					L4=*p++;
					asd=L1|(L2<<8)|(L3<<16)|(L4<<24);
					sprintf(biao,"%ld",asd);
					strcat(Table_data.ZGZSJ,biao);
					break;
				case 0x2d : //功率峰值 kw
					L1=*p++;
					L2=*p++;
					L3=*p++;
					L4=*p++;
					shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/10.0;
					sprintf(biao,"%.1f",shuju1);
					strcat(Table_data.RGL,biao);
					break;
				case 0x3b : //当前流量l/h
					L1=*p++;
					L2=*p++;
					L3=*p++;
					L4=*p++;
					asd=L1|(L2<<8)|(L3<<16)|(L4<<24);
					sprintf(biao,"%ld",asd);
					strcat(Table_data.XSLL,biao);
					break;
				case 0x59 : //进水温度
					L1=*p++;
					L2=*p++;
					L3=*p++;
					L4=*p++;
					shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/100.0;
					sprintf(biao,"%.2f",shuju1);
					strcat(Table_data.JKWD,biao);
					break;
				case 0x5d : //回水温度
					L1=*p++;
					L2=*p++;
					L3=*p++;
					L4=*p++;
					shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/100.0;
					sprintf(biao,"%.2f",shuju1);
					strcat(Table_data.CKWD,biao);
					break;
				case 0x61 : //供水温差
					L1=*p++;
					L2=*p++;
					L3=*p++;
					L4=*p++;
					shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/100.0;
					sprintf(biao,"%.2f",shuju1);
					break;
				case 0x6d : //日期时间
					L1=*p++;
					L2=*p++;
					L3=*p++;
					L4=*p++;
					asd=L1|(L2<<8)|(L3<<16)|(L4<<24);
					
					DIV=(L4>>4)&0x0f;
					DIV=(DIV<<3)|(L3>>5);//年
					asd=2000+DIV;
					Table_data.SSRQ[0]=((asd/1000)%10)+0x30;
					Table_data.SSRQ[1]=((asd/100)%10)+0x30;
					Table_data.SSRQ[2]=((asd/10)%10)+0x30;
					Table_data.SSRQ[3]=(asd%10)+0x30;
					Table_data.SSRQ[4]='-';
					DIV=L4&0x0f;//月
					Table_data.SSRQ[5]=DIV/10+0x30;
					Table_data.SSRQ[6]=DIV%10+0x30;
					Table_data.SSRQ[7]='-';
				//	asd=asd*100+DIV;
					DIV=L3&0x1f;//日
					Table_data.SSRQ[8]=DIV/10+0x30;
					Table_data.SSRQ[9]=DIV%10+0x30;
					Table_data.SSRQ[10]=0;
				
					
					DIV=L2&0x1f;//时
					sprintf(Table_data.SSSJ,"%d",DIV);
					strcat(Table_data.SSSJ,":");//
					DIV=L1&0x3f;//分
					sprintf(biao,"%d",DIV);
					strcat(Table_data.SSSJ,biao);//实时时间
					break;
				default: break;
					}
				break;
		case 0x0a :
			VIF=*p++;
			switch(VIF)
				{
				case 0x5b : //供水管温度 （°C）
					L1=*p++;
					L1=(L1>>4)*10+(L1&0x0f);
					L2=*p++;
					L2=(L2>>4)*10+(L2&0x0f);
					asd=L1+L2*100;
					sprintf(biao,"%ld",asd);
					strcat(Table_data.JKWD,biao);
					break;
				case 0x5f : // 回水管温度 （°C）
					L1=*p++;
					L1=(L1>>4)*10+(L1&0x0f);
					L2=*p++;
					L2=(L2>>4)*10+(L2&0x0f);
					asd=L1+L2*100;
					sprintf(biao,"%d",asd);
					strcat(Table_data.CKWD,biao);
					break;
				case 0x62 : //供回水温差（ °C/10）
					L1=*p++;
					L1=(L1>>4)*10+(L1&0x0f);
					L2=*p++;
					L2=(L2>>4)*10+(L2&0x0f);
					asd=L1+L2*100;
					shuju1=asd/10.0;
					sprintf(biao,"%.1f",shuju1);
					break;
				default: break;
				}
				break;
		case 0x0b : 
				VIF=*p++;
				switch(VIF)
					{
					case 0x2d : //功率KW
						L1=*p++;
						L1=(L1>>4)*10+(L1&0x0f);
						L2=*p++;
						L2=(L2>>4)*10+(L2&0x0f);
						L3=*p++;
						L3=(L3>>4)*10+(L3&0x0f);
						shuju1=(L1+L2*100+L3*100000)/10.0;
						sprintf(biao,"%.1f",shuju1);
						strcat(Table_data.RGL,biao);
						break;
					case 0x3b : //当前流量m3/h
						L1=*p++;
						L1=(L1>>4)*10+(L1&0x0f);
						L2=*p++;
						L2=(L2>>4)*10+(L2&0x0f);
						L3=*p++;
						L3=(L3>>4)*10+(L3&0x0f);
						shuju1=(L1+L2*100+L3*100000)/1000.0;
						sprintf(biao,"%.3f",shuju1);
						strcat(Table_data.XSLL,biao);
						break;
					default: break;
					}
				break;
		case 0x0c : 
				{
				VIF=*p++;
				switch(VIF)
					{
					case 0x06 : //kwh、累计热量
						L1=*p++;
						L1=(L1>>4)*10+(L1&0x0f);
						L2=*p++;
						L2=(L2>>4)*10+(L2&0x0f);
						L3=*p++;
						L3=(L3>>4)*10+(L3&0x0f);
						L4=*p++;
						L4=(L4>>4)*10+(L4&0x0f);
						asd=L1+L2*100+L3*10000+L4*1000000;
						sprintf(biao,"%d",asd);
						strcat(Table_data.BCRL,biao);
					//	strcat(Table_data.BCDW,"6");
						strcat(Table_data.BCDW,"kwh");
						break;
					case 0x0F : //Mj、累计热量
						L1=*p++;
						L2=*p++;
						L3=*p++;
						L4=*p++;
						shuju1=(L1+L2*100+L3*10000+L4*1000000)/100.0;
						sprintf(biao,"%.2f",shuju1);
						break;
					case 0x14 : 
						L1=*p++;
						L1=(L1>>4)*10+(L1&0x0f);
						L2=*p++;
						L2=(L2>>4)*10+(L2&0x0f);
						L3=*p++;
						L3=(L3>>4)*10+(L3&0x0f);
						L4=*p++;
						L4=(L4>>4)*10+(L4&0x0f);
						shuju1=(L1+L2*100+L3*10000+L4*1000000)/100.0;
						sprintf(biao,"%.2f",shuju1);
						strcat(Table_data.LJLL,biao);
						break;
					case 0x22 : 
						L1=*p++;
						L1=(L1>>4)*10+(L1&0x0f);
						L2=*p++;
						L2=(L2>>4)*10+(L2&0x0f);
						L3=*p++;
						L3=(L3>>4)*10+(L3&0x0f);
						L4=*p++;
						L4=(L4>>4)*10+(L4&0x0f);
						asd=L1+L2*100+L3*10000+L4*1000000;
						sprintf(biao,"%d",asd);
						strcat(Table_data.ZGZSJ,biao);
						break;
					case 0x78 : //生产号
						L1=*p++;
						L1=(L1>>4)*10+(L1&0x0f);
						L2=*p++;
						L2=(L2>>4)*10+(L2&0x0f);
						L3=*p++;
						L3=(L3>>4)*10+(L3&0x0f);
						L4=*p++;
						L4=(L4>>4)*10+(L4&0x0f);
						asd=L1+L2*100+L3*10000+L4*1000000;
						sprintf(biao,"%d",asd);
						break;
					default: break;
					}
					break;
					}
		case 0x14 : 
				VIF=*p++;
				switch(VIF)
					{
					case 0x2d : //功率峰值
						L1=*p++;
						L2=*p++;
						L3=*p++;
						L4=*p++;
						shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/10.0;
						sprintf(biao,"%.1f",shuju1);
						break;
					case 0x3b : //流量峰值
						L1=*p++;
						L2=*p++;
						L3=*p++;
						L4=*p++;
						asd=L1|(L2<<8)|(L3<<16)|(L4<<24);
						sprintf(biao,"%ld",asd);
						break;
					default: break;
					}
					break;
		case 0x3c : 
				VIF=*p++;
				switch(VIF)
					{
					case 0x22 : 
						L1=*p++;
						L1=(L1>>4)*10+(L1&0x0f);
						L2=*p++;
						L2=(L2>>4)*10+(L2&0x0f);
						L3=*p++;
						L3=(L3>>4)*10+(L3&0x0f);
						L4=*p++;
						L4=(L4>>4)*10+(L4&0x0f);
						asd=L1+L2*100+L3*10000+L4*1000000;
						sprintf(biao,"%ld",asd);
						break;
					default: break;
					}
				break;
		case 0x42 : 
					VIF=*p++;
					switch(VIF)
						{
						case 0x6c : //目标日期
							L1=*p++;
							L2=*p++;
							asd=L1|(L2<<8);
							sprintf(biao,"%d",asd);
						//	strcpy(Table_data.SSRQ,biao);
							break;
						default: break;
						}
					break;
		case 0x44 : 
			VIF=*p++;
			switch(VIF)
				{
				case 0x06 : //历史值目标能量
					L1=*p++;
					L2=*p++;
					L3=*p++;
					L4=*p++;
					asd=L1|(L2<<8)|(L3<<16)|(L4<<24);
					sprintf(biao,"%d",asd);
					break;
				case 0x0f : //读取能量
					L1=*p++;
					L2=*p++;
					L3=*p++;
					L4=*p++;
					shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/100.0;
					sprintf(biao,"%.2f",shuju1);
					break;
				case 0x14 : //读取体积
					L1=*p++;
					L2=*p++;
					L3=*p++;
					L4=*p++;
					shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/100.0;
					sprintf(biao,"%.2f",shuju1);
					break;
				default: break;
				}
			break;
		case 0x4c : 
			VIF=*p++;
			switch(VIF)
				{
				case 0x14 : 
					L1=*p++;
					L1=(L1>>4)*10+(L1&0x0f);
					L2=*p++;
					L2=(L2>>4)*10+(L2&0x0f);
					L3=*p++;
					L3=(L3>>4)*10+(L3&0x0f);
					L4=*p++;
					L4=(L4>>4)*10+(L4&0x0f);
					asd=L1+L2*100+L3*10000+L4*1000000;
					shuju1=asd/100.0;
					break;
				case 0x06 : //kWh
					L1=*p++;
					L1=(L1>>4)*10+(L1&0x0f);
					L2=*p++;
					L2=(L2>>4)*10+(L2&0x0f);
					L3=*p++;
					L3=(L3>>4)*10+(L3&0x0f);
					L4=*p++;
					L4=(L4>>4)*10+(L4&0x0f);
					asd=L1+L2*100+L3*10000+L4*1000000;
					break;
				default: break;
				}
			break;
		case 0x54 : 
			VIF=*p++;
			switch(VIF)
				{
				case 0x2d : //读取功率峰值
					L1=*p++;
					L2=*p++;
					L3=*p++;
					L4=*p++;
					shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/10.0;
					sprintf(biao,"%.1f",shuju1);
					break;
				case 0x3b : //读取流量峰值
					L1=*p++;
					L2=*p++;
					L3=*p++;
					L4=*p++;
					asd=L1|(L2<<8)|(L3<<16)|(L4<<24);
					sprintf(biao,"%ld",asd);		
					break;
				default: break;
				}
			break;
		case 0x7c : 
			VIF=*p++;
			switch(VIF)
				{
				case 0x22 : 
					L1=*p++;
					L1=(L1>>4)*10+(L1&0x0f);
					L2=*p++;
					L2=(L2>>4)*10+(L2&0x0f);
					L3=*p++;
					L3=(L3>>4)*10+(L3&0x0f);
					L4=*p++;
					L4=(L4>>4)*10+(L4&0x0f);
					asd=L1+L2*100+L3*10000+L4*1000000;
					break;
				default: break;
				}
			break;
		case 0x84 : 
			DIFE=*p++;
			switch(DIFE)
				{
				case 0x10 : //
					VIF=*p++;
					if(VIF==0x06)//kwh
						{
						L1=*p++;
						L2=*p++;
						L3=*p++;
						L4=*p++;
						asd=L1|(L2<<8)|(L3<<16)|(L4<<24);
						sprintf(biao,"%d",asd);
						}
					if(VIF==0x0f)//mj
						{
						L1=*p++;
						L2=*p++;
						L3=*p++;
						L4=*p++;
						shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/100.0;
						sprintf(biao,"%.2f",shuju1);
						}
						break;
				case 0x20 : //
					VIF=*p++;
					if(VIF==0x06)//kwh
						{
						L1=*p++;
						L2=*p++;
						L3=*p++;
						L4=*p++;
						asd=L1|(L2<<8)|(L3<<16)|(L4<<24);
						sprintf(biao,"%d",asd);
						}
					if(VIF==0x0f)//mj
						{
						L1=*p++;
						L2=*p++;
						L3=*p++;
						L4=*p++;
						shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/100.0;
						sprintf(biao,"%.2f",shuju1);
						}
						break;
				case 0x40 : //脉冲计数器A体积
					VIF=*p++;
					if(VIF==0x14)
						{
						L1=*p++;
						L2=*p++;
						L3=*p++;
						L4=*p++;
						shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/100.0;
						sprintf(biao,"%f",shuju1);
						}
						break;
				case 0x80 : 
					VIF=*p++;
					if(VIF==0x40)
						{
						VIF=*p++;
						if(VIF==0x14)
							{
							L1=*p++;
							L2=*p++;
							L3=*p++;
							L4=*p++;
							shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/100.0;
							sprintf(biao,"%f",shuju1);
							}
						}	
					break;
				case 0xc0 : 
					{
					DIFE=*p++;
					if(DIFE==0x40)//
						{
							VIF=*p++;
							if(VIF==0x06)//kwh
								{
								L1=*p++;
								L2=*p++;
								L3=*p++;
								L4=*p++;
								asd=L1|(L2<<8)|(L3<<16)|(L4<<24);
								sprintf(biao,"%d",asd);
								}
							if(VIF==0x0f)//mj
								{
								L1=*p++;
								L2=*p++;
								L3=*p++;
								L4=*p++;
								shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/100.0;
								sprintf(biao,"%.2f",shuju1);
								}
							}
						}	
					break;
				default: break;
					}
				break;
		case 0x8c : 
			DIFE=*p++;
			switch(DIFE)
				{
				case 0x01 : 
					VIF=*p++;
					if(VIF==0x06)
						{
						L1=*p++;
						L1=(L1>>4)*10+(L1&0x0f);
						L2=*p++;
						L2=(L2>>4)*10+(L2&0x0f);
						L3=*p++;
						L3=(L3>>4)*10+(L3&0x0f);
						L4=*p++;
						L4=(L4>>4)*10+(L4&0x0f);
						asd=L1+L2*100+L3*10000+L4*1000000;
						}
						break;
				case 0x02 : 
					VIF=*p++;
					if(VIF==0x06)
						{
						L1=*p++;
						L1=(L1>>4)*10+(L1&0x0f);
						L2=*p++;
						L2=(L2>>4)*10+(L2&0x0f);
						L3=*p++;
						L3=(L3>>4)*10+(L3&0x0f);
						L4=*p++;
						L4=(L4>>4)*10+(L4&0x0f);
						asd=L1+L2*100+L3*10000+L4*1000000;
						}
					break;
				case 0x03 : 
					VIF=*p++;
					if(VIF==0x06)
					{
						L1=*p++;
						L1=(L1>>4)*10+(L1&0x0f);
						L2=*p++;
						L2=(L2>>4)*10+(L2&0x0f);
						L3=*p++;
						L3=(L3>>4)*10+(L3&0x0f);
						L4=*p++;
						L4=(L4>>4)*10+(L4&0x0f);
						asd=L1+L2*100+L3*10000+L4*1000000;
						}
					break;
				case 0x04 : 
					VIF=*p++;
					if(VIF==0x06)
					{
						L1=*p++;
						L1=(L1>>4)*10+(L1&0x0f);
						L2=*p++;
						L2=(L2>>4)*10+(L2&0x0f);
						L3=*p++;
						L3=(L3>>4)*10+(L3&0x0f);
						L4=*p++;
						L4=(L4>>4)*10+(L4&0x0f);
						asd=L1+L2*100+L3*10000+L4*1000000;
						}
					break;
				case 0x05 : 
					VIF=*p++;
					if(VIF==0x06)
					{
						L1=*p++;
						L1=(L1>>4)*10+(L1&0x0f);
						L2=*p++;
						L2=(L2>>4)*10+(L2&0x0f);
						L3=*p++;
						L3=(L3>>4)*10+(L3&0x0f);
						L4=*p++;
						L4=(L4>>4)*10+(L4&0x0f);
						asd=L1+L2*100+L3*10000+L4*1000000;
						}
					break;
				case 0x06 : 
					VIF=*p++;
					if(VIF==0x06)
						{
						L1=*p++;
						L1=(L1>>4)*10+(L1&0x0f);
						L2=*p++;
						L2=(L2>>4)*10+(L2&0x0f);
						L3=*p++;
						L3=(L3>>4)*10+(L3&0x0f);
						L4=*p++;
						L4=(L4>>4)*10+(L4&0x0f);
						asd=L1+L2*100+L3*10000+L4*1000000;
						}
					break;
				case 0x07 : 
					VIF=*p++;
					if(VIF==0x06)
						{
						L1=*p++;
						L1=(L1>>4)*10+(L1&0x0f);
						L2=*p++;
						L2=(L2>>4)*10+(L2&0x0f);
						L3=*p++;
						L3=(L3>>4)*10+(L3&0x0f);
						L4=*p++;
						L4=(L4>>4)*10+(L4&0x0f);
						asd=L1+L2*100+L3*10000+L4*1000000;
						}
					break;
				case 0x08 : 
					VIF=*p++;
					if(VIF==0x06)
					{
						L1=*p++;
						L1=(L1>>4)*10+(L1&0x0f);
						L2=*p++;
						L2=(L2>>4)*10+(L2&0x0f);
						L3=*p++;
						L3=(L3>>4)*10+(L3&0x0f);
						L4=*p++;
						L4=(L4>>4)*10+(L4&0x0f);
						asd=L1+L2*100+L3*10000+L4*1000000;
						}
					break;
				default: break;
				}
				break;
		case 0xc4 : 
			DIFE=*p++;
			switch(DIFE)
				{
				case 0x10 : //
					VIF=*p++;
					if(VIF==0x06)
						{
						L1=*p++;
						L2=*p++;
						L3=*p++;
						L4=*p++;
						asd=L1|(L2<<8)|(L3<<16)|(L4<<24);
						sprintf(biao,"%ld",asd);
						}	
					if(VIF==0x0F)
						{
						L1=*p++;
						L2=*p++;
						L3=*p++;
						L4=*p++;
						shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/100.0;
						sprintf(biao,"%0.2f",shuju1);
						}	
					break;
				case 0x20 : //
					VIF=*p++;
					if(VIF==0x06)
						{
						L1=*p++;
						L2=*p++;
						L3=*p++;
						L4=*p++;
						asd=L1|(L2<<8)|(L3<<16)|(L4<<24);
						sprintf(biao,"%ld",asd);
						}	
					if(VIF==0x0F)
						{
						L1=*p++;
						L2=*p++;
						L3=*p++;
						L4=*p++;
						shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/100.0;
						sprintf(biao,"%0.2f",shuju1);
						}	
					break;
				case 0x40 : //脉冲计数器A体积
					VIF=*p++;
					if(VIF==0x14)
						{
						L1=*p++;
						L2=*p++;
						L3=*p++;
						L4=*p++;
						shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/100.0;
						sprintf(biao,"%0.2f",shuju1);
						}	
					break;
				case 0x80 : 
					DIFE=*p++;
					if(DIFE==0x40)//脉冲计数器B体积
						{
						VIF=*p++;
						if(VIF==0x14)
							{
							L1=*p++;
							L2=*p++;
							L3=*p++;
							L4=*p++;
							shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/100.0;
							sprintf(biao,"%0.2f",shuju1);
							}
						}	
						break;
				case 0xc0 : 
					DIFE=*p++;
					if(DIFE==0x40)//
						{
						VIF=*p++;
						if(VIF==0x06)
							{
							L1=*p++;
							L2=*p++;
							L3=*p++;
							L4=*p++;
							asd=L1|(L2<<8)|(L3<<16)|(L4<<24);	
							sprintf(biao,"%ld",asd);	
							}	
						if(VIF==0x0f)
							{
							L1=*p++;
							L2=*p++;
							L3=*p++;
							L4=*p++;
							shuju1=(L1|(L2<<8)|(L3<<16)|(L4<<24))/100.0;	
							sprintf(biao,"%.2f",shuju1);	
							}	
						}
						break;
					default: break;
					}
				break;
		case 0xcc : 
				DIFE=*p++;
				switch(DIFE)
					{
					case 0x01 : 
						VIF=*p++;
						if(VIF==0x06)
						{
							L1=*p++;
							L1=(L1>>4)*10+(L1&0x0f);
							L2=*p++;
							L2=(L2>>4)*10+(L2&0x0f);
							L3=*p++;
							L3=(L3>>4)*10+(L3&0x0f);
							L4=*p++;
							L4=(L4>>4)*10+(L4&0x0f);
							asd=L1+L2*100+L3*10000+L4*1000000;
							}
						break;
					case 0x02 : 
						VIF=*p++;
						if(VIF==0x06)
							{
							L1=*p++;
							L1=(L1>>4)*10+(L1&0x0f);
							L2=*p++;
							L2=(L2>>4)*10+(L2&0x0f);
							L3=*p++;
							L3=(L3>>4)*10+(L3&0x0f);
							L4=*p++;
							L4=(L4>>4)*10+(L4&0x0f);
							asd=L1+L2*100+L3*10000+L4*1000000;
							}
						break;
					case 0x03 : 
						VIF=*p++;
						if(VIF==0x06)
						{
							L1=*p++;
							L1=(L1>>4)*10+(L1&0x0f);
							L2=*p++;
							L2=(L2>>4)*10+(L2&0x0f);
							L3=*p++;
							L3=(L3>>4)*10+(L3&0x0f);
							L4=*p++;
							L4=(L4>>4)*10+(L4&0x0f);
							asd=L1+L2*100+L3*10000+L4*1000000;
							}
							break;
					case 0x04 : 
						VIF=*p++;
						if(VIF==0x06)
							{
							L1=*p++;
							L1=(L1>>4)*10+(L1&0x0f);
							L2=*p++;
							L2=(L2>>4)*10+(L2&0x0f);
							L3=*p++;
							L3=(L3>>4)*10+(L3&0x0f);
							L4=*p++;
							L4=(L4>>4)*10+(L4&0x0f);
							asd=L1+L2*100+L3*10000+L4*1000000;
							}
						break;
					case 0x05 : 
						VIF=*p++;
						if(VIF==0x06)
						{
							L1=*p++;
							L1=(L1>>4)*10+(L1&0x0f);
							L2=*p++;
							L2=(L2>>4)*10+(L2&0x0f);
							L3=*p++;
							L3=(L3>>4)*10+(L3&0x0f);
							L4=*p++;
							L4=(L4>>4)*10+(L4&0x0f);
							asd=L1+L2*100+L3*10000+L4*1000000;
							}
						break;
					case 0x06 : 
						VIF=*p++;
						if(VIF==0x06)
						{
							L1=*p++;
							L1=(L1>>4)*10+(L1&0x0f);
							L2=*p++;
							L2=(L2>>4)*10+(L2&0x0f);
							L3=*p++;
							L3=(L3>>4)*10+(L3&0x0f);
							L4=*p++;
							L4=(L4>>4)*10+(L4&0x0f);
							asd=L1+L2*100+L3*10000+L4*1000000;
							}
						break;
					case 0x07 : 
						VIF=*p++;
						if(VIF==0x06)
						{
							L1=*p++;
							L1=(L1>>4)*10+(L1&0x0f);
							L2=*p++;
							L2=(L2>>4)*10+(L2&0x0f);
							L3=*p++;
							L3=(L3>>4)*10+(L3&0x0f);
							L4=*p++;
							L4=(L4>>4)*10+(L4&0x0f);
							asd=L1+L2*100+L3*10000+L4*1000000;
							}
						break;
					}
				break;
		case 0x0f :return 0; 
		default: break;
		}
		}
	}
void danwei(u8 c)
	{
	switch(c)
		{
		case 0x02 : strcpy(Table_data.BCDW,"wh");break;
		case 0x05 : strcpy(Table_data.BCDW,"kwh");break;
		case 0x08 : strcpy(Table_data.BCDW,"MWh");break;
		case 0x0a : strcpy(Table_data.BCDW,"MWh×100");break;
		case 0x01 : strcpy(Table_data.BCDW,"J");break;
		case 0x0b : strcpy(Table_data.BCDW,"kJ");break;
		case 0x0e : strcpy(Table_data.BCDW,"MJ");break;
		case 0x11 : strcpy(Table_data.BCDW,"GJ");break;
		case 0x13 : strcpy(Table_data.BCDW,"GJ×100");break;
		case 0x14 : strcpy(Table_data.BCDW,"w");break;
		case 0x17 : strcpy(Table_data.BCDW,"kW");break;
		case 0x1a : strcpy(Table_data.BCDW,"MW");break;
		case 0x29 : strcpy(Table_data.BCDW,"L");break;
		case 0x2c : strcpy(Table_data.BCDW,"m3");break;
		case 0x32 : strcpy(Table_data.BCDW,"L/h");break;
		case 0x35 : strcpy(Table_data.BCDW,"m3/h");break;
		}
	}
	
/*******************************************************************************
* Function Name  : Agreement_188
* Description    : 解析188协议。
* Input          : buf、data_len
* Output         : None
* Return         : -1、0、1
*******************************************************************************/
char Agreement_188(u8 *buf,u16 data_len)
	{
	u8 *p;
	char data1,data2,data3,data4;
	p=buf;

	while(*p++!=0x68);
	switch(*p)
		{
		case 0x10 :break;//冷水水表
		case 0x11 :break;//生活热水水表
		case 0x12 :break;//直饮水水表
		case 0x13 :break;//中水水表
		case 0x20 : //热量表（计热量）
				{
				Table_data.BJBM[0]=(p[7]>>4)+0x30;
				Table_data.BJBM[1]=(p[7]&0x0f)+0x30;
				Table_data.BJBM[2]=(p[6]>>4)+0x30;
				Table_data.BJBM[3]=(p[6]&0x0f)+0x30;
				Table_data.BJBM[4]=(p[5]>>4)+0x30;
				Table_data.BJBM[5]=(p[5]&0x0f)+0x30;
				Table_data.BJBM[6]=(p[4]>>4)+0x30;
				Table_data.BJBM[7]=(p[4]&0x0f)+0x30;
				Table_data.BJBM[8]=(p[3]>>4)+0x30;
				Table_data.BJBM[9]=(p[3]&0x0f)+0x30;
				Table_data.BJBM[10]=(p[2]>>4)+0x30;
				Table_data.BJBM[11]=(p[2]&0x0f)+0x30;
				Table_data.BJBM[12]=(p[1]>>4)+0x30;
				Table_data.BJBM[13]=(p[1]&0x0f)+0x30;
				Table_data.BJBM[14]=0;
				p=&p[13];
				//结算日热量
				data1=*p++;
				data2=*p++;
				data3=*p++;
				data4=*p++;
				Table_data.BCRL[0]=(data4>>4)+0x30;
				Table_data.BCRL[1]=(data4&0x0f)+0x30;
				Table_data.BCRL[2]=(data3>>4)+0x30;
				Table_data.BCRL[3]=(data3&0x0f)+0x30;
				Table_data.BCRL[4]=(data2>>4)+0x30;
				Table_data.BCRL[5]=(data2&0x0f)+0x30;
				Table_data.BCRL[6]='.';
				Table_data.BCRL[7]=(data1>>4)+0x30;
				Table_data.BCRL[8]=(data1&0x0f)+0x30;
				Table_data.BCRL[9]=0;
				danwei(*p);
				*p++;
				//当前热量
				data1=*p++;
				data1=(data1>>4)*10+(data1&0x0f);
				data2=*p++;
				data2=(data2>>4)*10+(data2&0x0f);
				data3=*p++;
				data3=(data3>>4)*10+(data3&0x0f);
				data4=*p++;
				data4=(data4>>4)*10+(data4&0x0f);
				*p++;
				//热功率
				data1=*p++;
				data2=*p++;
				data3=*p++;
				data4=*p++;
				Table_data.RGL[0]=(data4>>4)+0x30;
				Table_data.RGL[1]=(data4&0x0f)+0x30;
				Table_data.RGL[2]=(data3>>4)+0x30;
				Table_data.RGL[3]=(data3&0x0f)+0x30;
				Table_data.RGL[4]=(data2>>4)+0x30;
				Table_data.RGL[5]=(data2&0x0f)+0x30;
				Table_data.RGL[6]='.';
				Table_data.RGL[7]=(data1>>4)+0x30;
				Table_data.RGL[8]=(data1&0x0f)+0x30;
				Table_data.RGL[9]=0;
				*p++;
				//流量
				data1=*p++;
				data2=*p++;
				data3=*p++;
				data4=*p++;
				Table_data.XSLL[0]=(data4>>4)+0x30;
				Table_data.XSLL[1]=(data4&0x0f)+0x30;
				Table_data.XSLL[2]=(data3>>4)+0x30;
				Table_data.XSLL[3]=(data3&0x0f)+0x30;
				Table_data.XSLL[4]=(data2>>4)+0x30;
				Table_data.XSLL[5]=(data2&0x0f)+0x30;
				Table_data.XSLL[6]='.';
				Table_data.XSLL[7]=(data1>>4)+0x30;
				Table_data.XSLL[8]=(data1&0x0f)+0x30;
				Table_data.XSLL[9]=0;;
				*p++;
				//累积流量
				data1=*p++;
				data2=*p++;
				data3=*p++;
				data4=*p++;
				Table_data.LJLL[0]=(data4>>4)+0x30;
				Table_data.LJLL[1]=(data4&0x0f)+0x30;
				Table_data.LJLL[2]=(data3>>4)+0x30;
				Table_data.LJLL[3]=(data3&0x0f)+0x30;
				Table_data.LJLL[4]=(data2>>4)+0x30;
				Table_data.LJLL[5]=(data2&0x0f)+0x30;
				Table_data.LJLL[6]='.';
				Table_data.LJLL[7]=(data1>>4)+0x30;
				Table_data.LJLL[8]=(data1&0x0f)+0x30;
				Table_data.LJLL[9]=0;;
				*p++;
				//供水温度
				data1=*p++;
				data2=*p++;
				data3=*p++;
				Table_data.JKWD[0]=(data3>>4)+0x30;
				Table_data.JKWD[1]=(data3&0x0f)+0x30;
				Table_data.JKWD[2]=(data2>>4)+0x30;
				Table_data.JKWD[3]=(data2&0x0f)+0x30;
				Table_data.JKWD[4]='.';
				Table_data.JKWD[5]=(data1>>4)+0x30;
				Table_data.JKWD[6]=(data1&0x0f)+0x30;
				Table_data.JKWD[7]=0;

				//回水温度
				data1=*p++;
				data2=*p++;
				data3=*p++;
				Table_data.CKWD[0]=(data3>>4)+0x30;
				Table_data.CKWD[1]=(data3&0x0f)+0x30;
				Table_data.CKWD[2]=(data2>>4)+0x30;
				Table_data.CKWD[3]=(data2&0x0f)+0x30;
				Table_data.CKWD[4]='.';
				Table_data.CKWD[5]=(data1>>4)+0x30;
				Table_data.CKWD[6]=(data1&0x0f)+0x30;
				Table_data.CKWD[7]=0;
				//累计工作时间
				data1=*p++;
				data2=*p++;
				data3=*p++;
				Table_data.ZGZSJ[0]=(data3>>4)+0x30;
				Table_data.ZGZSJ[1]=(data3&0x0f)+0x30;
				Table_data.ZGZSJ[2]=(data2>>4)+0x30;
				Table_data.ZGZSJ[3]=(data2&0x0f)+0x30;
				Table_data.ZGZSJ[4]=(data1>>4)+0x30;
				Table_data.ZGZSJ[5]=(data1&0x0f)+0x30;
				Table_data.ZGZSJ[6]=0;
				//实时时间
				data1=*p++;//秒
				data2=*p++;//分
				data3=*p++;//时
				Table_data.SSSJ[0]=(data3>>4)+0x30;
				Table_data.SSSJ[1]=(data3&0x0f)+0x30;
				Table_data.SSSJ[2]=':';
				Table_data.SSSJ[3]=(data2>>4)+0x30;
				Table_data.SSSJ[4]=(data2&0x0f)+0x30;
				Table_data.SSSJ[5]=':';
				Table_data.SSSJ[6]=(data1>>4)+0x30;
				Table_data.SSSJ[7]=(data1&0x0f)+0x30;
				Table_data.SSSJ[8]=0;
				//实时日期
				data1=*p++;
				data2=*p++;
				data3=*p++;
				data4=*p++;
				Table_data.SSRQ[0]=(data4>>4)+0x30;
				Table_data.SSRQ[1]=(data4&0x0f)+0x30;
				Table_data.SSRQ[2]=(data3>>4)+0x30;
				Table_data.SSRQ[3]=(data3&0x0f)+0x30;
				Table_data.SSRQ[4]='-';
				Table_data.SSRQ[5]=(data2>>4)+0x30;
				Table_data.SSRQ[6]=(data2&0x0f)+0x30;
				Table_data.SSRQ[7]='-';
				Table_data.SSRQ[8]=(data1>>4)+0x30;
				Table_data.SSRQ[9]=(data1&0x0f)+0x30;
				Table_data.SSRQ[10]=0;
				//表具状态
				data1=*p++;
				Table_data.BJZT[0]=(data1>>4)+0x30;
				Table_data.BJZT[1]=(data1&0x0f)+0x30;
				Table_data.BJZT[2]=0;
				//表具名称
				Table_data.BXMC[0]='1';
				Table_data.BXMC[1]='8';
				Table_data.BXMC[2]='8';
				Table_data.BXMC[3]=0;
				break;				
				}
		case 21 :break;//热量表（计冷量）
		case 30 :break;//燃气表
		case 40 :break;//电表
		
		}
	return 0;
	}
