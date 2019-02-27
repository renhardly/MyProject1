
#include "platform_config.h"
#include "ErrCode.h"

/******����1*********/
/*
RS232�ӿ�
buf:���͵����ݽṹ
sum�����ݳ���
*/
void UART_1(u8 *buf,u16 sum)
	{
	u16 i=0;
  M_RS485_LED_ON();
	while(sum!=i)
		{ 
		USART_SendData(USART1, buf[i++]); 
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) ;//�������ݼĴ�����
//		IWDG_ReloadCounter();
//		RstDog();
		}
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) ;//������ɱ�־λ
	M_RS485_LED_OFF();
	}
/*******����2*********/
/*
���RS485�ӿ�
buf:���͵����ݽṹ
sum�����ݳ���
*/
void UART_2(u8 *buf,u16 sum)
	{
	u16 i=0;
	S_RS485_LED_ON();
	for(i=0;i<sum;i++)
		{ 
		USART_SendData(USART2, buf[i]); 
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) ;//�ȴ�һ�ֽ����ݷ�����
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) ;//�ȴ�һ�ֽ����ݷ�����
//		IWDG_ReloadCounter();
	//	RstDog();
		}
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) ;//�ȴ�һ�ֽ����ݷ����� 
	//	IWDG_ReloadCounter();
	//	RstDog();
	S_RS485_LED_OFF();
	}
/*******����2*********/
/*
����ģ�顢DTU RS485�ӿ�
buf:���͵����ݽṹ
sum�����ݳ���
*/
void UART_3(u8 *buf,u16 sum)
	{
	u16 i=0;
	M_RS485_LED_ON();//����485���ͽ��ն�
	for(i=0;i<sum;i++)
		{ 
		USART_SendData(USART3, buf[i]); 
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET) ;//�ȴ�һ�ֽ����ݷ�����
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) ;//�ȴ�һ�ֽ����ݷ�����
//		IWDG_ReloadCounter();
//		RstDog();
		}
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) ;//�ȴ�һ�ֽ����ݷ����� 
		//IWDG_ReloadCounter();
		//RstDog();
	M_RS485_LED_OFF();

	}
/*******����4*********/
/*
M-bus���
buf:���͵����ݽṹ
sum�����ݳ���
*/
void UART_4(u8 *buf,u16 sum)
	{
	  u16 i=0;
		S_RS485_LED_ON();
		for(i=0;i<sum;i++)
		{ 
		UART4->DR=(buf[i] & (u16)0x01FF);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET) ;//�ȴ�һ�ֽ����ݷ�����
//		IWDG_ReloadCounter();
		//	RstDog();
		}
	  while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET) ;//�ȴ�һ�ֽ����ݷ����� 
		//IWDG_ReloadCounter();
		//RstDog();
		S_RS485_LED_OFF();
	}
/*******����5*********/
/*
buf:���͵����ݽṹ
sum�����ݳ���
*/
void UART_5(u8 *buf,u16 sum)
	{
	u16 i=0;
//	RS232_2_LED_ON();
	for(i=0;i<sum;i++)
		{ 
		UART5->DR=(buf[i] & (u16)0x01FF);
		while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET) ;//�ȴ�һ�ֽ����ݷ�����
		while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET) ;//�ȴ�һ�ֽ����ݷ����� 
		}
//	RS232_2_LED_OFF();

	}
/************ѡ��com���Ͷ˿�**************/
/*
k���˿�ѡ��λ
buf_com�����͵�����
sum�����ݳ���
*/
void Send_UART(u8 k,u8 *buf_com,u16 sum)
	{
	//RstDog();
//	while((sum/80!=0))
//	{
//		
//		switch(k)
//		{
//		case 1 : UART_1(buf_com,80);//�˿�COM1
//				 break;
//		case 2 : UART_2(buf_com,80);//�˿�COM2
//				 break;
//		case 3 : UART_3(buf_com,80);//�˿�COM3
//				 break;
//		case 4 : UART_4(buf_com,80);//�˿�COM4
//				 break;
//		case 5 : UART_5(buf_com,80);//�˿�COM5
//				 break;
//		default:
//				break;
//		}
//	}
//		switch(k)
//		{
//		case 1 : UART_1(buf_com,sum);//�˿�COM1
//				 break;
//		case 2 : UART_2(buf_com,sum);//�˿�COM2
//				 break;
//		case 3 : UART_3(buf_com,sum);//�˿�COM3
//				 break;
//		case 4 : UART_4(buf_com,sum);//�˿�COM4
//				 break;
//		case 5 : UART_5(buf_com,sum);//�˿�COM5
//				 break;
//		default:
//				break;
//		}
	switch(k)
		{
		case 1 : UART_1(buf_com,sum);//�˿�COM1
				 break;
		case 2 : UART_2(buf_com,sum);//�˿�COM2
				 break;
		case 3 : UART_3(buf_com,sum);//�˿�COM3
				 break;
		case 4 : UART_4(buf_com,sum);//�˿�COM4
				 break;
		case 5 : UART_5(buf_com,sum);//�˿�COM5
				 break;
		default:
				break;
		}
	}
/*******����4*********/
/*
buf:���͵����ݽṹ
sum�����ݳ���
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
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) ;//�ȴ�һ�ֽ����ݷ�����
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) ;//�ȴ�һ�ֽ����ݷ�����
//		IWDG_ReloadCounter();
//		RstDog(); 
		USART_SendData(UART4,RXD_BUF[i]); 
		while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET) ;//�ȴ�һ�ֽ����ݷ�����
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET) ;//�ȴ�һ�ֽ����ݷ�����
//		IWDG_ReloadCounter();
//		RstDog();
		i++;
		}
  while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET) ;//�ȴ�һ�ֽ����ݷ����� 
	S_RS485_LED_OFF();
	RX_CTL_ON();
	}

/* USART1 .... UART5 configuration ----------*/

void USART_Configuration(void)
	{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate =2400;					// ������
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;	// ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_Even;		// У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure USART1-5 */
	USART_Init(USART2, &USART_InitStructure);//USART2 UART4 2400 8 E 1 ����ͨѶ
	USART_Init(UART4, &USART_InitStructure);
	USART_Init(USART3, &USART_InitStructure);//�ɼ�������ģ��
	USART_Init(UART5, &USART_InitStructure);
	
	USART_InitStructure.USART_BaudRate =9600;					// ������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;		// У��λ
	USART_Init(USART1, &USART_InitStructure);//USART1 USART3 9600 8 N 1 ����λ����DTU ͨѶ
	
 	
	/* ����USART1-5�Ľ����ж� */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

	/* ����USART1-5 */
	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART2, ENABLE);
	USART_Cmd(USART3, ENABLE);
	USART_Cmd(UART4, ENABLE);
	USART_Cmd(UART5, ENABLE);
	
	}

