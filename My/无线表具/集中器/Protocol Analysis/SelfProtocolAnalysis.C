#include "platform_config.h"
#include <EXT_Flash.h>
#include "ErrCode.h"
#include	"FILE_SYS.H"
#include <string.h>
#include <stdio.h>
extern u8 G_UART_Num;
u8 Index_Equ(u32 MeterIndexAbsoluteAddress,u16 Len)
{
    u8 DataIndex;
    SPI_Flash_Read(MeterIndexAbsoluteAddress, G_PageTmp,Len);
    DataIndex=CRC8(G_PageTmp,Len-1);
    if(G_PageTmp[Len-1]==0xff)
    {
        return 1;
    }
    else if(DataIndex==G_PageTmp[Len-1])
    {
        return 1;
    }
    else
    {
        return 0;
    }

}
STRUCT_CHAR HEX2ASC(u8 *pData,u16 DataLen)
{

    u16 i=0,j=0;
    u8 DataTemp[2],DataChange;
    STRUCT_CHAR Data1;
    for(i=0; i<DataLen*2;)
    {
        DataChange=pData[(i/2)];

        DataTemp[0]=(DataChange&0xf0)>>4;
        DataChange=pData[(i/2)];
        DataTemp[1]=(DataChange&0x0f);
        for(j=0; j<2; j++)
        {
            if(DataTemp[j]<=9)
            {
                Data1.Data_String[i]=DataTemp[j]+0x30;
            }
            else if((DataTemp[j]<=0x0f)&&(DataTemp[j]>=0x0a))
            {
                Data1.Data_String[i]=DataTemp[j]+0x37;
            }
            else break;
            i++;
        }
    }
    return Data1;
}
 void SD_Clear(void)
{
	u8 STAT=0;
	UINT8 FILE_PATH_INFO[256]={0};
	P_FAT_DIR_INFO	pDir;
	STAT = CH376FileOpenPath((u8 *)"/*\0");  /* 枚举多级目录下的文件或者目录,输入缓冲区必须在RAM中 */
			
	while (STAT== USB_INT_DISK_READ)	/* 枚举到匹配的文件 */
		{
//		IWDG_ReloadCounter();
		CH376ReadBlock((u8 *)FILE_PATH_INFO);  /* 读取枚举到的文件的FAT_DIR_INFO结构,返回长度总是sizeof( FAT_DIR_INFO ) */
		pDir = (P_FAT_DIR_INFO)FILE_PATH_INFO;  /* 当前文件目录信息 */
		if (pDir -> DIR_Name[0] != '.')
			{  /* 不是本级或者上级目录名则继续,否则必须丢弃不处理 */
			if ( pDir -> DIR_Name[0] == 0x05 )
				{
				pDir -> DIR_Name[0] = 0xE5;  /* 特殊字符替换 */
				}
					 CH376FileOpen(pDir -> DIR_Name);
		             CH376FileErase(pDir -> DIR_Name );//删除子目录
			}
		STAT = CH376FileOpenPath((u8 *)"/*\0"); 
		xWriteCH376Cmd( CMD0H_FILE_ENUM_GO );  /* 继续枚举文件和目录 */
		xEndCH376Cmd( );
		STAT = Wait376Interrupt( );
		}	
}
/*******************************************************************************
* Function Name  : Init_JZQ_S
* Description    : JZQ重启设置
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Init_JZQ_S(void)
{
    u8 i;
    u8 cjq_check[20];
	SPI_Flash_Read(JZQ_ADDR,G_JZQ.ADDR,8);
    memset(&G_status,0,sizeof(JZQ_Status));
    SPI_Flash_Read(CJQ_INDEX,cjq_check,20);
    for(i=0; i<20; i++)
    {
        if(cjq_check[i]!=0xff)
        {
            u8 tmp[8];
            SPI_Flash_Read(CJQ_ADDR+i*8,tmp,8);
            G_CJQ_status[i].flag=1;
            G_CJQ_status[i].Counter=0;
        }
        else
        {
            G_CJQ_status[i].flag=0;
        }
    }
    WireLess_Networking();
}
/*******************************************************************************
* Function Name  : Read_JZQ_CJQ_Addr
* Description    : 读取集中器中采集器的所有地址
* Input          : Buf：数据数组，Len：数据长度
* Output         : None
* Return         : None
*******************************************************************************/
//void Read_JZQ_CJQ_Addr(u8 *Buf,u16 Len)
//{
//    u8 buf[20][8],i,CR1,cjq_num[1],cjq_count=0,cjq_index_check[1],cjq_index[20];
//    u8 buf2[]= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},Up[255];
//    if(Len!=28)
//    {
//        Err_Ack(Buf,CmdErr);
//        return;
//    }
//    SPI_Flash_Read(CJQ_NUM,cjq_num,1);
//    SPI_Flash_Read(CJQ_INDEX,cjq_index,20);
//    CR1=CRC8(cjq_index,20);
//    SPI_Flash_Read(CJQ_INDEX_CHECK,cjq_index_check,1);
//    for(i=0; i<20; i++)
//    {
//        SPI_Flash_Read(CJQ_ADDR,buf[i],8);
//        if((memcmp(buf[i],buf2,8)!=0))
//        {
//            memcpy(&Up[30+cjq_count],buf[i],8);
//            cjq_count++;
//        }
//    }
//    if((CR1==cjq_index_check[0])&&(cjq_count==cjq_num[0]))
//    {
//        Up[0]=0x68;
//        Up[1]=0x80;
//        memcpy(&Up[2],&Buf[2],28);
//        Up[30+cjq_count*8]=CRC8(Up,30+cjq_count*8);
//        Up[30+cjq_count*8+1]=0x16 ;
//        UART_4(Up,30+cjq_count*8+2);
//    }
//    else
//    {
//        Err_Ack(Buf,ReqDelOrAddCjq);
//    }
//}
/*******************************************************************************
* Function Name  : Read_JZQ_CJQ_Addr
* Description    : 读取集中器地址
* Input          : Buf：数据数组，Len：数据长度
* Output         : None
* Return         : None
*******************************************************************************/

s32 Read_JZQAddr(u8 *Buf,u16 Len)
{
    u8 buf[8];
    u8 i=0;
    extern u8  G_UART_Num;
    if(Len!=32)
    {
        Err_Ack(Buf,CmdErr);
        return Failed;
    }
    SPI_Flash_Read(JZQ_ADDR,buf,8);
    Buf[1]=0x80;
    for(i=0; i<8; i++)
    {
        Buf[i+2]=buf[i];
    }

    Buf[Len-2]=CRC8(Buf,Len-2);
	_delay_ms(buf[0]); //通过编号来规避冲突。lixiaowei
    Send_UART(G_UART_Num,Buf,Len);	
    return Success;
}
/*******************************************************************************
* Function Name  : Init_SD
* Description    : 初始化采集器
* Input          : Buf：数据数组，Len：数据长度
* Output         : None
* Return         : None
*******************************************************************************/

UINT8 UserConfiguration[14]="\\用户配置";//根目录下新建文件夹需要加“\\”
UINT8 FileName[14]="JZQCONFG.TXT";

