#include "stm32f10x_lib.h"
#include <stdio.h>
#include "platform_config.h"
#include <InterFlash.h>
#include "stm32f10x_flash.h"
#include "ErrCode.h"
#include "spi.h"
/**********************************************************************************
* Function Name  : Switch on/off Valve
* Description    : 写开关阀
* Input          : Buf：“读仪表编号”帧，Len：指令长度
* Output         : None
* Return         : return 1：操作成功，return 0：操作失败，
***********************************************************************************/
u8 Switch_Valve(u8 *Buf,u16 Len)
{
    extern u8 G_UART_Num;
    u8 RecData[17],CR;
    u8 i;
    CR=CRC8(Buf,Len-2);
    if(Buf[Len-2]==CR)
    {
        for(i=0; i<9; i++)
            RecData[i]=Buf[10+i];                 //组68 T 表地址

        RecData[9]=0x04;					  //控制字
        RecData[10]=0x04;					   //L
        RecData[11]=0x17;					   //标识符
        RecData[12]=0xA0;
        RecData[13]=0x00;
        RecData[14]=Buf[21]; 				   //55or99

        RecData[15]=Add_Up(RecData,15);         //和校验
        RecData[16]=0x16;						//结束符

//		Send_UART(G_UART_Num,RecData,17);		//通过串口5发送到表具进行开关阀操作
        UART_5(RecData,17);
        return 1;
    }
    else
    {
        Err_Ack(Buf,DataErr);
        return 0;
    }
}
/**********************************************************************************
* Function Name  : Write_Ele_and_Mechan_Syn_Data
* Description    : 写机电同步数据
* Input          : Buf：“读仪表编号”帧，Len：指令长度
* Output         : None
* Return         : None
***********************************************************************************/
u8 Write_Ele_and_Mechan_Syn_Data(u8 *Buf,u16 Len)
{
    extern u8 G_UART_Num;
    u8 RecData[21],CR;
    u8 m,n;

    CR=CRC8(Buf,Len-2);
    if(CR==Buf[Len-2])
    {
        for(m=0; m<9; m++)
            RecData[m]=Buf[m+10]; //68H	T	Addr

        RecData[9]=0x16;
        RecData[10]=0x08;
        RecData[11]=0x16;
        RecData[12]=0xA0;
        RecData[13]=0x00;

        for(n=0; n<5; n++)
            RecData[14+n]=Buf[Len-7+n];

        RecData[19]=Add_Up(RecData,19);
        RecData[20]=0x16;

        //Send_UART(G_UART_Num,RecData,17);		//通过串口5发送到表具进行开关阀操作
        UART_5(RecData,21);
        return 1;
    }
    else
    {
        Err_Ack(Buf,DataErr);
        return 0;
    }
}
/*******************************************************************************
* Function Name  :  Up_Meter_Data	（暂为开关阀  电机数据  上传）
* Description    : 表具上传的数据
* Input          : DataBuf：表具上传数据，DataLen：数据长度
* Output         : None
* Return         : None
*******************************************************************************/
void Up_Meter_Data(u8 *DataBuf,u16 DataLen)
{
    u8 m,n,j,CS,RecData[28],Read_Cjq_ID[8];
    CS=Add_Up (DataBuf,DataLen-2);
    if(DataBuf[DataLen-2]==CS)
    {
        RecData[0]=0x68;
        RecData[1]=0x80;    //组协议头

        ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,Read_Cjq_ID,8);
        for(m=0; m<8; m++)
            RecData[m+2]=Read_Cjq_ID[m];	   //组采集器地址

        for(n=0; n<9; n++)
            RecData[10+n]=DataBuf[n];  //装载 68H	T	Addr

        if((DataBuf[11]==0x17)&&(DataBuf[12]==0xA0) )
        {
            RecData[19]=0x21;
            RecData[20]=0x01;	  ///??? 49 05

            if(DataBuf[DataLen-4]==0x80)
                RecData[21]=0x55;
            else if(DataBuf[DataLen-4]==0x81)
                RecData[21]=0x99;

            RecData[22]=CRC8(RecData,22);
            RecData[23]=0x16;

            UART_3(RecData,24);//上传到集中器
        }
        else if((DataBuf[11]==0x16)&&(DataBuf[12]==0xA0) )
        {
            RecData[19]=0x49;
            RecData[20]=0x05;

            for(j=0; j<5; j++)
                RecData[21+j]=DataBuf[14+j];

            RecData[26]=CRC8(RecData,26);
            RecData[27]=0x16;

            UART_3(RecData,28);//上传到集中器
        }

    }
    else
    {
        RecData[0]=0x68;
        RecData[1]=0xC0;    //组协议头

        ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,Read_Cjq_ID,8);
        for(m=0; m<8; m++)
            RecData[m+2]=Read_Cjq_ID[m];	   //组采集器地址

        for(n=0; n<9; n++)
            RecData[10+n]=DataBuf[n];  //装载 68H	T	Addr

        if(DataBuf[9]==0xD6)
            RecData[19]=0x49;
        else
            RecData[19]=0x21;

        RecData[20]=0x01;
        RecData[21]=DataErr;

        RecData[22]=CRC8(RecData,22);
        RecData[23]=0x16;
        UART_3(RecData,24);//上传到集中器
    }
}
/*******************************************************************************
* Function Name  :  Up_Meter_Auto_Data
* Description    : 向集中器上传 表具主动上传的数据
* Input          : DataBuf：表具上传数据，DataLen：数据长度
* Output         : None
* Return         : None
*******************************************************************************/
u8 Up_Meter_Auto_Data(u8 *DataBuf,u16 DataLen)
{
    u8 m,n;		//将表具上传的数据存到地址6中
    u8 Read_Cjq_ID[8],ReadAutoMeterData[255];
//	u8 MeterID[8],IndexBuf[256],MeterCheck,i=0,j=0;
    extern u8 G_UART_Num;
//	u8 buf0[17]={0x68,0x10,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x04,0x04,0x17,0xA0,0x00,0x55,0xF1,0x16};
//	MeterCheck=CRC8(&DataBuf[2],7);							 //计算上传表具索引值
//	ReadFlash_Byte(METER_CHECK_PAGEADDR,IndexBuf,256);
//	 for(i=0;i<255;i++)
//		{
//		if(MeterCheck==IndexBuf[i])
//			{
//				ReadFlash_Byte(METER_NUM_PAGEADDR+i*8,MeterID,7);
//				for(j=0;j<6;j++)
//				{
//					if(MeterID[j]==DataBuf[j+2])break;	  //表具ID号是否相同
//				}
//				if (j==8)//表号相同
//				{

//					 WriteFlash_Page(METER_DATA_PAGEADDR-((i/2)*2048)+((i%2)*PAGESIZE/2),0,&DataBuf[12],10);//将表具信息写入到flash中存表具数据的相应位置

    ReadAutoMeterData[0]=0x68;
    ReadAutoMeterData[1]=0x80;	    //组协议头

    ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,Read_Cjq_ID,8);
    for(m=0; m<8; m++)
        ReadAutoMeterData[m+2]=Read_Cjq_ID[m];	   //组采集器地址

    for(n=0; n<DataLen-2; n++)
        ReadAutoMeterData[n+10]=DataBuf[n];       //采集器地址之后组校验码之前数据

    ReadAutoMeterData[DataLen+10-2]=CRC8(ReadAutoMeterData,DataLen+10-2);//组校验值

    ReadAutoMeterData[DataLen+10-1]=0x16;	// 组结束符

//		 			 Send_UART(G_UART_Num,ReadAutoMeterData,DataLen+10); //向集中器发送数据
    UART_3(ReadAutoMeterData,DataLen+10);
//					 UART_1(ReadAutoMeterData,DataLen+10);
//					 UART_5(buf0,17);
//					 return (1);
//				}
//				else
//				 	return (0);
//			}
//	    }
    return 0;
}
