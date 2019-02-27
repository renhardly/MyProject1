#include "stm32f10x_lib.h"
#include "platform_config.h"
#include "FILE_SYS.H"

/*******************************************************************************
* Function Name  : Upload_historical_data
* Description    : 上传历史数据
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Upload_historical_data(char *ch)
	{
	u8 s=0,c=0;
	u32 file_pt=0,file_p=0;
	u8 buf[256];
	char SrcName[64]="\\20120201.CSV";
	
	strcpy(SrcName,"\\用户配置.TXT");
//	strcpy(SrcName,"\\20120201.CSV");
	
	s = CH376FileOpen((u8 *)SrcName );  /* 打开文件,该文件在C51子目录下 */
	if ((s != ERR_MISS_DIR) && (s != ERR_MISS_FILE)&& (s == 0x14))
		{  
		file_pt=CH376GetFileSize();//计算文件长度
		do
		{
		CH376ByteRead(&c,1,NULL );
		buf[file_p%256]=c;
		file_p++;
		if((file_p%256)==0)
			send_uart(2,buf,256);//GPRS
		if(file_pt<=file_p)
			{
			if((file_p%256)!=0)
				send_uart(2,buf,file_p%256);//GPRS
			break;
			}
		}while(1);
		s = CH376FileClose( TRUE );//关闭打开的文件
		mStopIfError( s );
		}
	//	else return error;
	}
/*******************************************************************************
* Function Name  : ADD_Meter
* Description    : 添加表具
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Add_Meter(char *ch)
	{
	u8 s;
//	u16 User_Num;
	u32 file_pt;
	char buf[40]="123,卡姆鲁普401,1111";
//	u8 Table_Addr[7]={0},ch_buf[14]={0};
//	p=buf;
	
	s = CH376FileOpen((u8 *)"\\用户配置.TXT" );  //打开文件,该文件在C51子目录下
	if ((s != ERR_MISS_DIR) && (s != ERR_MISS_FILE))
		{  
		IWDG_ReloadCounter();
		file_pt=CH376GetFileSize();//计算文件长度
		CH376ByteLocate(file_pt);//移动文件指针
		
		strcat(buf, "\n");
		s = CH376ByteWrite((u8 *)buf, strlen(buf), NULL );//* 以字节为单位向当前位置写入数据块
		s = CH376FileClose( TRUE );  //* 关闭文件,对于字节读写建议自动更新文件长度
		mStopIfError( s );
		/*
		SPI_FLASH_BufferRead(ch_buf,0,2);//读取多少块表？
		User_Num=(ch_buf[0]<<8)|ch_buf[1];
		User_Num++;
		for(j=0;j<14;j++)
			{
			if(*p==',') {p++;break;}
			if((*p>='0')&&(*p<='9'))
				ch_buf[j]=*p-0x30;		//将ASCII字符转换成BCD码
				else ch_buf[j]=0;
			p++;
			}
			i=j;
		for(;j<14;j++) ch_buf[j]=0;
		
		addr=ch_buf[8];
		for(j=9;j<i;j++)
			{
			addr<<=4;
			addr|=ch_buf[j];
			}
		Table_Addr[0]=addr&0xff;
		Table_Addr[1]=(addr>>8)&0xff;
		Table_Addr[2]=(addr>>16)&0xff;
		addr=ch_buf[0];
		if(i>=8) i=8;
		for(j=1;j<i;j++)
			{
			addr<<=4;
			addr|=ch_buf[j];
			}
		Table_Addr[3]=addr&0xff;
		Table_Addr[4]=(addr>>8)&0xff;
		Table_Addr[5]=(addr>>16)&0xff;
		Table_Addr[6]=(addr>>24)&0xff;
		
		SPI_FLASH_BufferWrite(Table_Addr,128+User_Num*15,7);//表具编码
		if(!strncmp(p,"卡姆鲁普401",11)) Table_Addr[0]=1;//13757协议
		if(!strncmp(p,"卡姆鲁普402",11)) Table_Addr[0]=2;//13757协议
		if(!strncmp(p,"兰吉尔",6)) 	  	 Table_Addr[0]=3;//13757协议
		if(!strncmp(p,"188",3)) 		 Table_Addr[0]=4;//13757协议
		while(*(p++)!=',');
		SPI_FLASH_BufferWrite((u8 *)Table_Addr,128+User_Num*15+7,1);//表协议
	
		for(j=0;j<8;j++)
			{
			if(*p=='\0') {p++;break;}
			if((*p>='0')&&(*p<='9'))
				ch_buf[j]=*p-0x30;		//将ASCII字符转换成BCD码
				else ch_buf[j]=0;
			p++;
			}
		for(;j<8;j++) ch_buf[j]=0;
		addr=ch_buf[7];
		for(j=6;j>=0;j--)
			{
			addr<<=4;
			addr|=ch_buf[j];
			if(j==0) break;
			}
		Table_Addr[0]=(addr>>24)&0xff;//高字节
		Table_Addr[1]=(addr>>16)&0xff;
		Table_Addr[2]=(addr>>8)&0xff;
		Table_Addr[3]=addr&0xff;//低字节
		SPI_FLASH_BufferWrite(Table_Addr,128+User_Num*15+8,4);//采集器地址*/
		}
	Analysis_user_profiles();//读取表地址，表协议
	}