s32  Init_SD(void)
{
	UINT8 Stat=0; 	
	UINT8 InitContent[]="集中器地址,主无线模块地址,采集器数量,表具总数量,工作模式,数据结转时间,定时抄表时间间隔\r\n"	;
	UINT8 InitParmt[]="AAAAAAAAAAAAAAAA,AAAAAAAAAAAAAAAA,00,0000,01,00:00:00,002100\r\n"; 
	SD_Clear();	//清空SD卡
	Stat=CH376DirCreate(UserConfiguration);
	Stat=CH376FileCreate(FileName);
	Stat=CH376FileOpen(UserConfiguration);
	if(Stat==ERR_OPEN_DIR)
	{
	 	Stat=CH376FileOpen(FileName);  
		Stat=CH376ByteWrite(InitContent,strlen((char *)InitContent),NULL);
		Stat=CH376ByteWrite(InitParmt,strlen((char *)InitParmt),NULL);
		if(Stat==0x14)
		 {
		 CH376FileClose(TRUE); 
		 return Success;
		 }
		else
		{
		 CH376FileClose(TRUE); 
		 return WriteSDErr;
		}
		
	}
	else 
		 {
		   	CH376FileClose(TRUE); 
		    return WriteSDErr;
		 	//mStopIfError( Stat );
		 }

}
u8 ChangeJZQAddr_Flash(u8 *Buf,u16 Len)
{
    u8 Stat=0;
    u8 buf[8];
    Stat=SPI_Flash_Write(JZQ_ADDR,&Buf[30],8);//写集中器地址
    Stat=SPI_Flash_Write(JZQ_RFADDR,&Buf[34],4);//写主无线模块地址
    SPI_Flash_Read(JZQ_ADDR,buf,8);   //读取flash
    if(Stat==0x01)
        return Stat;
    else
        return WriteEXTFlashErr;
}
u8 ChangeJZQAddr_SD(u8 *Buf,u16 Len)
{
    u8 Stat=0,i;
    u8 buf[8];
    STRUCT_CHAR JZQAddr;
    Stat=CH376FileOpen(UserConfiguration);
    for(i=0; i<8; i++) //地址顺序需要调整
    {
        buf[i]=Buf[37-i];
    }
    if(Stat==ERR_OPEN_DIR)
    {
        JZQAddr=HEX2ASC(buf,8);
        JZQAddr.Data_String[16]=','	;
        Stat=CH376FileOpen(FileName);
        CH376ByteLocate(JZQINITNUM);//移动文件指针
        Stat=CH376ByteWrite( JZQAddr.Data_String, 17, NULL); //集中器地址

        Stat=CH376ByteWrite( &JZQAddr.Data_String[8], 8, NULL);	//主无线模块地址
        if(Stat==0x14)
        {
            CH376FileClose(TRUE);
            return 1;
        }
        else
        {
            CH376FileClose(TRUE);
            return WriteSDErr;
            //mStopIfError( Stat );
        }

    }
    else
    {
        return WriteSDErr;
        //mStopIfError( Stat );
    }
}
u8 Change_JZQAddr(u8 *Buf,u16 Len)
{
    u8 Stat1=0,Stat2=0;
    u8 i=0;
    extern u8  G_UART_Num;
    Stat1=ChangeJZQAddr_Flash(Buf,Len);
    if(Stat1==0x01)
    {
        SPI_Flash_Read(JZQ_ADDR,G_JZQ.ADDR,8);
		Stat2=ChangeJZQAddr_SD(Buf,Len);
        if(Stat2==0x01)
        {
            Buf[1]=0x80;
            for(i=0; i<8; i++)
            {
                Buf[i+2]=Buf[i+30];
            }
            Buf[Len-2]=CRC8(Buf,Len-2);
            Send_UART(G_UART_Num,Buf,Len);
            return 1;
        }
        else
        {
            Err_Ack(Buf,Stat2);
            return 0;
        }
    }
    else
    {
        Err_Ack(Buf,Stat1);
        return 0;
    }

}
s32 Deal_One_CJQ_Flash(u8 *Buf,u16 Len,u8 cmd)
{
    u8 CJQNum=0,i,j;
    u8 Count=0;
    u8 CJQAddrSpace=0;
    u8 CJQIndex=0xff;
    u8 buf[8];
    CJQIndex=CRC8(&Buf[11],8);
    if(CJQIndex==0xff)
    {
        CJQIndex++;
    }
    if(Index_Equ(CJQ_INDEX,CJQ_NUM_SIZE+1))
    {
        SPI_Flash_Read(CJQ_NUM,&CJQNum,1);
        SPI_Flash_Read(CJQ_INDEX,G_PageTmp,CJQ_NUM_SIZE);
        if(CJQNum!=CJQ_NUM_SIZE)
        {
            for(i=0; i<CJQ_NUM_SIZE; i++)
            {
                if(G_PageTmp[i]==0xff)
                {
                    CJQAddrSpace=i;
                    break;
                }
            }
            for(i=0; i<CJQ_NUM_SIZE; i++)
            {
                if(Count<CJQNum)
                {
                    if(G_PageTmp[i]!=0xff)
                    {
                        if(G_PageTmp[i]==CJQIndex)
                        {
                            SPI_Flash_Read(CJQ_ADDR+i*8, buf,8);
                            for(j=0; j<8; j++)
                            {
                                if(buf[j]!=Buf[11+j]) break;
                            }
                            if(j==8)
                            {
								if(cmd==DEL_ONE)
								{
									G_PageTmp[i]=0xff;
									SPI_Flash_Write(CJQ_INDEX,G_PageTmp,CJQ_NUM_SIZE);
									CJQNum--;
									SPI_Flash_Write(CJQ_NUM,&CJQNum,1);
									return Success;
								}
								else if(cmd==ALT_ONE)
								{
									SPI_Flash_Write(CJQ_ADDR+i*8, Buf+30,8);
									G_PageTmp[i]=CRC8(Buf+30,8);
									SPI_Flash_Write(CJQ_INDEX,G_PageTmp,CJQ_NUM_SIZE);
									memcpy(Buf+11,Buf+30,8);
									Right_Ack_Len0(Buf,Len);
									return Success;
								}
								else if(cmd==ADD_ONE)
								{
									return DoubleCJQ;
								}
								else if(cmd==INI_ONE)
								{
								    DEL_CJQ_Meters(Buf,Len);
									return Success;
								}
								else
								{
									return Failed;
								}
                                
                            }
                        }
                        Count++;
                    }
                }
                else //没有重复的采集器
                {
                    if(cmd!=ADD_ONE)
						return Failed;
					CJQNum++;
                    SPI_Flash_Write(CJQ_NUM,&CJQNum,1);//更新采集器数量
                    SPI_Flash_Write(CJQ_INDEX+CJQAddrSpace,&CJQIndex,1);//写采集器索引到flash
                    SPI_Flash_Read(CJQ_INDEX, G_PageTmp,CJQ_NUM_SIZE);
                    CJQIndex=CRC8(G_PageTmp,CJQ_NUM_SIZE);
                    if(CJQIndex==0xff)
                    {
                        CJQIndex++;
                    }
                    SPI_Flash_Write(CJQ_INDEX_CHECK,&CJQIndex,1);//写采集器索引的校验到flash
                    SPI_Flash_Write(CJQ_ADDR+CJQAddrSpace*8,&Buf[11],8);//写采集器地址的校验到flash
                    SPI_Flash_Write(CJQ_RFADDR+CJQAddrSpace*8,&Buf[15],4);//写采集器地址的校验到flash
                    return Success;
                }
            }
        }
        else
        {
            return StorageSpaceOverFlow;
        }
    }
    else
        return 0;
    return 0;
}
s32 Deal_One_CJQ_SD(u8 *Buf,u16 Len,u8 cmd)
 {
 	P_FAT_DIR_INFO	pDir;
	u8 Stat=0,i,buf[8];
	u8 FileEquFlag=0,DirEquFlag=0;
	STRUCT_CHAR CJQAddr;
	UINT8 FILE_PATH_INFO[256]={0};
	UINT8 NowFileName[12]="14560814.TXT";
	UINT8 FileData[35]="采集器地址,表具编码，表具类型\r\n"	;
	Stat = CH376FileOpenPath((u8 *)"/*\0");  /* 根目录下枚举文件或者目录，多级目录下的文件或者目录,输入缓冲区必须在RAM中 */
		while (Stat== USB_INT_DISK_READ)	/* 枚举到匹配的文件 */
		{
//		IWDG_ReloadCounter();
		CH376ReadBlock((u8 *)FILE_PATH_INFO);  /* 读取枚举到的文件的FAT_DIR_INFO结构,返回长度总是sizeof( FAT_DIR_INFO ) */
		pDir = (P_FAT_DIR_INFO)FILE_PATH_INFO;  /* 当前文件目录信息 */
		if (pDir -> DIR_Name[0] != '.')
			{  /* 不是本级或者上级目录名则继续,否则必须丢弃不处理 */
			if ( pDir -> DIR_Name[0] == 0x05 )
				{
				pDir -> DIR_Name[0] = 0xE5;  /* 特殊字符替换 */
				}
			/*检查是否是“用户配置”文件夹*/
			if((pDir -> DIR_Name[0]==0xD3)&&(pDir -> DIR_Name[1]==0xC3)&&(pDir -> DIR_Name[2]==0xBB))
				{
//				IWDG_ReloadCounter();
				//SPI_FLASH_BufferWrite((u8 *)pDir -> DIR_Name,File_Quantity*8+655360,8);
				DirEquFlag=1;
				break;
				}
			pDir -> DIR_Attr = 0;  /* 强制文件名字符串结束以便打印输出 */
			}
		xWriteCH376Cmd( CMD0H_FILE_ENUM_GO );  /* 继续枚举文件和目录 */
		xEndCH376Cmd( );
		Stat = Wait376Interrupt( );
		}
		if(DirEquFlag==1) //找到“用户配置”文件夹
		{
		for(i=0;i<8;i++)  //将采集器地址反转存储（高位在前，地位在后）
		{
			buf[i]=Buf[18-i];
		}
		CJQAddr=HEX2ASC(buf,8);
		Stat =CH376FileOpen(UserConfiguration);
		Stat = CH376FileOpenPath((u8 *)"*\0");  /* 当前目录下枚举文件及子目录，多级目录下的文件或者目录,输入缓冲区必须在RAM中 */
		while (Stat==USB_INT_DISK_READ)	/* 枚举到匹配的文件 */
		{
//		IWDG_ReloadCounter();
		CH376ReadBlock((u8 *)FILE_PATH_INFO);  /* 读取枚举到的文件的FAT_DIR_INFO结构,返回长度总是sizeof( FAT_DIR_INFO ) */
		pDir = (P_FAT_DIR_INFO)FILE_PATH_INFO;  /* 当前文件目录信息 */
		if (pDir -> DIR_Name[0] != '.')
			{  /* 不是本级或者上级目录名则继续,否则必须丢弃不处理 */
			if ( pDir -> DIR_Name[0] == 0x05 )
				{
				pDir -> DIR_Name[0] = 0xE5;  /* 特殊字符替换 */
				}
			/*检查是否是“用户配置”文件夹*/
			for(i=0;i<8;i++)
			{
				if(pDir -> DIR_Name[i]!=CJQAddr.Data_String[i+8])
				break;
			}
			if(i==8)
			{	
				FileEquFlag=0;
				if(cmd==ADD_ONE)
	  				return DoubleCJQ;
				else if(cmd==ALT_ONE)
				{ 	
					u8 tmp[40];
					memcpy(tmp,Buf,40);
					memcpy(tmp+11,tmp+30,8);
					if(Deal_One_CJQ_SD(tmp,Len,ADD_ONE)!=Success)
						return Failed;
					Stat=CH376FileClose(TRUE);
					Stat=CH376FileOpen(UserConfiguration);
					for(i=0;i<8;i++)   //采集器地址低8位作为文件名
					{
					NowFileName[i]=CJQAddr.Data_String[i+8];	
					}
					Stat=CH376FileOpen(NowFileName);
//					cpy_file(dest_file,NowFileName);?????
					CH376FileErase(NowFileName);
					Stat=CH376FileClose(TRUE);					
					return Success;
				}
				else if(cmd==DEL_ONE)
				{
					Stat=CH376FileClose(TRUE);
					Stat=CH376FileOpen(UserConfiguration);
					for(i=0;i<8;i++)   //采集器地址低8位作为文件名
					{
					NowFileName[i]=CJQAddr.Data_String[i+8];	
					}
					Stat=CH376FileOpen(NowFileName);
					CH376FileErase(NowFileName);
					Stat=CH376FileClose(TRUE);
					return Success;	
				}
				else if(cmd==INI_ONE)
				{
				 	for(i=0;i<8;i++)   //采集器地址低8位作为文件名
					{
					NowFileName[i]=CJQAddr.Data_String[i+8];	
					}
					Stat=CH376FileCreate(NowFileName);
					Stat=CH376FileClose(TRUE);
					Stat=CH376FileOpen(UserConfiguration);
					
				 	Stat=CH376FileOpen(NowFileName);
					Stat=CH376ByteWrite(FileData,strlen((char *)FileData),NULL);
					Stat=CH376FileClose(TRUE);
					return Success;
				}
				else
					return Failed;
			}
			else 
			{
				if(cmd!=ADD_ONE)
					return Failed;
				FileEquFlag=1;
			}
			pDir -> DIR_Attr = 0;  /* 强制文件名字符串结束以便打印输出 */
		   }
		xWriteCH376Cmd( CMD0H_FILE_ENUM_GO );  /* 继续枚举文件和目录 */
		xEndCH376Cmd( );
		Stat = Wait376Interrupt( );
		}
		 if(FileEquFlag==1)	//没有找到重复的采集器 ，那么新建此采集器文件
		 {
		 	for(i=0;i<8;i++)   //采集器地址低8位作为文件名
			{
			NowFileName[i]=CJQAddr.Data_String[i+8];	
			}
			Stat=CH376FileCreate(NowFileName);
			Stat=CH376FileClose(TRUE);
			Stat=CH376FileOpen(UserConfiguration);
			
		 	Stat=CH376FileOpen(NowFileName);
			Stat=CH376ByteWrite(FileData,strlen((char *)FileData),NULL);
			Stat=CH376FileClose(TRUE);
			return Success;
		 }
		}
		return Failed;

 }
