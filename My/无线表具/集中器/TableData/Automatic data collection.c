#include "stm32f10x_lib.h"
#include "platform_config.h"
#include "FILE_SYS.H"
#include <string.h>



/*******************************************************************************
* Function Name  : jiaoyan
* Description    : ���յ�������У��
* Input          : TX_BUF,flash_buf
* Output         : Null
* Return         : None
*******************************************************************************/
u8 jiaoyan(u8 *buf,u16 len)
	{
	u16 i;
	u8 CRC1=0;
	CRC1=buf[0];
	for(i=1;i<len-2;i++)
		CRC1+=buf[i];
	return CRC1;
	}

/*******************************************************************************
* Function Name  : biaoju
* Description    : ���ݱ�����ͣ���Ӳɼ�����ַ�ͱ�ߵ�ַ
* Input          : TX_BUF,flash_buf
* Output         : Null
* Return         : None
*******************************************************************************/
void biaoju(u8 *TX_BUF,u8* flash_buf)
	{
	u8 i;
	TX_BUF[2]=flash_buf[8];
	TX_BUF[3]=flash_buf[9];
	TX_BUF[4]=flash_buf[10];
	TX_BUF[5]=flash_buf[11];
	switch(flash_buf[7])
		{
		case 1 ://��ķ³��401
			{
			TX_BUF[17]=flash_buf[3];
			TX_BUF[18]=flash_buf[4];
			TX_BUF[19]=flash_buf[5];
			TX_BUF[20]=flash_buf[6];
			TX_BUF[21]=0x2d;
			TX_BUF[22]=0x2c;
			TX_BUF[23]=0x02;
			TX_BUF[24]=0x0c;
			
			TX_BUF[25]=TX_BUF[14];
			for(i=15;i<25;i++)
				TX_BUF[25]+=TX_BUF[i];
			//	TX_BUF[25]=TX_BUF[14]+TX_BUF[15]+TX_BUF[16]+TX_BUF[17]+TX_BUF[18]+TX_BUF[19]+TX_BUF[20]+TX_BUF[21]+TX_BUF[22]+TX_BUF[23]+TX_BUF[24];
			TX_BUF[27]=TX_BUF[0];
			for(i=1;i<27;i++)
				TX_BUF[27]+=TX_BUF[i];
				break;
			}
		case 2 ://��ķ³��402
			{
			TX_BUF[17]=flash_buf[3];
			TX_BUF[18]=flash_buf[4];
			TX_BUF[19]=flash_buf[5];
			TX_BUF[20]=flash_buf[6];
			TX_BUF[21]=0x2d;
			TX_BUF[22]=0x2c;
			TX_BUF[23]=0x0b;
			TX_BUF[24]=0x0c;
			
			TX_BUF[25]=TX_BUF[14];
			for(i=15;i<25;i++)
				TX_BUF[25]+=TX_BUF[i];
			
			TX_BUF[27]=TX_BUF[0];
			for(i=1;i<27;i++)
				TX_BUF[27]+=TX_BUF[i];
			break;
			}
		case 3 ://������
			{
			TX_BUF[17]=flash_buf[3];
			TX_BUF[18]=flash_buf[4];
			TX_BUF[19]=flash_buf[5];
			TX_BUF[20]=flash_buf[6];
			TX_BUF[21]=0xa7;
			TX_BUF[22]=0x32;
			TX_BUF[23]=0x03;
			TX_BUF[24]=0x04;
			
			TX_BUF[25]=TX_BUF[14];
			for(i=15;i<25;i++)
				TX_BUF[25]+=TX_BUF[i];
				
			TX_BUF[27]=TX_BUF[0];
			for(i=1;i<27;i++)
				TX_BUF[27]+=TX_BUF[i];
			break;
			}
		case 4 : //188
			{
			TX_BUF[15]=flash_buf[0];
			TX_BUF[16]=flash_buf[1];
			TX_BUF[17]=flash_buf[2];
			TX_BUF[18]=flash_buf[3];
			TX_BUF[19]=flash_buf[4];
			TX_BUF[20]=flash_buf[5];
			TX_BUF[21]=flash_buf[6];
			
			TX_BUF[27]=TX_BUF[13];
			for(i=14;i<27;i++)
				TX_BUF[27]+=TX_BUF[i];
			TX_BUF[29]=TX_BUF[0];
			for(i=1;i<29;i++)
				TX_BUF[29]+=TX_BUF[i];
			break;
			}
		}
	}
