/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#include "ErrCode.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/
/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : 上位机发送命令帧结束中断
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8  G_UpSendCmd[128];
u8  G_RecvData[128];
u16 G_UpSendCmdLen,G_RecvDataLen;
u8  G_UART_Num;
u8 G_Deal_flag=0;
extern u8 G_Len_TMP;
void TIM2_IRQHandler(void)
{
	u8 *pUpSendCmd,i=0;
	u8 Check;
	TIM2->CR1=0;
	TIM2->CNT=0;
	TIM2->CCR1=40;
//	IWDG_ReloadCounter();	//喂狗
//	RstDog();
if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清中断标志
	if(G_UpSendCmdLen<32)
	return;
	/*--------------------下行数据-----------------------*/
	if((GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6))||(GetWorkingMode()==1))//管理模式下
	{
		pUpSendCmd=G_UpSendCmd;
		while(*pUpSendCmd!=0x68)
		{
			 pUpSendCmd++;
			 i++;
			 G_UpSendCmdLen--;
		}

		Check=CRC8(pUpSendCmd, G_UpSendCmdLen-2);
		if(Check==pUpSendCmd[G_UpSendCmdLen-2])
		{
  			G_Len_TMP=G_UpSendCmdLen;
			G_Deal_flag=1;	
		}
	}
	else //透明数据传输
	{
	}
  }
  G_UpSendCmdLen=0;
}

/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : 采集器自动抄表时间间隔设置中断
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void TIM3_IRQHandler(void)
{
/*	static u8 Count=0;
	u8 TimeBuf[3];
	u8 TimeTemp[3];
	u8 i=0;
	u16 TimeSec;
	extern u8 G_CycleMeterReadTimeoverFlag;
	TIM_Cmd(TIM3, DISABLE);
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
  		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//清中断标志
		if(Check_Index_Equ())//EXT_flash
		{
		   SPI_Flash_Read(EXT_TIME_INTERVAL,TimeBuf,3);
		}
		else 
		{
			ReadFlash_Byte(TIME_INTERVAL,TimeBuf,3);
		//	Copy_Inter_To_EXT(); //时间长，是否能够加到中断？？
		}
		for(i=0;i<3;i++)
		{
			TimeTemp[i]=BCD2HEX(TimeBuf[i]);
		}
		TimeSec=TimeTemp[0]*24*60*60+TimeTemp[1]*60*60+TimeTemp[2]*60;
		if(Count>=(TimeSec/15))	
		{
		  Count=0;
		  G_CycleMeterReadTimeoverFlag=1;
		}  
		else
		{
		  Count++;
		  G_CycleMeterReadTimeoverFlag=0;
		}
	}*/
}
  
/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : 下位机发送数据帧结束中断
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 G_GotMeterDataFlag=0;
void TIM4_IRQHandler(void)
{
	u8 *pMeterDataRecv;
	extern u8 G_Auto_Flag;
    TIM4->CR1=0;
	TIM4->CNT=0;
	TIM4->CCR1=50;
//	IWDG_ReloadCounter();	//喂狗
//	RstDog();
	if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	{
	    TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//清中断标志
		/*--------------------下行无线模块数据-----------------------*/
		if(G_UART_Num == 4)
		{
			if((G_RecvData[0]==0xFD)&&(G_RecvData[1]==0xFD)&&(G_RecvData[2]==0xFD)&&(G_RecvData[3]==0xFD)) //模块操作返回数据
			{
				G_WireLess_Back = 1;
				G_RecvDataLen = 0;
				return;
			}
//			else if((G_RecvData[0]!=0xFD)&&(G_RecvData[1]!=0x68))	
//			{
//				//在此处调用合并分片函数 Combine_Fragment
//				Combine_Fragment(G_RecvData,G_RecvDataLen);
//				G_RecvDataLen = 0;
//	
//			}
		}
		/*--------------------下行数据-----------------------*/
		if((GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6))||(GetWorkingMode()==1))	//管理模式
			{
			if(G_RecvDataLen>10)
				{
					pMeterDataRecv=G_RecvData;
					while(*pMeterDataRecv!=0x68)	//去除0xFE,剩余完整帧数据
						{
						  pMeterDataRecv++;
						  G_RecvDataLen--;
						}
					CJQUp_Protocol_Analysis(pMeterDataRecv,G_RecvDataLen);
								 				
			    }

			}
		else  // 透明模式
			{
				;	
			}
   	}
  G_RecvDataLen=0;
}
/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{
 TIM2->CNT=0;
 if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
	G_UpSendCmd[G_UpSendCmdLen++]= USART_ReceiveData(USART1); 
	if(G_UpSendCmdLen>127)
	 G_UpSendCmdLen=0;
	TIM2->CR1=1;
	G_UART_Num=1;
  }
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
 TIM4->CNT=0;
 if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
	G_RecvData[G_RecvDataLen++]= USART_ReceiveData(USART2); 
	 if(G_RecvDataLen>127)
	 G_RecvDataLen=0;
	TIM4->CR1=1;
  }
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{
  TIM4->CNT=0;
 if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
  G_RecvData[G_RecvDataLen++]= USART_ReceiveData(USART3);
	 if(G_RecvDataLen>127)
	 G_RecvDataLen=0;
  TIM4->CR1=1;

  }
}
/*******************************************************************************
* Function Name  : UART4_IRQHandler
* Description    : This function handles UART4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART4_IRQHandler(void)
{
 TIM2->CNT=0;
 if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
  {
	G_UpSendCmd[G_UpSendCmdLen++]= USART_ReceiveData(UART4);  
	 if(G_UpSendCmdLen>127)
	 G_UpSendCmdLen=0;
	TIM2->CR1=1;
	G_UART_Num=4;
  }
}

/*******************************************************************************
* Function Name  : UART5_IRQHandler
* Description    : This function handles UART5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART5_IRQHandler(void)
{
	TIM2->CNT=0;
 if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
  {
    G_UpSendCmd[G_UpSendCmdLen++]=USART_ReceiveData(UART5); 
	 if(G_UpSendCmdLen>127)
	 G_UpSendCmdLen=0;
	TIM2->CR1=1;
//	Dir=0;
	G_UART_Num=5;   
  }
}
/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
u8 G_RTC_Check_Flag=1;
u8 rtc_tmp[6]={0};
void SysTick_Handler(void)
{	
	u16 i;
	static char tim4dog=0;
	static u32 counter_25h=0;
	static u16 couner_5m=0;
	if(tim4dog==50)
	{
		IWDG_ReloadCounter();  //500毫秒喂一次狗
		tim4dog=0;
	}
	else
	{
		tim4dog++;
	}
	for(i=0;i<20;i++)
	{
		if(G_CJQ_status[i].flag==1)
		{
			if(G_CJQ_status[i].Counter>25*60*60*100)
				G_status.ST36=1;
			else
				G_status.ST36=0;
		}
	}
	counter_25h++;
	if(counter_25h>25*60*60*100)
	{
		counter_25h=0;
		if(G_RTC_Check_Flag==1)	
		{
			G_status.ST14=1;
		}
		G_RTC_Check_Flag=1;
	}

	couner_5m++;
	if(couner_5m>200)
	{
		u8 tmp;
		couner_5m=0;
		tmp=rtc_tmp[0];
		P8563_gettime(rtc_tmp);
		if(tmp==rtc_tmp[0])
			G_status.ST30=1;
		else
			G_status.ST30=0;	
	}
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
