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
#include <InterFlash.h>
#include "ErrCode.h"
u8 G_PageTmp[PAGESIZE]= {0};
u8 G_CycleMeterReadTimeoverFlag;
u8 G_CMD_num=0;
/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern u8 G_Deal_flag;
extern u8 G_Meter_Comm_Status[32];//����
extern u8 G_Read_Meter_Commu_Flag;//25Сʱ����ͨ��״̬��־
void CJQ_init(void)	  //�ɼ�����ʼ���� �����ʼ����ȷ����1
{
    memset(&G_status,0,sizeof(CJQ_Status));
	ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,G_CJQ.ADDR,8);
    CJQ_RTC_Start();
    if(Set_WireLess_Parameter(TYPE_ADDR,G_CJQ.ADDR)==1)G_status.ST32=1;
	memset(G_Meter_Comm_Status,0xFF,32);//ͨ�ŵĳ�ʼ״̬ȫΪ1����������-����
}
u8  G_Len_TMP;
CJQ G_CJQ;
void IWDG_init(void);
u8 G_Up_SD_flag=0;

int main(void)
{
    u8  UpSendCmd_tmp[128];
	u8 i,j,location=0,tmp[19];  //�޸ı�ST-����

	u8 CmdBuf[23]={0x68,0x00,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x10,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x87,0x00,0xff,0x16};//������


	/* ���� ���Ź�*/
    //IWDG_init();
    /* ϵͳʱ������ */
    RCC_Configuration();
    /* �ж����� */
    NVIC_Configuration();
    /* ����I/O�� */
    GPIO_Configuration();
    /*����SPI flash*/
    SPI_Flash_Init();
    /* ���ö�ʱ�� */
    TIM_Configuration();
    /* ����USART */
    USART_Configuration();//UART3��SPI2�г�ͻ
    /*ϵͳ�δ��ʼ�� */
    SysTick_Config(SystemCoreClock / 100);
    G_CycleMeterReadTimeoverFlag=1;
    /*---------------------------------------------------------------------*/
    CJQ_init() ;
	//Init_CJQ(CmdBuf,23);
    while (1)
    {
		if(G_Up_SD_flag==1)				   //��ʱ��������������
		{
			G_Up_SD_flag=0;
			memcpy(CmdBuf+2,G_CJQ.ADDR,8);
	        CmdBuf[21]=CRC8(tmp,21);
			Read_All_Meter_Data(CmdBuf,23);
			
		}
		if(G_Deal_flag==1)
        {
            G_Deal_flag=0;
            memcpy(UpSendCmd_tmp,G_UpSendCmd,G_Len_TMP);
            Computer_Concentrator_Protocol_Analysis(UpSendCmd_tmp, G_Len_TMP);
        }
		if(G_Read_Meter_Commu_Flag==1) //ÿ25Сʱ��һ�α�ͨ��״̬���飬�Բ���������λ�Ǳ�״̬ST10-����
		{
			G_Read_Meter_Commu_Flag=0;
			for(i=0;i<32;i++)
			{
				if(G_Meter_Comm_Status[i]!=0)
				{
					for(j=0;j<8;j++)
					{
						if((G_Meter_Comm_Status[i]&(0x01<<j))!=0)
						{
							location = i*8+j;
							SPI_Flash_Read(EXT_METER_DATA_PAGEADDR+((location/2)*4096)+((location%2)*PAGESIZE/2),tmp,19);
							tmp[18] |= 1; //ST10��1
							G_status.ST34=1;//��ͨѶ�쳣
							SPI_Flash_Write(EXT_METER_DATA_PAGEADDR+((location/2)*4096)+((location%2)*PAGESIZE/2),tmp,19);//�޸��ⲿflash�еı�����
			   				WriteFlash_Page(METER_DATA_PAGEADDR-((location/2)*2048)+((location%2)*PAGESIZE/2),0,tmp,19);//�޸��ڲ�flash�еı�����
						}
					}
				}
			}
		}//ST10�޸����
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
    while (1)
    {
    }
}
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
