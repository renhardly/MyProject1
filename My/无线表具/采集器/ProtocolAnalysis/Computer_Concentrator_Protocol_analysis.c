#include "platform_config.h"
#include <InterFlash.h>
#include "ErrCode.h"
/*******************************************************************************
* Function Name  : Computer_Concentrator_Protocol_Analysis
* Description    : ���������ֳֻ�������ָ�������Ӧ������
* Input          : Buf����λ�����͵����ݣ�Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
extern u32 G_Point[METER_HEAP_PSIZE];
 u8 G_Meter_BS0,G_Meter_BS1;
 u8 Up_flag=0;//������ӱ�߱��
u8  Computer_Concentrator_Protocol_Analysis(u8 *Buf,u16 Len)
{
    u8 CJQ_Check=0;
	if(memcmp(G_CJQ.ADDR,Buf+6,2)!=0)
		G_status.ST25=1;
    switch(Buf[19])
    {
    case 0x01://����ֻ�������
    {
        CJQ_Check=CJQ_Addr_Check(Buf); // ���ɼ�����ַ�Ƿ���ȷ
        if(CJQ_Check==0)
        {
            return 0;
        }
		 else
        {
            Read_One_Meter_Data(Buf,Len);//�������ɼ����еı�����
        }
        break;
    }
	case 0x02://�����б������
    {
        delay_ms(G_CJQ.ADDR[0]); //ͨ���������ܳ�ͻ��lixiaowei
		Read_All_Meter_Data(Buf,Len);
        break;
    }

	case 0x03://����߰�װ��Ϣ���Ѱ�װ��ߣ�δ��װ��ߣ���λ����
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
	case 0x04://���Ѱ�װ���
	case 0x07://���Ѱ�װ���
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
				G_Meter_BS1=0x81;							  //������ʽ	hu
				Meter_Up_FrameFormat(Buf,Len);
			}
			else
            Read_All_Installed_Meter_Number(Buf,Len);
        }
        break;
    }
	case 0x05://��δ��װ���
	{
		CJQ_Check=CJQ_Addr_Check(Buf); // ���ɼ�����ַ�Ƿ���ȷ
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
	case 0x06: //���ɼ�����ַ
    {
		Read_CJQAddr(Buf,Len);
        break;
    }
	case 0x0A://��������	hu		�õ��ֳֻ���������
    {
        CJQ_Check=CJQ_Addr_Check(Buf); // ���ɼ�����ַ�Ƿ���ȷ
        if(CJQ_Check==0)
        {
            return 0;
        }
        else
        {
			Buf[19]=0x01;
			G_Meter_BS0=0x05;
			G_Meter_BS1=0x81;							  //������ʽ	hu
			Meter_RD_Purchase_Amount(Buf,Len);
        }
        break;
    }
    case 0x0B://���۸��	hu
    {
        CJQ_Check=CJQ_Addr_Check(Buf); // ���ɼ�����ַ�Ƿ���ȷ
        if(CJQ_Check==0)
        {

            return 0;
        }
        else
        {
			Buf[19]=0x01;
			G_Meter_BS0=0x02;
			G_Meter_BS1=0x81;							  //������ʽ	hu
			Meter_Read_Price_List(Buf,Len);
        }
        break;
    }



    case 0x08: //����ֻ���ʵʱ����
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
	case 0x11://�����������һ̨�ɼ��������б�ߵ�������Ϣ����������ߵı����Ϣ
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
	case 0x21:	  //д���ط�
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
    case 0x22: //���һ̨������
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
    case 0x2A://����һ̨������
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
    case 0x2B: //ɾ��һ̨������
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
    case 0x2C: //ɾ��ȫ��������
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
    case 0x31 ://���Ĳɼ�����ַ
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
        //���Ĳɼ�����ַ�е������ַ���루A4A5��
        break;
    }
    case 0x33://��ʼ���ɼ���
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
	case 0x40:	  //д�۸��	  hu
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
	case 0x41:	  //д������
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
	case 0x42:	 //д������
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
	case 0x43:	 //д�������������
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
	case 0x44:	  //д����Կ
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
	case 0x45: //д��׼ʱ��
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
    case 0x49:	  //д����ͬ������
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
	case 0x4A:		//дʣ���ʣ������
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
        CJQ_Wireless_NetType_Read(Buf,Len);//��ȡ�������ͣ����Ǹ����ҵ�����ģ�鼰������ʽ�����������һ����
        break;
    }
    case 0x71:
    {
        Read_CJQAddr(Buf,Len);//��ȡ���磨��������ַ����A4A5��
        break;
    }
	    case 0x74:
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        Back_Wireless_Parameter(Buf,Len);//��ȡ�ӽڵ���� ����ģ��Ĳ�����
        break;
    }
    case 0x75:
    {
        CJQ_Check=CJQ_Addr_Check(Buf);
        if(CJQ_Check==0)
        {

            return 0;
        }
        Back_Wireless_Addr(Buf,Len);//��ȡ�ӽڵ���� ����ģ��Ĳ�����
        break;
    }

    case 0x77 : //���ɼ�������ģʽ
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
    case 0x78 : //���ɼ���������ѯʱ����
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
        //�����������ͣ����Ǹ����ҵ�����ģ�鼰������ʽ�����������һ����
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
        //���Ĳɼ�����ַ
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
    case 0x82 : //���Ĳɼ�������ģʽ
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
    case 0x83 : //���Ĳɼ���������ѯʱ����
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

    case 0x85: //����ͬ����Уʱ��
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
	case 0x86://�����б������
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
/**��߷��ش���֡��֡�ϴ�***/
u8  Meter_Up_Err_FrameFormat(u8 *DataBuf,u16 DataLen)
{
	    u8 RecData[28],Read_Cjq_ID[8];
		if(DataLen<16)
		return 1;

        RecData[0]=0x68;
        RecData[1]=0xC0;    //��Э��ͷ

        SPI_Flash_Read(EXT_CJQ_PARAMETER_PAGEADDR,Read_Cjq_ID,8);  //���ⲿfalsh�ɼ����ĵ�ַ
		memcpy(RecData+2,Read_Cjq_ID,8); //��ɼ�����ַ
		memcpy(RecData+10,DataBuf,10);  //װ�� 68H	T	Addr   C

        RecData[20]=0x01;
		RecData[21]=CheckCodeErr;

        RecData[22]=CRC8(RecData,22);
        RecData[23]=0x16;


        UART_3(RecData,24);//�ϴ���������
		return 0;
	
}
 
