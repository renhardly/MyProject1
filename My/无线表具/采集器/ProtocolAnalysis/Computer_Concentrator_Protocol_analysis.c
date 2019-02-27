#include "platform_config.h"
#include <InterFlash.h>
#include "ErrCode.h"
/*******************************************************************************
* Function Name  : Computer_Concentrator_Protocol_Analysis
* Description    : 集中器、手持机发来的指令，解析相应的命令
* Input          : Buf：上位机发送的数据，Len：数据长度
* Output         : None
* Return         : None
*******************************************************************************/
extern u32 G_Point[METER_HEAP_PSIZE];
 u8 G_Meter_BS0,G_Meter_BS1;
 u8 Up_flag=0;//批量添加表具标号
u8  Computer_Concentrator_Protocol_Analysis(u8 *Buf,u16 Len)
{
    u8 CJQ_Check=0;
	if(memcmp(G_CJQ.ADDR,Buf+6,2)!=0)
		G_status.ST25=1;
    switch(Buf[19])
    {
    case 0x01://读单只表具数据
    {
        CJQ_Check=CJQ_Addr_Check(Buf); // 检查采集器地址是否正确
        if(CJQ_Check==0)
        {
            return 0;
        }
		 else
        {
            Read_One_Meter_Data(Buf,Len);//被动读采集器中的表数据
        }
        break;
    }
	case 0x02://读所有表具数据
    {
        delay_ms(G_CJQ.ADDR[0]); //通过编号来规避冲突。lixiaowei
		Read_All_Meter_Data(Buf,Len);
        break;
    }

	case 0x03://读表具安装信息：已安装表具，未安装表具，空位数量
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {
            return 0;
        }
        else
        {
            Read_Meter_Installation_Information(Buf,Len);
        }
        break;
    }
	case 0x04://读已安装表号
	case 0x07://读已安装表号
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {
            return 0;
        }
        else
        {
			if(G_Ative)
			{
				Buf[19]=0x03;
				G_Meter_BS0=0x0A;
				G_Meter_BS1=0x81;							  //主动方式	hu
				Meter_Up_FrameFormat(Buf,Len);
			}
			else
            Read_All_Installed_Meter_Number(Buf,Len);
        }
        break;
    }
	case 0x05://读未安装表号
	{
		CJQ_Check=CJQ_Addr_Check(Buf); // 检查采集器地址是否正确
		if(CJQ_Check==0)
		{
		   return 0;
		}
		else
		{
		  Read_All_unstalled_Meter_Number(Buf,Len);
		}
		break;
	}
	case 0x06: //读采集器地址
    {
		Read_CJQAddr(Buf,Len);
        break;
    }
	case 0x0A://读购入金额	hu		用到手持机？？？？
    {
        CJQ_Check=CJQ_Addr_Check(Buf); // 检查采集器地址是否正确
        if(CJQ_Check==0)
        {
            return 0;
        }
        else
        {
			Buf[19]=0x01;
			G_Meter_BS0=0x05;
			G_Meter_BS1=0x81;							  //主动方式	hu
			Meter_RD_Purchase_Amount(Buf,Len);
        }
        break;
    }
    case 0x0B://读价格表	hu
    {
        CJQ_Check=CJQ_Addr_Check(Buf); // 检查采集器地址是否正确
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {
			Buf[19]=0x01;
			G_Meter_BS0=0x02;
			G_Meter_BS1=0x81;							  //主动方式	hu
			Meter_Read_Price_List(Buf,Len);
        }
        break;
    }



    case 0x08: //读单只表具实时数据
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {
            Read_One_Meter_Realtime_Data(Buf,Len);
        }

        break;
    }