s32 Deal_One_Meter_Flash(u8 *Buf,u16 Len,u8 cmd)
{
    u8 CJQNum=0,i=0,j=0;
    u8 MeterNum=0,AllMeterNum=0;
    u8 Count=0;
    u8 CJQIndex,MeterIndex,MeterIndexCheck;
    u8 CJQSpacePosition;
    u8 MeterSpacePosition;
    u8 buf[8];
    CJQIndex=CRC8(&Buf[11],8);
    if(CJQIndex==0xff)
    {
        CJQIndex++;
    }
    MeterIndex=CRC8(&Buf[21],7);
    if(MeterIndex==0xff)
    {
        MeterIndex++;
    }
    SPI_Flash_Read(CJQ_NUM, &CJQNum,1);
    SPI_Flash_Read(CJQ_INDEX, G_PageTmp,CJQ_NUM_SIZE);
    /**************寻找采集器****************/
    for(i=0; i<=CJQ_NUM_SIZE; i++)
    {
        if(Count<CJQNum)
        {
            if(G_PageTmp[i]!=0xff)
            {
                if(G_PageTmp[i]==CJQIndex)
                {
                    SPI_Flash_Read(CJQ_ADDR+i*8, buf,8);
                    for(j=0; j<8; j++)
                    {
                        if(buf[j]!=Buf[j+11]) break;
                    }
                    if(j==8)
                    {
                        CJQSpacePosition=i;
                        break;
                    }
                }
                Count++;
            }
        }
        else
        {
            return NotFindCJQ;
        }

    }
    /***************查找表具**********************************/
    Count=0;
    SPI_Flash_Read(CJQSpacePosition+CJQ_METER_NUM,&MeterNum,1); //读出表具数量
    SPI_Flash_Read(CJQ_METER_INDEX+(CJQSpacePosition*METERINDEX_TO_METERINDEX), G_PageTmp,255);	//表具索引
    MeterSpacePosition=0;
    /*****查询是否已经存满255只表具********/
    if(MeterNum!=255)
    {
        for(i=0; i<255; i++)
        {
            if(G_PageTmp[i]==0xff)
            {
                MeterSpacePosition=i;
                break;
            }
        }
        if(i==255)
        {
            return StorageSpaceOverFlow;
        }
        /*********查询是否已经存在此表************/
        for(i=0; i<255; i++)
        {
            if(Count<MeterNum)
            {
                if( G_PageTmp[i]!=0xff)
                {
                    if( G_PageTmp[i]==MeterIndex)
                    {
                        SPI_Flash_Read(i*8+CJQ_METER_ADDR+(CJQSpacePosition*METERINDEX_TO_METERINDEX),buf,8); //读出表具地址
                        for(j=0; j<8; j++)
                        {
                            if(buf[j]!=Buf[j+20]) break;
                        }
                        if(j==8)
                        {
                            if(cmd==ADD_ONE)
								return DoubleMeter;
							else if(cmd==ALT_ONE)
							{
								memcpy(buf+1,Buf+30,7);
								SPI_Flash_Write(i*8+CJQ_METER_ADDR+(CJQSpacePosition*METERINDEX_TO_METERINDEX),buf,8); //读出表具地址
								return Success;
							}
							else if(cmd==DEL_ONE)
							{
							    MeterNum--;
								SPI_Flash_Write(CJQSpacePosition+CJQ_METER_NUM,&MeterNum,1); //读出表具数量
								G_PageTmp[i]=0xff;
							    SPI_Flash_Write(CJQ_METER_INDEX+(CJQSpacePosition*METERINDEX_TO_METERINDEX), G_PageTmp,255);	//表具索引
								return Success;
							}
							else
								return CmdErr;
								
                        }
                    }
                }
                Count++;
            }
            else 
			{
				if(cmd!=ADD_ONE)
					return Failed;
				break;
			}
        }
    }
    else
    {
        return StorageSpaceOverFlow;
    }
    /*****相应位置添加表具******/
    MeterNum++;
    SPI_Flash_Read(JZQ_ALL_METER_NUM,&AllMeterNum,1); //表具总数量
    AllMeterNum++;
    SPI_Flash_Write(JZQ_ALL_METER_NUM,&AllMeterNum,1);
    SPI_Flash_Write(CJQSpacePosition+CJQ_METER_NUM,&MeterNum,1);//写表具数量
    SPI_Flash_Write(CJQ_METER_INDEX+(CJQSpacePosition*METERINDEX_TO_METERINDEX)+MeterSpacePosition,&MeterIndex,1);//写表具索引
    SPI_Flash_Read(CJQ_METER_INDEX+(CJQSpacePosition*METERINDEX_TO_METERINDEX), G_PageTmp,255);	//表具索引
    MeterIndexCheck=CRC8(G_PageTmp,255);
    if(MeterIndexCheck==0xff)
    {
        MeterIndexCheck++;
    }
    SPI_Flash_Write(CJQ_METER_INDEX+(CJQSpacePosition*METERINDEX_TO_METERINDEX)+255,&MeterIndexCheck,1);

    SPI_Flash_Write(CJQ_METER_ADDR+(CJQSpacePosition*METERINDEX_TO_METERINDEX)+MeterSpacePosition*8,&Buf[20],8);
    return Success;
}
/*******************************************************************************
* Function Name  : Deal_One_Meter_SD
* Description    : 单个表数据操作
* Input          : Buf：数据数组，Len：数据长度，cmd：ADD_ONE_METER，DEL_ONE_METER，ALT_ONE_METER
* Output         : None
* Return         : None
*******************************************************************************/


