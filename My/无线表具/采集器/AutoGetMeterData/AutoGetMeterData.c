
#include "platform_config.h"
//#include "I2C_Driver.h"
#include <InterFlash.h>
#include "ErrCode.h"
/*******************************************************************************
* Function Name  : TimeDelay
* Description    : ��ʱ��3.5S��ʱ����ʱʱ��С��3.5S����0����������3.5S����1��
* Input          : Null
* Output         : Null
* Return         : return 0:��ʶ��λ��return 1����ʱ
*******************************************************************************/
u8 TimeDelay()
{
    extern u8 G_GotMeterDataFlag;
    u32 i=0x40000;
    while(i--)
    {
//        IWDG_ReloadCounter();
        RstDog();
        if(G_GotMeterDataFlag==1)
        {
            G_GotMeterDataFlag=0;
            return 0;
        }
    }
    G_GotMeterDataFlag=0;
    return 1;
}
/*******************************************************************************
* Function Name  : Save_Meter_Data
* Description    : ����߷��ص�data��洢���ڲ�flash���ⲿflash��Ӧλ��
* Input          : SaveLocation��Data�洢λ��
				   DataBuf��Ҫ�洢��Data ����
				   DataLen��Ҫ�洢��Data����
* Output         : Null
* Return         : None
*******************************************************************************/
u8 Auto_Save_Meter_Data(u8 SaveLocation ,u8 *DataBuf,u16 DataLen)//�洢data��
{
    u8 State1,State2;

    State1=SPI_Flash_Write(EXT_METER_DATA_PAGEADDR+((SaveLocation/2)*4096)+((SaveLocation%2)*(PAGESIZE/2)),DataBuf,DataLen);//д�ⲿflash
    State2=WriteFlash_Page(METER_DATA_PAGEADDR-((SaveLocation/2)*2048)+((SaveLocation%2)*(PAGESIZE/2)),0,DataBuf,DataLen);//д���ڲ�flash
    if((State1==1)&&(State2==1))
        return 1;
    else
        return 0;
}
/*******************************************************************************
* Function Name  : Auto_Get_Meter_Data
* Description    : ѭ�����ͱ��ַ���г�����ȡ����ȷ����û�����ݷ���ʱ��
	                 ��ȡ3�Ρ�����߷��ص���Ϣ�е�data����д洢��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 G_Auto_Flag=0;
void Auto_Get_Meter_Data(void)
{
    extern u8 G_Auto_MeterDataLen,G_GotMeterDataFlag;
    extern u8 G_MeterDataRecv[260];
    extern u8 G_CycleMeterReadTimeoverFlag;
    u8 MeterDataRecv[260];
    u8 MeterNum=0,Count=0,CMD_Count=0,i=0,j=0,m=0;
    u8 CheckCode;
    u8 MeterCheck[256];
    u8 MeterAddr[8];
    u8 Conmmand[19]= {0XFE,0XFE,0XFE,0X68,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X03,0X1F,0X90,0X00,0X00,0X16};
    G_Auto_Flag=1;
    G_CycleMeterReadTimeoverFlag=0;
//	pMeterDataRecv=G_MeterDataRecv;
    TIM_Cmd(TIM3, DISABLE);
//	TIM_ITConfig(TIM3, TIM_IT_Update , DISABLE);
    if(Check_Index_Equ()==1)  //���ⲿflash����
    {
        SPI_Flash_Read(EXT_METER_NUM_ADDR,&MeterNum,1);//��ȡ�������
        SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR, MeterCheck,255);//��ȡ�������
        for(i=0; i<256; i++)
        {
            if(Count<MeterNum)
            {
                if(MeterCheck[i]!=0xff)
                {
                    Count++;
                    SPI_Flash_Read(EXT_METER_NUM_PAGEADDR+i*8, MeterAddr,8);//��ȡ��ߵ�ַ
                    if(MeterAddr[0]!=0x90)	//����13757Э���豸
                    {
                        for(j=0; j<8; j++)
                        {
                            Conmmand[j+4]=MeterAddr[j];
                        }
                        Conmmand[17]=Add_Up(&Conmmand[3],14);
                        CMD_Count=1;
repeat1:
                        G_Auto_MeterDataLen=0;
                        USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
                        USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
                        Auto_sent(Conmmand,19);//���ͳ���ָ��
                        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
                        USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
//						delay_init(72);	     //��ʱ��ʼ��
//				        delay_ms(80);
                        if((!TimeDelay())&&(G_Auto_MeterDataLen>10))
                        {
                            j=0;
                            while(G_MeterDataRecv[j]!=0x68)
                            {
                                j++;
                            }
                            G_Auto_MeterDataLen=G_Auto_MeterDataLen-j;

                            if(j<20)
                            {
                                for(m=0; m<G_Auto_MeterDataLen; m++)
                                {
                                    MeterDataRecv[m]=G_MeterDataRecv[j];
                                    j++;
                                }
                                CheckCode=Add_Up(MeterDataRecv,G_Auto_MeterDataLen-2);
                                if(CheckCode==MeterDataRecv[G_Auto_MeterDataLen-2])
                                {
                                    Auto_Save_Meter_Data(i,&MeterDataRecv[14],G_Auto_MeterDataLen-16);//�洢data��
                                    //	break;
                                }
                                else  //У����������·��ͳ���ָ��
                                {
                                    if (CMD_Count<3)
                                    {
                                        CMD_Count++;
                                        goto repeat1;
                                    }
                                }
                            }
                            else //�������
                            {
                                if (CMD_Count<3)
                                {
                                    CMD_Count++;
                                    goto repeat1;
                                }
                            }

                        }

                        else//û�н��յ�����
                        {
                            if (CMD_Count<3)
                            {
                                CMD_Count++;
                                goto repeat1;
                            }
                        }

                    }

                }
            }
            if (Count==MeterNum)break;
        }
    }
    else if (Check_Index_Equ()==0)//���ڲ�flash����
    {
        ReadFlash_Byte(METER_NUM_ADDR,&MeterNum,1);//��ȡ�������
        ReadFlash_Byte(METER_CHECK_PAGEADDR, MeterCheck,256);//��ȡ�������
        for(i=0; i<256; i++)
        {
            if(Count<MeterNum)
            {
                if(MeterCheck[i]!=0xff)
                {
                    Count++;
                    ReadFlash_Byte(METER_NUM_PAGEADDR+i*8, MeterAddr,8);//��ȡ��ߵ�ַ
                    if(MeterAddr[0]!=0x90)	//����13757Э���豸
                    {
                        for(j=0; j<8; j++)
                        {
                            Conmmand[j+4]=MeterAddr[j];
                        }
                        Conmmand[17]=Add_Up(&Conmmand[3],14);
                        CMD_Count=1;
repeat2:
                        USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
                        USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
                        Auto_sent(Conmmand,19);//���ͳ���ָ��
                        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
                        USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
                        if(!TimeDelay()&&(G_Auto_MeterDataLen>10))
                        {
                            j=0;
                            while(G_MeterDataRecv[j]!=0x68)
                            {
                                j++;
                            }
                            G_Auto_MeterDataLen=G_Auto_MeterDataLen-j;
                            if(j<20)
                            {
                                for(m=0; m<G_Auto_MeterDataLen; m++)
                                {
                                    MeterDataRecv[m]=G_MeterDataRecv[j];
                                    j++;
                                }
                                CheckCode=Add_Up(MeterDataRecv,G_Auto_MeterDataLen-2);
                                if(CheckCode==MeterDataRecv[G_Auto_MeterDataLen-2])
                                {
                                    Auto_Save_Meter_Data(i,&MeterDataRecv[14],G_Auto_MeterDataLen-16);//�洢data��
                                    //break;
                                }
                                else  //У����������·��ͳ���ָ��
                                {
                                    if (CMD_Count<3)
                                    {
                                        CMD_Count++;
                                        goto repeat2;
                                    }
                                }
                            }
                            else //�������
                            {
                                if (CMD_Count<3)
                                {
                                    CMD_Count++;
                                    goto repeat2;
                                }
                            }

                        }
                        else //û�н��յ�����
                        {
                            if (CMD_Count<3)
                            {
                                CMD_Count++;
                                goto repeat2;
                            }
                        }

                    }

                }
            }
            //	else if (Count==MeterNum)break;
        }
        Copy_Inter_To_EXT();   //ͳһ����flash
    }
    TIM_ITConfig(TIM3, TIM_IT_Update , ENABLE);
    TIM_Cmd(TIM3, ENABLE);
    G_Auto_Flag=0;
}


