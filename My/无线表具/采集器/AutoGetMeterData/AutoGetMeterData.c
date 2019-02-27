
#include "platform_config.h"
//#include "I2C_Driver.h"
#include <InterFlash.h>
#include "ErrCode.h"
/*******************************************************************************
* Function Name  : TimeDelay
* Description    : 定时器3.5S定时，定时时间小于3.5S返回0，超过等于3.5S返回1；
* Input          : Null
* Output         : Null
* Return         : return 0:标识复位，return 1：超时
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
* Description    : 将表具返回的data域存储在内部flash和外部flash相应位置
* Input          : SaveLocation：Data存储位置
				   DataBuf：要存储的Data 缓存
				   DataLen：要存储的Data长度
* Output         : Null
* Return         : None
*******************************************************************************/
u8 Auto_Save_Meter_Data(u8 SaveLocation ,u8 *DataBuf,u16 DataLen)//存储data域
{
    u8 State1,State2;

    State1=SPI_Flash_Write(EXT_METER_DATA_PAGEADDR+((SaveLocation/2)*4096)+((SaveLocation%2)*(PAGESIZE/2)),DataBuf,DataLen);//写外部flash
    State2=WriteFlash_Page(METER_DATA_PAGEADDR-((SaveLocation/2)*2048)+((SaveLocation%2)*(PAGESIZE/2)),0,DataBuf,DataLen);//写入内部flash
    if((State1==1)&&(State2==1))
        return 1;
    else
        return 0;
}
/*******************************************************************************
* Function Name  : Auto_Get_Meter_Data
* Description    : 循环发送表地址进行抄表，抄取不正确或者没有数据返回时，
	                 抄取3次。将表具返回的信息中的data域进行存储。
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
    if(Check_Index_Equ()==1)  //对外部flash操作
    {
        SPI_Flash_Read(EXT_METER_NUM_ADDR,&MeterNum,1);//读取表具数量
        SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR, MeterCheck,255);//读取表具索引
        for(i=0; i<256; i++)
        {
            if(Count<MeterNum)
            {
                if(MeterCheck[i]!=0xff)
                {
                    Count++;
                    SPI_Flash_Read(EXT_METER_NUM_PAGEADDR+i*8, MeterAddr,8);//读取表具地址
                    if(MeterAddr[0]!=0x90)	//不是13757协议设备
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
                        Auto_sent(Conmmand,19);//发送抄表指令
                        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
                        USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
//						delay_init(72);	     //延时初始化
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
                                    Auto_Save_Meter_Data(i,&MeterDataRecv[14],G_Auto_MeterDataLen-16);//存储data域
                                    //	break;
                                }
                                else  //校验码错误，重新发送抄表指令
                                {
                                    if (CMD_Count<3)
                                    {
                                        CMD_Count++;
                                        goto repeat1;
                                    }
                                }
                            }
                            else //传输错误
                            {
                                if (CMD_Count<3)
                                {
                                    CMD_Count++;
                                    goto repeat1;
                                }
                            }

                        }

                        else//没有接收到数据
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
    else if (Check_Index_Equ()==0)//对内部flash操作
    {
        ReadFlash_Byte(METER_NUM_ADDR,&MeterNum,1);//读取表具数量
        ReadFlash_Byte(METER_CHECK_PAGEADDR, MeterCheck,256);//读取表具索引
        for(i=0; i<256; i++)
        {
            if(Count<MeterNum)
            {
                if(MeterCheck[i]!=0xff)
                {
                    Count++;
                    ReadFlash_Byte(METER_NUM_PAGEADDR+i*8, MeterAddr,8);//读取表具地址
                    if(MeterAddr[0]!=0x90)	//不是13757协议设备
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
                        Auto_sent(Conmmand,19);//发送抄表指令
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
                                    Auto_Save_Meter_Data(i,&MeterDataRecv[14],G_Auto_MeterDataLen-16);//存储data域
                                    //break;
                                }
                                else  //校验码错误，重新发送抄表指令
                                {
                                    if (CMD_Count<3)
                                    {
                                        CMD_Count++;
                                        goto repeat2;
                                    }
                                }
                            }
                            else //传输错误
                            {
                                if (CMD_Count<3)
                                {
                                    CMD_Count++;
                                    goto repeat2;
                                }
                            }

                        }
                        else //没有接收到数据
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
        Copy_Inter_To_EXT();   //统一内外flash
    }
    TIM_ITConfig(TIM3, TIM_IT_Update , ENABLE);
    TIM_Cmd(TIM3, ENABLE);
    G_Auto_Flag=0;
}


