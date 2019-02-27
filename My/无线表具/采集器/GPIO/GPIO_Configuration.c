
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
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3|GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Configure USART1 USART2 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
/*-----------------------USART3 USART4----------------------------------------*/
  /* Configure UART4 Rx (PC.11) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /* Configure UART3 Rx (PB.11) as input floating */
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* Configure UART4 Tx (PC.10) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /* Configure UART3 Tx (PB.10) as input floating */
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  /* Configure  UART5 Tx (PC.12) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
/*-----------------------USART3 USART4----------------------------------------*/
  /* Configure UART5 Rx (PD.2) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
      /*配置外部flash  W25Q128 引脚配置*/
    /* Configure SPI1 pins: SCK, MISO and MOSI */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* 外部flash  W25Q128 片选CS */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	
	  /* 配置485为接收模式，TX 、RX指示灯灭 */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);

	  M_RS485_LED_OFF();
	  S_RS485_LED_OFF();



		/*电压检测引脚控制*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	  /*m-bus控制信号  PE3 TX ,PE4 RX*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_Init(GPIOE, &GPIO_InitStructure);
	  TX_CTL_ON();
	  RX_CTL_ON();

	   /* Configure ADDR as input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*电压过载检测*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* Configure I2C1 pins: SCL and SDA */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  	GPIO_Init(GPIOB, &GPIO_InitStructure); 
  	/*****************************************************/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOE, &GPIO_InitStructure); 

 	/* Configure PC.01, PC.02 and PC.03 (ADC Channel11, Channel12 and Channel13)
   	as analog input -----------------------------------------------------------*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3| GPIO_Pin_4;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);


  	GPIO_SetBits(GPIOC, GPIO_Pin_6);
  	GPIO_SetBits(GPIOC, GPIO_Pin_6);
  	GPIO_SetBits(GPIOC, GPIO_Pin_7);
  	GPIO_SetBits(GPIOC, GPIO_Pin_8);
  	GPIO_SetBits(GPIOC, GPIO_Pin_9);
  
  	GPIO_SetBits(GPIOD, GPIO_Pin_12);
  	GPIO_SetBits(GPIOD, GPIO_Pin_13);
  	GPIO_SetBits(GPIOD, GPIO_Pin_14);
  	GPIO_SetBits(GPIOD, GPIO_Pin_15);
}