/*******************************************************************************
* Function Name  : Delete_Meter
* Description    : 删除表具
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Delete_Meter(char *p)
	{
	u8 s,flash_buf[15],j;
	char buf[30],ch[7]={0x60,0x20,0x25,0x19};
	u16 User_Num,i;

	s =CH376FileCreate( (u8 *) "\\用户配置.TXT" );
	mStopIfError( s );
	SPI_FLASH_BufferRead(flash_buf,0,2);//读取多少块表？
	User_Num=(flash_buf[0]<<8)|flash_buf[1];
	for(i=0;i<User_Num;i++) //循环，连续发送读取表具指令
		{//读取用户仪表地址、仪表类型、仪表协议
		SPI_FLASH_BufferRead(flash_buf,128+i*15,15);//读取多少块表？
		if(!((flash_buf[0]==ch[6])&&(flash_buf[1]==ch[5])&&(flash_buf[2]==ch[4])&&(flash_buf[3]==ch[3])
					&&(flash_buf[4]==ch[2])&&(flash_buf[5]==ch[1])&&(flash_buf[6]==ch[0])))
			{
			if(flash_buf[7]==4)
				{
				buf[0]=((flash_buf[6]>>4)&0x0f)+0x30;
				buf[1]=(flash_buf[6]&0x0f)+0x30;
				buf[2]=((flash_buf[5]>>4)&0x0f)+0x30;
				buf[3]=(flash_buf[5]&0x0f)+0x30;
				buf[4]=((flash_buf[4]>>4)&0x0f)+0x30;
				buf[5]=(flash_buf[4]&0x0f)+0x30;
				buf[6]=((flash_buf[3]>>4)&0x0f)+0x30;
				buf[7]=(flash_buf[3]&0x0f)+0x30;
				buf[8]=((flash_buf[2]>>4)&0x0f)+0x30;
				buf[9]=(flash_buf[2]&0x0f)+0x30;
				buf[10]=((flash_buf[1]>>4)&0x0f)+0x30;
				buf[11]=(flash_buf[1]&0x0f)+0x30;
				buf[12]=((flash_buf[0]>>4)&0x0f)+0x30;
				buf[13]=(flash_buf[0]&0x0f)+0x30;
				buf[14]=',';
				buf[15]=0;
				}
				else 
				{
				buf[0]=((flash_buf[6]>>4)&0x0f)+0x30;
				buf[1]=(flash_buf[6]&0x0f)+0x30;
				buf[2]=((flash_buf[5]>>4)&0x0f)+0x30;
				buf[3]=(flash_buf[5]&0x0f)+0x30;
				buf[4]=((flash_buf[4]>>4)&0x0f)+0x30;
				buf[5]=(flash_buf[4]&0x0f)+0x30;
				buf[6]=((flash_buf[3]>>4)&0x0f)+0x30;
				buf[7]=(flash_buf[3]&0x0f)+0x30;
				buf[8]=',';
				buf[9]=0;
				}
			s = CH376ByteWrite((u8 *)buf,strlen(buf), NULL );/* 以字节为单位向当前位置写入数据块 */
			switch(flash_buf[7])
				{
				case 1 :CH376ByteWrite("卡姆鲁普401,",strlen("卡姆鲁普401,"), NULL );/* 以字节为单位向当前位置写入数据块 */
						break;
				case 2 :CH376ByteWrite("卡姆鲁普402,",strlen("卡姆鲁普402,"), NULL );/* 以字节为单位向当前位置写入数据块 */
						break;
				case 3 :CH376ByteWrite("兰吉尔,",strlen("兰吉尔,"), NULL );/* 以字节为单位向当前位置写入数据块 */
						break;
				case 4 :CH376ByteWrite("188,",strlen("188,"), NULL );/* 以字节为单位向当前位置写入数据块 */
						break;
				}
			for(j=0;j<30;j++)
				buf[j]=0;
			buf[0]=((flash_buf[8]>>4)&0x0f)+0x30;
			buf[1]=(flash_buf[8]&0x0f)+0x30;
			buf[2]=((flash_buf[9]>>4)&0x0f)+0x30;
			buf[3]=(flash_buf[9]&0x0f)+0x30;
			buf[4]=((flash_buf[10]>>4)&0x0f)+0x30;
			buf[5]=(flash_buf[10]&0x0f)+0x30;
			buf[6]=((flash_buf[11]>>4)&0x0f)+0x30;
			buf[7]=(flash_buf[11]&0x0f)+0x30;
			strcat(buf,"\n");

			s = CH376ByteWrite((u8 *)buf,strlen(buf), NULL );/* 以字节为单位向当前位置写入数据块 */
			
			}
		}
		s = CH376FileClose( TRUE );  /* 关闭文件,对于字节读写建议自动更新文件长度 */
		mStopIfError( s );
		Analysis_user_profiles();//读取表地址，表协议
	}
/*******************************************************************************
* Function Name  : Modify_Meter
* Description    : 修改表具
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Modify_Meter(char *ch)
	{
	
	}
/*******************************************************************************
* Function Name  : Modify_Addr
* Description    : 修改集中器地址
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Modify_Addr(char *ch)
	{
//	I2C_BufferWrite(buf1,19,0,0xa1);
	}
/*******************************************************************************
* Function Name  : Clock_synchronization
* Description    : 集中器时钟同步
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Clock_synchronization(char *ch)
	{
	u8* pBuffer;
	I2C_BufferWrite(pBuffer,7,2,0xA3);
	}