//	case 0x0A:
//    {
//        CJQ_Check=CJQ_Addr_Check(Buf);
//        if(CJQ_Check==0)
//        {
//
//            return 0;
//        }
//        else
//        {
//            Read_All_Meter_Data(Buf,Len);
//        }
//
//        break;
//    }
    case 0x10:
    {
        u8 tmp[8]={0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa};
		CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {
            if(memcmp(Buf+2,tmp,8)==0)
			 	CJQ_RTC_Set_ALL(Buf,Len);
			return 0;
        }
        else
        {
			CJQ_RTC_Set_ONE(Buf,Len);
        }
        break;
    }
	case 0x11://清除集中器中一台采集器内所有表具的数据信息，但保留表具的编号信息
	{
		 CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {
		 Clear_Meter_Data(Buf,Len);
		 }
		break;
	}
	case 0x21:	  //写开关阀
    {
		 CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {

			Switch_Valve(Buf,Len);
        break;
		}
    }
    case 0x22: //添加一台管理表具
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {
			if(Add_One_Meter(Buf,Len)==1)
            Right_Ack_Len0(Buf, Len);
        }

        break;
    }
		case 0x23:
    {
	
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {	
			u8 RecData[23],i;
			for(i=0;i<Buf[20]/7;i++)
			{
				memcpy(&RecData[12],&Buf[21+i*7],7);

				Add_One_Meter_t(RecData,23);
			}	 
			Buf[1]=0x80;
			Buf[Len-2]=CRC8(Buf,Len-2);
			Buf[Len-1]=0x16;
			UART_3(Buf,Len);
        }

        break;
    }
    case 0x2A://更改一台管理表具
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {
            Change_One_Meter(Buf,Len);
        }
        break;
    }
    case 0x2B: //删除一台管理表具
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {
            Delete_One_Meter(Buf,Len);
        }

        break;
    }
    case 0x2C: //删除全部管理表具
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {
            Delete_All_Meter(Buf,Len);
        }

        break;
    }
    case 0x31 ://更改采集器地址
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {
            Change_CJQAddr(Buf,Len);
        }
        break;
    }
    case 0x32:
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
		CJQ_NetAddr_Alter(Buf,Len);
        //更改采集器地址中的网络地址编码（A4A5）
        break;
    }
    case 0x33://初始化采集器
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
		else
		{
			Init_CJQ(Buf,Len);
		}	
        break;
    }
	case 0x40:	  //写价格表	  hu
	{  	       
		 CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {
	
		Write_Price_List(Buf,Len);
		
		}
		break;
	}
	case 0x41:	  //写结算日
	{
	    CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {	
			Settlement_Date(Buf,Len);		
		}
		break;
	}
	case 0x42:	 //写抄表日
	{
	    CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {
            return 0;
        }
        else
        {		
	   		Meter_Reading(Buf,Len);
		}
		break;
	}
	case 0x43:	 //写购入金额（购买量）
	{
	     CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {
            return 0;
        }
        else
        {
		Meter_WR_Purchase_Amount(Buf,Len);
		}
		break;
	}
	case 0x44:	  //写新密钥
	{
	    CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {

		
		Meter_New_Key(Buf,Len);
		
		}
		break;
	}
	case 0x45: //写标准时间
    {

        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {

           Write_Meter_RTC_FLAG(Buf,Len);
        }
        break;
    }
    case 0x49:	  //写机电同步数据
    {
	      CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {

        Write_Ele_and_Mechan_Syn_Data(Buf,Len);
        break;
		}
    }
	case 0x4A:		//写剩余金额（剩余量）
	{
		CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {


		Meter_WR_Remaining_Amount(Buf,Len);
		
		}
		break;
	}
    case 0x61:
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {
            return 0;
        }
        else
        {
            CJQ_Status_Read(Buf,Len);
        }
		break;
    }
	    case 0x70:
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        CJQ_Wireless_NetType_Read(Buf,Len);//读取网络类型（用那个厂家的无线模块及组网形式），配合设置一起做
        break;
    }
    case 0x71:
    {
        Read_CJQAddr(Buf,Len);//读取网络（子网）地址（读A4A5）
        break;
    }
	    case 0x74:
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        Back_Wireless_Parameter(Buf,Len);//读取子节点参数 无线模块的参数）
        break;
    }
    case 0x75:
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        Back_Wireless_Addr(Buf,Len);//读取子节点参数 无线模块的参数）
        break;
    }

    case 0x77 : //读采集器工作模式
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {
            Read_CJQ_Work_Mode(Buf,Len);
        }
        break;
    }
    case 0x78 : //读采集器抄表轮询时间间隔
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {
            Read_CJQ_Time_Interval(Buf,Len);
        }
        break;
    }
	case 0x79:
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {
            return 0;
        }
        else
        {
            CJQ_RTC_Read(Buf,Len);
        }
    }
	case 0x7A:
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        CJQ_Wireless_NetType_Set(Buf,Len);
        //设置网络类型（用那个厂家的无线模块及组网形式），配合设置一起做
        break;
    }
	case 0x7d:
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
		CJQ_Addr_Alter(Buf,Len);
        //更改采集器地址
        break;
    }
    case 0x80:
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        Set_Wireless_Parameter(Buf,Len);
        break;
    }
    case 0x81:
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        Set_Wireless_Addr(Buf,Len);
        break;
    }
    case 0x82 : //更改采集器工作模式
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {
            Change_CJQ_Work_Mode(Buf,Len);
        }
        break;
    }
    case 0x83 : //更改采集器抄表轮询时间间隔
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {
            Change_CJQ_Time_Interval(Buf,Len);
        }
        break;
    }
    case 0x84:
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {
            return 0;
        }
        else
        {
            CJQ_RTC_Set(Buf,Len);
        }
        break;
    }

    case 0x85: //网络同步（校时）
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {
            return 0;
        }
        else
        {
            CJQ_RTC_Set(Buf,Len);
            Write_Meter_RTC_FLAG(Buf,Len);
        }
        break;
    }
	case 0x86://读所有表具数据
    {
		CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {
            return 0;
        }
		Read_All_Meter_Data(Buf,Len);
        break;
    }
    default:												
        break;
    }
    return 0;
}
/**表具返回错误帧组帧上传***/
u8  Meter_Up_Err_FrameFormat(u8 *DataBuf,u16 DataLen)
{
	    u8 RecData[28],Read_Cjq_ID[8];
		if(DataLen<16)
		return 1;

        RecData[0]=0x68;
        RecData[1]=0xC0;    //组协议头

        SPI_Flash_Read(EXT_CJQ_PARAMETER_PAGEADDR,Read_Cjq_ID,8);  //读外部falsh采集器的地址
		memcpy(RecData+2,Read_Cjq_ID,8); //组采集器地址
		memcpy(RecData+10,DataBuf,10);  //装载 68H	T	Addr   C

        RecData[20]=0x01;
		RecData[21]=CheckCodeErr;

        RecData[22]=CRC8(RecData,22);
        RecData[23]=0x16;


        UART_3(RecData,24);//上传到集中器
		return 0;
	
}
 
