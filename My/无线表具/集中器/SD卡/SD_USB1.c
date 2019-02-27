#include "stm32f10x_lib.h"
#include "FILE_SYS.H"
#include "platform_config.h"
extern u8 data_buf[50];
/*******************************************************************************
* Function Name  : SD_USB
* Description    : 从SD卡中将文件拷贝到U盘
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SD_USB(void)
	{
	u8 i,j,s,c;
	static u8 buf[256];
	u32 file_pt,file_p,page_len;
	char SrcName[64]="\\20120101.CSV";
	
	SrcName[1]=(data_buf[1]/10)+0x30;
	SrcName[2]=(data_buf[1]%10)+0x30;
	SrcName[3]=(data_buf[2]/10)+0x30;
	SrcName[4]=(data_buf[2]%10)+0x30;
	
	for(i=11;i<=16;i++)//11月至来年4月
			{
			if(i!=12)
				c=i%12;
			SrcName[5]=(c/10)+0x30;
			SrcName[6]=(c%10)+0x30;
			for(j=1;j<=31;j++)
				{
				IWDG_ReloadCounter();
			//	mDelaymS(100 );
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==1)//检查SD卡是否连接,等待SD卡插入
					mDelaymS(100 );
				SrcName[7]=(j/10)+0x30;
				SrcName[8]=(j%10)+0x30;
				SPI_FLASH_SectorErase(0);//Flash扇区擦除
		/*********************读出SD卡的文件***************************/
				s = CH376FileOpen((u8 *)SrcName );  // 打开文件
				if ((s != ERR_MISS_DIR) && (s != ERR_MISS_FILE))
					{ 
					file_pt=CH376GetFileSize();//计算文件长度
					page_len=0;
					file_p=0;
					do
						{
						IWDG_ReloadCounter();
						CH376ByteRead(&c,1,NULL );
						buf[file_p%256]=c;
						file_p++;
						if((file_p%256)==0)
							{
							//CAN_LED_ON();
							IWDG_ReloadCounter();
							SPI_FLASH_BufferWrite((u8 *)buf,(page_len++)*256,256);
							//CAN_LED_OFF();
							}
						if(file_pt<=file_p)
							{
							IWDG_ReloadCounter();
							if((file_p%256)!=0)
								{
								//CAN_LED_ON();
								SPI_FLASH_BufferWrite((u8 *)buf,(page_len++)*256,file_p%256);
								//CAN_LED_OFF();
								}
							break;
							}
						}while(1);
					s = CH376FileClose( TRUE );//关闭打开的文件
					mStopIfError( s );
					}
					else continue;
			/*********************向USB设备写入数据***************************/
				xWriteCH376Cmd( CMD11_SET_USB_MODE );  // 设备USB工作模式 
				xWriteCH376Data( 0x06 );//模式代码为06H时切换到已启用的 USB 主机方式，自动产生 SOF 包	
		//		mDelaymS(100 );
				IWDG_ReloadCounter();
				while(CH376DiskConnect() != 0x14) //检测到USB设备
					mDelaymS(100 );
				s=CH376FileCreate((u8 *)SrcName);//在根目录或者当前目录下新建文件,如果文件已经存在那么先删除
				if ((s != ERR_MISS_DIR) && (s != ERR_MISS_FILE))
					{
					for(file_pt=0;file_pt<page_len;file_pt++)
						{
						IWDG_ReloadCounter();
						SPI_FLASH_BufferRead(buf,file_pt*256,256);//读取多少块表？
						if(file_pt==(page_len-1))
							{
						//	CAN_LED_ON();
							CH376ByteWrite((u8 *)buf,(file_p%256), NULL );// 以字节为单位向当前位置写入数据块
						//	CAN_LED_OFF();
							}	
							else 
								{
							//	CAN_LED_ON();
								CH376ByteWrite((u8 *)buf,256, NULL );// 以字节为单位向当前位置写入数据块 
							//	CAN_LED_OFF();
								}
						}
					s = CH376FileClose( TRUE );//关闭打开的文件
					mStopIfError( s );
					}
				xWriteCH376Cmd( CMD11_SET_USB_MODE );  // 设备USB工作模式 
				xWriteCH376Data( 0x03 );//模式代码为03H时切换到SD卡主机模式，用于管理和存取SD卡中的文件；
				}
			}
	xWriteCH376Cmd( CMD11_SET_USB_MODE );  /* 设备USB工作模式 */
	xWriteCH376Data( 0x03 );//模式代码为03H时切换到SD卡主机模式，用于管理和存取SD卡中的文件；
	Analysis_user_profiles();
	USB_LED_ON();
	while(CH376DiskConnect() == 0x14) //检测到USB设备
		{
		IWDG_ReloadCounter();
		mDelaymS(100 );
		}
	USB_LED_OFF();
	}
