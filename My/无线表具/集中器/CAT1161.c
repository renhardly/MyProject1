/*
CAT1161Ϊ����΢������ϵͳ�ṩ��һ�������Ĵ洢���͵�Դ��ؽ���������������õ͹���CMOS������16k��Ӳ���洢��д�������ܵĴ���EEPROM �洢�������ڵ��籣���ĵ�Դ��ص�·��һ�����Ź���ʱ�����ɵ�һ��оƬ�ϡ��洢������I2C ���߽ӿڡ���ϵͳ���������Ӳ�����Ŷ�����ֹ�򡰹���ʱ��1.6��Ŀ��Ź���·����λϵͳ��ʹϵͳ�ָ�������

�ܽ�������鿴datasheet��

�ܽ�����
WP:д����
  ���ùܽ���VCC �������������洢�����б�д������ֻ���������ܽ���GND����������ʱ�����Զ��������������Ķ�/д������
RESET/ RESET����λI/O
  �����ǿ�©�������������λ�������롣�ùܽ��ϵ�ǿ�Ƹ�λ������ʹ���������ͱ��ָ�λ��RESET��������һ���������裬��RESET �������������衣
SDA���������ݵ�ַ��
  ˫��������/��ַ�ܽţ����ڷ��ͺͽ������ݡ�SDA�ܽ��ǿ�©���������������©���򼯵缫��·������������߻�
  �����1.6����SDA�������ݴ��䣬���Ź���ʱ���������
SCL������ʱ���ź�
  ����ʱ���ź����롣

u8 RDCAT1161(u8 Addr)��void WRCAT1161(u8 Addr,Bdata)�ֱ���CAT1161���ֽڶ�д����
*/
#include "platform_config.h"

#define SCL_H() GPIO_SetBits(GPIOB, GPIO_Pin_6);
#define SCL_L() GPIO_ResetBits(GPIOB, GPIO_Pin_6);
#define SDA_H() GPIO_SetBits(GPIOB, GPIO_Pin_7);
#define SDA_L() GPIO_ResetBits(GPIOB, GPIO_Pin_7);
#define SDA 	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)
//#define SCL P2_2
//#define SDA P2_3

#define PAGE 0x00 //���ݴ�ŵ�ҳ��������ַ�ĸ���λ

void Delay()
{
  u16 i = 1000;
  while(i--);
}

//ʱ��͵�ƽ��ʱ10us
void Tlow()
{
  u8 i = 100;
  while(i--);
} 

//����д����������ʱ10ms
void Twr()
{
  u8 i,j;
  for(i=50;i>0;i--)
  {

   for(j=200;j>0;j--);
  }
} 

//ι��
void RstDog()
{   
  SDA_L();
  Delay();
  SDA_H();
}

//��EEPROM��ȡһ���ֽ�
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
ÿ���Ź�оƬд��һ���ֽ����ݣ�оƬ�����ڵھŸ�ʱ�������ڽ�SDA�Ӹ�������Ϊ��Ӧ��һ����˵��ֻҪ��ʱ�Գ������л�Ӧ������ǳ���

����ֻҪ����һ��ʱ�����ڣ����жϻ�ӦҲ���ԡ�
*/
//�ȴ���Ӧ
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

//��EEPROMд��һ���ֽ�
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

//������ʼʱ��
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

//��������ʱ��
void ExitX()
{     
  SDA_L();
  Delay();
  SCL_H();
  Delay();
  SDA_H();
  Delay();
}

//��������ʼ
void StartRD()
{
  StartX();
  WRByte(0xA1 | (PAGE<<1));
}

//д������ʼ
void StartWR()
{
  StartX();
  WRByte(0xA0 | (PAGE<<1)); 
}

//��EEPROM
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

//дEEPROM
void WRCAT1161(u8 Addr,u8 Bdata)
{
  Twr();
  StartWR();
  WRByte(Addr);    
  WRByte(Bdata);  
  ExitX();
}