/*******************************************************************************
* Function Name  : Meter_Up_FrameFormat_NO_Data	
* Description    : 表具上传的数据
* Input          : DataBuf：表具上传数据，DataLen：数据长度
* Output         : None
* Return         : None
*******************************************************************************/
void Meter_Up_FrameFormat_NO_Data(u8 *DataBuf,u16 DataLen)
{
    u8 m=0,n=0,CS,RecData[53],Read_Cjq_ID[8];
   	CS=Add_Up(DataBuf,DataLen-2);
	if(((DataBuf[DataLen-2]==CS)&&(DataBuf[9]==0x84))||((DataBuf[DataLen-2]==CS)&&(DataBuf[9]==0x96)))
	 	{
				        RecData[0]=0x68;
				       	RecData[1]=0x80;    //组协议头
				
				        SPI_Flash_Read(EXT_CJQ_PARAMETER_PAGEADDR,Read_Cjq_ID,8);  //读外部falsh采集器的地址
				        for(m=0; m<8; m++)
				            RecData[m+2]=Read_Cjq_ID[m];	   //组采集器地址
				
				        for(n=0; n<9; n++)
				           RecData[10+n]=DataBuf[n];  //装载 68H	T	Addr
		
				        if((DataBuf[11]==0x17)&&(DataBuf[12]==0xA0) )	 //开关阀
				        {
				            RecData[19]=0x21;
				            RecData[20]=0x01;	  
				
				            if(DataBuf[DataLen-4]==0x80)
				                RecData[21]=0x55;
				            else if(DataBuf[DataLen-4]==0x81)
				                RecData[21]=0x99;
				
				            RecData[22]=CRC8(RecData,22);
				            RecData[23]=0x16;
				
				            UART_3(RecData,24);//上传到集中器
				        }
						
				        else if((DataBuf[11]==0x16)&&(DataBuf[12]==0xA0) )	  //机电
				        {
				            RecData[19]=0x49;
				            RecData[20]=0x05;
				
//				            for(l=0; l<5; l++)
//				                RecData[21+l]=DataBuf[14+l];
							memset(&RecData[21],0,5);	  //?????临时为00000
				
				            RecData[26]=CRC8(RecData,26);
				            RecData[27]=0x16;
				
				            UART_3(RecData,28);//上传到集中器
				        }
						 else if((DataBuf[11]==0x10)&&(DataBuf[12]==0xA0))		//写价格表
						 {
						 	RecData[19]=0x40;
				            RecData[20]=0x30;	  
				
							memset(&RecData[21],0,30);	  //?????临时为00000
				
				            RecData[51]=CRC8(RecData,51);
				            RecData[52]=0x16;
				
				            UART_3(RecData,53);//上传到集中器
						 	
						 }
	    		} 

}

