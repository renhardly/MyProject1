#include <stdio.h>
#include "platform_config.h"
#include <InterFlash.h>
#include "stm32f10x_flash.h"
#include "ErrCode.h"
#include "spi.h"
/**********************************************************************************
* Function Name  : Switch on/off Valve
* Description    : 写开关阀
* Input          : Buf：“  ”帧，Len：指令长度
* Output         : None
* Return         : return 1：操作成功，return 0：操作失败，
***********************************************************************************/
u32 G_Point[METER_HEAP_PSIZE];
void Switch_Valve(u8 *Buf,u16 Len)
{
    u16 i;
	u8 index;
	if(Len!=24)
	{
		Err_Ack(Buf,CmdErr);
		return;
	} 
	index=Return_Meter_Index(Buf+11);
	for(i=0;i<METER_HEAP_PSIZE;i++)
	{
	 	if(G_Point[i]==0)
		{
			CMD_CONTRL *p=malloc(sizeof(CMD_CONTRL));
			memset(p,0,sizeof(CMD_CONTRL));
			p->data_p=malloc(1);
			p->data_p[0]=Buf[Len-3];
			p->type=METER_VALVE_SET;
			p->cmd_index[index/16]=1<<(index%16);
			p->counter_down=25;
			G_Point[i]=	(u32)p;
			break;
			
		}	
	}
}
 /**开关阀下发命令***/
