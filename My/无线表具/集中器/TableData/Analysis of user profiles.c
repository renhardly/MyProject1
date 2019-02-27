#include "stm32f10x_lib.h"
#include "platform_config.h"
#include "FILE_SYS.H"
#include <string.h>

/*******************************************************************************
* Function Name  : Analysis_user_profiles
* Description    : �û����ݷ��������ַ�ͱ�Э�飩
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

	/* ����ָ��λ�õ���������*/
	SPI_FLASH_SectorErase(0);//Flash��������
/*	while (CH376DiskConnect() != USB_INT_SUCCESS)
			{  //���U���Ƿ�����,�ȴ�U�̲���,����SD��,�����ɵ�Ƭ��ֱ�Ӳ�ѯSD�����Ĳ��״̬����
			mDelaymS(100 );
			}*/
	IWDG_ReloadCounter();
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==1)//���SD���Ƿ�����,�ȴ�SD������
		{
		mDelaymS(100 );
		IWDG_ReloadCounter();
		}
		strcpy(SrcName,"\\�û�����.TXT");
		s = CH376FileOpen((u8 *)SrcName );  /* ���ļ�,���ļ���C51��Ŀ¼�� */
		if ((s != ERR_MISS_DIR) && (s != ERR_MISS_FILE))
			{ 
			/*�ļ�ָ��ָ��ڶ���*/
			file_len_R=CH376GetFileSize();//�����ļ�����
			file_pt=0;
			User_Num=0;
			do
				{
				CH376ByteRead( &ch,1,NULL );
				file_pt++;
				}
				while(ch!='\n');//���з�
			/*��ʼ�����ļ�����*/
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
					while(ch!=',');//�ָ��	
				ch_buf[ch_len-1]=0;
				switch(i)
					{
					case 0 : 
							for(j=0;j<14;j++)
								if((ch_buf[j]>='0')&&(ch_buf[j]<='9'))
									ch_buf[j]-=0x30;		//��ASCII�ַ�ת����BCD��
									else ch_buf[j]=0;
							Table_Addr[0]=(ch_buf[12]<<4)|ch_buf[13];
							Table_Addr[1]=(ch_buf[10]<<4)|ch_buf[11];
							Table_Addr[2]=(ch_buf[8]<<4)|ch_buf[9];
							Table_Addr[3]=(ch_buf[6]<<4)|ch_buf[7];
							Table_Addr[4]=(ch_buf[4]<<4)|ch_buf[5];
							Table_Addr[5]=(ch_buf[2]<<4)|ch_buf[3];
							Table_Addr[6]=(ch_buf[0]<<4)|ch_buf[1];
							SPI_FLASH_BufferWrite(Table_Addr,128+User_Num*15,7);//��߱���
							break;
					case 1 : 
						//	ch_buf[ch_len-2]=0;
							if(!strcmp(ch_buf,"��ķ³��401")) ch_buf[0]=1;//13757Э��
							if(!strcmp(ch_buf,"��ķ³��402")) ch_buf[0]=2;//13757Э��
							if(!strcmp(ch_buf,"������")) 	  ch_buf[0]=3;//13757Э��
							if(!strcmp(ch_buf,"188")) 		  ch_buf[0]=4;//13757Э��
							SPI_FLASH_BufferWrite((u8 *)ch_buf,128+User_Num*15+7,1);//��Э��
							break;
					case 2 : 
							for(j=0;j<8;j++)
								if((ch_buf[j]>='0')|(ch_buf[j]<='9'))
									ch_buf[j]-=0x30;		//��ASCII�ַ�ת����BCD��
							addr=ch_buf[0];
							for(j=1;j<ch_len-2;j++)
								{
								addr<<=4;
								addr|=ch_buf[j];
								}
							Table_Addr[0]=(addr>>24)&0xff;//���ֽ�
							Table_Addr[1]=(addr>>16)&0xff;
							Table_Addr[2]=(addr>>8)&0xff;
							Table_Addr[3]=addr&0xff;//���ֽ�
							SPI_FLASH_BufferWrite(Table_Addr,128+User_Num*15+8,4);//�ɼ�����ַ
							break;
					}
				if(i>=2){i=0;User_Num++;}
					else i++;
				if(file_pt>=file_len_R-2) 
					break;//�ļ���β
				}while(1);
			s = CH376FileClose( TRUE );//�رմ򿪵��ļ�
			mStopIfError( s );
			}
	ch_buf[0]=User_Num>>8;
	ch_buf[1]=User_Num&0xff;
	SPI_FLASH_BufferWrite((u8 *)ch_buf,0,2);//��¼һ����Ҫ��ȡ���ٿ���
	}
