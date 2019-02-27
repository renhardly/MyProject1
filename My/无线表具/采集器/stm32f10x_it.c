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
#include "stm32f10x_it.h"
#include "platform_config.h"
#include "ErrCode.h"
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : 上位机发送命令帧结束中断
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8  G_UpSendCmd[128];
u8  G_MeterDataRecv[128];
u16 G_UpSendCmdLen,G_MeterDataLen,G_Auto_MeterDataLen;
u8  G_UART_Num;
u8  G_Get_Realtime_Data_Flag=0,G_Get_All_Meter_Realtime_Data_Flag=0;
u8 G_Deal_flag=0;
extern u8 G_Len_TMP;

void TIM2_IRQHandler(void)
{
    u8 *pUpSendCmd,i=0;
    TIM2->CR1=0;
    TIM2->CNT=0;
    TIM2->CCR1=40;
//    IWDG_ReloadCounter();	//喂狗
//    RstDog();
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清中断标志
        /*--------------------上行无线模块数据-----------------------*/
        if(G_UART_Num == 3)
        {

            if((G_UpSendCmd[0]==0xFD)&&(G_UpSendCmd[1]==0xFD)&&(G_UpSendCmd[2]==0xFD)&&(G_UpSendCmd[3]==0xFD)) //模块操作返回数据
            {
                G_WireLess_Back = 1;
                G_UpSendCmdLen = 0;
                return;
            }
//		else if((G_UpSendCmd[0]!=0xFD)&&(G_UpSendCmd[1]!=0x68))
//		{
//			//在此处调用合并分片函数 Combine_Fragment
//			Combine_Fragment(G_UpSendCmd,G_UpSendCmdLen);
//			G_UpSendCmdLen = 0;
//
//		}
        }
        /*--------------------下行数据-----------------------*/
        if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15))||(GetWorkingMode()==1))//管理模式下
        {
            pUpSendCmd=G_UpSendCmd;
            while(*pUpSendCmd!=0x68)
            {
                pUpSendCmd++;
                i++;
                G_UpSendCmdLen--;
                if(G_UpSendCmdLen<=2)
                    break;
            }
            if((i<20)&&(G_UpSendCmdLen<256)&&(G_UpSendCmdLen>5)) //前导码小于20个，指令总长度不大于255
            {
                u8 Check=CRC8(pUpSendCmd, G_UpSendCmdLen-2);
                if(Check==pUpSendCmd[G_UpSendCmdLen-2])
                {
                    G_Deal_flag=1;
                    G_Len_TMP=G_UpSendCmdLen;
                }
            }
        }
        else //透明数据传输
        {
            ;
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

void TIM3_IRQHandler(void)	//24小时计数器
{
    static u8 AddCount=0;
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
        for(i=0; i<3; i++)
        {
            TimeTemp[i]=BCD2HEX(TimeBuf[i]);
        }
        TimeSec=TimeTemp[0]*24*60*60+TimeTemp[1]*60*60+TimeTemp[2]*60;
        if(AddCount>=(TimeSec/15))
        {
            AddCount=0;
            G_CycleMeterReadTimeoverFlag=1;
        }
        else
        {
            AddCount++;
            G_CycleMeterReadTimeoverFlag=0;
        }
    }
    TIM_Cmd(TIM3, ENABLE);
}