void _Switch_Valve(u8 *meteradd,u8 *data,u8 len)
{
    u8 tmp[17];
	tmp[0]=0x68;
	memcpy(tmp+1,meteradd,8);

    tmp[9]=0x04;
    tmp[10]=0x04;
    tmp[11]=0x17;
    tmp[12]=0xa0;
    tmp[13]=0x00;
	tmp[14]=*data;
    tmp[15]=Add_Up(tmp,15);
    tmp[16]=0x16;
    UART_5(tmp,17);
}
/**********************************************************************************
* Function Name  : Write_Ele_and_Mechan_Syn_Data
* Description    : 写机电同步数据
* Input          : Buf：“ ”帧，Len：指令长度
* Output         : None
* Return         : None
***********************************************************************************/
void Write_Ele_and_Mechan_Syn_Data(u8 *Buf,u16 Len)
{
    u16 i;
	u8 index;
	if(Len!=28)
	{
		Err_Ack(Buf,CmdErr);
		return;
	}
	index=Return_Meter_Index(Buf+11);
	for(i=0;i<METER_HEAP_PSIZE;i++)
	{
	 	if(G_Point[i]==0)
		{
			CMD_CONTRL *p=malloc(sizeof(CMD_CONTRL));
			memset(p,0,sizeof(CMD_CONTRL));
			p->data_p=malloc(5);
			p->data_p[0]=Buf[Len-7];
			p->data_p[1]=Buf[Len-6];
			p->data_p[2]=Buf[Len-5];
			p->data_p[3]=Buf[Len-4];
			p->data_p[4]=Buf[Len-3];
			p->type=METER_ELCSYN_SET;
			p->cmd_index[index/16]=1<<(index%16);
			p->counter_down=25;
			G_Point[i]=	(u32)p;
			break;
		}	
	}
}
/**机电发命令***/
void _Write_Ele_and_Mechan_Syn_Data(u8 *meteradd,u8 *data,u8 len)
{
    u8 tmp[21];
	tmp[0]=0x68;
	memcpy(tmp+1,meteradd,8);

    tmp[9]=0x16;
    tmp[10]=0x08;
    tmp[11]=0x16;
    tmp[12]=0xa0;
    tmp[13]=0x00;

	memcpy(&tmp[14],data,len);
    tmp[14+len]=Add_Up(tmp,14+len);
    tmp[15+len]=0x16;
    UART_5(tmp,16+len);
}
/*******写价格表*************/
void Write_Price_List(u8 *Buf,u16 Len)
{
    u16 i;
	u8 index;
	if(Len<23)
	{
		Err_Ack(Buf,CmdErr);
		return;
	} 
	index=Return_Meter_Index(Buf+11); //????
	for(i=0;i<METER_HEAP_PSIZE;i++)
	{
	 	if(G_Point[i]==0)
		{
			CMD_CONTRL *p=malloc(sizeof(CMD_CONTRL));
			memset(p,0,sizeof(CMD_CONTRL));
			p->data_p=malloc(Len-23);
			p->data_p[0]=Buf[21];
			p->type=METER_Write_Price_SET;
			p->cmd_index[index/16]=1<<(index%16);
			p->counter_down=25;
			G_Point[i]=	(u32)p;
			break;
		}	
	}
}
/**写价格表组帧**/
void _Write_Price_List(u8 *meteradd,u8 *data,u8 len)
{
    u8 tmp[47];
	tmp[0]=0x68;
	memcpy(tmp+1,meteradd,8);

    tmp[9]=0x04;
    tmp[10]=0x1E;
    tmp[11]=0x10;
    tmp[12]=0xa0;
    tmp[13]=0x00;

	memcpy(&tmp[14],data,len);
    tmp[15+len]=Add_Up(tmp,15+len);
    tmp[16+len]=0x16;
    UART_5(tmp,17+len);
}
/*******结算日写*************/
void Settlement_Date(u8 *Buf,u16 Len)
{
    u16 i;
	u8 index;
	if(Len<23)
	{
		Err_Ack(Buf,CmdErr);
		return;
	} 
	index=Return_Meter_Index(Buf+11); //????
	for(i=0;i<METER_HEAP_PSIZE;i++)
	{
	 	if(G_Point[i]==0)
		{
			CMD_CONTRL *p=malloc(sizeof(CMD_CONTRL));
			memset(p,0,sizeof(CMD_CONTRL));
			p->data_p=malloc(Len-23);
			p->data_p[0]=Buf[21];
			p->type=METER_Settlement_Date_SET;
			p->cmd_index[index/16]=1<<(index%16);
			p->counter_down=25;
			G_Point[i]=	(u32)p;
			break;
		}	
	}
}
/**写结算日组帧**/
void _Settlement_Date(u8 *meteradd,u8 *data,u8 len)
{
    u8 tmp[20];
	tmp[0]=0x68;
	memcpy(tmp+1,meteradd,8);

    tmp[9]=0x04;
    tmp[10]=0x04;
    tmp[11]=0x11;
    tmp[12]=0xa0;
    tmp[13]=0x00;

	memcpy(&tmp[14],data,len);
    tmp[15+len]=Add_Up(tmp,15+len);
    tmp[16+len]=0x16;
    UART_5(tmp,17+len);
}
/*******写抄表日*************/
void Meter_Reading(u8 *Buf,u16 Len)
{
    u16 i;
	u8 index;
	if(Len<23)
	{
		Err_Ack(Buf,CmdErr);
		return;
	} 
	index=Return_Meter_Index(Buf+11); //????
	for(i=0;i<METER_HEAP_PSIZE;i++)
	{
	 	if(G_Point[i]==0)
		{
			CMD_CONTRL *p=malloc(sizeof(CMD_CONTRL));
			memset(p,0,sizeof(CMD_CONTRL));
			p->data_p=malloc(Len-23);
			p->data_p[0]=Buf[21];
			p->type=METER_Meter_Reading_SET;
			p->cmd_index[index/16]=1<<(index%16);
			p->counter_down=25;
			G_Point[i]=	(u32)p;
			break;
		}	
	}
}
void _Meter_Reading(u8 *meteradd,u8 *data,u8 len)
{
    u8 tmp[20];
	tmp[0]=0x68;
	memcpy(tmp+1,meteradd,8);

    tmp[9]=0x04;
    tmp[10]=0x04;
    tmp[11]=0x12;
    tmp[12]=0xa0;
    tmp[13]=0x00;

	memcpy(&tmp[14],data,len);
    tmp[15+len]=Add_Up(tmp,15+len);
    tmp[16+len]=0x16;
    UART_5(tmp,17+len);
}
/*******写购入金额*************/
void Meter_WR_Purchase_Amount(u8 *Buf,u16 Len)
{
    u16 i;
	u8 index;
	if(Len<23)
	{
		Err_Ack(Buf,CmdErr);
		return;
	} 
	index=Return_Meter_Index(Buf+11); //????
	for(i=0;i<METER_HEAP_PSIZE;i++)
	{
	 	if(G_Point[i]==0)
		{
			CMD_CONTRL *p=malloc(sizeof(CMD_CONTRL));
			memset(p,0,sizeof(CMD_CONTRL));
			p->data_p=malloc(Len-23);
			p->data_p[0]=Buf[21];
			p->type=METER_WR_Purchase_Amount_SET;
			p->cmd_index[index/16]=1<<(index%16);
			p->counter_down=25;
			G_Point[i]=	(u32)p;
			break;
		}	
	}
}
/**购入金额组帧**/
void _Meter_WR_Purchase_Amount(u8 *meteradd,u8 *data,u8 len)
{
    u8 tmp[28];
	tmp[0]=0x68;
	memcpy(tmp+1,meteradd,8);

    tmp[9]=0x04;
    tmp[10]=0x0C;
    tmp[11]=0x13;
    tmp[12]=0xa0;
    tmp[13]=0x00;

	memcpy(&tmp[14],data,len);
    tmp[15+len]=Add_Up(tmp,15+len);
    tmp[16+len]=0x16;
    UART_5(tmp,17+len);
}
/*******写新密钥*************/
void Meter_New_Key(u8 *Buf,u16 Len)
{
    u16 i;
	u8 index;
	if(Len<23)
	{
		Err_Ack(Buf,CmdErr);
		return;
	} 
	index=Return_Meter_Index(Buf+11); //????
	for(i=0;i<METER_HEAP_PSIZE;i++)
	{
	 	if(G_Point[i]==0)
		{
			CMD_CONTRL *p=malloc(sizeof(CMD_CONTRL));
			memset(p,0,sizeof(CMD_CONTRL));
			p->data_p=malloc(Len-23);
			p->data_p[0]=Buf[21];
			p->type=METER_New_Key_SET;
			p->cmd_index[index/16]=1<<(index%16);
			p->counter_down=25;
			G_Point[i]=	(u32)p;
			break;
		}	
	}
}
/**写新密钥组帧**/
void _Meter_New_Key(u8 *meteradd,u8 *data,u8 len)
{
    u8 tmp[28];
	tmp[0]=0x68;
	memcpy(tmp+1,meteradd,8);

    tmp[9]=0x04;
    tmp[10]=0x0C;
    tmp[11]=0x14;
    tmp[12]=0xa0;
    tmp[13]=0x00;

	memcpy(&tmp[14],data,len);
    tmp[15+len]=Add_Up(tmp,15+len);
    tmp[16+len]=0x16;
    UART_5(tmp,17+len);
}
/*******写剩余金额*************/
void Meter_WR_Remaining_Amount(u8 *Buf,u16 Len)
{
    u16 i;
	u8 index;
	if(Len<23)
	{
		Err_Ack(Buf,CmdErr);
		return;
	} 
	index=Return_Meter_Index(Buf+11); //????
	for(i=0;i<METER_HEAP_PSIZE;i++)
	{
	 	if(G_Point[i]==0)
		{
			CMD_CONTRL *p=malloc(sizeof(CMD_CONTRL));
			memset(p,0,sizeof(CMD_CONTRL));
			p->data_p=malloc(Len-23);
			p->data_p[0]=Buf[21];
			p->type=Meter_WR_Remaining_Amount_SET;
			p->cmd_index[index/16]=1<<(index%16);
			p->counter_down=25;
			G_Point[i]=	(u32)p;
			break;
		}	
	}
}
/**写剩余金额组帧**/
void _Meter_WR_Remaining_Amount(u8 *meteradd,u8 *data,u8 len)
{
    u8 tmp[23];
	tmp[0]=0x68;
	memcpy(tmp+1,meteradd,8);

    tmp[9]=0x34;
    tmp[10]=0x07;
    tmp[11]=0x34;
    tmp[12]=0xa0;
    tmp[13]=0x00;

	memcpy(&tmp[14],data,len);
    tmp[15+len]=Add_Up(tmp,15+len);
    tmp[16+len]=0x16;
    UART_5(tmp,17+len);
}
/*******读价格表*************/
void Meter_Read_Price_List(u8 *Buf,u16 Len)
{
    u16 i;
	u8 index;
	if(Len<23)
	{
		Err_Ack(Buf,CmdErr);
		return;
	} 
	index=Return_Meter_Index(Buf+11); //????
	for(i=0;i<METER_HEAP_PSIZE;i++)
	{
	 	if(G_Point[i]==0)
		{
			CMD_CONTRL *p=malloc(sizeof(CMD_CONTRL));
			memset(p,0,sizeof(CMD_CONTRL));
			p->data_p=malloc(Len-23);
			p->data_p[0]=Buf[21];
			p->type=Meter_Read_Price_List_SET;
			p->cmd_index[index/16]=1<<(index%16);
			p->counter_down=25;
			G_Point[i]=	(u32)p;
			break;
		}	
	}
}
/**读表价格组帧**/
void _Meter_Read_Price_List(u8 *meteradd,u8 *data,u8 len)
{
    u8 tmp[19];
	tmp[0]=0x68;
	memcpy(tmp+1,meteradd,8);

    tmp[9]=0x01;
    tmp[10]=0x03;
    tmp[11]=0x02;
    tmp[12]=0x81;
    tmp[13]=0x00;

	memcpy(&tmp[14],data,len);
    tmp[15+len]=Add_Up(tmp,15+len);
    tmp[16+len]=0x16;
    UART_5(tmp,17+len);
}
/*******读购入金额*************/
void Meter_RD_Purchase_Amount(u8 *Buf,u16 Len)
{
    u16 i;
	u8 index;
	if(Len<23)
	{
		Err_Ack(Buf,CmdErr);
		return;
	} 
	index=Return_Meter_Index(Buf+11); 
	for(i=0;i<METER_HEAP_PSIZE;i++)
	{
	 	if(G_Point[i]==0)
		{
			CMD_CONTRL *p=malloc(sizeof(CMD_CONTRL));
			memset(p,0,sizeof(CMD_CONTRL));
			p->data_p=malloc(Len-23);
			p->data_p[0]=Buf[21];
			p->type=Meter_RD_Purchase_Amount_SET;
			p->cmd_index[index/16]=1<<(index%16);
			p->counter_down=25;
			G_Point[i]=	(u32)p;
			break;
		}	
	}
}
/**读购入金额组帧**/
void _Meter_RD_Purchase_Amount(u8 *meteradd,u8 *data,u8 len)
{
    u8 tmp[19];
	tmp[0]=0x68;
	memcpy(tmp+1,meteradd,8);

    tmp[9]=0x01;
    tmp[10]=0x03;
    tmp[11]=0x05;
    tmp[12]=0x81;
    tmp[13]=0x00;

	memcpy(&tmp[14],data,len);
    tmp[15+len]=Add_Up(tmp,15+len);
    tmp[16+len]=0x16;
    UART_5(tmp,17+len);
}
/*******读表地址*************/
void Meter_RD_ADDR(u8 *Buf,u16 Len)
{
    u16 i;
	u8 index;
	if(Len<23)
	{
		Err_Ack(Buf,CmdErr);
		return;
	} 
	index=Return_Meter_Index(Buf+11); 
	for(i=0;i<METER_HEAP_PSIZE;i++)
	{
	 	if(G_Point[i]==0)
		{
			CMD_CONTRL *p=malloc(sizeof(CMD_CONTRL));
			memset(p,0,sizeof(CMD_CONTRL));
			p->data_p=malloc(Len-23);
			p->data_p[0]=Buf[21];
			p->type=Meter_RD_ADDR_SET;
			p->cmd_index[index/16]=1<<(index%16);
			p->counter_down=25;
			G_Point[i]=	(u32)p;
			break;
		}	
	}
}
/**读表地址组帧**/
void _Meter_RD_ADDR(u8 *meteradd,u8 *data,u8 len)
{
    u8 tmp[19];
	tmp[0]=0x68;
	memcpy(tmp+1,meteradd,8);

    tmp[9]=0x03;
    tmp[10]=0x03;
    tmp[11]=0x0a;
    tmp[12]=0x81;
    tmp[13]=0x00;

	memcpy(&tmp[14],data,len);
    tmp[15+len]=Add_Up(tmp,15+len);
    tmp[16+len]=0x16;
    UART_5(tmp,17+len);
}
/*******************************************************************************
* Function Name  : Up_Meter_Data	（暂为开关阀  电机数据  上传）
* Description    : 表具上传的数据
* Input          : DataBuf：表具上传数据，DataLen：数据长度
* Output         : None
* Return         : None
*******************************************************************************/
//void Up_Meter_Data(u8 *DataBuf,u16 DataLen)
//{
//    u8 RecData[28],Read_Cjq_ID[8];
////	if(Len!=28)?????
////	{
////		METER_Err_Ack(DataBuf,IllegalDataFormat);
////		return;
////	}
//    if((DataBuf[9]==0x84)||(DataBuf[9]==0x96))
//    {
//        RecData[0]=0x68;
//        RecData[1]=0x80;    //组协议头
//
//        SPI_Flash_Read(EXT_CJQ_PARAMETER_PAGEADDR,Read_Cjq_ID,8);  //读外部falsh采集器的地址
//		memcpy(RecData+2,Read_Cjq_ID,8); //组采集器地址
//		memcpy(RecData+10,DataBuf,9);  //装载 68H	T	Addr
//        if((DataBuf[11]==0x17)&&(DataBuf[12]==0xA0) )
//        {
//            RecData[19]=0x21;
//            RecData[20]=0x01;
//
//            if(DataBuf[DataLen-4]==0x80)
//                RecData[21]=0x55;
//            else if(DataBuf[DataLen-4]==0x81)
//                RecData[21]=0x99;
//
//            RecData[22]=CRC8(RecData,22);
//            RecData[23]=0x16;
//
//            UART_3(RecData,24);//上传到集中器
//        }
//        else if((DataBuf[11]==0x16)&&(DataBuf[12]==0xA0) )
//        {
//            RecData[19]=0x49;
//            RecData[20]=0x05;
//			memcpy(RecData+21,DataBuf+14,5);
//            RecData[26]=CRC8(RecData,26);
//            RecData[27]=0x16;
//            UART_3(RecData,28);//上传到集中器
//        }
//    }
//
//}
/*******************************************************************************
* Function Name  : Store_Msg_Send
* Description    : 组合上发命令，用于ZJQ数据存储
* Input          : DataBuf：表具上传数据，CMDaBuf：表具上传命令
* Output         : None
* Return         : None
*******************************************************************************/
void Store_Msg_Send(u8 *DataBuf,u8 *CMDaBuf)
{
	u8 tmp[42];
	tmp[0]=0x68;
	tmp[1]=0x80;
	memcpy(tmp+2,G_CJQ.ADDR,8);
	memcpy(tmp+10,CMDaBuf,9);
	tmp[19]=0x87;//自定义协议,仅用于采集器和集中器间的通讯
	tmp[20]=0x13;
	memcpy(tmp+21,DataBuf,19);
	tmp[40]=CRC8(tmp,40);
	tmp[41]=0x16;
	UART_3(tmp,42);

}
/*******************************************************************************
* Function Name  : Up_Meter_Auto_Data
* Description    : 存储表具主动上传的数据，并置表通信标志位-刘勇
* Input          : DataBuf：表具上传数据，DataLen：数据长度
* Output         : None
* Return         : None
*******************************************************************************/