u8	Deal_One_Meter_SD(u8 *Buf,u16 Len,u8 cmd)
{
    P_FAT_DIR_INFO	pDir;
    u8 Stat=0,i,buf[8];
    u32 FileSize=0;
    u8 FileEquFlag=0,DirEquFlag=0;
    STRUCT_CHAR ACSIIData;
    UINT8 FILE_PATH_INFO[25]= {0};
	u8 meter_add[15];
	sprintf((char *)meter_add,"%02x%02x%02x%02x%02x%02x%02x\0",Buf[21],Buf[22],Buf[23],Buf[24],Buf[25],Buf[26],Buf[27]);
    Stat = CH376FileOpenPath((u8 *)"/*\0");  /* 根目录下枚举文件或者目录，多级目录下的文件或者目录,输入缓冲区必须在RAM中 */
    while (Stat== USB_INT_DISK_READ)	/* 枚举到匹配的文件 */
    {
//        IWDG_ReloadCounter();
        CH376ReadBlock((u8 *)FILE_PATH_INFO);  /* 读取枚举到的文件的FAT_DIR_INFO结构,返回长度总是sizeof( FAT_DIR_INFO ) */
        pDir = (P_FAT_DIR_INFO)FILE_PATH_INFO;  /* 当前文件目录信息 */
        if (pDir -> DIR_Name[0] != '.')
        {
            /* 不是本级或者上级目录名则继续,否则必须丢弃不处理 */
            if ( pDir -> DIR_Name[0] == 0x05 )
            {
                pDir -> DIR_Name[0] = 0xE5;  /* 特殊字符替换 */
            }
            /*检查是否是“用户配置”文件夹*/
            if((pDir -> DIR_Name[0]==0xD3)&&(pDir -> DIR_Name[1]==0xC3)&&(pDir -> DIR_Name[2]==0xBB)&&(pDir -> DIR_Name[3]==0xA7))
            {
//                IWDG_ReloadCounter();
                DirEquFlag=1;
                break;
            }
            pDir -> DIR_Attr = 0;  /* 强制文件名字符串结束以便打印输出 */
        }
        xWriteCH376Cmd( CMD0H_FILE_ENUM_GO );  /* 继续枚举文件和目录 */
        xEndCH376Cmd( );
        Stat = Wait376Interrupt( );
    }
    if(DirEquFlag==1) //找到“用户配置”文件夹
    {
        for(i=0; i<8; i++) //将采集器地址反转存储（高位在前，地位在后）
        {
            buf[i]=Buf[18-i];
        }
        ACSIIData=HEX2ASC(buf,8);
        Stat =CH376FileOpen(UserConfiguration);
        Stat = CH376FileOpenPath((u8 *)"*\0");  /* 当前目录下枚举文件及子目录，多级目录下的文件或者目录,输入缓冲区必须在RAM中 */
        while (Stat==USB_INT_DISK_READ)	/* 枚举到匹配的文件 */
        {
//            IWDG_ReloadCounter();
            CH376ReadBlock((u8 *)FILE_PATH_INFO);  /* 读取枚举到的文件的FAT_DIR_INFO结构,返回长度总是sizeof( FAT_DIR_INFO ) */
            pDir = (P_FAT_DIR_INFO)FILE_PATH_INFO;  /* 当前文件目录信息 */
            if (pDir -> DIR_Name[0] != '.')
            {
                /* 不是本级或者上级目录名则继续,否则必须丢弃不处理 */
                if ( pDir -> DIR_Name[0] == 0x05 )
                {
                    pDir -> DIR_Name[0] = 0xE5;  /* 特殊字符替换 */
                }
                /*检查是否是此采集器对应的文件*/
                for(i=0; i<8; i++)
                {
                    if(pDir -> DIR_Name[i]!=ACSIIData.Data_String[i+8])
                        break;
                }
                if(i==8)
                {
                    /*不去判断是否已经存在此表具，直接添加（因为程序能够执行到这来，说明对flash已经做过判断，是没有重复的表具的）*/
                    FileEquFlag=0;
                    Stat =CH376FileOpen(pDir -> DIR_Name);//打开文件
                    FileSize=CH376GetFileSize();

					if(cmd==ALT_ONE)
					{
						u8 tmp[80],File_p=0;
						s32 ret=0;
						File_p=0;
						for(;;)
						{
							CH376ByteRead(tmp,62,NULL);
							if(memcmp(tmp+17,meter_add,14)==0)
							{
								sprintf((char *)meter_add,"%02x%02x%02x%02x%02x%02x%02x\0",Buf[30],Buf[31],Buf[32],Buf[33],Buf[34],Buf[35],Buf[36]);
								memcpy(tmp+17,meter_add,14);
								CH376ByteWrite(tmp,62,NULL);
	                    		return Success;
							}
							File_p+=62;
							if(File_p>FileSize)
							{
								CH376FileClose(TRUE);
								return Failed;
							}	
							CH376ByteLocate(File_p);
						}
					}
					else if(cmd==ADD_ONE)
					{
	                    CH376ByteLocate(FileSize);//移动文件指针到文件末尾
	                    /*采集器编码*/
	                    for(i=0; i<8; i++) //将采集器地址反转存储（高位在前，地位在后）
	                    {
	                        buf[i]=Buf[18-i];
	                    }
	                    ACSIIData=HEX2ASC(buf,8);
	                    ACSIIData.Data_String[16]=',';
	                    Stat=CH376ByteWrite(ACSIIData.Data_String,17,NULL);
	                    /*写表具编码*/
	                    for(i=0; i<7; i++) //将表具地址反转存储（高位在前，地位在后）
	                    {
	                        buf[i]=Buf[27-i];
	                    }
	                    ACSIIData=HEX2ASC(buf,7);
	                    ACSIIData.Data_String[i*2]=',' ;
	                    Stat=CH376ByteWrite(ACSIIData.Data_String,i*2+1,NULL);
	                    /*表具类型*/
	                    ACSIIData=HEX2ASC(&Buf[20],1);
	                    ACSIIData.Data_String[2]='\r';
	                    ACSIIData.Data_String[3]='\n' ;
	                    Stat=CH376ByteWrite(ACSIIData.Data_String,4,NULL);
	                    CH376FileClose(TRUE);
	
	                    Stat =CH376FileOpen(UserConfiguration);
	                    Stat =CH376FileOpen(FileName);//打开集中器配置文件JZQCOFG.TXT
	                    CH376ByteLocate(SD_ALLMETERNUM_OFFSET);//移动文件指针
	                    Stat =CH376ByteRead(buf,4, NULL);
	
	
	
	                    CH376FileClose(TRUE);
	                    return Success;
					}
					else if(cmd==DEL_ONE)
					{
						u8 tmp[100],File_p=0;
						s32 ret=0;
						File_p=0;
						for(;;)
						{
							CH376ByteRead(tmp,62,NULL);
							if(memcmp(tmp+17,meter_add,14)==0)
							{
								for(;File_p<FileSize;)
								{
									CH376ByteRead(tmp,62,NULL);
									CH376ByteLocate(File_p-124);
									CH376ByteWrite(tmp,62,NULL);
									File_p+=62;
								}
								CH376FileClose(TRUE);
	                   			return Success;	
							}
							File_p+=62;
							if(File_p>FileSize)
							{
								CH376FileClose(TRUE);
								return Failed;
							}	
							CH376ByteLocate(File_p);
						}						
					}
					else
					{
						return CmdErr;
					}

                }
                else
                {
                    FileEquFlag=1;
                }
                pDir -> DIR_Attr = 0;  /* 强制文件名字符串结束以便打印输出 */
            }
            xWriteCH376Cmd( CMD0H_FILE_ENUM_GO );  /* 继续枚举文件和目录 */
            xEndCH376Cmd( );
            Stat = Wait376Interrupt( );
        }
        if(FileEquFlag==1)	//没有找到采集器
        {
            return NotFindCJQInSD;
        }
    }
    return NotFindConfgFolder;
}
s32 Deal_One_Meter(u8 *Buf,u16 Len,u8 cmd)
{
    u8 Stat;

    Stat=Deal_One_Meter_Flash(Buf,Len,cmd);
    if(Stat==Success)
    {
        Stat=Deal_One_Meter_SD(Buf,Len,cmd);
        if(Stat==Success)
        {
            Right_Ack_Len0(Buf,Len);
            return Success;
        }
        else
        {
            Err_Ack(Buf,Stat );
            return Failed;
        }
    }
    else
    {
        Err_Ack(Buf,Stat);
        return Failed;
    }
}
s32 Add_Multi_Meter(u8 *Buf,u16 Len)
{
    u8 i;
	u8 tmp[32];
	if(Len<39)	
	{
		Err_Ack(Buf,CmdErr);
		return Failed;
	}
	memcpy(tmp,Buf,32);
	tmp[31]=0x16;
	tmp[29]=0;
	if(Buf[29]!=(Len-32))	
	{
		Err_Ack(Buf,CmdErr);
		return Failed;
	}
	for(i=0;i<Buf[29]/7;i++)
	{
	 	memcpy(tmp+21,Buf+30+i*7,7);
		tmp[30]=CRC8(tmp,30);
		Deal_One_Meter_Flash(Buf,Len,ADD_ONE);
		Deal_One_Meter_SD(Buf,Len,ADD_ONE);
	}
	Right_Ack_Len0(Buf,Len);
	return Success;
}
s32 Add_Multi_CJQ(u8 *Buf,u16 Len)
{
    u8 i;
	u8 tmp[32];
	if(Len<40)	
	{
		Err_Ack(Buf,CmdErr);
		return Failed;
	}
	memcpy(tmp,Buf,32);
	tmp[31]=0x16;
	tmp[29]=0;
	if(Buf[29]!=(Len-32))	
	{
		Err_Ack(Buf,CmdErr);
		return Failed;
	}
	for(i=0;i<Buf[29]/8;i++)
	{
	 	memcpy(tmp+11,Buf+30+i*8,8);
		tmp[30]=CRC8(tmp,30);
		Deal_One_CJQ_Flash(Buf,Len,ADD_ONE);
		Deal_One_CJQ_SD(Buf,Len,ADD_ONE);
	}
	Right_Ack_Len0(Buf,Len);
	return Success;
}
s32  Init_Flash(void)
{
    u8 JZQ_Init[160]= {0};
    u8 i,Stat=0;
    SPI_Flash_Erase_Chip(); //整片擦除
    for(i=0; i<160; i++)
    {
        JZQ_Init[i]=0xAA;
    }
    SPI_Flash_Write(JZQ_ADDR,JZQ_Init,8);//初始化集中器地址为全AA
    SPI_Flash_Write(JZQ_RFADDR,JZQ_Init,8);//初始化集中器无线模块主节点地址为全AA
    SPI_Flash_Write(CJQ_ADDR,JZQ_Init,160);//初始化采集器地址全AA
    SPI_Flash_Write(CJQ_RFADDR,JZQ_Init,160);//初始化采集器无线模块子节点地址全AA
    JZQ_Init[0]=0x01;
    SPI_Flash_Write(JZQ_WORKMODE,JZQ_Init,1);//初始化集中器工作模式
    for(i=0; i<20; i++)
    {
        JZQ_Init[i]=0;
    }
    SPI_Flash_Write(CJQ_NUM,JZQ_Init,1);//初始化采集器数量为0
    SPI_Flash_Write(JZQ_ALL_METER_NUM,JZQ_Init,2);//初始化集中器中表具总数量为0
    SPI_Flash_Write(JZQ_DATA_CARRY_TIME,JZQ_Init,3);// 初始化集中器数据结转时间为00:00:00

    Stat=SPI_Flash_Write(CJQ_METER_NUM,JZQ_Init,20);//初始化各采集器中表具数量为0
    JZQ_Init[1]=0x21;
    Stat=SPI_Flash_Write(JZQ_TIME_INTERVAL,JZQ_Init,3);// 初始化集中器定时抄表时间间隔
	if(Stat!=Success)
		return Stat;
	return Success;
}
s32  Init_JZQ(u8 *Buf,u16 Len)
{
    u8 Stat1=0,Stat2=0;
    Stat1=Init_Flash();
    if(Stat1==Success)	//flash优先权
    {
        Stat2=Init_SD();
        if(Stat2==Success)
        {
            Right_Ack_Len0(Buf, Len);
            return Success;

        }
        else
        {
            Err_Ack(Buf,Stat2);
            return Failed;
        }
    }
    Err_Ack(Buf,Stat1);
    return Failed;
}
s32 Read_Installed_CJQAddr_SD(u8 *Buf,u16 Len)
{
    P_FAT_DIR_INFO	pDir;
    u8 Stat=0,i,FileSize=0,buf[8];
    u8 FileEquFlag=0,DirEquFlag=0;
    STRUCT_CHAR ACSIIData;
    UINT8 FILE_PATH_INFO[25]= {0};
    Stat = CH376FileOpenPath((u8 *)"/*\0");  /* 根目录下枚举文件或者目录，多级目录下的文件或者目录,输入缓冲区必须在RAM中 */
    while (Stat== USB_INT_DISK_READ)	/* 枚举到匹配的文件 */
    {
//        IWDG_ReloadCounter();
        CH376ReadBlock((u8 *)FILE_PATH_INFO);  /* 读取枚举到的文件的FAT_DIR_INFO结构,返回长度总是sizeof( FAT_DIR_INFO ) */
        pDir = (P_FAT_DIR_INFO)FILE_PATH_INFO;  /* 当前文件目录信息 */
        if (pDir -> DIR_Name[0] != '.')
        {
            /* 不是本级或者上级目录名则继续,否则必须丢弃不处理 */
            if ( pDir -> DIR_Name[0] == 0x05 )
            {
                pDir -> DIR_Name[0] = 0xE5;  /* 特殊字符替换 */
            }
            /*检查是否是“用户配置”文件夹*/
            if((pDir -> DIR_Name[0]==0xD3)&&(pDir -> DIR_Name[1]==0xC3)&&(pDir -> DIR_Name[2]==0xBB)&&(pDir -> DIR_Name[3]==0xA7))
            {
//                IWDG_ReloadCounter();
                DirEquFlag=1;
                break;
            }
            pDir -> DIR_Attr = 0;  /* 强制文件名字符串结束以便打印输出 */
        }
        xWriteCH376Cmd( CMD0H_FILE_ENUM_GO );  /* 继续枚举文件和目录 */
        xEndCH376Cmd( );
        Stat = Wait376Interrupt( );
    }
    if(DirEquFlag==1) //找到“用户配置”文件夹
    {
        for(i=0; i<8; i++) //将采集器地址反转存储（高位在前，地位在后）
        {
            buf[i]=Buf[18-i];
        }
        ACSIIData=HEX2ASC(buf,8);
        Stat =CH376FileOpen(UserConfiguration);
        Stat = CH376FileOpenPath((u8 *)"*\0");  /* 当前目录下枚举文件及子目录，多级目录下的文件或者目录,输入缓冲区必须在RAM中 */
        while (Stat==USB_INT_DISK_READ)	/* 枚举到匹配的文件 */
        {
//            IWDG_ReloadCounter();
            CH376ReadBlock((u8 *)FILE_PATH_INFO);  /* 读取枚举到的文件的FAT_DIR_INFO结构,返回长度总是sizeof( FAT_DIR_INFO ) */
            pDir = (P_FAT_DIR_INFO)FILE_PATH_INFO;  /* 当前文件目录信息 */
            if (pDir -> DIR_Name[0] != '.')
            {
                /* 不是本级或者上级目录名则继续,否则必须丢弃不处理 */
                if ( pDir -> DIR_Name[0] == 0x05 )
                {
                    pDir -> DIR_Name[0] = 0xE5;  /* 特殊字符替换 */
                }
                /*检查是否是此采集器对应的文件*/
                for(i=0; i<8; i++)
                {
                    if(pDir -> DIR_Name[i]!=ACSIIData.Data_String[i+8])
                        break;
                }
                if(i==8)
                {
                    /*不去判断是否已经存在此表具，直接添加（因为程序能够执行到这来，说明对flash已经做过判断，是没有重复的表具的）*/
                    FileEquFlag=0;
                    Stat =CH376FileOpen(pDir -> DIR_Name);//打开文件
                    FileSize=CH376GetFileSize( );
                    CH376ByteLocate(FileSize);//移动文件指针到文件末尾
                    /*采集器编码*/
                    for(i=0; i<8; i++) //将采集器地址反转存储（高位在前，地位在后）
                    {
                        buf[i]=Buf[18-i];
                    }
                    ACSIIData=HEX2ASC(buf,8);
                    ACSIIData.Data_String[17]=',';
                    Stat=CH376ByteWrite(ACSIIData.Data_String,17,NULL);
                    /*写表具编码*/
                    for(i=0; i<7; i++) //将表具地址反转存储（高位在前，地位在后）
                    {
                        buf[i]=Buf[27-i];
                    }
                    ACSIIData=HEX2ASC(buf,7);
                    ACSIIData.Data_String[i*2]=',' ;
                    Stat=CH376ByteWrite(ACSIIData.Data_String,i*2+1,NULL);
                    /*表具类型*/
                    ACSIIData=HEX2ASC(&Buf[20],1);
                    ACSIIData.Data_String[2]='\r';
                    ACSIIData.Data_String[3]='\n' ;
                    Stat=CH376ByteWrite(ACSIIData.Data_String,4,NULL);
                    CH376FileClose(TRUE);

                    Stat =CH376FileOpen(UserConfiguration);
                    Stat =CH376FileOpen(FileName);//打开集中器配置文件JZQCOFG.TXT
                    CH376ByteLocate(SD_ALLMETERNUM_OFFSET);//移动文件指针
                    Stat =CH376ByteRead(buf,4, NULL);
					CH376FileClose(TRUE);
                    return Success;
                }
                else
                {
                    FileEquFlag=1;
                }
                pDir -> DIR_Attr = 0;  /* 强制文件名字符串结束以便打印输出 */
            }
            xWriteCH376Cmd( CMD0H_FILE_ENUM_GO );  /* 继续枚举文件和目录 */
            xEndCH376Cmd( );
            Stat = Wait376Interrupt( );
        }
        if(FileEquFlag==1)	//没有找到采集器
        {
            return NotFindCJQInSD;
        }
    }
   return NotFindConfgFolder;
}
s32 Read_Installed_CJQAddr(u8 *Buf,u16 Len)
{
    u8 i=0,j=0;

    u8 CJQNum=0,Count=0,Num=0;
    u8 CJQIndex[50];
    u8 CJQAddr[120];//存储采集器地址15*8；
    SPI_Flash_Read(CJQ_NUM,&CJQNum,1);//读取flash中采集器数量
    SPI_Flash_Read(CJQ_INDEX,CJQIndex,CJQNum);//读取flash中采集器索引

    if(CJQNum==0)  //没有添加采集器
    {
        Right_Ack_Len0(Buf,Len);
        return Failed;
    }
    else
    {
        for(i=0; i<CJQ_Num; i++)
        {
            if(Count<CJQNum)
            {
                if(CJQIndex[i]!=0xff)
                {
                    if(Num==15)
                    {
                        Buf[1]=0x80;
                        Buf[29]=8*Num;
                        for(j=0; j<120; j++)
                        {
                            Buf[30+j]=CJQAddr[j];
                        }
                        Buf[30+j]=CRC8(Buf,30+j);
                        Buf[31+j]=0x16;
                        _delay_ms(500);
                        Send_UART(G_UART_Num,Buf,j+32);
                        Num=0;
                    }
                    SPI_Flash_Read(CJQ_ADDR+i*8,&CJQAddr[Num*8],8);
                    Num++;
                    Count++;
                }

            }
            else if((Count==CJQNum)&&(Num!=0))
            {
                Buf[1]=0x80;
                Buf[29]=8*Num;
                for(j=0; j<8*Num; j++)
                {
                    Buf[30+j]=CJQAddr[j];
                }
                Buf[30+j]=CRC8(Buf,30+j);
                Buf[31+j]=0x16;
                _delay_ms(500);
                Send_UART(G_UART_Num,Buf,j+32);
                Count++;
                break;
            }
            else break;
        }
    }
    return Success;
}
s32 Deal_One_CJQ(u8 *Buf,u16 Len,u8 cmd)
{
    u8 Stat1=0,Stat2=0;
	if(Len!=32)
    {
        Err_Ack(Buf,IllegalDataFormat);
        return Failed;
    }
    Stat1=Deal_One_CJQ_Flash(Buf,Len,cmd);
    if(Stat1==Success)//对flash操作正确的情况下，才允许对SD卡操作
    {
        Stat2=Deal_One_CJQ_SD(Buf,Len,cmd);
        if(Stat2==Success)
        {
            Right_Ack_Len0(Buf,Len);
            return Success;
        }
        else
        {
            Err_Ack(Buf,Stat2);	//返回SD卡中反馈的错误信息
            return Failed;
        }
    }
    else
    {
        Err_Ack(Buf,Stat1);	//返回外部flash中反馈的错误信息
        return Failed;
    }

}
/*******************************************************************************
* Function Name  : CJQ_Addr_Check
* Description    : 是否归属于该集中器
* Input          : meter_add 表具地址指针
* Output         : None
* Return         : -1 没有此表具；否则返回对应表具索引
*******************************************************************************/
s32 CJQ_Addr_Check(u8 *Buf)
{
	u8 i=0;
    u8 CJQNum=0,Count=0;
    u8 CJQIndex[50];
	u8 Check_Code;
    SPI_Flash_Read(CJQ_NUM,&CJQNum,1);//读取flash中采集器数量
    SPI_Flash_Read(CJQ_INDEX,CJQIndex,CJQNum);//读取flash中采集器索引
    if(CJQNum==0)  //没有添加采集器
    {
        return Failed;
    }
	Check_Code=CRC8(Buf,8);
	for(i=0; i<CJQ_Num; i++)
	{
	    if(Count<CJQNum)
	    {
	        if(CJQIndex[i]!=0xff)
	        {
	            if(Check_Code==CJQIndex[i])
					{
						u8 tmp[8];
						SPI_Flash_Read(CJQ_ADDR+i*8,tmp,8);
						if(memcmp(tmp,Buf,8)==0)
							return i;
					}
	            Count++;
	        }
	    }
		else
			return Failed;
	}
	return Failed;
}
/*******************************************************************************
* Function Name  : Meter_Addr_Check
* Description    : 是否归属于该集中器
* Input          : meter_add 表具地址指针
* Output         : None
* Return         : -1 没有此表具；否则返回对应表具索引
*******************************************************************************/
s32 Meter_Addr_Check(u8 *Meteradd,u8 CJQSpacePosition)
{
    u8 i=0;
    u8 MeterNum=0;
    u8 Count=0;
    u8 MeterIndex;
	u8 meterindex[255];
	u8 ttt[8];
	memcpy(ttt,Meteradd,8);
    MeterIndex=CRC8(Meteradd+1,7);
	/***************查找表具**********************************/
    SPI_Flash_Read(CJQSpacePosition+CJQ_METER_NUM,&MeterNum,1); //读出表具数量
    SPI_Flash_Read(CJQ_METER_INDEX+(CJQSpacePosition*METERINDEX_TO_METERINDEX), meterindex,255);	//表具索引
	if(MeterNum==0)
		return Failed;
    /*********查询是否已经存在此表************/
    for(i=0; i<255; i++)
    {
        if(Count<MeterNum)
        {
            if( meterindex[i]!=0xff)
            {
                if( meterindex[i]==MeterIndex)
                {
                    u8 buf[8];
					SPI_Flash_Read(i*8+CJQ_METER_ADDR+(CJQSpacePosition*METERINDEX_TO_METERINDEX),buf,8); //读出表具地址
                    if(memcmp(buf,Meteradd,8)==0)
						return i;
                }
            }
            Count++;
        }
        else 
			return Failed;
    }
	return Failed;

}
/***********************************************************************************
* Function Name  : JZQ_Flash_SD_Store
* Description    : 集中器FLASH,SD卡中数据存储
* Input          : *Buf：上位机发送的命令帧
* Output         : None
* Return         : return 0：采集器地址不正确
				   return 1：外部flash中采集器地址正确
				   return 2：外部flash中采集器地址不正确，内部flash中采集器地址正确
************************************************************************************/
s32 JZQ_Flash_SD_Store(u8 *DataBuf,u16 Len)
{
	u8 i;	
	u8 G_8563_RealTime[6];
	s16 meter_i;
	u8 Level1Folder[6];//年 月  	一级文件夹
	u8 Level2Folder[7];//年 月 日 二级文件夹
	u8 FileName[13];// 采集器地址低4位作为文件名
	u8 Content[59]="集中器地址,采集器地址,表具类型,表具地址,实时时间,表具数据\r\n"	;
	u8 Addr[18];
	u32 FileSize=0;
	UINT8 STAT=0;
	s8 cjq_i=CJQ_Addr_Check(DataBuf+2);
	if(cjq_i==Failed)return Failed;
	meter_i=Meter_Addr_Check(DataBuf+11,cjq_i);
	if(meter_i==Failed)return Failed;
	else
	{
		u8 *p=malloc(19*2);
		SPI_Flash_Write(CJQ_METER_DATA+(cjq_i*METERINDEX_TO_METERINDEX)+meter_i*2048,DataBuf+21,19);
		P8563_gettime(G_8563_RealTime);
		sprintf((char *)Level1Folder,"/%02x%02x\0",G_8563_RealTime[5],G_8563_RealTime[4]);
		sprintf((char *)Level2Folder,"%02x%02x%02x\0",G_8563_RealTime[5],G_8563_RealTime[4],G_8563_RealTime[3]);
		sprintf((char *)FileName,"%02x%02x%02x%02x.TXT\0",DataBuf[14],DataBuf[13],DataBuf[12],DataBuf[11]);
		STAT=CH376DirCreate( Level1Folder );  /* 在根目录下新建目录(文件夹)并打开,如果目录已经存在那么直接打开 */
		STAT=CH376DirCreate( Level2Folder );  /* 在根目录或者当前目录下新建文件,如果文件已经存在那么先删除 */
		STAT=CH376FileClose( TRUE ); 
		STAT=CH376FileOpen(Level1Folder ); 
		STAT=CH376FileOpen(Level2Folder ); 
		STAT=CH376FileOpen(FileName);  /* 在根目录或者当前目录下打开文件或者目录(文件夹) */
		if(STAT!=0x14)//不能成功打开默认为此文件不存在，新建文件并打开
		{
			STAT=CH376FileCreate( FileName);  /* 在根目录或者当前目录下新建文件,如果文件已经存在那么先删除 */
			STAT=CH376FileClose( TRUE ); 
			STAT=CH376FileOpen(Level1Folder ); 
			STAT=CH376FileOpen(Level2Folder ); 
			STAT=CH376FileOpen(FileName);  /* 在根目录或者当前目录下打开文件或者目录(文件夹) */
		}
		FileSize=CH376GetFileSize(  );  /* 读取当前文件长度 */
		CH376ByteLocate(FileSize);//移动文件指针
		if(FileSize<10)
		{ 
		    CH376ByteLocate(0);//移动文件指针
			STAT=CH376ByteWrite( Content,59, NULL); //文件头
		}
		sprintf((char *)Addr,"%02x%02x%02x%02x%02x%02x%02x%02x,",G_JZQ.ADDR[0],G_JZQ.ADDR[1],G_JZQ.ADDR[2],G_JZQ.ADDR[3],G_JZQ.ADDR[4],G_JZQ.ADDR[5],G_JZQ.ADDR[6],G_JZQ.ADDR[7]);	
		STAT=CH376ByteWrite(Addr,17, NULL);	//集中器地址
		sprintf((char *)Addr,"%02x%02x%02x%02x%02x%02x%02x%02x,",DataBuf[2],DataBuf[3],DataBuf[4],DataBuf[5],DataBuf[6],DataBuf[7],DataBuf[8],DataBuf[9]);
		STAT=CH376ByteWrite(Addr,17, NULL);//采集器地址
		sprintf((char *)Addr,"%02x,",DataBuf[11]);
		STAT=CH376ByteWrite(Addr,3, NULL);//表具类型
		sprintf((char *)Addr,"%02x%02x%02x%02x%02x%02x%02x,",DataBuf[12],DataBuf[13],DataBuf[14],DataBuf[15],DataBuf[16],DataBuf[17],DataBuf[18]);
		STAT=CH376ByteWrite(Addr,15, NULL);//表具地址
		sprintf((char *)Addr,"%02x/%02x/%02x %02x:%02x:%02x,",G_8563_RealTime[5],G_8563_RealTime[4],G_8563_RealTime[3],G_8563_RealTime[2],G_8563_RealTime[1],G_8563_RealTime[0]);
		STAT=CH376ByteWrite(Addr,18, NULL);//实时时间
		for(i=0;i<19;i++)
		{
			sprintf((char *)p+i*2,"%02x",DataBuf[21+i]);
		}	
		STAT=CH376ByteWrite( p,19*2, NULL); //表具数据
		STAT=CH376ByteWrite("\r\n",2, NULL);//
		CH376FileClose( TRUE );  /* 关闭当前已经打开的文件或者目录(文件夹) */
		free(p);
	}
	return Success;
}
/***********************************************************************************
* Function Name  : Read_JZQ_Flash_Data
* Description    : 读取集中器中一台采集器中的一台表具的当前（集中器中的）数据信息
* Input          : *Buf：上位机发送的命令帧
* Output         : None
* Return         : return 0：采集器地址不正确
				   return 1：外部flash中采集器地址正确
				   return 2：外部flash中采集器地址不正确，内部flash中采集器地址正确
************************************************************************************/
s32 Read_JZQ_Flash_Data(u8 *DataBuf,u16 Len)
{
	s32 meter_i,cjq_i;
	u8 tmp[51];
	if(Len!=32)
		return Failed;
	cjq_i=CJQ_Addr_Check(DataBuf+11);
	if(cjq_i==Failed)return Failed;
	meter_i=Meter_Addr_Check(DataBuf+20,cjq_i);
	if(meter_i==Failed)return Failed;
	memcpy(tmp,DataBuf,Len);
	SPI_Flash_Read(CJQ_METER_DATA+(cjq_i*METERINDEX_TO_METERINDEX)+meter_i*2048,tmp+30,19);
	tmp[1]=0x80;
	tmp[29]=19;
	tmp[49]=CRC8(tmp,49);
	tmp[50]=0x16;
	Send_UART(G_UART_Num,tmp,51);
	return Success;
}
/***********************************************************************************
* Function Name  : Back_JZQ_Work_Mode
* Description    : 读取集中器中一台采集器中的一台表具的当前（集中器中的）数据信息
* Input          : *Buf：上位机发送的命令帧
* Output         : None
* Return         : return 0：采集器地址不正确
				   return 1：外部flash中采集器地址正确
				   return 2：外部flash中采集器地址不正确，内部flash中采集器地址正确
************************************************************************************/
s32 Back_JZQ_Work_Mode(u8 *DataBuf,u16 Len)
{
	u8 tmp[33];
	if(Len!=32)
		return Failed;
	memcpy(tmp,DataBuf,Len);
	tmp[1]=0x80;
	tmp[29]=1;
	tmp[30]=GetWorkingMode();
	tmp[31]=CRC8(tmp,31);
	tmp[32]=0x16;
	Send_UART(G_UART_Num,tmp,33);
	return Success;
}
/***********************************************************************************
* Function Name  : DEL_JZQ_Meter_Data
* Description    : 清除集中器中一台采集器内所有表具的数据信息，但保留表具的编号信息。
* Input          : *Buf：上位机发送的命令帧
* Output         : None
* Return         : return 0：采集器地址不正确
				   return 1：外部flash中采集器地址正确
				   return 2：外部flash中采集器地址不正确，内部flash中采集器地址正确
************************************************************************************/
s32 DEL_JZQ_Meter_Data(u8 *DataBuf,u16 Len)
{
	u8 i;
	u32 tmp;
	s32 cjqindex; 
	if(Len!=32)
		return Failed;
	cjqindex=CJQ_Addr_Check(DataBuf+11);
	if(cjqindex==Failed)
		return Failed;
	for(i=0;i<128;i++)
	{
		tmp=CJQ_METER_DATA+(cjqindex*METERINDEX_TO_METERINDEX)+0x1000*i;
		SPI_Flash_Erase_Sector(tmp/4096);
	}	
	return Success;
}
/***********************************************************************************
* Function Name  : DEL_CJQ_Meters
* Description    : 删除集中器中一台采集器内的全部管理表具的编号及其数据信息
* Input          : *Buf：上位机发送的命令帧
* Output         : None
* Return         : return 0：采集器地址不正确
				   return 1：外部flash中采集器地址正确
				   return 2：外部flash中采集器地址不正确，内部flash中采集器地址正确
************************************************************************************/
s32 DEL_CJQ_Meters(u8 *Buf,u16 Len)
{
	u8 MeterNum=0,meterindex[255],CJQSpacePosition,i,tmp;
	if(Len!=32)
		return Failed;
	MeterNum=0;
	memset(meterindex,0xff,255);
	CJQSpacePosition=CJQ_Addr_Check(Buf+11);
	SPI_Flash_Write(CJQSpacePosition+CJQ_METER_NUM,&MeterNum,1); //读出表具数量
    SPI_Flash_Write(CJQ_METER_INDEX+(CJQSpacePosition*METERINDEX_TO_METERINDEX), meterindex,255);	//表具索引
	for(i=0;i<128;i++)
	{
		tmp=CJQ_METER_DATA+(CJQSpacePosition*METERINDEX_TO_METERINDEX)+0x1000*i;
		SPI_Flash_Erase_Sector(tmp/4096);
	}
	Right_Ack_Len0(Buf,Len);
	return Success;
}
/***********************************************************************************
* Function Name  : Back_JZQ_JieZhuan_Time
* Description    : 读取集中器数据结转时间
* Input          : *Buf：上位机发送的命令帧
* Output         : None
* Return         : 
************************************************************************************/
s32 Back_JZQ_JieZhuan_Time(u8 *Buf,u16 Len)
{
	u8 tmp[35],tmp_t[3];

	if(Len!=35)
		return Failed;
	memcpy(tmp,Buf,Len);
    SPI_Flash_Read(JZQ_DATA_CARRY_TIME,tmp_t,3);
	memcpy(tmp+30,tmp_t,3);
	tmp[1]=0x80;
	tmp[29]=3;
	tmp[33]=CRC8(tmp,33);
	tmp[34]=0x16;
	Send_UART(G_UART_Num,tmp,35);	
	return Success;
}
/*******************************************************************************
* Function Name  : ChangeWorkingMode
* Description    : 
* Input          : Buf：数据数组，Len：数据长度
* Output         : None
* Return         : None
*******************************************************************************/
s32 ChangeWorkingMode(u8 *Buf,u16 Len)
{
    u8 WrkMode;
	if(Len!=33)
	{
		Err_Ack(Buf,CmdErr);
        return Failed;
	}
	WrkMode=Buf[30];
    SPI_Flash_Write(JZQ_WORKMODE,&WrkMode,1);//读取外部flash中工作模式位
	CH376FileOpen(UserConfiguration);
	CH376FileOpen(FileName);
	CH376ByteLocate(strlen((char *)UserConfiguration)+43);
	WrkMode=WrkMode+0x30;
	CH376ByteWrite(&WrkMode,1,NULL);
	CH376FileClose( TRUE );
	Right_Ack_Len0(Buf,Len);
    return WrkMode;
}
/*******************************************************************************
* Function Name  : ChangeJieZhuan_Time
* Description    : 
* Input          : Buf：数据数组，Len：数据长度
* Output         : None
* Return         : None
*******************************************************************************/
s32 ChangeJieZhuan_Time(u8 *Buf,u16 Len)
{
	u8 tmp_t[3],time[10];
	if(Len!=35)
	{
		Err_Ack(Buf,CmdErr);
        return Failed;
	}
	memcpy(tmp_t,Buf+30,3);
    SPI_Flash_Write(JZQ_DATA_CARRY_TIME,tmp_t,3);//读取外部flash中工作模式位
	CH376FileOpen(UserConfiguration);
	CH376FileOpen(FileName);
	CH376ByteLocate(strlen((char *)UserConfiguration)+45);
	sprintf((char *)time,"%02x:%02x:%02x",Buf[30],Buf[31],Buf[32]);
	CH376ByteWrite(time,8,NULL);
	CH376FileClose( TRUE );
	Right_Ack_Len0(Buf,Len);	
    return Success;
}
/*******************************************************************************
* Function Name  : Return_Meter_History_Data
* Description    : 
* Input          : Buf：数据数组，Len：数据长度
* Output         : None
* Return         : None
*******************************************************************************/
s32 Return_Meter_History_Data(u8 *Buf,u16 Len)
{
	u8 tmp[20],data[128],local_i;
	u8 meter_add[15];
	u32 file_size,head;
	if(Len!=36)
	{
		Err_Ack(Buf,CmdErr);
        return Failed;
	}
	sprintf((char *)tmp,"/%02x%02x\0",Buf[31],Buf[32]);
	CH376FileOpen(tmp);
	sprintf((char *)tmp,"%02x%02x%02x\0",Buf[31],Buf[32],Buf[33]);
	CH376FileOpen(tmp);
	sprintf((char *)tmp,"%02x%02x%02x%02x.TXT\0",Buf[14],Buf[13],Buf[12],Buf[11]);
	CH376FileOpen(tmp);
	head=strlen("集中器地址,采集器地址,表具类型,表具地址,实时时间,表具数据\r\n");
	CH376ByteLocate(head);
	file_size=CH376GetFileSize();
	sprintf((char *)meter_add,"%02x%02x%02x%02x%02x%02x%02x\0",Buf[21],Buf[22],Buf[23],Buf[24],Buf[25],Buf[26],Buf[27]);
	do
	{
		memset(data,0,128);
		local_i=CH376ByteRead(data,110,NULL);
		if(strstr((char *)data,(char *)meter_add)!=NULL)
		{
		 	u8 cmd[55];
			memcpy(cmd,Buf,Len);
			memcpy(cmd+34,data+89,19);
			cmd[1]=0x80;
			cmd[29]=23;
			cmd[53]=CRC8(cmd,53);
			cmd[54]=0x16;
			Send_UART(G_UART_Num,cmd,55);
			CH376FileClose( TRUE );
			return Success;
		}
		if(local_i<110)
		{	
			CH376FileClose( TRUE );
			return Failed;
		}
		head=head+local_i;
		if(head>file_size)
		{	
			CH376FileClose( TRUE );
			return Failed;
		}	
		CH376ByteLocate(head);
	}while(1);
}