/**表具上发协议组帧***/
 u8  Meter_Up_FrameFormat(u8 *DataBuf,u16 DataLen)	//hu
{
    	u8 RecData[64],Read_Cjq_ID[8],i;
		if(DataLen<16)
		return 1;

        RecData[0]=0x68;
        RecData[1]=0x80;    //组协议头

        SPI_Flash_Read(EXT_CJQ_PARAMETER_PAGEADDR,Read_Cjq_ID,8);  //读外部falsh采集器的地址
		memcpy(RecData+2,Read_Cjq_ID,8); //组采集器地址
		memcpy(RecData+10,DataBuf,10);  //装载 68H	T	Addr   C

        RecData[20]=DataBuf[10]-3;
		for(i=0;i<(DataBuf[10]-3);i++)
        	RecData[21+i]=DataBuf[11+i];	 

        RecData[21+i]=CRC8(RecData,21+i);
        RecData[22+i]=0x16;


        UART_3(RecData,24+i);//上传到集中器
		return 0;

}
/*******************************************************************************
* Function Name  : Meter_Up_FrameFormat_NO_Data
* Description    : 
* Input          : Buf：数据数组，Len：数据长度
* Output         : None
* Return         : None
*******************************************************************************/
void Meter_Up_FrameFormat_NO_Data_NO_ST(u8 *pMeterDataRecv,u16 G_RecvDataLen) //组包
{
    extern u8 G_UART_Num;
	u8 SendUpFrameFormat[30];
    u8 tmp[8];
	if(G_RecvDataLen!=16)
    {
        METER_Err_Ack(pMeterDataRecv,CmdErr);
        return;
    }
    SPI_Flash_Read(EXT_CJQ_PARAMETER_PAGEADDR,tmp,8);
    SendUpFrameFormat[0]=0x68;
    SendUpFrameFormat[1]=0X80;
	memcpy(SendUpFrameFormat+2,tmp,8);
	memcpy(SendUpFrameFormat+10,pMeterDataRecv,9);
	switch(pMeterDataRecv[11])	
	{
		case 0x11:	 //写结算日
		 	SendUpFrameFormat[19]=0x41;
			SendUpFrameFormat[20]=0x01;
			SendUpFrameFormat[21]=0x00; //????日期暂为0
			SendUpFrameFormat[22]=CRC8(SendUpFrameFormat,22);
		    SendUpFrameFormat[23]=0x16;
		    Send_UART(G_UART_Num,SendUpFrameFormat,24);
		    break;
		case 0x12:	//写抄表日  
		 	SendUpFrameFormat[19]=0x42;
			SendUpFrameFormat[20]=0x01;
			SendUpFrameFormat[21]=0x00; //????日期暂为0
			SendUpFrameFormat[22]=CRC8(SendUpFrameFormat,22);
		    SendUpFrameFormat[23]=0x16;
		    Send_UART(G_UART_Num,SendUpFrameFormat,24);
		    break;
	//	case 0x15:	 //写标准时间？？？？？网络同步校时
	//	 	SendUpFrameFormat[19]=0x41;
	//		SendUpFrameFormat[20]=0x01;
//		    P8563_gettime(tmp);
//			exchange(tmp,7);
//			memcpy(SendUpFrameFormat+21,tmp,7);
//		    SendUpFrameFormat[28]=CRC8(SendUpFrameFormat,28);
//		    SendUpFrameFormat[29]=0x16;
//		    Send_UART(G_UART_Num,SendUpFrameFormat,30);
	//	    break;
	}

}
/****表返回数据***/
u8  _Meter_Up_Protocol_Analysis(u8 *Buf,u16 Len)		//hu
{
	if(Len<16)
		return 1;
	switch(Buf[9])
	{
		case 0x81:
		{
			switch(Buf[11])
			{
				case 0x1F:	   //读当前计量数据..	主动方式	  
				{
					if(Len!=35)
					  return 1;
					Buf[9]=	0x01;
					Meter_Up_FrameFormat(Buf,Len);
					break;	
				}
				case 0x02:	   //读价格表  ..
				{
					if(Len!=46)
					  return 1;
					Buf[9]=	0x0B;
					Meter_Up_FrameFormat(Buf,Len);
					break;					
				}
				case 0x05:	   //读购入金额	 . .
				{
					if(Len!=34)
					  return 1;
					Buf[9]=	0x0E;
					Meter_Up_FrameFormat(Buf,Len);
					break;					
				}
				case 0x0A:	   //读地址	？？？？  B C协议没有
				{
					if(Len!=16)
					  return 1;
					Buf[9]=	0x07;
					Meter_Up_FrameFormat(Buf,Len);
					break;					
				}
				default:
				{
					break;
				}
			}
		}

		case (0x84):
		{
			switch(Buf[11])
			{
				case 0x10:	   //写价格表..		   有异常返回
				{
					if(Len!=18)
					  return 1;
						Meter_Up_FrameFormat_NO_Data(Buf,Len);
					break;	
				}
				case 0x11:	   //写结算日 ..
				{
					if(Len!=16)
					  return 1;
					Meter_Up_FrameFormat_NO_Data_NO_ST(Buf,Len);
					break;					
				}
				case 0x12:	   //写抄表日  ..
				{
					if(Len!=16)
					  return 1;
					Meter_Up_FrameFormat_NO_Data_NO_ST(Buf,Len);
					break;					
				}
				case 0x13:	   //写购入金额	.. 
				{
					if(Len!=25)
					  return 1;
					Buf[9]=	0x43;
					Meter_Up_FrameFormat(Buf,Len);
					break;					
				}
				case 0x14:	   //写新密钥..	
				{
					if(Len!=17)
					  return 1;
					Buf[9]=	0x44;
					Meter_Up_FrameFormat(Buf,Len);
					break;					
				}
				case 0x15:	   //写标准时间	 ?????? 网络校时
			    {
//					if(Len!=16)
//					  return 1;
//					Buf[9]=	0x45;
//					Meter_Up_FrameFormat(Buf,Len);
					Write_Meter_RTC_ACK(Buf,Len);
					break;										
				}
				case 0x17:	   //写阀门控制	 
				{
					if(Len!=18)
					  return 1;
//					Buf[9]=	0x21;
//
//					Meter_Up_FrameFormat(Buf,Len);
					 Meter_Up_FrameFormat_NO_Data(Buf,Len);
					break;						
				}
				case 0x19:	   //仪表出厂启用	有异常返回	  ？？？？？暂时B、C没有
				{
					break;					
				}
				default:
				{
					break;
				}
			}
		break;
		}
		case 0x15:   //写地址
		{
	
			break;
		}
		case 0x96:	 //	 写机电同步数据
		{
			if(Len!=18)
				 return 1;
//			Buf[9]=	0x49;
//			memset(&Buf[11],0,5);//????? 需日后解决
//			Meter_Up_FrameFormat(Buf,Len);
			Meter_Up_FrameFormat_NO_Data(Buf,Len);
			break;
		}
		case 0xAC:	//写仪表基本参数	？？？？？暂时B、C没有
		{

			if(Len<16)
				 return 1;
			Meter_Up_FrameFormat(Buf,Len);
			
			break;
		}
		case 0xAE:	  //仪表初始化	？？？？？	暂时B、C没有
		{
		   	if(Len!=18)
				 return 1;
		
			Meter_Up_FrameFormat(Buf,Len);

			break;
		}
		case 0xB4:	  //写剩余金额	.. 
		{
			if(Len!=20)
				 return 1;
			Buf[9]=	0x4A;
			Meter_Up_FrameFormat(Buf,Len);
			break;
		}
		case 0xAF:	//写预付费方式命令 ？？？？？ 暂时B、C没有
		{

			if(Len!=19)
				 return 1;
			Meter_Up_FrameFormat(Buf,Len);

			break;  
		}
	    case 0xC1://当前计量数据
		case 0xC4: //写价格表
		case 0xD6://机电
		case 0xC3:
		case 0xEC:
		case 0xEE:
		case 0xEF:
		Meter_Up_Err_FrameFormat(Buf,Len);	
		default:
        break;
	}
	return 0;
}
u8  Meter_Protocol_Analysis(u8 *Buf,u16 Len)
{
	if(Len<16)
		return 1;
	switch(Buf[9])
	{
		case 0x04:
		break;
		default:
        break;
	}
	return 0;
}

