
#include "FILE_SYS.H"
#include "string.h"
#include "HAL.H"
/*******************************************************************************
*	函数名：mStopIfError
*	输  入:	iError, 错误代码
*	输  出:
*	功能说明：检查操作状态,如果错误则显示错误代码并停机,
*	应该替换为实际的处理措施,例如显示错误信息,等待用户确认后重试等
*/
void mStopIfError(u8 iError)
{
	if (iError == USB_INT_SUCCESS)
	{
		return;  /* 操作成功 */
	}
	while(1)
	{
		/* 红色LED闪烁 */
		GPIO_ResetBits(GPIOC,  GPIO_Pin_14);
		mDelaymS( 200 );
		GPIO_SetBits(GPIOC,  GPIO_Pin_14);
		mDelaymS( 200 );
	}
}


void TestCH376(void)
{
	u8 i,s;
//	P_FAT_DIR_INFO	pDir;
	char	SrcName[64];
	char	TarName[64];
	char	FolderName[64];
	char	buf[64];

	s = mInitCH376Host( );  /* 初始化CH376 */
	mStopIfError( s );

	while(1) 
	{

/*		while (CH376DiskConnect() != USB_INT_SUCCESS)
		{  //检查U盘是否连接,等待U盘插入,对于SD卡,可以由单片机直接查询SD卡座的插拔状态引脚 *
			mDelaymS(100 );
		} */
		while(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8)==1)//检查SD卡是否连接,等待SD卡插入
			{
			mDelaymS(100 );
			}
		mDelaymS(250);  /* 延时,可选操作,有的USB存储器需要几十毫秒的延时 */
		/* 对于检测到USB设备的,最多等待100*50mS,主要针对有些MP3太慢,对于检测到USB设备并且连接DISK_MOUNTED的,最多等待5*50mS,主要针对DiskReady不过的 */
		for (i = 0; i < 5; i ++ )
		{  /* 最长等待时间,100*50mS */
			mDelaymS(50);
			s = CH376DiskMount();  /* 初始化磁盘并测试磁盘是否就绪 */
			if (s == USB_INT_SUCCESS)
			{
				break;  /* 准备好 */
			}
			else if (s == ERR_DISK_DISCON)/* 磁盘尚未连接,可能磁盘已经断开 */
			{
				break;  /* 检测到断开,重新检测并计时 */
			}
			if (CH376GetDiskStatus() >= DEF_DISK_MOUNTED && i >= 5)
			{
				break;  /* 有的U盘总是返回未准备好,不过可以忽略,只要其建立连接MOUNTED且尝试5*50mS */
			}
		}
		if (s == ERR_DISK_DISCON)/* 磁盘尚未连接,可能磁盘已经断开 */
		{  /* 检测到断开,重新检测并计时 */
			continue;
		}
		if (CH376GetDiskStatus( ) < DEF_DISK_MOUNTED)
		{  /* 未知USB设备,例如USB键盘、打印机等 */
			goto UnknownUsbDevice;
		}

		/* 读取原文件 */
		strcpy(SrcName, "\\123.CSV");  /* 源文件名,多级目录下的文件名和路径名必须复制到RAM中再处理,而根目录或者当前目录下的文件名可以在RAM或者ROM中 */
		strcpy(TarName, "\\20130830.CSV" );  /* 目标文件名 */
		strcpy(FolderName, "\\采集器数据" );  /* 目标文件名 */

		//CH376FileCreate((u8 *)TarName); //根目录或当前目录下创建文件
		CH376DirCreate( (u8 *)FolderName ); 

		mDelaymS(80);
		s = CH376FileOpenPath((u8 *)TarName );  /* 打开文件,该文件在C51子目录下 */
		if ((s != ERR_MISS_DIR) && (s != ERR_MISS_FILE))
		{  
			//mStopIfError(s);
			s = CH376FileOpenPath((u8 *)TarName );//打开文件
			strcpy( buf, "1,2,3,4,5,6,7,8,9,0\n\r" );
			s = CH376ByteWrite((u8 *)buf, strlen(buf), NULL );  /* 以字节为单位向当前位置写入数据块 */
			strcpy( buf, "山,东,三,龙,智能技术有限公司" );
			s = CH376ByteWrite((u8 *)buf, strlen(buf), NULL );  /* 以字节为单位向当前位置写入数据块 */
			buf[0]=0x04;
			buf[1]=0x07;
			buf[2]=0x31;
			buf[3]=0x67;
			buf[4]=0x1;
			buf[5]=0x14;
			buf[6]=0x45;
			buf[7]=0x12;
			buf[8]=0xde;
			buf[9]=0xbc;
			s = CH376ByteWrite((u8 *)buf, strlen(buf), NULL );  /* 以字节为单位向当前位置写入数据块 */


			mStopIfError( s );

			s = CH376FileClose( TRUE );  /* 关闭文件,对于字节读写建议自动更新文件长度 */
			mStopIfError( s );

		}
		if(s==ERR_MISS_FILE);/* 指定路径的文件没有找到,可能是文件名称错误 */
		if(s==ERR_MISS_DIR);/* 指定路径的某个子目录(文件夹)没有找到,可能是目录名称错误 */
UnknownUsbDevice:

		while ( CH376DiskConnect( ) == USB_INT_SUCCESS ) 
		{  
			/* 检查U盘是否连接,等待U盘拔出 */
			mDelaymS( 100 );
		}
		mDelaymS(200 );
	}
}
