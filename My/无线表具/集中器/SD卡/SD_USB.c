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
	u8 s,c,sector;
	static u8 buf[256];
	u32 file_pt,file_p,page_len,File_Quantity,i;
	char SrcName[64]="\\20120101.CSV";
	P_FAT_DIR_INFO	pDir;
	
	s = CH376FileOpenPath((u8 *)"\\*.CSV");  /* 枚举多级目录下的文件或者目录,输入缓冲区必须在RAM中 */
	File_Quantity=0;
	IWDG_ReloadCounter();
	SPI_FLASH_SectorErase(655360);//Flash扇区擦除
	SPI_FLASH_SectorErase(720896);//Flash扇区擦除
	while (s == USB_INT_DISK_READ)
		{	/* 枚举到匹配的文件 */
		IWDG_ReloadCounter();
		CH376ReadBlock((u8 *)buf);  /* 读取枚举到的文件的FAT_DIR_INFO结构,返回长度总是sizeof( FAT_DIR_INFO ) */
		pDir = (P_FAT_DIR_INFO)buf;  /* 当前文件目录信息 */
		if (pDir -> DIR_Name[0] != '.')
			{  /* 不是本级或者上级目录名则继续,否则必须丢弃不处理 */
			if ( pDir -> DIR_Name[0] == 0x05 )
				{
				pDir -> DIR_Name[0] = 0xE5;  /* 特殊字符替换 */
				}
			/*检查是否是.CSV文件,"."在路径中不占用空间*/
			if((pDir -> DIR_Name[8]=='C')&(pDir -> DIR_Name[9]=='S')&(pDir -> DIR_Name[10]=='V'))
				{
				IWDG_ReloadCounter();
				SPI_FLASH_BufferWrite((u8 *)pDir -> DIR_Name,File_Quantity*8+655360,8);
				File_Quantity++;
				}
			pDir -> DIR_Attr = 0;  /* 强制文件名字符串结束以便打印输出 */
			}
		xWriteCH376Cmd( CMD0H_FILE_ENUM_GO );  /* 继续枚举文件和目录 */
		xEndCH376Cmd( );
		s = Wait376Interrupt( );
		}
		for(i=0;i<File_Quantity;i++)
	/*------------------------------------------------------------------------------------------*/		
			{
			SPI_FLASH_BufferRead(buf,i*8+655360,8);
			SrcName[1]=buf[0];
			SrcName[2]=buf[1];
			SrcName[3]=buf[2];
			SrcName[4]=buf[3];
			SrcName[5]=buf[4];
			SrcName[6]=buf[5];
			SrcName[7]=buf[6];
			SrcName[8]=buf[7];
			IWDG_ReloadCounter();
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==1)//检查SD卡是否连接,等待SD卡插入
				{mDelaymS(100 );IWDG_ReloadCounter();}
			mDelaymS(200 );
			/*********************读出SD卡的文件***************************/
			s = CH376FileOpen((u8 *)SrcName );  // 打开文件
			if ((s != ERR_MISS_DIR) && (s != ERR_MISS_FILE))
				{ 
				file_pt=CH376GetFileSize();//计算文件长度
				/*---------------------------------------------------*/
				sector=file_pt/65536;//文件大小、占用几个扇区
				if(sector==0) SPI_FLASH_SectorErase(0);//Flash扇区擦除
					else do
							SPI_FLASH_SectorErase(sector*65536);//Flash扇区擦除
							while(sector--);
				/*---------------------------------------------------*/
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
						//	CAN_LED_OFF();
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
				{mDelaymS(100 );IWDG_ReloadCounter();}
			mDelaymS(200 );
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
	/*------------------------------------------------------------------------------------------*/	
	xWriteCH376Cmd( CMD11_SET_USB_MODE );  /* 设备USB工作模式 */
	xWriteCH376Data( 0x03 );//模式代码为03H时切换到SD卡主机模式，用于管理和存取SD卡中的文件；
	Analysis_user_profiles();
	USB_LED_ON();
	while(CH376DiskConnect() == 0x14) //等待U盘拔出
		{
		IWDG_ReloadCounter();
		mDelaymS(200 );
		}
	USB_LED_OFF();
	mDelaymS(200 );
	}
