#include "platform_config.h"


/******串口1*********/
/*
buf:发送的数据结构
sum：数据长度
*/
void UART_1(u8 *buf,u16 sum)
	{
	u16 i=0;
	UP_RS485_send();   //使能串口1发送
//	RS232_2_LED_ON();
	while(sum!=i)
		{ 
		USART_SendData(USART1, buf[i++]); 
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) ;//等待一字节数据发送完
//		IWDG_ReloadCounter();
		}
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) ;//等待一字节数据发送完
	UP_RS485_Receive();
//	RS232_2_LED_OFF();
	}
/*******串口2*********/
/*
buf:发送的数据结构
sum：数据长度
*/
void UART_2(u8 *buf,u16 sum)
	{
	u16 i=0;
//	RS232_1_LED_ON();
	for(i=0;i<sum;i++)
		{ 
		USART_SendData(USART2, buf[i]); 
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) ;//等待一字节数据发送完
//		IWDG_ReloadCounter();
		}
//	RS232_1_LED_OFF();
	}
/*******串口2*********/
/*
buf:发送的数据结构
sum：数据长度
*/
void UART_3(u8 *buf,u16 sum)
	{
	u16 i=0;
	
	Down_RS485_send();
	for(i=0;i<sum;i++)
		{ 
		USART_SendData(USART3, buf[i]); 
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET) ;//等待一字节数据发送完
//		IWDG_ReloadCounter();
		}
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) ;//等待一字节数据发送完 
	Down_RS485_Receive();
	}
/*******串口4*********/
/*
buf:发送的数据结构
sum：数据长度
*/
void UART_4(u8 *buf,u16 sum)
	{
	u16 i=0;
	UP_RS485_send();
	for(i=0;i<sum;i++)
		{ 
		USART_SendData(UART4, buf[i]);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET) ;//等待一字节数据发送完
//		IWDG_ReloadCounter();
		}
	UP_RS485_Receive();
	}
/*******串口4*********/
/*
buf:发送的数据结构
sum：数据长度
*/
void Auto_Sent(u8 *RXD_BUF,u16 sum)
	{
	u16 i=0;
//	RS485_send();//控制485发送接收端
   // UP_RS485_send();
	Down_RS485_send(); //允许发送//采集器接口
//	RS232_2_LED_ON();
	while(sum!=i)
		{ 
//		IWDG_ReloadCounter();
		USART_SendData(USART2, RXD_BUF[i]); 
		USART_SendData(USART3,RXD_BUF[i]); 
	//	USART_SendData(UART4,RXD_BUF[i]); 
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) ;//等待一字节数据发送完
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET) ;//等待一字节数据发送完
        while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) ;//等待一字节数据发送完
	//	while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET) ;//等待一字节数据发送完
		i++;
		}
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) ;//等待一字节数据发送完 
//	UP_RS485_Receive();
	Down_RS485_Receive();
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
		default:
				break;
		}
	}
/* USART1 .... UART4 configuration ----------*/

void USART_Configuration(void)
	{
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate =2400;					// 波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;	// 数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// 停止位
	USART_InitStructure.USART_Parity = USART_Parity_Even;		// 校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		
	USART_Init(USART2, &USART_InitStructure);
	USART_Init(USART3, &USART_InitStructure);
	
	USART_InitStructure.USART_BaudRate =9600;					// 波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// 数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// 停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;		// 校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	/* Configure USART1-4 */
	USART_Init(USART1, &USART_InitStructure);

	USART_Init(USART3, &USART_InitStructure);
 	USART_Init(UART4, &USART_InitStructure);
	/* 启用USART1-4的接收中断 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	/* 启用USART1-4 */
	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART2, ENABLE);
	USART_Cmd(USART3, ENABLE);
	USART_Cmd(UART4, ENABLE);
	
	}

