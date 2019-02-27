#include "stm32f10x_lib.h"
#include "FILE_SYS.H"
#include "platform_config.h"
extern u8 data_buf[50];
/*******************************************************************************
* Function Name  : SD_USB
* Description    : ��SD���н��ļ�������U��
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
	
	s = CH376FileOpenPath((u8 *)"\\*.CSV");  /* ö�ٶ༶Ŀ¼�µ��ļ�����Ŀ¼,���뻺����������RAM�� */
	File_Quantity=0;
	IWDG_ReloadCounter();
	SPI_FLASH_SectorErase(655360);//Flash��������
	SPI_FLASH_SectorErase(720896);//Flash��������
	while (s == USB_INT_DISK_READ)
		{	/* ö�ٵ�ƥ����ļ� */
		IWDG_ReloadCounter();
		CH376ReadBlock((u8 *)buf);  /* ��ȡö�ٵ����ļ���FAT_DIR_INFO�ṹ,���س�������sizeof( FAT_DIR_INFO ) */
		pDir = (P_FAT_DIR_INFO)buf;  /* ��ǰ�ļ�Ŀ¼��Ϣ */
		if (pDir -> DIR_Name[0] != '.')
			{  /* ���Ǳ��������ϼ�Ŀ¼�������,������붪�������� */
			if ( pDir -> DIR_Name[0] == 0x05 )
				{
				pDir -> DIR_Name[0] = 0xE5;  /* �����ַ��滻 */
				}
			/*����Ƿ���.CSV�ļ�,"."��·���в�ռ�ÿռ�*/
			if((pDir -> DIR_Name[8]=='C')&(pDir -> DIR_Name[9]=='S')&(pDir -> DIR_Name[10]=='V'))
				{
				IWDG_ReloadCounter();
				SPI_FLASH_BufferWrite((u8 *)pDir -> DIR_Name,File_Quantity*8+655360,8);
				File_Quantity++;
				}
			pDir -> DIR_Attr = 0;  /* ǿ���ļ����ַ��������Ա��ӡ��� */
			}
		xWriteCH376Cmd( CMD0H_FILE_ENUM_GO );  /* ����ö���ļ���Ŀ¼ */
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
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==1)//���SD���Ƿ�����,�ȴ�SD������
				{mDelaymS(100 );IWDG_ReloadCounter();}
			mDelaymS(200 );
			/*********************����SD�����ļ�***************************/
			s = CH376FileOpen((u8 *)SrcName );  // ���ļ�
			if ((s != ERR_MISS_DIR) && (s != ERR_MISS_FILE))
				{ 
				file_pt=CH376GetFileSize();//�����ļ�����
				/*---------------------------------------------------*/
				sector=file_pt/65536;//�ļ���С��ռ�ü�������
				if(sector==0) SPI_FLASH_SectorErase(0);//Flash��������
					else do
							SPI_FLASH_SectorErase(sector*65536);//Flash��������
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
				s = CH376FileClose( TRUE );//�رմ򿪵��ļ�
				mStopIfError( s );
				}
				else continue;
			/*********************��USB�豸д������***************************/
			xWriteCH376Cmd( CMD11_SET_USB_MODE );  // �豸USB����ģʽ 
			xWriteCH376Data( 0x06 );//ģʽ����Ϊ06Hʱ�л��������õ� USB ������ʽ���Զ����� SOF ��	
	//		mDelaymS(100 );
			IWDG_ReloadCounter();
			while(CH376DiskConnect() != 0x14) //��⵽USB�豸
				{mDelaymS(100 );IWDG_ReloadCounter();}
			mDelaymS(200 );
			s=CH376FileCreate((u8 *)SrcName);//�ڸ�Ŀ¼���ߵ�ǰĿ¼���½��ļ�,����ļ��Ѿ�������ô��ɾ��
			if ((s != ERR_MISS_DIR) && (s != ERR_MISS_FILE))
				{
				for(file_pt=0;file_pt<page_len;file_pt++)
					{
					IWDG_ReloadCounter();
					SPI_FLASH_BufferRead(buf,file_pt*256,256);//��ȡ���ٿ��
					if(file_pt==(page_len-1))
						{
					//	CAN_LED_ON();
						CH376ByteWrite((u8 *)buf,(file_p%256), NULL );// ���ֽ�Ϊ��λ��ǰλ��д�����ݿ�
					//	CAN_LED_OFF();
						}	
						else 
							{
						//	CAN_LED_ON();
							CH376ByteWrite((u8 *)buf,256, NULL );// ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� 
						//	CAN_LED_OFF();
							}
					}
				s = CH376FileClose( TRUE );//�رմ򿪵��ļ�
				mStopIfError( s );
				}
			xWriteCH376Cmd( CMD11_SET_USB_MODE );  // �豸USB����ģʽ 
			xWriteCH376Data( 0x03 );//ģʽ����Ϊ03Hʱ�л���SD������ģʽ�����ڹ���ʹ�ȡSD���е��ļ���
		}
	/*------------------------------------------------------------------------------------------*/	
	xWriteCH376Cmd( CMD11_SET_USB_MODE );  /* �豸USB����ģʽ */
	xWriteCH376Data( 0x03 );//ģʽ����Ϊ03Hʱ�л���SD������ģʽ�����ڹ���ʹ�ȡSD���е��ļ���
	Analysis_user_profiles();
	USB_LED_ON();
	while(CH376DiskConnect() == 0x14) //�ȴ�U�̰γ�
		{
		IWDG_ReloadCounter();
		mDelaymS(200 );
		}
	USB_LED_OFF();
	mDelaymS(200 );
	}