/*******************************************************************************
* Function Name  : Meter_Up_FrameFormat_NO_Data	
* Description    : ����ϴ�������
* Input          : DataBuf������ϴ����ݣ�DataLen�����ݳ���
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
				       	RecData[1]=0x80;    //��Э��ͷ
				
				        SPI_Flash_Read(EXT_CJQ_PARAMETER_PAGEADDR,Read_Cjq_ID,8);  //���ⲿfalsh�ɼ����ĵ�ַ
				        for(m=0; m<8; m++)
				            RecData[m+2]=Read_Cjq_ID[m];	   //��ɼ�����ַ
				
				        for(n=0; n<9; n++)
				           RecData[10+n]=DataBuf[n];  //װ�� 68H	T	Addr
		
				        if((DataBuf[11]==0x17)&&(DataBuf[12]==0xA0) )	 //���ط�
				        {
				            RecData[19]=0x21;
				            RecData[20]=0x01;	  
				
				            if(DataBuf[DataLen-4]==0x80)
				                RecData[21]=0x55;
				            else if(DataBuf[DataLen-4]==0x81)
				                RecData[21]=0x99;
				
				            RecData[22]=CRC8(RecData,22);
				            RecData[23]=0x16;
				
				            UART_3(RecData,24);//�ϴ���������
				        }
						
				        else if((DataBuf[11]==0x16)&&(DataBuf[12]==0xA0) )	  //����
				        {
				            RecData[19]=0x49;
				            RecData[20]=0x05;
				
//				            for(l=0; l<5; l++)
//				                RecData[21+l]=DataBuf[14+l];
							memset(&RecData[21],0,5);	  //?????��ʱΪ00000
				
				            RecData[26]=CRC8(RecData,26);
				            RecData[27]=0x16;
				
				            UART_3(RecData,28);//�ϴ���������
				        }
						 else if((DataBuf[11]==0x10)&&(DataBuf[12]==0xA0))		//д�۸��
						 {
						 	RecData[19]=0x40;
				            RecData[20]=0x30;	  
				
							memset(&RecData[21],0,30);	  //?????��ʱΪ00000
				
				            RecData[51]=CRC8(RecData,51);
				            RecData[52]=0x16;
				
				            UART_3(RecData,53);//�ϴ���������
						 	
						 }
	    		} 

}

/**����Ϸ�Э����֡***/
 u8  Meter_Up_FrameFormat(u8 *DataBuf,u16 DataLen)	//hu
{
    	u8 RecData[64],Read_Cjq_ID[8],i;
		if(DataLen<16)
		return 1;

        RecData[0]=0x68;
        RecData[1]=0x80;    //��Э��ͷ

        SPI_Flash_Read(EXT_CJQ_PARAMETER_PAGEADDR,Read_Cjq_ID,8);  //���ⲿfalsh�ɼ����ĵ�ַ
		memcpy(RecData+2,Read_Cjq_ID,8); //��ɼ�����ַ
		memcpy(RecData+10,DataBuf,10);  //װ�� 68H	T	Addr   C

        RecData[20]=DataBuf[10]-3;
		for(i=0;i<(DataBuf[10]-3);i++)
        	RecData[21+i]=DataBuf[11+i];	 

        RecData[21+i]=CRC8(RecData,21+i);
        RecData[22+i]=0x16;


        UART_3(RecData,24+i);//�ϴ���������
		return 0;

}
/*******************************************************************************
* Function Name  : Meter_Up_FrameFormat_NO_Data
* Description    : 
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void Meter_Up_FrameFormat_NO_Data_NO_ST(u8 *pMeterDataRecv,u16 G_RecvDataLen) //���
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
		case 0x11:	 //д������
		 	SendUpFrameFormat[19]=0x41;
			SendUpFrameFormat[20]=0x01;
			SendUpFrameFormat[21]=0x00; //????������Ϊ0
			SendUpFrameFormat[22]=CRC8(SendUpFrameFormat,22);
		    SendUpFrameFormat[23]=0x16;
		    Send_UART(G_UART_Num,SendUpFrameFormat,24);
		    break;
		case 0x12:	//д������  
		 	SendUpFrameFormat[19]=0x42;
			SendUpFrameFormat[20]=0x01;
			SendUpFrameFormat[21]=0x00; //????������Ϊ0
			SendUpFrameFormat[22]=CRC8(SendUpFrameFormat,22);
		    SendUpFrameFormat[23]=0x16;
		    Send_UART(G_UART_Num,SendUpFrameFormat,24);
		    break;
	//	case 0x15:	 //д��׼ʱ�䣿������������ͬ��Уʱ
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
/****��������***/
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
				case 0x1F:	   //����ǰ��������..	������ʽ	  
				{
					if(Len!=35)
					  return 1;
					Buf[9]=	0x01;
					Meter_Up_FrameFormat(Buf,Len);
					break;	
				}
				case 0x02:	   //���۸��  ..
				{
					if(Len!=46)
					  return 1;
					Buf[9]=	0x0B;
					Meter_Up_FrameFormat(Buf,Len);
					break;					
				}
				case 0x05:	   //��������	 . .
				{
					if(Len!=34)
					  return 1;
					Buf[9]=	0x0E;
					Meter_Up_FrameFormat(Buf,Len);
					break;					
				}
				case 0x0A:	   //����ַ	��������  B CЭ��û��
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
				case 0x10:	   //д�۸��..		   ���쳣����
				{
					if(Len!=18)
					  return 1;
						Meter_Up_FrameFormat_NO_Data(Buf,Len);
					break;	
				}
				case 0x11:	   //д������ ..
				{
					if(Len!=16)
					  return 1;
					Meter_Up_FrameFormat_NO_Data_NO_ST(Buf,Len);
					break;					
				}
				case 0x12:	   //д������  ..
				{
					if(Len!=16)
					  return 1;
					Meter_Up_FrameFormat_NO_Data_NO_ST(Buf,Len);
					break;					
				}
				case 0x13:	   //д������	.. 
				{
					if(Len!=25)
					  return 1;
					Buf[9]=	0x43;
					Meter_Up_FrameFormat(Buf,Len);
					break;					
				}
				case 0x14:	   //д����Կ..	
				{
					if(Len!=17)
					  return 1;
					Buf[9]=	0x44;
					Meter_Up_FrameFormat(Buf,Len);
					break;					
				}
				case 0x15:	   //д��׼ʱ��	 ?????? ����Уʱ
			    {
//					if(Len!=16)
//					  return 1;
//					Buf[9]=	0x45;
//					Meter_Up_FrameFormat(Buf,Len);
					Write_Meter_RTC_ACK(Buf,Len);
					break;										
				}
				case 0x17:	   //д���ſ���	 
				{
					if(Len!=18)
					  return 1;
//					Buf[9]=	0x21;
//
//					Meter_Up_FrameFormat(Buf,Len);
					 Meter_Up_FrameFormat_NO_Data(Buf,Len);
					break;						
				}
				case 0x19:	   //�Ǳ��������	���쳣����	  ������������ʱB��Cû��
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
		case 0x15:   //д��ַ
		{
	
			break;
		}
		case 0x96:	 //	 д����ͬ������
		{
			if(Len!=18)
				 return 1;
//			Buf[9]=	0x49;
//			memset(&Buf[11],0,5);//????? ���պ���
//			Meter_Up_FrameFormat(Buf,Len);
			Meter_Up_FrameFormat_NO_Data(Buf,Len);
			break;
		}
		case 0xAC:	//д�Ǳ��������	������������ʱB��Cû��
		{

			if(Len<16)
				 return 1;
			Meter_Up_FrameFormat(Buf,Len);
			
			break;
		}
		case 0xAE:	  //�Ǳ��ʼ��	����������	��ʱB��Cû��
		{
		   	if(Len!=18)
				 return 1;
		
			Meter_Up_FrameFormat(Buf,Len);

			break;
		}
		case 0xB4:	  //дʣ����	.. 
		{
			if(Len!=20)
				 return 1;
			Buf[9]=	0x4A;
			Meter_Up_FrameFormat(Buf,Len);
			break;
		}
		case 0xAF:	//дԤ���ѷ�ʽ���� ���������� ��ʱB��Cû��
		{

			if(Len!=19)
				 return 1;
			Meter_Up_FrameFormat(Buf,Len);

			break;  
		}
	    case 0xC1://��ǰ��������
		case 0xC4: //д�۸��
		case 0xD6://����
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

