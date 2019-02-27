
#include "platform_config.h"
/*******************************************************************************
* Function Name  : GPIO配置
* Description    : 配置不同的GPIO端口。
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  
GPIO_InitTypeDef GPIO_InitStructure;

/*-----------------------USART1 USART2----------------------------------------*/

  /* Configure USART1 USART2 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3|GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Configure USART1 USART2 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
/*-----------------------USART3 USART4----------------------------------------*/
  /* Configure UART4 Rx (PC.11) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /* Configure UART3 Rx (PB.11) as input floating */
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* Configure UART4 Tx (PC.10) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /* Configure UART3 Tx (PB.10) as input floating */
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Configure I2C1 pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
  
  /* Configure LED as input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  TX_LED_OFF();	// 灯灭，同时向下485允许接收
  RX_LED_OFF();// 灯灭，同时向上485允许接收
  USB_LED_OFF();//
  APC230_EN_H1();
  APC230_EN_H2();
  
  /**薄码开关**/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /**无线模块——上行wifi 蓝牙 DTU模块**/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /**无线模块——采集器**/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Configure SPI1 pins: SCK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure  SPI1  pins:CS */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


 /* Configure PC.04 (ADC Channel14) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
/**************************通过******************************************************/
/* 配置 CH376 SPI 功能引脚 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
		//
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_Init(GPIOC, &GPIO_InitStructure);					
	//	CH376_SCS_1();  /* 禁止SPI片选 
		GPIO_SetBits(GPIOB,  GPIO_Pin_12);
	//	CH376_SCK_1();  /* 默认为高电平,SPI模式3,也可以用SPI模式0,但模拟程序可能需稍做修改 	
		GPIO_SetBits(GPIOB,  GPIO_Pin_13);
		
	
}
