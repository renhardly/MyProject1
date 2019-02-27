/*
CAT1161为基于微控器的系统提供了一个完整的存储器和电源监控解决方案。它们利用低功耗CMOS技术将16k带硬件存储器写保护功能的串行EEPROM 存储器、用于掉电保护的电源监控电路和一个看门狗定时器集成到一块芯片上。存储器采用I2C 总线接口。当系统由于软件或硬件干扰而被终止或“挂起”时，1.6秒的看门狗电路将复位系统，使系统恢复正常。

管脚配置请查看datasheet。

管脚描述
WP:写保护
  若该管脚与VCC 相连，则整个存储器阵列被写保护（只读）。当管脚与GND相连或悬空时，可以对器件进行正常的读/写操作。
RESET/ RESET：复位I/O
  它们是开漏输出，可用作复位触发输入。该管脚上的强制复位条件可使器件启动和保持复位。RESET脚需连接一个下拉电阻，而RESET 需连接上拉电阻。
SDA：串行数据地址线
  双向串行数据/地址管脚，用于发送和接收数据。SDA管脚是开漏输出，可与其它开漏极或集电极开路输出器件进行线或。
  如果在1.6秒内SDA上无数据传输，看门狗定时器会溢出。
SCL：串行时钟信号
  串行时钟信号输入。

u8 RDCAT1161(u8 Addr)、void WRCAT1161(u8 Addr,Bdata)分别是CAT1161的字节读写程序
*/
#include "platform_config.h"

#define SCL_H() GPIO_SetBits(GPIOB, GPIO_Pin_6);
#define SCL_L() GPIO_ResetBits(GPIOB, GPIO_Pin_6);
#define SDA_H() GPIO_SetBits(GPIOB, GPIO_Pin_7);
#define SDA_L() GPIO_ResetBits(GPIOB, GPIO_Pin_7);
#define SDA 	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)
//#define SCL P2_2
//#define SDA P2_3

#define PAGE 0x00 //数据存放的页数，即地址的高三位

void Delay()
{
  u16 i = 1000;
  while(i--);
}

//时序低电平延时10us
void Tlow()
{
  u8 i = 100;
  while(i--);
} 

//读、写操作结束延时10ms
void Twr()
{
  u8 i,j;
  for(i=50;i>0;i--)
  {

   for(j=200;j>0;j--);
  }
} 

//喂狗
void RstDog()
{   
  SDA_L();
  Delay();
  SDA_H();
}

//从EEPROM读取一个字节
u8 RDByte()
	{
	  u8 i;
	  u8 temp = 0;
	  for(i=8;i>0;i--)
		{
		SCL_L(); 
		Delay();
		SDA_H(); 
		Delay();
		temp = temp<<1;
		SCL_H();
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))
		 {
		  temp = temp|0x01;
		 }
		Delay();
		}
	  SCL_L();
	  Delay();
	  SDA_H();
	  return temp;
	}

/*
每向看门狗芯片写入一个字节数据，芯片都会在第九个时钟周期内将SDA从高拉低作为回应。一般来说，只要延时稍长都会有回应，情况非常可

靠，只要产生一个时钟周期，不判断回应也可以。
*/
//等待回应
void WaitAck()
{
  SCL_L(); 
  Delay();
  SCL_H();
  Delay();
  while(SDA);
  Delay();
  SCL_L(); 
  Delay();
  SDA_H();
  Delay();
}

//向EEPROM写入一个字节
void WRByte(u8 sb)
{
  u8 i;
  for(i=8;i>0;i--)
  {      
	 SCL_L();
	 Tlow();
	 //SDA = (bit)(sb & 0x80);
	 if(sb&0x80) 
	 	{
		SDA_H();
		}
		else 
			{
			SDA_L();
			}
	 sb = sb<<1;
	 Delay();
	 SCL_H(); 
	 Delay();
  }   
  SCL_L();
  WaitAck();
}

//操作开始时序
void StartX()
{  
GPIOB->IDR|=0x80;
  SDA_H();
  Delay();
  SCL_H();
  Delay();
  SDA_L();
  Delay();
  SCL_L();
  Delay();
}

//操作结束时序
void ExitX()
{     
  SDA_L();
  Delay();
  SCL_H();
  Delay();
  SDA_H();
  Delay();
}

//读操作开始
void StartRD()
{
  StartX();
  WRByte(0xA1 | (PAGE<<1));
}

//写操作开始
void StartWR()
{
  StartX();
  WRByte(0xA0 | (PAGE<<1)); 
}

//读EEPROM
u8 RDCAT1161(u8 Addr)
{
  u8 temp;
  StartWR();
  WRByte(Addr);
  StartRD();
  temp = RDByte();
  ExitX(); 
  return temp;
}

//写EEPROM
void WRCAT1161(u8 Addr,u8 Bdata)
{
  Twr();
  StartWR();
  WRByte(Addr);    
  WRByte(Bdata);  
  ExitX();
}

