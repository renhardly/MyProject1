
#include "platform_config.h"
#include "ErrCode.h"

/******串口1*********/
/*
RS232接口
buf:发送的数据结构
sum：数据长度
*/
void UART_1(u8 *buf,u16 sum)
	{
	u16 i=0;
  M_RS485_LED_ON();
	while(sum!=i)
		{ 
		USART_SendData(USART1, buf[i++]); 
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) ;//发送数据寄存器空
//		IWDG_ReloadCounter();
//		RstDog();
		}
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) ;//发送完成标志位
	M_RS485_LED_OFF();
	}
/*******串口2*********/
/*
表具RS485接口
buf:发送的数据结构
sum：数据长度
*/
void UART_2(u8 *buf,u16 sum)
	{
	u16 i=0;
	S_RS485_LED_ON();
	for(i=0;i<sum;i++)
		{ 
		USART_SendData(USART2, buf[i]); 
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) ;//等待一字节数据发送完
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) ;//等待一字节数据发送完
//		IWDG_ReloadCounter();
	//	RstDog();
		}
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) ;//等待一字节数据发送完 
	//	IWDG_ReloadCounter();
	//	RstDog();
	S_RS485_LED_OFF();
	}
/*******串口2*********/
/*
无线模块、DTU RS485接口
buf:发送的数据结构
sum：数据长度
*/
void UART_3(u8 *buf,u16 sum)
	{
	u16 i=0;
	M_RS485_LED_ON();//控制485发送接收端
	for(i=0;i<sum;i++)
		{ 
		USART_SendData(USART3, buf[i]); 
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET) ;//等待一字节数据发送完
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) ;//等待一字节数据发送完
//		IWDG_ReloadCounter();
//		RstDog();
		}
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) ;//等待一字节数据发送完 
		//IWDG_ReloadCounter();
		//RstDog();
	M_RS485_LED_OFF();

	}
/*******串口4*********/
/*
M-bus表具
buf:发送的数据结构
sum：数据长度
*/
void UART_4(u8 *buf,u16 sum)
	{
	  u16 i=0;
		S_RS485_LED_ON();
		for(i=0;i<sum;i++)
		{ 
		UART4->DR=(buf[i] & (u16)0x01FF);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET) ;//等待一字节数据发送完
//		IWDG_ReloadCounter();
		//	RstDog();
		}
	  while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET) ;//等待一字节数据发送完 
		//IWDG_ReloadCounter();
		//RstDog();
		S_RS485_LED_OFF();
	}
/*******串口5*********/
/*
buf:发送的数据结构
sum：数据长度
*/
void UART_5(u8 *buf,u16 sum)
	{
	u16 i=0;
//	RS232_2_LED_ON();
	for(i=0;i<sum;i++)
		{ 
		UART5->DR=(buf[i] & (u16)0x01FF);
		while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET) ;//等待一字节数据发送完
		while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET) ;//等待一字节数据发送完 
		}
//	RS232_2_LED_OFF();

	}
/************选择com发送端口**************/
/*
k：端口选择位
buf_com：发送的数据
sum：数据长度
*/
void Send_UART(u8 k,u8 *buf_com,u16 sum)
	{
	//RstDog();
//	while((sum/80!=0))
//	{
//		
//		switch(k)
//		{
//		case 1 : UART_1(buf_com,80);//端口COM1
//				 break;
//		case 2 : UART_2(buf_com,80);//端口COM2
//				 break;
//		case 3 : UART_3(buf_com,80);//端口COM3
//				 break;
//		case 4 : UART_4(buf_com,80);//端口COM4
//				 break;
//		case 5 : UART_5(buf_com,80);//端口COM5
//				 break;
//		default:
//				break;
//		}
//	}
//		switch(k)
//		{
//		case 1 : UART_1(buf_com,sum);//端口COM1
//				 break;
//		case 2 : UART_2(buf_com,sum);//端口COM2
//				 break;
//		case 3 : UART_3(buf_com,sum);//端口COM3
//				 break;
//		case 4 : UART_4(buf_com,sum);//端口COM4
//				 break;
//		case 5 : UART_5(buf_com,sum);//端口COM5
//				 break;
//		default:
//				break;
//		}
	switch(k)
		{
		case 1 : UART_1(buf_com,sum);//端口COM1
				 break;
		case 2 : UART_2(buf_com,sum);//端口COM2
				 break;
		case 3 : UART_3(buf_com,sum);//端口COM3
				 break;
		case 4 : UART_4(buf_com,sum);//端口COM4
				 break;
		case 5 : UART_5(buf_com,sum);//端口COM5
				 break;
		default:
				break;
		}
	}
/*******串口4*********/
/*
buf:发送的数据结构
sum：数据长度
*/
void Auto_sent(u8 *RXD_BUF,u16 sum)
	{
	u16 i=0;
//	RX_CTL_OFF();
	S_RS485_LED_ON();
  //GPRS_CS_ENABLE();
	while(sum!=i)
		{ 
		USART_SendData(USART2, RXD_BUF[i]); 
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) ;//等待一字节数据发送完
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) ;//等待一字节数据发送完
//		IWDG_ReloadCounter();
//		RstDog(); 
		USART_SendData(UART4,RXD_BUF[i]); 
		while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET) ;//等待一字节数据发送完
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET) ;//等待一字节数据发送完
//		IWDG_ReloadCounter();
//		RstDog();
		i++;
		}
  while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET) ;//等待一字节数据发送完 
	S_RS485_LED_OFF();
	RX_CTL_ON();
	}

/* USART1 .... UART5 configuration ----------*/

void USART_Configuration(void)
	{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate =2400;					// 波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;	// 数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// 停止位
	USART_InitStructure.USART_Parity = USART_Parity_Even;		// 校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure USART1-5 */
	USART_Init(USART2, &USART_InitStructure);//USART2 UART4 2400 8 E 1 与表具通讯
	USART_Init(UART4, &USART_InitStructure);
	USART_Init(USART3, &USART_InitStructure);//采集器上行模块
	USART_Init(UART5, &USART_InitStructure);
	
	USART_InitStructure.USART_BaudRate =9600;					// 波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// 数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// 停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;		// 校验位
	USART_Init(USART1, &USART_InitStructure);//USART1 USART3 9600 8 N 1 与上位机和DTU 通讯
	
 	
	/* 启用USART1-5的接收中断 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

	/* 启用USART1-5 */
	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART2, ENABLE);
	USART_Cmd(USART3, ENABLE);
	USART_Cmd(UART4, ENABLE);
	USART_Cmd(UART5, ENABLE);
	
	}

