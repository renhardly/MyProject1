#include "stm32f10x_lib.h"
#include "platform_config.h"
#include "FILE_SYS.H"

/*******************************************************************************
* Function Name  : Upload_historical_data
* Description    : �ϴ���ʷ����
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
	
	strcpy(SrcName,"\\�û�����.TXT");
//	strcpy(SrcName,"\\20120201.CSV");
	
	s = CH376FileOpen((u8 *)SrcName );  /* ���ļ�,���ļ���C51��Ŀ¼�� */
	if ((s != ERR_MISS_DIR) && (s != ERR_MISS_FILE)&& (s == 0x14))
		{  
		file_pt=CH376GetFileSize();//�����ļ�����
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
		s = CH376FileClose( TRUE );//�رմ򿪵��ļ�
		mStopIfError( s );
		}
	//	else return error;
	}
/*******************************************************************************
* Function Name  : ADD_Meter
* Description    : ��ӱ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Add_Meter(char *ch)
	{
	u8 s;
//	u16 User_Num;
	u32 file_pt;
	char buf[40]="123,��ķ³��401,1111";
//	u8 Table_Addr[7]={0},ch_buf[14]={0};
//	p=buf;
	
	s = CH376FileOpen((u8 *)"\\�û�����.TXT" );  //���ļ�,���ļ���C51��Ŀ¼��
	if ((s != ERR_MISS_DIR) && (s != ERR_MISS_FILE))
		{  
		IWDG_ReloadCounter();
		file_pt=CH376GetFileSize();//�����ļ�����
		CH376ByteLocate(file_pt);//�ƶ��ļ�ָ��
		
		strcat(buf, "\n");
		s = CH376ByteWrite((u8 *)buf, strlen(buf), NULL );//* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ�
		s = CH376FileClose( TRUE );  //* �ر��ļ�,�����ֽڶ�д�����Զ������ļ�����
		mStopIfError( s );
		/*
		SPI_FLASH_BufferRead(ch_buf,0,2);//��ȡ���ٿ��
		User_Num=(ch_buf[0]<<8)|ch_buf[1];
		User_Num++;
		for(j=0;j<14;j++)
			{
			if(*p==',') {p++;break;}
			if((*p>='0')&&(*p<='9'))
				ch_buf[j]=*p-0x30;		//��ASCII�ַ�ת����BCD��
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
		
		SPI_FLASH_BufferWrite(Table_Addr,128+User_Num*15,7);//��߱���
		if(!strncmp(p,"��ķ³��401",11)) Table_Addr[0]=1;//13757Э��
		if(!strncmp(p,"��ķ³��402",11)) Table_Addr[0]=2;//13757Э��
		if(!strncmp(p,"������",6)) 	  	 Table_Addr[0]=3;//13757Э��
		if(!strncmp(p,"188",3)) 		 Table_Addr[0]=4;//13757Э��
		while(*(p++)!=',');
		SPI_FLASH_BufferWrite((u8 *)Table_Addr,128+User_Num*15+7,1);//��Э��
	
		for(j=0;j<8;j++)
			{
			if(*p=='\0') {p++;break;}
			if((*p>='0')&&(*p<='9'))
				ch_buf[j]=*p-0x30;		//��ASCII�ַ�ת����BCD��
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
		Table_Addr[0]=(addr>>24)&0xff;//���ֽ�
		Table_Addr[1]=(addr>>16)&0xff;
		Table_Addr[2]=(addr>>8)&0xff;
		Table_Addr[3]=addr&0xff;//���ֽ�
		SPI_FLASH_BufferWrite(Table_Addr,128+User_Num*15+8,4);//�ɼ�����ַ*/
		}
	Analysis_user_profiles();//��ȡ���ַ����Э��
	}
/*******************************************************************************
* Function Name  : Delete_Meter
* Description    : ɾ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Delete_Meter(char *p)
	{
	u8 s,flash_buf[15],j;
	char buf[30],ch[7]={0x60,0x20,0x25,0x19};
	u16 User_Num,i;

	s =CH376FileCreate( (u8 *) "\\�û�����.TXT" );
	mStopIfError( s );
	SPI_FLASH_BufferRead(flash_buf,0,2);//��ȡ���ٿ��
	User_Num=(flash_buf[0]<<8)|flash_buf[1];
	for(i=0;i<User_Num;i++) //ѭ�����������Ͷ�ȡ���ָ��
		{//��ȡ�û��Ǳ��ַ���Ǳ����͡��Ǳ�Э��
		SPI_FLASH_BufferRead(flash_buf,128+i*15,15);//��ȡ���ٿ��
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
			s = CH376ByteWrite((u8 *)buf,strlen(buf), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
			switch(flash_buf[7])
				{
				case 1 :CH376ByteWrite("��ķ³��401,",strlen("��ķ³��401,"), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
						break;
				case 2 :CH376ByteWrite("��ķ³��402,",strlen("��ķ³��402,"), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
						break;
				case 3 :CH376ByteWrite("������,",strlen("������,"), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
						break;
				case 4 :CH376ByteWrite("188,",strlen("188,"), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
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

			s = CH376ByteWrite((u8 *)buf,strlen(buf), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
			
			}
		}
		s = CH376FileClose( TRUE );  /* �ر��ļ�,�����ֽڶ�д�����Զ������ļ����� */
		mStopIfError( s );
		Analysis_user_profiles();//��ȡ���ַ����Э��
	}
/*******************************************************************************
* Function Name  : Modify_Meter
* Description    : �޸ı��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Modify_Meter(char *ch)
	{
	
	}
/*******************************************************************************
* Function Name  : Modify_Addr
* Description    : �޸ļ�������ַ
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
* Description    : ������ʱ��ͬ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Clock_synchronization(char *ch)
	{
	u8* pBuffer;
	I2C_BufferWrite(pBuffer,7,2,0xA3);
	}
