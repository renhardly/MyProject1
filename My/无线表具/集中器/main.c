/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : Main program body
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "platform_config.h"
#include <EXT_Flash.h>
#include "spi.h"
#include "ErrCode.h"
JZQ_Status G_status;
JZQ G_JZQ;
CJQ_status G_CJQ_status[20];
u8  Init_SD(void);
STRUCT_CHAR ASC2HEX(u8 *pData,u16 DataLen)
{
	 u8  ChangedBuf[1024];
	 STRUCT_CHAR HexBuf;
	 u16 i=0,j=0;
	 u16 DataNum=0;
	 DataNum=DataLen/2+DataLen%2; //
	 if(DataLen%2==1) //奇数时补成偶数个，前面加0
	 {
	 	DataLen++;
	 	ChangedBuf[0]=0x30;
	 	for(i=1;i<DataLen;i++)
		{
		  ChangedBuf[i]=pData[i-1];
		}
		for(i=0;i<DataLen;i++)
		{
		 	pData[i]=ChangedBuf[i];
		}

	 }
	 for(i=0;i<DataLen;i++)
	 {
	 	if((pData[j]>=0x30)&&(pData[j]<=0x39))
		{
	 	 	ChangedBuf[i]=pData[j]-0x30;
		}
		else if((pData[j]>='A')&&(pData[j]<='F'))
		{
			ChangedBuf[i]=pData[j]-0x37;
		}
		else if((pData[j]>='a')&&(pData[j]<='f'))
		{
			ChangedBuf[i]=pData[j]-0x57;
		}
		else
		{
			 ;
		}
		j++;
	 }
	 for(i=0,j=0;i<DataNum;i++)
	 {
	 
	 	HexBuf.Data_String[i]=ChangedBuf[j]*0x10+ChangedBuf[j+1];
		j+=2;	
	 }
	 return  HexBuf;
}
void IWDG_init(void);
void Init_Configuration(void)
{
    /* 配置 看门狗*/
	//IWDG_init();
    /* 系统时钟配置 */
    RCC_Configuration();
    /* 中断配置 */
    NVIC_Configuration();
    /* 配置I/O口 */
    GPIO_Configuration();
    /* 配置定时器 */
    TIM_Configuration();
    /* 配置USART */
    USART_Configuration();
	EXTI_Configuration();
    /*配置EXTFlash SPI */
    SPI_Flash_Init();
    /*初始AD转换 */
    ADC_Status_Init();
    /*系统滴答初始化 */
    SysTick_Config(SystemCoreClock / 100);
	/* 初始化CH376 */
	mInitCH376Host( ); 
}
u8 G_PageTmp[PAGESIZE];
/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern u8 G_Deal_flag;
u8  G_Len_TMP;
void Init_JZQ_S(void);
u8  Init_Flash(void);
u8 Add_One_CJQ_SD(u8 *Buf,u16 Len);
int main(void)
{
//    u8 buf[32]={0x68,0x00,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
//            0x68,0x41,0x44,0x25,0x14,0x14,0x08,0x13,0x20,0x68,
//			0x20,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x43,0x00,
//			0x50,0x16};

	u8  UpSendCmd_tmp[128];
    Init_Configuration();
	//Init_JZQ(buf,32);
	//Add_One_CJQ_SD(buf,32);
	Init_JZQ_S();
  	JZQ_RTC_Start(); 
    while (1)
    {
		if(G_Deal_flag==1)
        {
            G_Deal_flag=0;
            memcpy(UpSendCmd_tmp,G_UpSendCmd,G_Len_TMP);
            Computer_Concentrator_Protocol_Analysis(UpSendCmd_tmp, G_Len_TMP);
        }
    }
}
#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
