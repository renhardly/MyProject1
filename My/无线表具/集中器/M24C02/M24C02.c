#include "stm32f10x_lib.h"
#include "platform_config.h"

#define SDA_H() GPIO_SetBits(GPIOE, GPIO_Pin_1);
#define SDA_L() GPIO_ResetBits(GPIOE, GPIO_Pin_1);


void Delay()
{
  u16 i = 100;
  while(i--);
}

 //喂狗
void RstDog()
{   
  SDA_L();
  Delay();
  SDA_H();
}

void M24C02(void)
	{
	u8 Rx1_Buffer[16];

//	I2C_EE_BufferWrite(Tx1_Buffer,0,16,0xa0);//写数据
//	I2C_EE_BufferRead(Rx1_Buffer,0, 16,0xa0);//读数据
	send_uart(4,Rx1_Buffer,16);
	}
