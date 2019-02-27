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
	u8 i,j,s,c;
	static u8 buf[256];
	u32 file_pt,file_p,page_len;
	char SrcName[64]="\\20120101.CSV";
	
	SrcName[1]=(data_buf[1]/10)+0x30;
	SrcName[2]=(data_buf[1]%10)+0x30;
	SrcName[3]=(data_buf[2]/10)+0x30;
	SrcName[4]=(data_buf[2]%10)+0x30;
	
	for(i=11;i<=16;i++)//11��������4��
			{
			if(i!=12)
				c=i%12;
			SrcName[5]=(c/10)+0x30;
			SrcName[6]=(c%10)+0x30;
			for(j=1;j<=31;j++)
				{
				IWDG_ReloadCounter();
			//	mDelaymS(100 );
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==1)//���SD���Ƿ�����,�ȴ�SD������
					mDelaymS(100 );
				SrcName[7]=(j/10)+0x30;
				SrcName[8]=(j%10)+0x30;
				SPI_FLASH_SectorErase(0);//Flash��������
		/*********************����SD�����ļ�***************************/
				s = CH376FileOpen((u8 *)SrcName );  // ���ļ�
				if ((s != ERR_MISS_DIR) && (s != ERR_MISS_FILE))
					{ 
					file_pt=CH376GetFileSize();//�����ļ�����
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
					mDelaymS(100 );
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
			}
	xWriteCH376Cmd( CMD11_SET_USB_MODE );  /* �豸USB����ģʽ */
	xWriteCH376Data( 0x03 );//ģʽ����Ϊ03Hʱ�л���SD������ģʽ�����ڹ���ʹ�ȡSD���е��ļ���
	Analysis_user_profiles();
	USB_LED_ON();
	while(CH376DiskConnect() == 0x14) //��⵽USB�豸
		{
		IWDG_ReloadCounter();
		mDelaymS(100 );
		}
	USB_LED_OFF();
	}