/*******************************************************************************
* Function Name  : dingshi
* Description    : ��ʱ��3.5S��ʱ����ʱʱ��С��3.5S����0����������3.5S����1��
* Input          : Null
* Output         : Null
* Return         : 0 And 1
*******************************************************************************/

u8 dingshi()
	{
	extern u8 TIM2_Sign;
	u32 i=0x700000;
	TIM2_Sign=0;
	while(i--)
		{
		IWDG_ReloadCounter();
		if(TIM2_Sign==1)return 0;
		}
	return 1;
	
	/*
	while(TIM2_Sign==0)
		{
		IWDG_ReloadCounter();
		if(TIM3_Sign==1)//��ʱ
			break;
		}//�ȴ���������
	if((TIM2_Sign==0)&&(TIM3_Sign==1))//û�н��յ����ݡ���ʱ
		{
		IWDG_ReloadCounter();
		TIM3_Sign=0;
		return 1;
		}
		else return 0;*/
	}
/*******************************************************************************
* Function Name  : baocun
* Description    : ����ɼ������ı���������ı��ļ�
* Input          : flash_buf
* Output         : �ı��ļ�
* Return         : Null
*******************************************************************************/
void baocun(u8 leixing)
	{
	extern struct A Table_data;
	char buf[20];
	strcpy(buf, Table_data.BJBM);//��߱���
	strcat(buf,",");
	CH376ByteWrite((u8 *)buf, strlen(buf), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
	strcpy(buf, Table_data.BCRL);//��������
	strcat(buf,",");
	CH376ByteWrite((u8 *)buf, strlen(buf), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
	strcpy(buf, Table_data.BCDW);//���ε�λ
	strcat(buf,",");
	CH376ByteWrite((u8 *)buf, strlen(buf), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
	strcpy(buf, Table_data.RGL);//�ȹ���
	strcat(buf,",");
	CH376ByteWrite((u8 *)buf, strlen(buf), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
	strcpy(buf, Table_data.LJLL);//�ۻ�����
	strcat(buf,",");
	CH376ByteWrite((u8 *)buf, strlen(buf), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
	strcpy(buf, Table_data.XSLL);//˲ʱ����
	strcat(buf,",");
	CH376ByteWrite((u8 *)buf, strlen(buf), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
	strcpy(buf, Table_data.JKWD);//�����¶�
	strcat(buf,",");
	CH376ByteWrite((u8 *)buf, strlen(buf), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
	strcpy(buf, Table_data.CKWD);//�����¶�
	strcat(buf,",");
	CH376ByteWrite((u8 *)buf, strlen(buf), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
	strcpy(buf, Table_data.ZGZSJ);//�ܹ���ʱ��
	strcat(buf,",");
	CH376ByteWrite((u8 *)buf, strlen(buf), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
	strcpy(buf, Table_data.SSRQ);//ʵʱ����
	strcat(buf,",");
	CH376ByteWrite((u8 *)buf, strlen(buf), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
	strcpy(buf, Table_data.SSSJ);//ʵʱʱ��
	strcat(buf,",");
	CH376ByteWrite((u8 *)buf, strlen(buf), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
	strcpy(buf, Table_data.BJZT);//���״̬
	strcat(buf,",");
	CH376ByteWrite((u8 *)buf, strlen(buf), NULL );/* ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� */
	if(leixing==1)
		CH376ByteWrite((u8 *)"��ķ³��401\r", strlen("��ķ³��401\r"), NULL );
	if(leixing==2)
		CH376ByteWrite((u8 *)"��ķ³��402\r", strlen("��ķ³��402\r"), NULL );
	if(leixing==3)
		CH376ByteWrite((u8 *)"������\r", strlen("������\r"), NULL );
	if(leixing==4)
		CH376ByteWrite((u8 *)"188\r", strlen("188\r"), NULL );
	}
/*******************************************************************************
* Function Name  : Automatic_data_collection
* Description    : ��������Զ��ɼ�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Automatic_data_collection(void)
	{
	extern struct A Table_data;
	extern u8 data_buf[50];
	extern u8 TXD_BUF[280];
	extern u16 TX_LEN1;
	static u8 TX_BUF[30]= {0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						   0x68,0x0B,0x0B,0x68,0x53,0xFD,0x52,0x22,0x73,0x22,
						   0x60,0x2D,0x2C,0x0B,0x0C,0x29,0x16,0x00,0x16};
	static u8 TX_BUF1[20]={0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						   0x10,0x5B,0xFD,0x58,0x16,0x00,0x16},flash_buf[15];
	static u8 TX_BUF2[31]={0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						   0xfe,0xfe,0xfe,0x68,0x20,0xAA,0xAA,0xAA,0xAA,0xAA,
						   0xAA,0xAA,0x01,0x03,0x1F,0x90,0x00,0xE1,0x16,0x00,0x16};
//	static u8 TX_BUF3[20]={0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//						   0x10,0x40,0xFD,0x3D,0x16,0x00,0x16};
	u8 i=0,*p,s,yanshi=0,j=0;
	char SrcName[20]="\\20120116.CSV",buf[100]={0};
	char biaozhiwei=0,CRC8;
	u16 User_Num;
	if((data_buf[14]==data_buf[3])&&(data_buf[15]==data_buf[4]))
		biaozhiwei=0;//����������Ѿ���ȡ������
		else biaozhiwei=1;
	if(biaozhiwei!=0)
		{
		data_buf[15]=data_buf[4];
		data_buf[14]=data_buf[3];
		/**��¼�����������ı������**/
		SrcName[1]=(data_buf[1]/10)+0x30;
		SrcName[2]=(data_buf[1]%10)+0x30;
		SrcName[3]=(data_buf[2]/10)+0x30;
		SrcName[4]=(data_buf[2]%10)+0x30;
		SrcName[5]=(data_buf[3]/10)+0x30;
		SrcName[6]=(data_buf[3]%10)+0x30;
		SrcName[7]=(data_buf[4]/10)+0x30;
		SrcName[8]=(data_buf[4]%10)+0x30;
		IWDG_ReloadCounter();
		
		s = CH376FileCreate((u8 *)SrcName);  // �ڸ�Ŀ¼���ߵ�ǰĿ¼���½��ļ�,����ļ��Ѿ�������ô��ɾ�� 
		if ((s != ERR_MISS_DIR) && (s != ERR_MISS_FILE))
			{  
			strcpy( buf, "��߱���,��������,���ε�λ,�ȹ���,�ۻ�����,˲ʱ����,�����¶�,�����¶�,�ܹ���ʱ��,ʵʱ����,ʵʱʱ��,���״̬,��������\n" );
			s = CH376ByteWrite((u8 *)buf, strlen(buf), NULL );  // ���ֽ�Ϊ��λ��ǰλ��д�����ݿ� 
			mStopIfError( s );
			}
		IWDG_ReloadCounter();
		SPI_FLASH_BufferRead(flash_buf,0,2);//��ȡ���ٿ��
		User_Num=(flash_buf[0]<<8)|flash_buf[1];
		while(User_Num) //ѭ�����������Ͷ�ȡ���ָ��
			{//��ȡ�û��Ǳ��ַ���Ǳ����͡��Ǳ�Э��
			IWDG_ReloadCounter();
			User_Num--;
			SPI_FLASH_BufferRead(flash_buf,128+User_Num*15,15);//��ȡ���ٿ��
			IWDG_ReloadCounter();
			/**************************************/
			if(flash_buf[7]==4) biaoju(TX_BUF2,flash_buf);//188
				else biaoju(TX_BUF,flash_buf);//13757
			/**************************************/
			one2:
			if(flash_buf[7]==4) Auto_sent(TX_BUF2,31);
				else Auto_sent(TX_BUF,29);
			TX_LEN1=0;
			if((!dingshi())&&(TX_LEN1>12))//����0�����ճɹ�����0ʧ��
				{
				yanshi=0;
				/**************************************/
				if(flash_buf[7]==4)//188Э��
					{
					if((TXD_BUF[0]==0x68)&&(TXD_BUF[1]==0x80))
						{
						j=0;
						p=&TXD_BUF[1];
						while(*p!=0x68) {p++;j++;}
						IWDG_ReloadCounter();
						CRC8=jiaoyan(p,TX_LEN1-(j+3));
						if(CRC8==TXD_BUF[TX_LEN1-4])
							{
							Agreement_188(p);//���ݽ���
							baocun(flash_buf[7]);//��������浽�ļ���
							}
							else goto one2;//У�������
						}
						else goto one2;//�������
					}
				else //13757 Э��
				/**************************************/
				if((TXD_BUF[10]==0xe5)&&(TXD_BUF[1]==0x80))
					{
					TX_BUF1[2]=flash_buf[8];//�ɼ�����ַ
					TX_BUF1[3]=flash_buf[9];//�ɼ�����ַ
					TX_BUF1[4]=flash_buf[10];//�ɼ�����ַ
					TX_BUF1[5]=flash_buf[11];//�ɼ�����ַ
				/*------------------------------------*/
					IWDG_ReloadCounter();
					TX_BUF1[15]=TX_BUF1[0];
					for(i=1;i<15;i++)
						TX_BUF1[15]+=TX_BUF1[i];
					Auto_sent(TX_BUF1,17);
					TX_LEN1=0;
				/*------------------------------------*/
					if((!dingshi())&&(TX_LEN1>12))//����0�����ճɹ�����0ʧ��
						{
						if((TXD_BUF[0]==0x68)&&(TXD_BUF[1]==0x80))
							{
							j=0;
							p=&TXD_BUF[1];
							while(*p!=0x68) {p++;j++;}
							IWDG_ReloadCounter();
							CRC8=jiaoyan(&p[4],TX_LEN1-(j+7));
							if(CRC8==TXD_BUF[TX_LEN1-4])
								{
								Agreement_13757(p,TX_LEN1-12);//13757Э�����
								baocun(flash_buf[7]);//��������浽�ļ���
								}
								else goto one2;//У�����
							IWDG_ReloadCounter();
							}
							else goto one2;//�������
						//END
						}
						else goto one2;//���ճ�ʱ������
					}
					else goto one2;//����ʧ��
				}
				else //����ʧ��
				{
				yanshi++;
				if(yanshi>7) yanshi=0;
					else goto one2;
				}
			IWDG_ReloadCounter();
			for(i=0;i<10;i++)
				{
				Table_data.BJBM[i]=0;
				Table_data.BCRL[i]=0;
				Table_data.BCDW[i]=0;
				Table_data.RGL[i]=0;
				Table_data.LJLL[i]=0;
				Table_data.XSLL[i]=0;
				Table_data.JKWD[i]=0;
				Table_data.CKWD[i]=0;
				Table_data.ZGZSJ[i]=0;
				Table_data.SSRQ[i]=0;
				Table_data.SSSJ[i]=0;
				Table_data.BJZT[i]=0;
				Table_data.BXMC[i]=0;
				}
			}
		s = CH376FileClose( TRUE );  /* �ر��ļ� */
		mStopIfError( s );
		I2C_BufferWrite(&data_buf[3],3,4,0xa1);
		}
	}