u8 G_Meter_Comm_Status[32];//表通信状态标志位，通信异常时按表号索引的序号对相应位置1-刘勇
void Up_Meter_Auto_Data(u8 *DataBuf,u16 DataLen)
{
    u8 i=0,MeterCheck,IndexBuf[256];		//将表具上传的数据存到地址6中
    u8 tmp[40];
	u8 rtc_tmp1[7]={0};
    MeterCheck=CRC8(&DataBuf[2],7);							 //计算上传表具索引值
    SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,IndexBuf,256);
    for(i=0; i<255; i++)
    {
		if(0xFF==IndexBuf[i])//无效的表号认为其通信正常-刘勇
		{
			//通信正常，相应的表通信标志位置0-刘勇
			G_Meter_Comm_Status[i/8] &= ~(0x01<<(i%8));	
		}
        if(MeterCheck==IndexBuf[i])
        {
            u8 MeterID[8];
			SPI_Flash_Read(EXT_METER_NUM_PAGEADDR+i*8,MeterID,8);
            if(memcmp(MeterID,DataBuf+1,8)==0)     //ID号相等
            {
				P8563_gettime(rtc_tmp1);//获取实时时间	  hu

                if(DataBuf[9]==0xB5)	//hu
				{
	                memcpy(tmp,DataBuf+14,5);
					memset(tmp+5,0xff,5);
					memcpy(tmp+10,rtc_tmp1,7);  //hu
	                memcpy(tmp+17,DataBuf+19,2);
	                SPI_Flash_Write(EXT_METER_DATA_PAGEADDR+((i/2)*4096)+((i%2)*PAGESIZE/2),tmp,19);//写入flash
	                WriteFlash_Page(METER_DATA_PAGEADDR-((i/2)*2048)+((i%2)*PAGESIZE/2),0,tmp,19);//将表具信息写入到flash中存表具数据的相应位置
	             //   Store_Msg_Send(tmp,DataBuf);
				    break;
				}
				else if(DataBuf[9]==0x01)  //hu
				{
	                memcpy(tmp,DataBuf+14,5);
					memcpy(tmp+5,DataBuf+14,5);
					memcpy(tmp+10,rtc_tmp1,7);  //hu
	                memcpy(tmp+17,DataBuf+19,2);
	                SPI_Flash_Write(EXT_METER_DATA_PAGEADDR+((i/2)*4096)+((i%2)*PAGESIZE/2),tmp,19);//写入flash
	                WriteFlash_Page(METER_DATA_PAGEADDR-((i/2)*2048)+((i%2)*PAGESIZE/2),0,tmp,19);//将表具信息写入到flash中存表具数据的相应位置
				//	Store_Msg_Send(tmp,DataBuf);
					break;	
				}
				//通信正常，相应的表通信标志位置0-刘勇
				G_Meter_Comm_Status[i/8] &= ~(0x01<<(i%8));	
				break;
            }

        }
    }
	if(i==255)
	{
		METER_Err_Ack(DataBuf,NotFindMeter);//?????控制码
	}
	
}