/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : 表具主动上传数据中断-刘勇
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 G_GotMeterDataFlag=0;
extern u32 G_Point[256];
void TIM4_IRQHandler(void)
{
    u8 *pMeterDataRecv;
    extern u8 G_Auto_Flag;
    u8 meter_add[8];
	s32 meter_index;
	u8 tmp[32]={0};
	u16 i;
    TIM4->CR1=0;
    TIM4->CNT=0;
    TIM4->CCR1=50;
//    IWDG_ReloadCounter();	//喂狗
//    RstDog();
    if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
        TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//清中断标志

        if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15))||(GetWorkingMode()==1))	//管理模式
        {
            if(G_MeterDataLen>10)
            {
                pMeterDataRecv=G_MeterDataRecv;
                G_Auto_MeterDataLen=G_MeterDataLen;
                while(*pMeterDataRecv!=0x68)	//去除0xFE,剩余完整帧数据
                {
                    pMeterDataRecv++;
                    G_MeterDataLen--;
                    if(G_MeterDataLen<=0)
                        break;
                }
				
                if(G_MeterDataLen<16)
                {
                    G_MeterDataLen=0;
					return;
                }
				//exchange(pMeterDataRecv+2,7);
				
                if(Add_Up(pMeterDataRecv,G_MeterDataLen-2)!=pMeterDataRecv[G_MeterDataLen-2])
                {
                 
					METER_Err_Ack(pMeterDataRecv,CheckCodeErr);
					G_MeterDataLen=0;
                    return;
                }
				if((pMeterDataRecv[11]==0x1F)&&(pMeterDataRecv[12]==0x90))
				{				
               		 Up_Meter_Auto_Data(pMeterDataRecv,G_MeterDataLen); //保存表具主动上传的数据，并置通信标志位-刘勇
				}
				else
				{
					 _Meter_Up_Protocol_Analysis(pMeterDataRecv,G_MeterDataLen);

				}
				G_MeterDataLen=0;
                memcpy(meter_add,pMeterDataRecv+1,8);
				meter_index=Return_Meter_Index(meter_add);
				if(meter_index==-1||meter_index>=METER_HEAP_PSIZE)
				{
					G_MeterDataLen=0;
					return;
				}
					
				for(i=0; i<METER_HEAP_PSIZE; i++)
                {
                    if(G_Point[i]!=0)
                    {
                        CMD_CONTRL *p=(CMD_CONTRL *)(G_Point[i]);
						if(p->cmd_index[meter_index/16]&(1<<(meter_index%16))!=0);//如果是对应此表
                        {
                            p->cmd_index[meter_index/16]&=~(1<<(meter_index%16));
						//	exchange(meter_add,7);
							switch(p->type)
							{
								case METER_RTC_SET:
									Write_Meter_RTC(meter_add,NULL,0);
								    break;
								case METER_VALVE_SET:
									_Switch_Valve(meter_add,p->data_p,1);
								    break;
								case METER_ELCSYN_SET:
									_Write_Ele_and_Mechan_Syn_Data(meter_add,p->data_p,5);
								    break;
								case METER_Settlement_Date_SET:
									_Settlement_Date(meter_add,p->data_p,4);
								    break;
								case METER_Meter_Reading_SET:
									_Meter_Reading(meter_add,p->data_p,4);
								    break;
								case METER_WR_Purchase_Amount_SET:
									_Meter_WR_Purchase_Amount(meter_add,p->data_p,12);
								    break;
								case METER_New_Key_SET:
									_Meter_New_Key(meter_add,p->data_p,12);
								    break;
								case Meter_WR_Remaining_Amount_SET:
									_Meter_WR_Remaining_Amount(meter_add,p->data_p,7);
								    break;
								case Meter_Read_Price_List_SET:
									_Meter_Read_Price_List(meter_add,p->data_p,3);
								    break;
								case Meter_RD_Purchase_Amount_SET:
									_Meter_RD_Purchase_Amount(meter_add,p->data_p,3);
								    break;
								case Meter_RD_ADDR_SET:
									_Meter_RD_ADDR(meter_add,p->data_p,3);
								    break;
								case METER_Write_Price_SET:
									_Write_Price_List(meter_add,p->data_p,30);
								    break;
							   default:
								  break;
							}	
							if(memcmp((u8 *)p->cmd_index,tmp,32)==0)
							{
	                            free(p->data_p);
	                            free(p);
								G_Point[i]=0;
							}
                            break;
                        }
                    }
				}

			   
            }

        }
        else  // 透明模式
        {
            ;
        }
    }

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
    TX_CTL_ON();
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        G_UpSendCmd[G_UpSendCmdLen++]= USART_ReceiveData(USART1);
        if(G_UpSendCmdLen>255)
        {
            G_UpSendCmdLen=0;
        }
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
        G_MeterDataRecv[G_MeterDataLen++]= USART_ReceiveData(USART2);
        if(G_MeterDataLen>128)
        {
            G_MeterDataLen=0;
        }
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
    TIM2->CNT=0;
    TX_CTL_ON();
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        G_UpSendCmd[G_UpSendCmdLen++]= USART_ReceiveData(USART3);
        if(G_UpSendCmdLen>128)
        {
            G_UpSendCmdLen=0;
        }
        TIM2->CR1=1;
        G_UART_Num=3;
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
    TIM4->CNT=0;
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        G_MeterDataRecv[G_MeterDataLen++]= USART_ReceiveData(UART4);
        if(G_MeterDataLen>128)
        {
            G_MeterDataLen=0;
        }
        TIM4->CR1=1;
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
    TIM4->CNT=0;
    TX_CTL_ON();
    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {
        G_MeterDataRecv[G_MeterDataLen++]=USART_ReceiveData(UART5);
        if(G_MeterDataLen>128) G_MeterDataLen=0;//防止数组越界
        TIM4->CR1=1;
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
u8 G_Read_Meter_Commu_Flag=0;
void SysTick_Handler(void)	   //10毫秒一次中断
{
    u16 i;
	static u8 tim4dog=0;
	static u32 counter_25h=0;
	static u16 counter_5m=0;
	static u32 counter_h=0;
	static u32 meter_commu_25h=0;  //每25小时读一次表通信状态数组，对不正常的置位仪表状态ST10-刘勇
	if(tim4dog==50)
	{
	//	IWDG_ReloadCounter();  //500毫秒喂一次狗
		tim4dog=0;
	}
	else
	{
		tim4dog++;
	}
	//每25小时读一次表通信状态数组，对不正常的置位仪表状态ST10-刘勇
	meter_commu_25h++;
    if(meter_commu_25h>25*60*60*100)
    {
        G_Read_Meter_Commu_Flag=1;
		meter_commu_25h=0;
    }

	counter_h++;
	if(counter_h>100*60*60)
	{
		counter_h=0;
		for(i=0; i<METER_HEAP_PSIZE; i++)
	    {
	        if(G_Point[i]!=0)
	        {
	            CMD_CONTRL *p=(CMD_CONTRL *)(G_Point[i]);
				if(p->counter_down>0)
	            	p->counter_down--;
				else
				{
					free(p->data_p);
					free(p);
                    G_Point[i]=0;
				}		
	        }
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
    counter_5m++;
    if(counter_5m>200)
    {
        u8 tmp;
		counter_5m=0;
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
