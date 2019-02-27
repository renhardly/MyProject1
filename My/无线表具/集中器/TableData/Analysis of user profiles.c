#include "stm32f10x_lib.h"
#include "platform_config.h"
#include "FILE_SYS.H"
#include <string.h>

/*******************************************************************************
* Function Name  : Analysis_user_profiles
* Description    : 用户数据分析（表地址和表协议）
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Analysis_user_profiles(void)
	{
	u8 i=0,j=0,ch=0,s=0;
	char SrcName[64]="",ch_len=0;
	u32 file_pt=0,file_len_R=0,addr;
	u16 User_Num=0;
	static char ch_buf[25];
	u8 Table_Addr[7];

	/* 擦除指定位置的扇区数据*/
	SPI_FLASH_SectorErase(0);//Flash扇区擦除
/*	while (CH376DiskConnect() != USB_INT_SUCCESS)
			{  //检查U盘是否连接,等待U盘插入,对于SD卡,可以由单片机直接查询SD卡座的插拔状态引脚
			mDelaymS(100 );
			}*/
	IWDG_ReloadCounter();
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==1)//检查SD卡是否连接,等待SD卡插入
		{
		mDelaymS(100 );
		IWDG_ReloadCounter();
		}
		strcpy(SrcName,"\\用户配置.TXT");
		s = CH376FileOpen((u8 *)SrcName );  /* 打开文件,该文件在C51子目录下 */
		if ((s != ERR_MISS_DIR) && (s != ERR_MISS_FILE))
			{ 
			/*文件指针指向第二行*/
			file_len_R=CH376GetFileSize();//计算文件长度
			file_pt=0;
			User_Num=0;
			do
				{
				CH376ByteRead( &ch,1,NULL );
				file_pt++;
				}
				while(ch!='\n');//换行符
			/*开始分析文件数据*/
			do
				{
				IWDG_ReloadCounter();
				ch_len=0;
				for(j=0;j<25;j++)
					ch_buf[j]=0;
				do
					{
					CH376ByteRead(&ch,1,NULL );
					file_pt++;
					ch_buf[ch_len++]=ch;
					if(ch=='\n') break;
					}
					while(ch!=',');//分割符	
				ch_buf[ch_len-1]=0;
				switch(i)
					{
					case 0 : 
							for(j=0;j<14;j++)
								if((ch_buf[j]>='0')&&(ch_buf[j]<='9'))
									ch_buf[j]-=0x30;		//将ASCII字符转换成BCD码
									else ch_buf[j]=0;
							Table_Addr[0]=(ch_buf[12]<<4)|ch_buf[13];
							Table_Addr[1]=(ch_buf[10]<<4)|ch_buf[11];
							Table_Addr[2]=(ch_buf[8]<<4)|ch_buf[9];
							Table_Addr[3]=(ch_buf[6]<<4)|ch_buf[7];
							Table_Addr[4]=(ch_buf[4]<<4)|ch_buf[5];
							Table_Addr[5]=(ch_buf[2]<<4)|ch_buf[3];
							Table_Addr[6]=(ch_buf[0]<<4)|ch_buf[1];
							SPI_FLASH_BufferWrite(Table_Addr,128+User_Num*15,7);//表具编码
							break;
					case 1 : 
						//	ch_buf[ch_len-2]=0;
							if(!strcmp(ch_buf,"卡姆鲁普401")) ch_buf[0]=1;//13757协议
							if(!strcmp(ch_buf,"卡姆鲁普402")) ch_buf[0]=2;//13757协议
							if(!strcmp(ch_buf,"兰吉尔")) 	  ch_buf[0]=3;//13757协议
							if(!strcmp(ch_buf,"188")) 		  ch_buf[0]=4;//13757协议
							SPI_FLASH_BufferWrite((u8 *)ch_buf,128+User_Num*15+7,1);//表协议
							break;
					case 2 : 
							for(j=0;j<8;j++)
								if((ch_buf[j]>='0')|(ch_buf[j]<='9'))
									ch_buf[j]-=0x30;		//将ASCII字符转换成BCD码
							addr=ch_buf[0];
							for(j=1;j<ch_len-2;j++)
								{
								addr<<=4;
								addr|=ch_buf[j];
								}
							Table_Addr[0]=(addr>>24)&0xff;//高字节
							Table_Addr[1]=(addr>>16)&0xff;
							Table_Addr[2]=(addr>>8)&0xff;
							Table_Addr[3]=addr&0xff;//低字节
							SPI_FLASH_BufferWrite(Table_Addr,128+User_Num*15+8,4);//采集器地址
							break;
					}
				if(i>=2){i=0;User_Num++;}
					else i++;
				if(file_pt>=file_len_R-2) 
					break;//文件结尾
				}while(1);
			s = CH376FileClose( TRUE );//关闭打开的文件
			mStopIfError( s );
			}
	ch_buf[0]=User_Num>>8;
	ch_buf[1]=User_Num&0xff;
	SPI_FLASH_BufferWrite((u8 *)ch_buf,0,2);//记录一共需要读取多少块表具
	}
