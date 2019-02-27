//#include "stm32f10x.h"
#include "platform_config.h"
#include "FILE_SYS.H"
#include "string.h"
#include <stdio.h>
#include	"HAL.H"
/*******************************************************************************
*	函数名：mStopIfError
*	输  入:	iError, 错误代码
*	输  出:
*	功能说明：检查操作状态,如果错误则显示错误代码并停机,
*	应该替换为实际的处理措施,例如显示错误信息,等待用户确认后重试等
********************************************************************************/
void mStopIfError(u8 iError)
{
	if (iError == USB_INT_SUCCESS)
	{
		return;  /* 操作成功 */
	}
//	printf("Error: %02X\n", iError );  /* 显示错误 */
	while(1)
	{
		/* LED闪烁 */
		GPIO_ResetBits(GPIOA,  GPIO_Pin_0);
		mDelaymS( 200 );
		GPIO_SetBits(GPIOA,  GPIO_Pin_0);
		mDelaymS( 200 );
	}
}
/*
UINT8	CH376ByteRead( PUINT8 buf, UINT16 ReqCount, PUINT16 RealCount );   以字节为单位从当前位置读取数据块 
UINT8	CH376ByteWrite( PUINT8 buf, UINT16 ReqCount, PUINT16 RealCount );  以字节为单位向当前位置写入数据块 
UINT8	CH376ByteLocate( UINT32 offset );   以字节为单位移动当前文件指针 
UINT8	CH376DirCreate( PUINT8 name );   在根目录下新建目录(文件夹)并打开,如果目录已经存在那么直接打开 
void	CH376WriteOfsBlock( PUINT8 buf, UINT8 ofs, UINT8 len );   向内部缓冲区指定偏移地址写入数据块 
void	CH376SetFileName( PUINT8 name );   设置将要操作的文件的文件名 

UINT8	CH376FileClose( UINT8 UpdateSz );   关闭当前已经打开的文件或者目录(文件夹) 
UINT8	CH376FileErase( PUINT8 PathName );   删除文件,如果已经打开则直接删除,否则对于文件会先打开再删除,支持多级目录路径 
UINT8	CH376FileOpen( PUINT8 name );   在根目录或者当前目录下打开文件或者目录(文件夹) 
UINT8	CH376FileCreate( PUINT8 name ); 在根目录或者当前目录下新建文件,如果文件已经存在那么先删除 

*/
/*
extern u8 data_buf[512];
void TestCH376(void)
{
	u8 i,s;
//	P_FAT_DIR_INFO	pDir;
	char	SrcName[64]="\\";
	char	buf[256],file_name[14];
	u32 b123;
	b123=data_buf[4]+data_buf[3]*100+data_buf[2]*10000+data_buf[1]*1000000;
	sprintf(file_name,"%ld",b123);
	strcat(SrcName,file_name);
	strcat(SrcName,".CSV");
//	if(1) 
//	{
	//	while (CH376DiskConnect() != USB_INT_SUCCESS)
		{  //检查U盘是否连接,等待U盘插入,对于SD卡,可以由单片机直接查询SD卡座的插拔状态引脚 *
			mDelaymS(100 );
		}
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==1)//检查SD卡是否连接,等待SD卡插入
			{
//			IWDG_ReloadCounter();
			mDelaymS(200 );
			
			}
		// 对于检测到USB设备的,最多等待100*50mS,主要针对有些MP3太慢,对于检测到USB设备并且连接DISK_MOUNTED的,最多等待5*50mS,主要针对DiskReady不过的 
		for (i = 0; i < 5; i ++ )
			{  // 最长等待时间,100*50mS 
			mDelaymS(50);
			s = CH376DiskMount();  // 初始化磁盘并测试磁盘是否就绪 
			if (s == USB_INT_SUCCESS)
				break;  // 准备好 
			else if (s == ERR_DISK_DISCON)/ 磁盘尚未连接,可能磁盘已经断开 
				break;  // 检测到断开,重新检测并计时 
			if (CH376GetDiskStatus() >= DEF_DISK_MOUNTED && i >= 5)
				break;  // 有的U盘总是返回未准备好,不过可以忽略,只要其建立连接MOUNTED且尝试5*50mS 
			}
		if (s == ERR_DISK_DISCON);// 磁盘尚未连接,可能磁盘已经断开 
		// 检测到断开,重新检测并计时 
//			continue;
	//	if (CH376GetDiskStatus( ) < DEF_DISK_MOUNTED)
		 未知USB设备,例如USB键盘、打印机等 
			goto UnknownUsbDevice;
		one1:
		s = CH376FileOpen((u8 *)SrcName );  // 打开文件,该文件在C51子目录下 
		if ((s != ERR_MISS_DIR) && (s != ERR_MISS_FILE))
			{  
			b123=CH376GetFileSize();//计算文件长度
			CH376ByteLocate(b123-1);//移动文件指针
			strcpy( buf, "用户编码,用户姓名,表具编码,本次热量,本次单位,结算日热量,结算日单位,热功率,累积流量,瞬时流量,进口温度,出口温度,总工作时间,实时日期,实时时间,本次日期,本次时间,表具状态,表型名称,起始读数,用户地址,联系电话,操作员姓名\n\r" );
			s = CH376ByteWrite((u8 *)buf, strlen(buf), NULL );  // 以字节为单位向当前位置写入数据块 
			mStopIfError( s );
			s = CH376FileClose( TRUE );  // 关闭文件,对于字节读写建议自动更新文件长度 
			mStopIfError( s );
			}
		if(s==ERR_MISS_FILE)// 指定路径的文件没有找到,可能是文件名称错误 
			{
			CH376FileErase((u8 *)SrcName);//删除指定的文件
			CH376FileCreatePath((u8 *)SrcName);//创建指定的文件
			goto one1;
			}
//	}
}*/
