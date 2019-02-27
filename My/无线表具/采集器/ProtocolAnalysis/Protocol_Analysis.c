#include <stdio.h>
#include "platform_config.h"
#include <InterFlash.h>
#include "stm32f10x_flash.h"
#include "ErrCode.h"
#include "spi.h"
void Clear_Meter_Data(u8* Buf,u16 Len)
{
	u8 i,tmp[23];
	u32 temp;
	for(i=0;i<128;i++)
	   {
			temp=EXT_METER_DATA_PAGEADDR+i*4096;
			SPI_Flash_Erase_Sector(temp/4096);
			FlashErase_Page(0x0803E000+i*2048);
		}
		memcpy(tmp,Buf,23);
		tmp[2]=0x80;
		tmp[21]=CRC8(tmp,21);
		UART_3(tmp,23);
}
/*******************************************************************************
* Function Name  : Write_Meter_RTC_ACK
* Description    : ���УʱӦ��
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void Write_Meter_RTC_ACK(u8 *pMeterDataRecv,u16 G_RecvDataLen) //���
{
    extern u8 G_UART_Num;
	u8 SendUpFrameFormat[29];
    u8 tmp[8];
	if(G_RecvDataLen!=16)
    {
        METER_Err_Ack(pMeterDataRecv,CmdErr);
        return;
    }
    SPI_Flash_Read(EXT_CJQ_PARAMETER_PAGEADDR,tmp,8);
    SendUpFrameFormat[0]=0x68;
    SendUpFrameFormat[1]=0X80;
	memcpy(SendUpFrameFormat+2,tmp,8);
	memcpy(SendUpFrameFormat+10,pMeterDataRecv,9);
    SendUpFrameFormat[19]=0x85;
	SendUpFrameFormat[20]=0x06;
    P8563_gettime(tmp);
	exchange(tmp,6);
	memcpy(SendUpFrameFormat+21,tmp,6);
    SendUpFrameFormat[27]=CRC8(SendUpFrameFormat,27);
    SendUpFrameFormat[28]=0x16;
    Send_UART(G_UART_Num,SendUpFrameFormat,29);
}
/*******************************************************************************
* Function Name  : Write_Meter_RTC_FLAG
* Description    : д��߱�׼ʱ���־λ
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
extern u32 G_Point[METER_HEAP_PSIZE];
void Write_Meter_RTC_FLAG(u8* Buf,u16 Len)
{
    extern u8 G_UART_Num;
	u8 index;
    u16 i=0;
    u8 RecData[30]= {0};
//	u8 mask_add[7]={0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa};
    if(Len==30||Len==29)
    {
    }
    else
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
	memcpy(RecData,Buf,Len);
	index=Return_Meter_Index(Buf+11);		
	for(i=0;i<METER_HEAP_PSIZE;i++)
	{
	 	if(G_Point[i]==0)
		{
			CMD_CONTRL *p=malloc(sizeof(CMD_CONTRL));	
			memset(p,0,sizeof(CMD_CONTRL));
			p->data_p=malloc(0);
			p->type=METER_RTC_SET;
			p->cmd_index[index/16]=1<<(index%16);
			p->counter_down=25;
			G_Point[i]=	(u32)p;
			break;
		}	
	}
    Buf[1]=0x80;
    Buf[Len-2]=CRC8(Buf,Len);
    Buf[Len-1]=0x16;
}
/*******************************************************************************
* Function Name  : Write_Meter_RTC
* Description    : ���Уʱ
* Input          : meteradd ��ߵ�ַ
* Output         : None
* Return         : None
*******************************************************************************/

void Write_Meter_RTC(u8 *meteradd,u8 *data,u8 len)
{
    u8 buf[6],tmp[23];
	tmp[0]=0x68;
	memcpy(tmp+1,meteradd,8);

    tmp[9]=0x04;
    tmp[10]=0x0c;
    tmp[11]=0x15;
    tmp[12]=0xa0;
    tmp[13]=0x00;
	tmp[14]=0x20;
	P8563_gettime(buf);
	exchange(buf,6);
	memcpy(tmp+15,buf,6); 
    tmp[21]=Add_Up(tmp,21);
    tmp[22]=0x16;
    UART_5(tmp,23);
}
/*******************************************************************************
* Function Name  : CJQ_RTC_Set
* Description    : ��������ͬ����Уʱ��
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void CJQ_RTC_Set(u8 *Buf,u16 Len)
{
    extern u8 G_UART_Num;
    u8 buf[7]= {0},mask[8]= {0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa};
    if(Len!=29)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    buf[0]=Buf[Len-3];//��
    buf[1]=Buf[Len-4];//��
    buf[2]=Buf[Len-5];//ʱ
    buf[3]=Buf[Len-6];//��
    buf[5]=Buf[Len-7];//��
    buf[6]=Buf[Len-8];//��
    P8563_settime(buf);
    if(memcmp(mask,&Buf[2],8)==0)
    {
        Err_Ack(Buf,IllegalDataFormat);
		return;
    }
    Buf[1]=0x80;
    Buf[Len-2]=CRC8(Buf,Len-2);
    Buf[Len-1]=0x16;
    Send_UART(G_UART_Num,Buf,Len);
}
/*******************************************************************************
* Function Name  : CJQ_RTC_Set_ALL
* Description    : ��������ͬ����Уʱ��
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void CJQ_RTC_Set_ALL(u8 *Buf,u16 Len)
{
    extern u8 G_UART_Num;
    u8 buf[7]= {0};
    if(Len!=30)
    {
        return;
    }
    buf[0]=Buf[Len-3];//��
    buf[1]=Buf[Len-4];//��
    buf[2]=Buf[Len-5];//ʱ
    buf[3]=Buf[Len-6];//��
    buf[5]=Buf[Len-7];//��
    buf[6]=Buf[Len-8];//��
    P8563_settime(buf);
}
/*******************************************************************************
* Function Name  : CJQ_RTC_Set_ONE
* Description    : ��������ͬ����Уʱ��
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void CJQ_RTC_Set_ONE(u8 *Buf,u16 Len)
{
    extern u8 G_UART_Num;
    u8 buf[7]= {0};
    if(Len!=30)
    {
        return;
    }
    buf[0]=Buf[Len-3];//��
    buf[1]=Buf[Len-4];//��
    buf[2]=Buf[Len-5];//ʱ
    buf[3]=Buf[Len-6];//��
    buf[5]=Buf[Len-7];//��
    buf[6]=Buf[Len-8];//��
    P8563_settime(buf);
    Buf[1]=0x80;
    Buf[Len-2]=CRC8(Buf,Len-2);
    Buf[Len-1]=0x16;
    Send_UART(G_UART_Num,Buf,Len);
}
/*******************************************************************************
* Function Name  : CJQ_RTC_Read
* Description    : ��ȡ�ɼ���ʵʱʱ��
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void CJQ_RTC_Read(u8 *Buf,u16 Len)
{
    extern u8 G_UART_Num;
    u8 rtc_tmp[6];
    u8 buf[38];
    if(Len!=23)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    memcpy(buf,Buf,Len);
    P8563_gettime(rtc_tmp);
	exchange(rtc_tmp,6);
    buf[1]=0x80;
    buf[20]=0x06;
    buf[21]=rtc_tmp[0];
    buf[22]=rtc_tmp[1];
    buf[23]=rtc_tmp[2];
    buf[24]=rtc_tmp[3];
    buf[25]=rtc_tmp[4];
    buf[26]=rtc_tmp[5];
    buf[27]=CRC8(buf,27);
    buf[28]=0x16;
    Send_UART(G_UART_Num,buf,29);
}
/***********************************************************************************
* Function Name  : CJQ_Status_Read
* Description    : ���Ĳɼ���״̬
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
************************************************************************************/
CJQ_Status G_status;
void CJQ_Status_Read(u8* Buf,u16 Len)
{
    extern u8 G_UART_Num;
    u8 tmp[257],tmp1[257],cmd[30];
    u8 param[6];//����ģ����
    u8 i,j,meter_check[256],tmp_add1[8],tmp_add2[8];  //������ظ���
    if(Len!=23)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    memcpy(cmd,Buf,Len);
    G_status.ST00=0;
    G_status.ST01=0;
    G_status.ST02=0;
    G_status.ST03=0;
    G_status.ST04=0;
    G_status.ST05=0;
    G_status.ST06=0;
    G_status.ST07=0;

    G_status.ST10=0;
    G_status.ST11=0;
    for(i=0; i<5; i++) //���flash�Ƿ���������
    {
        tmp[i]=rand()%256;
    }
    SPI_Flash_Write(EXT_FLASH_CHECK, tmp, 5);
    SPI_Flash_Read(EXT_FLASH_CHECK,tmp1,5);

    if(memcmp(tmp,tmp1,5)!=0)
        G_status.ST12=1;
    else
        G_status.ST12=0;


    G_status.ST13=0;
    //G_status.ST14=0;
    if(((1.42 - G_AD_Value[1]*3.3/4096)*1000/4.35 + 25)>65)//MCU�ڲ��¶�
        G_status.ST15=1;
    else
        G_status.ST15=0;
    G_status.ST16=0;

    if(Get_WireLess_Parameter(param)==6)//����ģ��
    {
        G_status.ST17=0;
    }
    else
    {
        G_status.ST17=1;
    }
    G_status.ST20=0;
    G_status.ST21=0;
    G_status.ST22=0;
    G_status.ST23=0;


    SPI_Flash_Read(EXT_CJQ_PARAMETER_PAGEADDR,tmp,CJQ_PARAMETER_SIZE);
    ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,tmp1,CJQ_PARAMETER_SIZE);
    if(memcmp(tmp,tmp1,100)!=0)
    {
        G_status.ST24=1;
    }
    else
    {
        SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,tmp,METER_CHECK_SIZE);
        ReadFlash_Byte(METER_CHECK_PAGEADDR,tmp1,METER_CHECK_SIZE);
        if(memcmp(tmp,tmp1,257)!=0)
        {
            G_status.ST24=1;
        }
        else
            G_status.ST24=0;
    }

//    G_status.ST25=0;
    G_status.ST26=0;
    G_status.ST27=0;

    //G_status.ST30=0;
    //G_status.ST31=0;

    G_status.ST33=0; //���	SPI_Flash_Read(EXT_METER_NUM_ADDR,&EXTMeterNum,1);
    SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,meter_check,256);
    for(i=0; i<256; i++)
    {
        for(j=i+1; j<256; j++)
        {
            if(meter_check[i]==meter_check[j])
            {
                if(meter_check[i]==0xFF)	 break;

                SPI_Flash_Read(EXT_METER_NUM_PAGEADDR+i*8,tmp_add1,8);
                SPI_Flash_Read(EXT_METER_NUM_PAGEADDR+j*8,tmp_add2,8);
                if(memcmp(tmp_add1,tmp_add2,8)==0)
                {
                    G_status.ST33=1; //����ظ�
                    break;
                }
            }
        }
        if(G_status.ST33==1) break;
    }
    //G_status.ST34=0;
    G_status.ST35=0;
    G_status.ST36=0;
    G_status.ST37=0;

    G_status.ST40=0;
    G_status.ST41=0;
    G_status.ST42=0;
    G_status.ST43=0;
    G_status.ST44=0;
    G_status.ST45=0;
    G_status.ST46=0;
    G_status.ST47=0;
    cmd[1]=0x80;
    cmd[20]=0x05;
    memcpy(cmd+21,&G_status,sizeof(G_status));
    cmd[26]=CRC8(cmd,26);
    cmd[27]=0x16;
    Send_UART(G_UART_Num,cmd,28);

}
/***********************************************************************************
* Function Name  : CJQ_Wireless_NetType_Read
* Description    : ��ȡ�ɼ�������ģ����������
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None

************************************************************************************/
void CJQ_Wireless_NetType_Read(u8* Buf,u16 Len)
{
    extern u8 G_UART_Num;
    u8 tmp[24];
	if(Len!=23)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    ReadFlash_Byte(WIRELESS_NETTYPE_ADDR,&tmp[21],1);
    tmp[1]=0x80;
    tmp[20]=0x01;
    tmp[22]=CRC8(Buf,Len-2);
    tmp[23]=0x16;
    Send_UART(G_UART_Num,tmp,24);
}
/***********************************************************************************
* Function Name  : CJQ_Wireless_NetType_Set
* Description    : ���Ĳɼ�������ģ����������
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None

************************************************************************************/
void CJQ_Wireless_NetType_Set(u8* Buf,u16 Len)
{
    extern u8 G_UART_Num;
    u8 tmp;
	if(Len!=24)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    tmp=Buf[Len-3];
    WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,21,&tmp,1);
    Buf[1]=0x80;
    Buf[Len-2]=CRC8(Buf,Len-2);
    Buf[Len-1]=0x16;
    Send_UART(G_UART_Num,Buf,Len);
}
/***********************************************************************************
* Function Name  : CJQ_NetAddr_Alter
* Description    : ���Ĳɼ��������ַ
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None

************************************************************************************/
void CJQ_NetAddr_Alter(u8* Buf,u16 Len)
{
    extern u8 G_UART_Num;
    u8 buf[8];
	if(Len!=31)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
	memcpy(buf,Buf+2,8);
    buf[4]=Buf[25];
    buf[5]=Buf[26];
    WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,0,buf,8);
    SPI_Flash_Write(EXT_CJQ_PARAMETER_PAGEADDR,buf,8);
    Buf[1]=0x80;
    Buf[6]=Buf[25];
    Buf[7]=Buf[26];
    Buf[Len-2]=CRC8(Buf,Len-2);
    Buf[Len-1]=0x16;
    Send_UART(G_UART_Num,Buf,Len);
}
/***********************************************************************************
* Function Name  : CJQ_NetAddr_Alter
* Description    : ���Ĳɼ�����ַ
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None

************************************************************************************/
void CJQ_Addr_Alter(u8* Buf,u16 Len)
{
    extern u8 G_UART_Num;
    u8 buf[8];
	if(Len!=31)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
	memcpy(buf,Buf+21,8);
    WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,0,buf,8);
    SPI_Flash_Write(EXT_CJQ_PARAMETER_PAGEADDR,buf,8);
    Buf[1]=0x80;
	memcpy(Buf+2,buf,8);
    Buf[Len-2]=CRC8(Buf,Len-2);
    Buf[Len-1]=0x16;
    Send_UART(G_UART_Num,Buf,Len);
}
/*******************************************************************************
* Function Name  : Back_Wireless_Addr
* Description    : ��������ģ���ַ����
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void Back_Wireless_Addr(u8* Buf,u16 Len)
{
    extern u8 G_UART_Num;
    u8 tmp[27],pParameter[6];
	if(Len!=23)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    memcpy(tmp,Buf,Len);
    tmp[1]=0x80;
    if(Get_WireLess_Parameter(pParameter)!=6)
	{
        Err_Ack(Buf,Radio_Read_Err);
        return;	
	}
    tmp[20]=0x04;
    tmp[21]=pParameter[0];
    tmp[22]=pParameter[1];
    tmp[23]=pParameter[2];
    tmp[24]=pParameter[3];
    tmp[25]=CRC8(tmp,25);
    tmp[26]=0x16;
    Send_UART(G_UART_Num,tmp,27);
}
/*******************************************************************************
* Function Name  : Back_Wireless_Parameter
* Description    : ��������ģ�����
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void Back_Wireless_Parameter(u8* Buf,u16 Len)
{
    extern u8 G_UART_Num;
    u8 tmp[29],pParameter[6];
	if(Len!=27)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    memcpy(tmp,Buf,Len);
    tmp[1]=0x80;
    Get_WireLess_Parameter(pParameter);
    tmp[20]=0x06;
    tmp[21]=pParameter[5];
    tmp[22]=0xff;
    tmp[23]=0;
    tmp[24]=pParameter[4];
    tmp[25]=0x04;
    tmp[26]=0x07;//�򿵿��в�����20K��0x07��ʾ19200���Ʊ�ʾ
    tmp[27]=CRC8(tmp,27);
    tmp[28]=0x16;
    Send_UART(G_UART_Num,tmp,29);
}
/*******************************************************************************
* Function Name  : Set_Wireless_Parameter
* Description    : ��������ģ�����
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void Set_Wireless_Parameter(u8* Buf,u16 Len)
{
    extern u8 G_UART_Num;
    u8 pParameter[6];
	if(Len!=29)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    pParameter[5]=Buf[21];
    Set_WireLess_Parameter(TYPE_TRANSMIT_POWER,pParameter+5);
    pParameter[4]=Buf[24];
    Set_WireLess_Parameter(TYPE_CHANNEL_NUM,pParameter+4);
    Buf[1]=0x80;
    Buf[Len-2]=CRC8(Buf,Len-2);
    Buf[Len-1]=0x16;
    Send_UART(G_UART_Num,Buf,Len);
}
/*******************************************************************************
* Function Name  : Set_Wireless_Addr
* Description    : ��������ģ���ַ
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void Set_Wireless_Addr(u8* Buf,u16 Len)
{
    extern u8 G_UART_Num;
    u8 pParameter[6];
	if(Len!=27)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    pParameter[0]=Buf[21];
    pParameter[1]=Buf[22];
    pParameter[2]=Buf[23];
    pParameter[3]=Buf[24];
    Set_WireLess_Parameter(TYPE_ADDR,pParameter);
    Buf[1]=0x80;
    Buf[Len-2]=CRC8(Buf,Len-2);
    Buf[Len-1]=0x16;
    Send_UART(G_UART_Num,Buf,Len);
}


/*******************************************************************************
* Function Name  : METER_Err_Ack
* Description    : ����쳣Ӧ��
* Input          : Buf���������飬ErrCode:�������
* Output         : None
* Return         : None
*******************************************************************************/
void METER_Err_Ack(u8 *Buf,u8 ErrCode)
{
    u8 RecData[28],Read_Cjq_ID[8];
	RecData[0]=0x68;
    RecData[1]=0xC0;    //��Э��ͷ

    SPI_Flash_Read(CJQ_PARAMETER_PAGEADDR,Read_Cjq_ID,8);
	memcpy(RecData+2,Read_Cjq_ID,8); //��ɼ�����ַ	   
	memcpy(RecData+10,Buf,9) ;//װ�� 68H	T	Addr 
    if(Buf[9]==0xD6) //?????
        RecData[19]=0x49;
    else
        RecData[19]=0x21;
    RecData[20]=0x01;
    RecData[21]=DataErr;
    RecData[22]=CRC8(RecData,22);
    RecData[23]=0x16;
    UART_3(RecData,24);//�ϴ���������
}
/*******************************************************************************
* Function Name  : Err_Ack
* Description    : �쳣Ӧ��
* Input          : Buf���������飬ErrCode:�������
* Output         : None
* Return         : None
*******************************************************************************/
void Err_Ack(u8 *Buf,u8 ErrCode)
{
    extern u8 G_UART_Num;
    Buf[1]=0xC0;
    Buf[20]=0x01;
    Buf[21]=ErrCode;//У�������
    Buf[22]=CRC8(Buf,22);
    Buf[23]=0x16;
    Send_UART(G_UART_Num,Buf,24);
}
/*******************************************************************************
* Function Name  : Right_Ack_Len0
* Description    : ��������յ����ݳ��Ⱦ�Ϊ0
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void Right_Ack_Len0(u8 *Buf,u16 Len)
{
    extern u8 G_UART_Num;
    Buf[1]=0x80;
    Buf[Len-2]=CRC8(Buf,Len-2);
    Send_UART(G_UART_Num,Buf,Len);
}
/*******************************************************************************
* Function Name  : Right_Ack_Change_CJQAddr
* Description    : ��������յ����ݳ�����ͬ
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void Right_Ack_Change_CJQAddr(u8 *Buf,u16 Len)
{
    u8 i;
    extern u8 G_UART_Num;
    Buf[1]=0x80;
    for(i=0; i<8; i++)
    {
        Buf[i+2]=Buf[i+21];
    }
    Buf[Len-2]=CRC8(Buf,Len-2);
    Send_UART(G_UART_Num,Buf,Len);
}
/*******************************************************************************
* Function Name  : Right_Ack_Change_MeterAddr
* Description    : ��������յ����ݳ�����ͬ
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void Right_Ack_Change_MeterAddr(u8 *Buf,u16 Len)
{
    u8 i;
    extern u8 G_UART_Num;
    Buf[1]=0x80;
    for(i=0; i<8; i++)
    {
        Buf[i+12]=Buf[i+21];
    }
    Buf[Len-2]=CRC8(Buf,Len-2);
    Send_UART(G_UART_Num,Buf,Len);
}
/***********************************************************************************
* Function Name  : CJQ_Addr_Check
* Description    : ���ɼ�����ַ�Ƿ���ȷ
* Input          : *Buf����λ�����͵�����֡
* Output         : None
* Return         : return 0���ɼ�����ַ����ȷ
				   return 1���ⲿflash�вɼ�����ַ��ȷ
				   return 2���ⲿflash�вɼ�����ַ����ȷ���ڲ�flash�вɼ�����ַ��ȷ
************************************************************************************/
u8 CJQ_Addr_Check(u8*Buf)
{
    u8 buf[8],i;
    SPI_Flash_Read(EXT_CJQ_PARAMETER_PAGEADDR,buf,8);
    for(i=0; i<8; i++)
    {
        if(buf[i]!=Buf[i+2])break;
    }
    if(i==8)
        return 1; //�ⲿflash�вɼ�����ַ��ȷ
    else
    {
        ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,buf,8);
        for(i=0; i<8; i++)
        {
            if(buf[i]!=Buf[i+2])break;
        }
        if(i==8)
        {
            ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,buf,8);
            SPI_Flash_Write(EXT_CJQ_PARAMETER_PAGEADDR,buf,8);
            return 2;//�ڲ�flash�вɼ�����ַ��ȷ
        }
        else
        {
            ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,buf,8);
            SPI_Flash_Write(EXT_CJQ_PARAMETER_PAGEADDR,buf,8);
            return 0;//	�����ɼ�����ַ������ȷ
        }
    }
}
/*******************************************************************************
* Function Name  : Copy_Inter_To_EXT
* Description    : ���ڲ�flash�е�������Ϣcopy�� �ⲿflash
* Input          : None
* Output         : None
* Return         : return1��copy�ɹ�
				   return 0��copy���ɹ�
*******************************************************************************/
u8  Copy_Inter_To_EXT(void)
{
    u8 WriteStat=0,i=0,buf[2048];
    ReadFlash_Byte(WENKONG_PAGEADDR,buf,WENKONG_SIZE); //�¿ص�ַ
    WriteStat=SPI_Flash_Write(EXT_WENKONG_PAGEADDR,buf,WENKONG_SIZE);

    ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,buf,CJQ_PARAMETER_SIZE);//�ɼ�������
    WriteStat=SPI_Flash_Write(EXT_CJQ_PARAMETER_PAGEADDR,buf,CJQ_PARAMETER_SIZE);

    ReadFlash_Byte(METER_CHECK_PAGEADDR,buf,METER_CHECK_SIZE);//�������
    WriteStat=SPI_Flash_Write(EXT_METER_CHECK_PAGEADDR,buf,METER_CHECK_SIZE);

    ReadFlash_Byte(METER_NUM_PAGEADDR,buf,METER_NUM_SIZE);//���
    WriteStat=SPI_Flash_Write(EXT_METER_NUM_PAGEADDR,buf,METER_NUM_SIZE);
    for(i=0; i<128; i++)	//���Ʊ�����
    {
        ReadFlash_Byte(METER_DATA_PAGEADDR-i*2048,buf,METER_DATA_SIZE); //��1K�ռ������
        WriteStat=SPI_Flash_Write(EXT_METER_DATA_PAGEADDR+i*4096,buf,METER_DATA_SIZE);

        ReadFlash_Byte(METER_DATA_PAGEADDR-i*2048+PAGESIZE/2,buf,METER_DATA_SIZE); //��1K�ռ������
        WriteStat=SPI_Flash_Write(EXT_METER_DATA_PAGEADDR+i*4096+PAGESIZE/2,buf,METER_DATA_SIZE);

    }
    return WriteStat;
}
/*******************************************************************************
* Function Name  : Copy_CJQ_Parameter_From_Inter_To_EXT
* Description    : ���ڲ�flash�еĲɼ�����ʼ������copy�� �ⲿflash
* Input          : None
* Output         : None
* Return         : return1��copy�ɹ�
				   return 0��copy���ɹ�
*******************************************************************************/
u8  Copy_CJQ_Parameter_From_Inter_To_EXT(void)
{
    u8 WriteStat=0,buf[2048];

    ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,buf,CJQ_PARAMETER_SIZE);//�ɼ�������
    WriteStat=SPI_Flash_Write(EXT_CJQ_PARAMETER_PAGEADDR,buf,CJQ_PARAMETER_SIZE);
    return WriteStat;
}
/*******************************************************************************
* Function Name  : Copy_MeterCheck_And_MeterCheckCheck_From_Inter_To_EXT
* Description    : ���ڲ�flash�еı������������У��copy�� �ⲿflash
* Input          : None
* Output         : None
* Return         : return1��copy�ɹ�
				   return 0��copy���ɹ�
*******************************************************************************/
u8  Copy_MeterCheck_And_MeterCheckCheck_From_Inter_To_EXT(void)
{
    u8 WriteStat=0,buf[2048];
    ReadFlash_Byte(METER_CHECK_PAGEADDR,buf,METER_CHECK_SIZE);//�������
    WriteStat=SPI_Flash_Write(EXT_METER_CHECK_PAGEADDR,buf,METER_CHECK_SIZE);
    return WriteStat;
}
/*******************************************************************************
* Function Name  : Copy_Meter_Num_From_Inter_To_EXT
* Description    : ���ڲ�flash�еı�ߵ�ַcopy�� �ⲿflash
* Input          : None
* Output         : None
* Return         : return1��copy�ɹ�
				   return 0��copy���ɹ�
*******************************************************************************/
u8  Copy_Meter_Num_From_Inter_To_EXT(void)
{
    u8 WriteStat=0,buf[2048];
    ReadFlash_Byte(METER_NUM_PAGEADDR,buf,METER_NUM_SIZE);//���
    WriteStat=SPI_Flash_Write(EXT_METER_NUM_PAGEADDR,buf,METER_NUM_SIZE);
    return WriteStat;
}
/*******************************************************************************
* Function Name  : Copy_Meter_Data_From_Inter_To_EXT
* Description    : ���ڲ�flash�еı������copy�� �ⲿflash
* Input          : None
* Output         : None
* Return         : return1��copy�ɹ�
				   return 0��copy���ɹ�
*******************************************************************************/
u8  Copy_Meter_Data_From_Inter_To_EXT(void)
{
    u8 WriteStat=0,i=0,buf[2048];
    for(i=0; i<128; i++)	//���Ʊ�����
    {
        ReadFlash_Byte(METER_DATA_PAGEADDR-i*2048,buf,METER_DATA_SIZE); //��1K�ռ������
        WriteStat=SPI_Flash_Write(EXT_METER_DATA_PAGEADDR+i*4096,buf,METER_DATA_SIZE);

        ReadFlash_Byte(METER_DATA_PAGEADDR-i*2048+PAGESIZE/2,buf,METER_DATA_SIZE); //��1K�ռ������
        WriteStat=SPI_Flash_Write(EXT_METER_DATA_PAGEADDR+i*4096+PAGESIZE/2,buf,METER_DATA_SIZE);

    }
    return WriteStat;
}
/*******************************************************************************
* Function Name  : Copy_WenKong_Address_From_Inter_To_EXT
* Description    : ���ڲ�flash�е��¿ص�ַcopy�� �ⲿflash
* Input          : None
* Output         : None
* Return         : return1��copy�ɹ�
				   return 0��copy���ɹ�
*******************************************************************************/
u8  Copy_WenKong_Address_From_Inter_To_EXT(void)
{
    u8 WriteStat=0,buf[2048];
    ReadFlash_Byte(WENKONG_PAGEADDR,buf,WENKONG_SIZE); //�¿ص�ַ
    WriteStat=SPI_Flash_Write(EXT_WENKONG_PAGEADDR,buf,WENKONG_SIZE);
    return WriteStat;
}
/*******************************************************************************
* Function Name  : Add_One_Meter   (��)
* Description    : ���һ̨������
* Input          : *Buf:���һ̨����������֡��
				   Len��֡����
* Output         : None
* Return         : return1����ӳɹ�
				   return 0����Ӳ��ɹ�
*******************************************************************************/
u8  Add_One_Meter_t(u8 *Buf,u16 Len)
{
    u8 EXTMeterNum=0,InterMeterNum=0,Count=0,i,j,Check_Space=0;
    u8 NowMeterCheck,MeterCheckCheck;
    u8 MeterCheck[256],buf[8]= {0};

    for(i=0; i<7; i++)
    {
        if(Buf[12+i]!=0x00)break;
    }
    if(i==7)
    {
        return MeterCodeIsAllZero;
    }
    NowMeterCheck=CRC8(&Buf[12],7);

    if(NowMeterCheck==0xff)
    {
        NowMeterCheck+=1;
    }
    ReadFlash_Byte(METER_NUM_ADDR,&InterMeterNum,1);
    SPI_Flash_Read(EXT_METER_NUM_ADDR,&EXTMeterNum,1);
    if((InterMeterNum==EXTMeterNum)&&(Check_Index_Equ()))//���ⲿflash����
    {
        SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,G_PageTmp,256);
        for(i=0; i<255; i++)
        {
            if(G_PageTmp[i]==0xff)
            {
                Check_Space=i;	//��¼��λλ��
                break;// �ҵ����˳�
            }
        }
        if(i==255)
        {
            return StorageSpaceOverFlow;
        }
        for(i=0; i<255; i++)
        {
//			if(Count<EXTMeterNum)
//			    {
            if(G_PageTmp[i]!=0xff)
            {
                if(G_PageTmp[i]==NowMeterCheck)
                {
                    SPI_Flash_Read(EXT_METER_NUM_PAGEADDR+i*8,buf,8);
                    for(j=0; j<8; j++)
                    {
                        if(buf[j]!=Buf[j+11]) break;
                    }
                    if(j==8)
                    {
                      
                        return DoubleMeter;
                    }
                }
                Count++;
            }
            //  }
            //	else  break;
        }
        EXTMeterNum+=1;
        SPI_Flash_Write(EXT_METER_CHECK_PAGEADDR+Check_Space,&NowMeterCheck,1);//�ⲿflashд����У����
        SPI_Flash_Write(EXT_METER_NUM_PAGEADDR+Check_Space*8,&Buf[11],8);//�ⲿflashд�������ͼ����
        SPI_Flash_Write(EXT_METER_NUM_ADDR,&EXTMeterNum,1);//�ⲿflashд��������
        SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,MeterCheck,255);
        MeterCheckCheck=CRC8(MeterCheck,255);
        if( MeterCheckCheck==0xff)
        {
            MeterCheckCheck+=1;
        }
        SPI_Flash_Write(EXT_METER_CHECK_PAGEADDR+255,&MeterCheckCheck,1);//�ⲿflashд����������У��
        if(Check_Space%2==0)
        {
            ReadFlash_Byte(METER_CHECK_PAGEADDR+Check_Space,buf,2 );
            buf[0]=NowMeterCheck;
            WriteFlash_Page(METER_CHECK_PAGEADDR,Check_Space,buf,2);//�ڲ�flashд����У����
        }
        else
        {
            ReadFlash_Byte(METER_CHECK_PAGEADDR+Check_Space-1,buf,2 );
            buf[1]=NowMeterCheck;
            WriteFlash_Page(METER_CHECK_PAGEADDR,Check_Space-1,buf,2);//�ڲ�flashд����У����
        }
        WriteFlash_Page(METER_NUM_PAGEADDR,Check_Space*8,&Buf[11],8);//�ڲ�flashд�������ͼ����
        ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR+16,buf,2 );
        buf[1]=EXTMeterNum;
        ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,MeterCheck,256);
        WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,16,buf,2);//�ڲ�flashд��������,����ż��ַд��
        ReadFlash_Byte(METER_CHECK_PAGEADDR,MeterCheck,256);
        MeterCheckCheck=CRC8(MeterCheck,255);
        if( MeterCheckCheck==0xff)
        {
            MeterCheckCheck+=1;
        }
        ReadFlash_Byte(METER_CHECK_PAGEADDR+254,buf,2 );
        buf[1]=MeterCheckCheck;
        WriteFlash_Page(METER_CHECK_PAGEADDR,254,buf,2);//�ڲ�flashд����������У��
        ReadFlash_Byte(METER_CHECK_PAGEADDR,MeterCheck,256);
   //     Right_Ack_Len0(Buf, Len);
        return Success;
    }
    else //���ڲ�flash����
    {
        ReadFlash_Byte(METER_CHECK_PAGEADDR,G_PageTmp,256 );
        for (i=0; i<255; i++)
        {
            if(G_PageTmp[i]==0xff)
            {
                Check_Space=i;	//��¼��λλ��
                break;
            }
        }
        if(i==255)
        {
           
            return StorageSpaceOverFlow;
        }
        for(i=0; i<255; i++)
        {
            if(Count<InterMeterNum)
            {
                if(G_PageTmp[i]!=0xff)
                {
                    if(G_PageTmp[i]==NowMeterCheck)
                    {
                        ReadFlash_Byte(METER_NUM_PAGEADDR+i*8,buf,8);
                        for(j=0; j<8; j++)
                        {
                            if(buf[j]!=Buf[j+11]) break;
                        }
                        if(j==8)
                        {
                            return DoubleMeter;
                        }
                    }
                    Count++;
                }
            }
            else  break;   //�ҵ���λλ���˳�
        }
        InterMeterNum+=1;
        if(Check_Space%2==0)
        {
            ReadFlash_Byte(METER_CHECK_PAGEADDR+Check_Space,buf,2 );
            buf[0]=NowMeterCheck;
            WriteFlash_Page(METER_CHECK_PAGEADDR,Check_Space,buf,2);//�ڲ�flashд����У����
        }
        else
        {
            ReadFlash_Byte(METER_CHECK_PAGEADDR+Check_Space-1,buf,2 );
            buf[1]=NowMeterCheck;
            WriteFlash_Page(METER_CHECK_PAGEADDR,Check_Space-1,buf,2);//�ڲ�flashд����У����
        }
        WriteFlash_Page(METER_NUM_PAGEADDR,Check_Space*8,&Buf[11],8);//�ڲ�flashд�������ͼ����
        ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR+16,buf,2 );
        buf[1]=InterMeterNum;
        WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,16,buf,2);//�ڲ�flashд��������
        ReadFlash_Byte(METER_CHECK_PAGEADDR,MeterCheck,256 );
        MeterCheckCheck=CRC8(MeterCheck,255);
        if( MeterCheckCheck==0xff)
        {
            MeterCheckCheck+=1;
        }
        ReadFlash_Byte(METER_CHECK_PAGEADDR+254,buf,2 );
        buf[1]=MeterCheckCheck;
        WriteFlash_Page(METER_CHECK_PAGEADDR,254,buf,2);//�ڲ�flashд����������У��
        ReadFlash_Byte(METER_CHECK_PAGEADDR,MeterCheck,256 );
        Copy_CJQ_Parameter_From_Inter_To_EXT();
        Copy_MeterCheck_And_MeterCheckCheck_From_Inter_To_EXT();
        Copy_Meter_Num_From_Inter_To_EXT();
       // Right_Ack_Len0(Buf, Len);

        return Success;
    }
}
/*******************************************************************************
* Function Name  : Add_One_Meter
* Description    : ���һ̨������
* Input          : *Buf:���һ̨����������֡��
				   Len��֡����
* Output         : None
* Return         : return1����ӳɹ�
				   return 0����Ӳ��ɹ�
*******************************************************************************/
u8  Add_One_Meter(u8 *Buf,u16 Len)
{
    u8 EXTMeterNum=0,InterMeterNum=0,Count=0,i,j,Check_Space=0;
    u8 NowMeterCheck,MeterCheckCheck;
    u8 MeterCheck[256],buf[8]= {0};

    for(i=0; i<7; i++)
    {
        if(Buf[12+i]!=0x00)break;
    }
    if(i==7)
    {
        Err_Ack(Buf,MeterCodeIsAllZero);	//�û����ȫΪ0
        return 0;
    }
    NowMeterCheck=CRC8(&Buf[12],7);

    if(NowMeterCheck==0xff)
    {
        NowMeterCheck+=1;
    }
    ReadFlash_Byte(METER_NUM_ADDR,&InterMeterNum,1);
    SPI_Flash_Read(EXT_METER_NUM_ADDR,&EXTMeterNum,1);
    if((InterMeterNum==EXTMeterNum)&&(Check_Index_Equ()))//���ⲿflash����
    {
        SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,G_PageTmp,256);
        for(i=0; i<255; i++)
        {
            if(G_PageTmp[i]==0xff)
            {
                Check_Space=i;	//��¼��λλ��
                break;// �ҵ����˳�
            }
        }
        if(i==255)
        {
            Err_Ack(Buf,StorageSpaceOverFlow);	//�洢�ռ�����
            return 0;
        }
        for(i=0; i<255; i++)
        {
//			if(Count<EXTMeterNum)
//			    {
            if(G_PageTmp[i]!=0xff)
            {
                if(G_PageTmp[i]==NowMeterCheck)
                {
                    SPI_Flash_Read(EXT_METER_NUM_PAGEADDR+i*8,buf,8);
                    for(j=0; j<8; j++)
                    {
                        if(buf[j]!=Buf[j+11]) break;
                    }
                    if(j==8)
                    {
                        Err_Ack(Buf,DoubleMeter);	//�û�����ظ�
                        return 0;
                    }
                }
                Count++;
            }
            //  }
            //	else  break;
        }
        EXTMeterNum+=1;
        SPI_Flash_Write(EXT_METER_CHECK_PAGEADDR+Check_Space,&NowMeterCheck,1);//�ⲿflashд����У����
        SPI_Flash_Write(EXT_METER_NUM_PAGEADDR+Check_Space*8,&Buf[11],8);//�ⲿflashд�������ͼ����
        SPI_Flash_Write(EXT_METER_NUM_ADDR,&EXTMeterNum,1);//�ⲿflashд��������
        SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,MeterCheck,255);
        MeterCheckCheck=CRC8(MeterCheck,255);
        if( MeterCheckCheck==0xff)
        {
            MeterCheckCheck+=1;
        }
        SPI_Flash_Write(EXT_METER_CHECK_PAGEADDR+255,&MeterCheckCheck,1);//�ⲿflashд����������У��
        if(Check_Space%2==0)
        {
            ReadFlash_Byte(METER_CHECK_PAGEADDR+Check_Space,buf,2 );
            buf[0]=NowMeterCheck;
            WriteFlash_Page(METER_CHECK_PAGEADDR,Check_Space,buf,2);//�ڲ�flashд����У����
        }
        else
        {
            ReadFlash_Byte(METER_CHECK_PAGEADDR+Check_Space-1,buf,2 );
            buf[1]=NowMeterCheck;
            WriteFlash_Page(METER_CHECK_PAGEADDR,Check_Space-1,buf,2);//�ڲ�flashд����У����
        }
        WriteFlash_Page(METER_NUM_PAGEADDR,Check_Space*8,&Buf[11],8);//�ڲ�flashд�������ͼ����
        ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR+16,buf,2 );
        buf[1]=EXTMeterNum;
        ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,MeterCheck,256);
        WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,16,buf,2);//�ڲ�flashд��������,����ż��ַд��
        ReadFlash_Byte(METER_CHECK_PAGEADDR,MeterCheck,256);
        MeterCheckCheck=CRC8(MeterCheck,255);
        if( MeterCheckCheck==0xff)
        {
            MeterCheckCheck+=1;
        }
        ReadFlash_Byte(METER_CHECK_PAGEADDR+254,buf,2 );
        buf[1]=MeterCheckCheck;
        WriteFlash_Page(METER_CHECK_PAGEADDR,254,buf,2);//�ڲ�flashд����������У��
        ReadFlash_Byte(METER_CHECK_PAGEADDR,MeterCheck,256);
   //     Right_Ack_Len0(Buf, Len);
        return 1;
    }
    else //���ڲ�flash����
    {
        ReadFlash_Byte(METER_CHECK_PAGEADDR,G_PageTmp,256 );
        for (i=0; i<255; i++)
        {
            if(G_PageTmp[i]==0xff)
            {
                Check_Space=i;	//��¼��λλ��
                break;
            }
        }
        if(i==255)
        {
            Err_Ack(Buf,StorageSpaceOverFlow); //�洢�ռ�����
            return 0;
        }
        for(i=0; i<255; i++)
        {
            if(Count<InterMeterNum)
            {
                if(G_PageTmp[i]!=0xff)
                {
                    if(G_PageTmp[i]==NowMeterCheck)
                    {
                        ReadFlash_Byte(METER_NUM_PAGEADDR+i*8,buf,8);
                        for(j=0; j<8; j++)
                        {
                            if(buf[j]!=Buf[j+11]) break;
                        }
                        if(j==8)
                        {
                            Err_Ack(Buf,DoubleMeter);	//�û�����ظ�
                            return 0;
                        }
                    }
                    Count++;
                }
            }
            else  break;   //�ҵ���λλ���˳�
        }
        InterMeterNum+=1;
        if(Check_Space%2==0)
        {
            ReadFlash_Byte(METER_CHECK_PAGEADDR+Check_Space,buf,2 );
            buf[0]=NowMeterCheck;
            WriteFlash_Page(METER_CHECK_PAGEADDR,Check_Space,buf,2);//�ڲ�flashд����У����
        }
        else
        {
            ReadFlash_Byte(METER_CHECK_PAGEADDR+Check_Space-1,buf,2 );
            buf[1]=NowMeterCheck;
            WriteFlash_Page(METER_CHECK_PAGEADDR,Check_Space-1,buf,2);//�ڲ�flashд����У����
        }
        WriteFlash_Page(METER_NUM_PAGEADDR,Check_Space*8,&Buf[11],8);//�ڲ�flashд�������ͼ����
        ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR+16,buf,2 );
        buf[1]=InterMeterNum;
        WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,16,buf,2);//�ڲ�flashд��������
        ReadFlash_Byte(METER_CHECK_PAGEADDR,MeterCheck,256 );
        MeterCheckCheck=CRC8(MeterCheck,255);
        if( MeterCheckCheck==0xff)
        {
            MeterCheckCheck+=1;
        }
        ReadFlash_Byte(METER_CHECK_PAGEADDR+254,buf,2 );
        buf[1]=MeterCheckCheck;
        WriteFlash_Page(METER_CHECK_PAGEADDR,254,buf,2);//�ڲ�flashд����������У��
        ReadFlash_Byte(METER_CHECK_PAGEADDR,MeterCheck,256 );
        Copy_CJQ_Parameter_From_Inter_To_EXT();
        Copy_MeterCheck_And_MeterCheckCheck_From_Inter_To_EXT();
        Copy_Meter_Num_From_Inter_To_EXT();
       // Right_Ack_Len0(Buf, Len);
        return 1;
    }
}
/*******************************************************************************
* Function Name  : Read_CJQAddr
* Description    : ��ȡ�ɼ�����ַ
* Input          : *Buf:��ȡ�ɼ�����ַ����֡��
				   Len��֡����
* Output         : None
* Return         : return1�����ⲿflash����
				   return 0�����ڲ�flash����
*******************************************************************************/
u8  Read_CJQAddr(u8 *Buf,u16 Len)
{
    u8 EXT_CJQAddr[8];
    u8 Inter_CJQAddr[8];
    u8 i=0;
    extern u8 G_UART_Num;
	if(Len!=23)
    {
        return 1;
    }
    ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,Inter_CJQAddr,8 );
    SPI_Flash_Read(EXT_CJQ_PARAMETER_PAGEADDR,EXT_CJQAddr,8);   //��ȡflash
    if (Check_Index_Equ()) //EXT_Flash
    {
        if(Inter_CJQAddr==EXT_CJQAddr)
        {
//			    SPI_Flash_Read(EXT_CJQ_PARAMETER_PAGEADDR,EXT_CJQAddr,8);   //��ȡEXT_Flash
            Buf[1]=0x80;
            for(i=0; i<8; i++)
            {
                Buf[i+2]=EXT_CJQAddr[i];
            }
            Buf[Len-2]=CRC8(Buf,Len-2);
            Send_UART(G_UART_Num,Buf,Len);
            return 1;
        }
        else
        {
//				ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,Inter_CJQAddr,8 );
            Buf[1]=0x80;
            for(i=0; i<8; i++)
            {
                Buf[i+2]=Inter_CJQAddr[i];
            }
            Buf[Len-2]=CRC8(Buf,Len-2);
            SPI_Flash_Write(EXT_CJQ_PARAMETER_PAGEADDR,Inter_CJQAddr,8);//д��flash
			delay_ms(Inter_CJQAddr[0]); //ͨ���������ܳ�ͻ��lixiaowei
            Send_UART(G_UART_Num,Buf,Len);
            return 0;
        }
    }
    else  //Inter_Flash
    {
        if(Inter_CJQAddr==EXT_CJQAddr)
        {
//			ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,Inter_CJQAddr,8 );
            Buf[1]=0x80;
            for(i=0; i<8; i++)
            {
                Buf[i+2]=Inter_CJQAddr[i];
            }
            Buf[Len-2]=CRC8(Buf,Len-2);
            Copy_MeterCheck_And_MeterCheckCheck_From_Inter_To_EXT();
            Copy_Meter_Num_From_Inter_To_EXT();
			delay_ms(Inter_CJQAddr[0]); //ͨ���������ܳ�ͻ��lixiaowei
            Send_UART(G_UART_Num,Buf,Len);
            return 0;
        }
        else
        {
//			 	ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,Inter_CJQAddr,8 );
            Buf[1]=0x80;
            for(i=0; i<8; i++)
            {
                Buf[i+2]=Inter_CJQAddr[i];
            }
            Buf[Len-2]=CRC8(Buf,Len-2);
            SPI_Flash_Write(EXT_CJQ_PARAMETER_PAGEADDR,Inter_CJQAddr,8);//д��flash
            Copy_MeterCheck_And_MeterCheckCheck_From_Inter_To_EXT();
            Copy_Meter_Num_From_Inter_To_EXT();
			delay_ms(Inter_CJQAddr[0]); //ͨ���������ܳ�ͻ��lixiaowei
            Send_UART(G_UART_Num,Buf,Len);
            return 0;
        }
    }
}
/*******************************************************************************
* Function Name  : Init_CJQ
* Description    : ��ʼ���ɼ������ɼ�����ַ������ģ���ַ����ʼ��Ϊȫ��AA����
                   ���������0������ģʽΪ1������ģʽ���������ʼ��Ϊ��FF��
* Input          : *Buf:��ʼ���ɼ����������飬
				   Len�����鳤��
* Output         : None
* Return         : return1���ɹ���ʼ����
				   return 0��δ�ɹ���ʼ��
*******************************************************************************/
u8	Init_CJQ(u8 *Buf,u16 Len)
{

    u8 i=0, CJQ_Check=0,buf[21];
    for (i=0; i<132; i++)	//132ҳ
    {
        FlashErase_Page(i*2048+0x0803E000);
    }
    SPI_Flash_Erase_Chip();    	  //��Ƭ�ⲿflash����
    for(i=0; i<16; i++)
    {
        buf[i]=0x00;
    }
    buf[16]=0x01;//����ģʽ
    buf[17]=0x00;//�������
    buf[18]=0x00;//����ʱ��
    buf[19]=0x00;//����ʱ��
    buf[20]=0x30;//����ʱ��
    CJQ_Check=WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,0,buf,21);
    if(CJQ_Check==0)//�ڲ�flashд����
    {
        Err_Ack(Buf,WriteInterFlashErr);
        return 0;
    }
    ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,buf,21);
    CJQ_Check=SPI_Flash_Write(EXT_CJQ_PARAMETER_PAGEADDR,buf,21);
    if(CJQ_Check==0)   //�ⲿflashд����
    {
        Err_Ack(Buf,WriteEXTFlashErr);
        return 0;
    }
    else 		 //����Ӧ��
    {
        Right_Ack_Len0(Buf,Len);
        return 1;
    }
}
/*******************************************************************************
* Function Name  : Change_CJQAddr
* Description    : ���Ĳɼ�����ַ
* Input          : *Buf:��ʼ���ɼ����������飬
				   Len�����鳤��
* Output         : None
*******************************************************************************/
void  Change_CJQAddr(u8 *Buf,u16 Len)
{
//		u8 BUF1[20];
    if (Check_Index_Equ())
    {
        WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,0,&Buf[21],8 );
        SPI_Flash_Write(EXT_CJQ_PARAMETER_PAGEADDR,&Buf[21],8);
//			ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,BUF1,21);
//			SPI_Flash_Read(EXT_CJQ_PARAMETER_PAGEADDR,BUF1,21);
        Right_Ack_Change_CJQAddr(Buf,Len);
    }
    else
    {
        WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,0,&Buf[21],8 );
        Right_Ack_Change_CJQAddr(Buf,Len);
        Copy_CJQ_Parameter_From_Inter_To_EXT();
        Copy_MeterCheck_And_MeterCheckCheck_From_Inter_To_EXT();
        Copy_Meter_Num_From_Inter_To_EXT();
//	   SPI_Flash_Read(EXT_CJQ_PARAMETER_PAGEADDR,BUF1,21);
    }
}
/*******************************************************************************
* Function Name  : Read_Data_Command
* Description    : ��ȡ��ֻ���ʵʱ����ʱ����ϳ���ָ��
* Input          : Buf����λ�����͵�����֡
* Output         : None
* Return         : None
*******************************************************************************/
void Read_Data_Command(u8*Buf)
{
    u8 buf[19]= {0xFE,0xFE,0xFE,0x68,0x20,0xAA,0xAA,0xAA,0xAA,0xAA,
                 0xAA,0xAA,0x01,0x03,0x1F,0x90,0x00,0xE1,0x16
                };
    u8 i;
    extern u8 G_Get_Realtime_Data_Flag;
    G_Get_Realtime_Data_Flag=1;
    for(i=0; i<7; i++)
    {
        buf[i+5]=Buf[i+12];
    }
    buf[17]=Add_Up(&buf[3],14);
    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
    USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
    Auto_sent(buf,19);
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}
/*******************************************************************************
* Function Name  : Read_All_Meter_Data_Command
* Description    : ��ȡ���б��ʵʱ����ʱ����ϳ���ָ��
* Input          : Buf����λ�����͵�����֡
* Output         : None
* Return         : None
*******************************************************************************/
void Read_All_Meter_Data_Command(u8*Buf)
{
    u8 buf[19]= {0xFE,0xFE,0xFE,0x68,0x20,0xAA,0xAA,0xAA,0xAA,0xAA,
                 0xAA,0xAA,0x01,0x03,0x1F,0x90,0x00,0xE1,0x16
                };
    u8 i=0,j,Count=0,MeterNum=0;
    u8 buf1[8];
    u8 CheckBuf[256];
    u8 ExtOrInter;
    extern u8 G_Get_All_Meter_Realtime_Data_Flag;

    ExtOrInter=Check_Index_Equ(); // �����������У���Ƿ���ȷ
    if(ExtOrInter) //����EXT_Flash
    {
        SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,CheckBuf,256);  //��ȡflash
        SPI_Flash_Read(EXT_METER_NUM_ADDR,&MeterNum,1);  //��ȡ�������
        for(i=0; i<256; i++)
        {
            if(Count<MeterNum)
            {
                if(CheckBuf[i]!=0xff)
                {
                    G_Get_All_Meter_Realtime_Data_Flag=1;
                    SPI_Flash_Read(EXT_METER_NUM_PAGEADDR+i*8+1,buf1,7);
                    for(j=0; j<7; j++)
                    {
                        buf[j+5]=buf1[j];
                    }
                    buf[17]=Add_Up(&buf[3],14);
                    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
                    USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
                    Auto_sent(buf,19);
                    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
                    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
                    Count++;
                    if(Count>=MeterNum)
                        break;
                    delay_ms(800);
                }
            }
            else  if(Count>=MeterNum)
            {
                Err_Ack(Buf,NotFindMeter);
                break;
            }
        }
    }

    else
    {
        ReadFlash_Byte(METER_CHECK_PAGEADDR,CheckBuf,256);
        ReadFlash_Byte(METER_NUM_ADDR,&MeterNum,1);
        for(i=0; i<255; i++)
        {
            if(Count<MeterNum)
            {
                if(CheckBuf[i]!=0xff)
                {
                    G_Get_All_Meter_Realtime_Data_Flag=1;
                    ReadFlash_Byte(METER_NUM_PAGEADDR+i*8+1,buf1,7);
                    for(j=0; j<7; j++)
                    {
                        buf[i+5]=buf1[i];
                    }
                    buf[17]=Add_Up(&buf[3],14);
                    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
                    USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
                    Auto_sent(buf,19);
                    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
                    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
                    //G_Get_Realtime_Data_Flag=0;
                    Count++;
                    if(Count>=MeterNum)
                        break;

                    delay_ms(800);
                }
            }
            else if(Count>=MeterNum)
            {
                Err_Ack(Buf,NotFindMeter);
                break;
            }
        }
        Copy_Inter_To_EXT(); //���ڲ�flash�е�������Ϣcopy�� �ⲿflash
    }
}
/*******************************************************************************
* Function Name  : Read_One_Meter_Realtime_Data
* Description    : ��ȡ��ֻ���ʵʱ����
* Input          : Buf����λ�����͵�����֡��Len:����֡����
* Output         : None
* Return         : None
*******************************************************************************/
void Read_One_Meter_Realtime_Data(u8 *Buf,u16 Len)
{
    Read_Data_Command(Buf);
}
/*******************************************************************************
* Function Name  : Read_All_Meter_Realtime_Data
* Description    : ��ȡ���б��ʵʱ����
* Input          : Buf����λ�����͵�����֡��Len:����֡����
* Output         : None
* Return         : None
*******************************************************************************/
void Read_All_Meter_Realtime_Data(u8 *Buf,u16 Len)
{
    Read_All_Meter_Data_Command(Buf);
}
/*******************************************************************************
* Function Name  : Return_Real_Data
* Description    : ��ȡ���ʵʱ����ָ���£����ر��ʵʱ����
* Input          : MeterDataRecv����߷��ص�����֡��MeterDataLen:����֡����
* Output         : None
* Return         : None
*******************************************************************************/
void Return_Real_Data(u8*MeterDataRecv,u16 MeterDataLen)
{
    u8 j,i;
    u8 EXTCJQAddr[8];
    u8 InterCJQAddr[8];
    extern u8 G_UART_Num;
    extern u8 G_Get_Realtime_Data_Flag;
    extern u8 G_Get_All_Meter_Realtime_Data_Flag;

    u8 PackedData[270]= {0x68,0x80,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
    SPI_Flash_Read(EXT_CJQ_PARAMETER_PAGEADDR,EXTCJQAddr,8);   //��ȡ�ⲿflash �ɼ�����ַ
    ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,InterCJQAddr,8 );
    for(i=0; i<8; i++)
    {
        if(EXTCJQAddr[i]!=InterCJQAddr[i]) break;
    }
    if(i==8)
    {
        for(j=0; j<8; j++)
        {
            PackedData[j+2]	=EXTCJQAddr[j];
        }
    }
    else
    {
        for(j=0; j<8; j++)
        {
            PackedData[i+2]	=InterCJQAddr[i];
        }
        SPI_Flash_Write(EXT_CJQ_PARAMETER_PAGEADDR,InterCJQAddr,8);//���ڲ�flash�ɼ�����ַд���ⲿflash
    }
    for(i=0; i<MeterDataLen; i++)
    {
        PackedData[i+10]=MeterDataRecv[i];
    }
    PackedData[i+10]=CRC8(PackedData,i+10);
    i++;
    PackedData[i+10]=0x16;
    Send_UART(G_UART_Num,PackedData,i+11);
    G_Get_Realtime_Data_Flag=0;
    G_Get_All_Meter_Realtime_Data_Flag=0;

}
/*******************************************************************************
* Function Name  : Check_Index_Equ
* Description    : �����������У���Ƿ���ȷ
* Input          : None
* Output         : None
* Return         : ��ȷ����1�����ⲿflash��������
                   ���򷵻�0���ⲿflash����ȷ����Ҫ���ڲ�flash������
*******************************************************************************/

u8 Check_Index_Equ(void)
{
    u8 EXTIndexCheck,InterIndexCheck;
    u8 IndexBuf[256];
    u8 IndexCheck;
    ReadFlash_Byte(METER_CHECK_PAGEADDR+255,&InterIndexCheck,1);
    SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR+255,&EXTIndexCheck,1);
    SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,IndexBuf,255);
    IndexCheck=CRC8(IndexBuf,255);
    if(IndexCheck==0xff)
    {
        IndexCheck++;
    }
    if ((InterIndexCheck==0xff)&&(EXTIndexCheck==0xff))
    {
        return 1; //���ⲿflash����
    }
    else if(EXTIndexCheck==IndexCheck)
    {
        return 1;  //���ⲿflash����
    }
    else
    {
        return 0; //���ڲ�flash����
    }
}
/*******************************************************************************
* Function Name  : Composite_Frame_Format
* Description    : ��϶�ȡ��ֻ�������֡��ʽ
* Input          : CmdBuf����λ�����͵�����֡��MeterLocation����ߵĴ洢λ��
				   ExtOrInter�����ⲿ�����ڲ�flash������ʶ��1Ϊ�ⲿflash��0Ϊ
				   �ڲ�flash
* Output         : None
* Return         : ��ȷ���ر�����1
                   ���򷵻�0
*******************************************************************************/
u8 Composite_Frame_Format(u8*CmdBuf,u16 MeterLocation,u8 ExtOrInter)
{
    u8 i;
    u8 ReadOneMeterData[270];
    u8 MeterData[256];
    extern u8 G_UART_Num;
    ReadOneMeterData[0]=0x68;
    ReadOneMeterData[1]=0x80;
    for(i=0; i<18; i++)
    {
        ReadOneMeterData[i+2]=CmdBuf[i+2];
    }
    if(ExtOrInter)
    {
        SPI_Flash_Read(EXT_METER_DATA_PAGEADDR+((MeterLocation/2)*4096)+((MeterLocation%2)*(PAGESIZE/2)),MeterData,256);   //��ȡflash
        for(i=0; i<43; i++)	//������Ч����λ��
        {
            if(MeterData[i]!=0xFF)break;
        }
        if(i==43)
        {
            Err_Ack(CmdBuf,DataErr);
            return 0;

        }
    }
    else
    {
        ReadFlash_Byte(METER_DATA_PAGEADDR-((MeterLocation/2)*2048)+((MeterLocation%2)*(PAGESIZE/2)),MeterData,256);
        for(i=0; i<43; i++) //������Ч����λ��
        {
            if(MeterData[i]!=0xFF)break;
        }
        if(i==43)
        {
            Err_Ack(CmdBuf,DataErr);
            return 0;
        }
    }
    switch (CmdBuf[11])
    {
    case 0x10:  //ˮ��
    case 0x11:
    case 0x12:
    case 0x13:
    {
        ReadOneMeterData[20]=0x13;
        for(i=0; i<19; i++)
        {
            ReadOneMeterData[i+21]=MeterData[i];
        }
        ReadOneMeterData[i+21]=CRC8(ReadOneMeterData,i+21);
        ReadOneMeterData[i+22]=0x16;
        Send_UART(G_UART_Num,ReadOneMeterData,i+23);
        break;
    }
    case 0x20:   //188�ȱ�
    case 0x21:
    case 0x25:
    case 0x29:
    {
        ReadOneMeterData[20]=0x2B;
        for(i=0; i<43; i++)
        {
            ReadOneMeterData[i+21]=MeterData[i];
        }
        ReadOneMeterData[i+21]=CRC8(ReadOneMeterData,i+21);
        i++;
        ReadOneMeterData[i+21]=0x16;
        Send_UART(G_UART_Num,ReadOneMeterData,i+22);
        break;
    }
    case 0x40:   //188�¿���
    case 0x49:
    {
        //
        break;
    }
    case 0x90:   //13757�ȱ�
    case 0x99:
    {
        //
        break;
    }

    }
    return 1;
}
/*******************************************************************************
* Function Name  : Composite_Read_All_Meter_Frame_Format
* Description    : ��϶�ȡ���б������֡��ʽ
* Input          : CmdBuf����λ�����͵�����֡��MeterLocation����ߵĴ洢λ��
				   ExtOrInter�����ⲿ�����ڲ�flash������ʶ��1Ϊ�ⲿflash��0Ϊ
				   �ڲ�flash
* Output         : None
* Return         : None
*******************************************************************************/
void Composite_Read_All_Meter_Frame_Format(u8*CmdBuf,u16 MeterLocation,u8 ExtOrInter)
{
    u8 i,j;
    u8 ReadOneMeterData[270];
    u8 MeterData[256];
    u8 buf[8];
    extern u8 G_UART_Num;
    ReadOneMeterData[0]=0x68;
    ReadOneMeterData[1]=0x80;
    for(i=0; i<9; i++)
    {
        ReadOneMeterData[i+2]=CmdBuf[i+2];
    }
    if(ExtOrInter)
    {
        SPI_Flash_Read(EXT_METER_NUM_PAGEADDR+MeterLocation*8,buf,8);
        for(j=0; j<8; j++)
        {
            ReadOneMeterData[i+2]=buf[j];
            i++;
        }
        SPI_Flash_Read(EXT_METER_DATA_PAGEADDR+((MeterLocation/2)*4096)+((MeterLocation%2)*(PAGESIZE/2)),MeterData,256);   //��ȡflash
    }
    else
    {
        ReadFlash_Byte(METER_NUM_PAGEADDR+i*8,buf,8);
        for(j=0; j<8; j++)
        {
            ReadOneMeterData[i+2]=buf[j];
            i++;
        }
        ReadFlash_Byte(METER_DATA_PAGEADDR-((MeterLocation/2)*2048)+((MeterLocation%2)*(PAGESIZE/2)),MeterData,256);
    }
    switch (CmdBuf[11])
    {
    case 0x10:  //ˮ��
    case 0x11:
    case 0x12:
    case 0x13:
    {
        ReadOneMeterData[i+2]=CmdBuf[19];
		ReadOneMeterData[i+3]=19; //���ݳ���
        for(i=0; i<19; i++)
        {
            ReadOneMeterData[i+20]=MeterData[i];
        }
        ReadOneMeterData[i+20]=CRC8(ReadOneMeterData,i+20);
        ReadOneMeterData[i+21]=0x16;
        Send_UART(3,ReadOneMeterData,i+22);	  //������
        break;
    }
    case 0x20:   //188�ȱ�
    case 0x21:
    case 0x25:
    case 0x29:
    {
        ReadOneMeterData[i+2]=0x02;
        ReadOneMeterData[20]=0x2B;
        for(i=0; i<43; i++)
        {
            ReadOneMeterData[i+21]=MeterData[i];
        }
        ReadOneMeterData[i+21]=CRC8(ReadOneMeterData,i+21);
        i++;
        ReadOneMeterData[i+21]=0x16;
        Send_UART(3,ReadOneMeterData,i+22);//������
        break;
    }
    case 0x40:   //188�¿���
    case 0x49:
    {
        //
        break;
    }
    case 0x90:   //13757�ȱ�
    case 0x99:
    {
        //
        break;
    }

    }
}
/*******************************************************************************
* Function Name  : Return_Meter_Index
* Description    : ���ر����FLASH�е�����
* Input          : meter_add ��ߵ�ַָ��
* Output         : None
* Return         : -1 û�д˱�ߣ����򷵻ض�Ӧ�������
*******************************************************************************/
s32 Return_Meter_Index(u8 *meter_add)
{
	u8 MeterCheck,i=0,j,Count=0,MeterNum=0;
    u8 buf[8];
    u8 CheckBuf[256];
	MeterCheck=CRC8(meter_add+1,7);
	ReadFlash_Byte(METER_CHECK_PAGEADDR,CheckBuf,256);
    ReadFlash_Byte(METER_NUM_ADDR,&MeterNum,1);
        for(i=0; i<255; i++)
        {
            if(Count<MeterNum)
            {
                if(CheckBuf[i]!=0xff)
                {
                    if(CheckBuf[i]==MeterCheck)
                    {
                        ReadFlash_Byte(METER_NUM_PAGEADDR+i*8,buf,8); //
                        for(j=0; j<8; j++)
                        {
                            if(meter_add[j]!=buf[j])break;
                        }
                        if(j==8)
                        {
							return i;
                        }
                    }
                    Count++;
                }
            }
		}
		return Failed;

}

/*******************************************************************************
* Function Name  : Read_One_Meter_Data
* Description    : ��ȡ�ɼ����д洢�ı������
* Input          : CmdBuf����λ�����͵�����֡��CmdLen����λ�����͵������
* Output         : None
* Return         : None
*******************************************************************************/
void Read_One_Meter_Data(u8 *CmdBuf,u16 CmdLen)
{
    u8 MeterCheck,i=0,j,Count=0,MeterNum=0;
    u8 buf[8];
    u8 CheckBuf[256];
    u8 ExtOrInter;
    ExtOrInter=Check_Index_Equ(); // �����������У���Ƿ���ȷ
    MeterCheck=CRC8(&CmdBuf[12],7);
    if(ExtOrInter) //����EXT_Flash
    {
        SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,CheckBuf,256);  //��ȡflash
        SPI_Flash_Read(EXT_METER_NUM_ADDR,&MeterNum,1);  //��ȡ�������
        for(i=0; i<256; i++)
        {
		 if(Count<MeterNum)
		  {
            if(CheckBuf[i]!=0xff)
            {
                if(CheckBuf[i]==MeterCheck)
                {
                    SPI_Flash_Read(EXT_METER_NUM_PAGEADDR+i*8,buf,8); //
                    for(j=0; j<8; j++)
                    {
                        if(CmdBuf[j+11]!=buf[j])break;
                    }
                    if(j==8) //ֻ�в鵽�������Ӧ�ı�߲�ִ���²�
                    {
                        Composite_Frame_Format(CmdBuf,i,ExtOrInter); //	��϶�ȡ��ֻ�������֡��ʽ
                        break;
                    }
                }
                Count++;
            }
			}
			else  if(Count>=MeterNum)
			{
				Err_Ack(CmdBuf,NotFindMeter);
				break;
			}
        }
    }

    else
    {
        ReadFlash_Byte(METER_CHECK_PAGEADDR,CheckBuf,256);
        ReadFlash_Byte(METER_NUM_ADDR,&MeterNum,1);
        for(i=0; i<255; i++)
        {
            if(Count<MeterNum)
            {
                if(CheckBuf[i]!=0xff)
                {
                    if(CheckBuf[i]==MeterCheck)
                    {
                        ReadFlash_Byte(METER_NUM_PAGEADDR+i*8,buf,8); //
                        for(j=0; j<8; j++)
                        {
                            if(CmdBuf[j+11]!=buf[j])break;
                        }
                        if(j==8)
                        {
                            ReadFlash_Byte(METER_DATA_PAGEADDR-((i/2)*2048)+((i%2)*PAGESIZE/2),CheckBuf,256);
                            SPI_Flash_Write(EXT_METER_DATA_PAGEADDR+((i/2)*4096)+((i%2)*PAGESIZE/2),CheckBuf,256);
                            Copy_Meter_Num_From_Inter_To_EXT();
                            Copy_MeterCheck_And_MeterCheckCheck_From_Inter_To_EXT();
                            Composite_Frame_Format(CmdBuf,i,ExtOrInter);
                            break;
                        }
                    }
                    Count++;
                }
            }
            else if(Count>=MeterNum)
            {
                Err_Ack(CmdBuf,NotFindMeter);
                break;
            }
        }
    }
}
/*******************************************************************************
* Function Name  : Read_All_Meter_Data
* Description    : ��ȡ�ɼ����д洢�����б������
* Input          : CmdBuf����λ�����͵�����֡��CmdLen����λ�����͵������
* Output         : None
* Return         : None
*******************************************************************************/
void Read_All_Meter_Data(u8 *CmdBuf,u16 CmdLen)
{
    u8 i=0,Count=0,MeterNum=0;

    u8 CheckBuf[256];
    u8 ExtOrInter;
    ExtOrInter=Check_Index_Equ(); // �����������У���Ƿ���ȷ
    if(ExtOrInter) //����EXT_Flash
    {
        SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,CheckBuf,256);  //��ȡflash
        SPI_Flash_Read(EXT_METER_NUM_ADDR,&MeterNum,1);  //��ȡ�������
        for(i=0; i<256; i++)
        {
            if(Count<MeterNum)
            {
                if(CheckBuf[i]!=0xff)
                {
                    Composite_Read_All_Meter_Frame_Format(CmdBuf,i,ExtOrInter);//��϶�ȡ���б������֡��ʽ
                    Count++;
                    if(Count>=MeterNum)
                        break;

                    delay_ms(1000);
                }
            }
            else  if(Count>=MeterNum)
            {
                Err_Ack(CmdBuf,NotFindMeter);
                break;
            }
        }
    }

    else
    {
        ReadFlash_Byte(METER_CHECK_PAGEADDR,CheckBuf,256);
        ReadFlash_Byte(METER_NUM_ADDR,&MeterNum,1);
        for(i=0; i<255; i++)
        {
            if(Count<MeterNum)
            {
                if(CheckBuf[i]!=0xff)
                {
                    Composite_Frame_Format(CmdBuf,i,ExtOrInter);
                    Count++;
                    if(Count>=MeterNum)
                        break;

                    delay_ms(1000);
                }
            }
            else if(Count>=MeterNum)
            {
                Err_Ack(CmdBuf,NotFindMeter);
                break;
            }
        }
        Copy_Inter_To_EXT(); //���ڲ�flash�е�������Ϣcopy�� �ⲿflash
    }

}


/*******************************************************************************
* Function Name  : Save_Meter_Data
* Description    : �����������ϴ�������
* Input          : DataBuf������ϴ����ݣ�DataLen�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
u8 Save_Meter_Data(u8 *DataBuf,u16 DataLen)
{
    u8 i=0,j=0,MeterCheck;
    u8 IndexBuf[256],MeterID[8];
    u8 Stat1,Stat2;
    MeterCheck=CRC8(&DataBuf[2],7);
    if(Check_Index_Equ()==1)   //�ⲿflash������ȷ�����ⲿflash����
    {
        SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,IndexBuf,256);
        for(i=0; i<255; i++)
        {
            if(MeterCheck==IndexBuf[i])
            {
                SPI_Flash_Read(EXT_METER_NUM_PAGEADDR+i*8,MeterID,8);
                for(j=0; j<8; j++)
                {
                    if(MeterID[j]==DataBuf[j+1])break;
                }
                if (j==8)
                {
                    Stat1=SPI_Flash_Write(EXT_METER_DATA_PAGEADDR+((i/2)*4096)+((i%2)*PAGESIZE/2),&DataBuf[15],DataLen-16);
                    Stat2=WriteFlash_Page(METER_DATA_PAGEADDR-((i/2)*2048)+((i%2)*PAGESIZE/2),0,&DataBuf[15],DataLen-16);
                    if((Stat1==1)&&(Stat2==1))
                        return (1);
                    else
                        return (0);
                }
            }
        }
        return (0);//�����ڴ˱��Ƿ�Ҫ��ӣ�
    }
    else if (Check_Index_Equ()==0) //���ڲ�flash����
    {
        ReadFlash_Byte(METER_CHECK_PAGEADDR,IndexBuf,256);
        for(i=0; i<255; i++)
        {
            if(MeterCheck==IndexBuf[i])
            {
                ReadFlash_Byte(METER_NUM_PAGEADDR+i*8,MeterID,8);
                for(j=0; j<8; j++)
                {
                    if(MeterID[j]==DataBuf[j+1])break;
                }
                if (j==8)
                {
                    Stat1=Copy_Inter_To_EXT();
                    Stat2=WriteFlash_Page(METER_DATA_PAGEADDR-((i/2)*2048)+((i%2)*PAGESIZE/2),0,&DataBuf[15],DataLen-16);
                    if((Stat1==1)&&(Stat2==1))
                        return (1);
                    else
                        return (0);
                }
            }
        }
        return (0);//�޴˱�ߣ��Ƿ�Ҫ��ӣ�
    }
    return 0;
}
/*******************************************************************************
* Function Name  : Change_One_Meter
* Description    : ����һ̨������
* Input          : Buf��������һ̨�����ߡ�֡��Len��ָ���
* Output         : None
* Return         : None
*******************************************************************************/
u8 Change_One_Meter(u8 *Buf,u16 Len)
{
    u8 	ExtOrInter=0,i=0,j=0,Count=0;
    u8  MeterNum=0;
    u8  MeterCheck[256];
    u8  OrgMeterCheck;
    u8  NewMeterCheck;
    u8  MeterAddr[8];
    u8  IndexCheck;
    u8  buf[8];
    ExtOrInter=Check_Index_Equ();
    OrgMeterCheck=CRC8(&Buf[12],7);
    NewMeterCheck=CRC8(&Buf[21],7);
    if(ExtOrInter)	//Extflash
    {
        SPI_Flash_Read(EXT_METER_NUM_ADDR,&MeterNum,1);
        SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,MeterCheck,256);
        for(i=0; i<255; i++) //�ж��Ƿ��Ѿ�����Ҫ�����ı��
        {
            if(Count<MeterNum)
            {
                if(MeterCheck[i]!=0xff)
                {
                    if(MeterCheck[i]==NewMeterCheck)
                    {
                        SPI_Flash_Read(EXT_METER_NUM_PAGEADDR+i*8,MeterAddr,8);
                        if(MeterAddr[0]==Buf[11])//���������ͬ
                        {
                            for(j=1; j<8; j++)
                            {
                                if(MeterAddr[j]!=Buf[j+20])break;
                            }
                            if(j==8)
                            {
                                Err_Ack(Buf,DoubleMeter); //����Ѿ�����
                                return 0;
                            }
                        }
                    }
                    Count++;
                }
            }
            else break;
        }
        Count=0;
        for(i=0; i<256; i++)
        {
            if(Count<MeterNum)
            {
                if(MeterCheck[i]!=0xff)
                {
                    if(MeterCheck[i]==OrgMeterCheck)
                    {
                        SPI_Flash_Read(EXT_METER_NUM_PAGEADDR+i*8,MeterAddr,8);
                        for(j=0; j<8; j++)
                        {
                            if(MeterAddr[j]!=Buf[j+11])break;
                        }
                        if(j==8)
                        {
                            for(j=0; j<255; j++)
                            {
                                MeterCheck[j]=0xff;
                            }
                            if(i%2)
                            {
                                ReadFlash_Byte(METER_CHECK_PAGEADDR+i-1,buf,2);
                                buf[1]=NewMeterCheck;
                                WriteFlash_Page(METER_CHECK_PAGEADDR,i-1,buf,2);
                            }
                            else
                            {
                                ReadFlash_Byte(METER_CHECK_PAGEADDR+i,buf,2);
                                buf[0]=NewMeterCheck;
                                WriteFlash_Page(METER_CHECK_PAGEADDR,i,buf,2);
                            }
                            buf[0]=Buf[11];//�������
                            for(j=0; j<7; j++)
                            {
                                buf[j+1]=Buf[j+21];
                            }
                            SPI_Flash_Write(EXT_METER_CHECK_PAGEADDR+i,&NewMeterCheck,1);//д��������
                            WriteFlash_Page(METER_NUM_PAGEADDR,i*8,buf,8); //д�������ͼ����
                            SPI_Flash_Write(EXT_METER_NUM_PAGEADDR+i*8,buf,8); //д�������ͼ����
                            SPI_Flash_Write(EXT_METER_DATA_PAGEADDR+((i/2)*4096)+((i%2)*PAGESIZE/2),MeterCheck,256);
                            WriteFlash_Page(METER_DATA_PAGEADDR-((i/2)*2048)+((i%2)*PAGESIZE/2),0,MeterCheck,256);
                            SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,MeterCheck,256);
                            IndexCheck=CRC8(MeterCheck,255);
                            if(IndexCheck==0xff)
                            {
                                IndexCheck++;
                            }
                            ReadFlash_Byte(METER_CHECK_PAGEADDR+254,MeterCheck,2 );
                            MeterCheck[1]=IndexCheck;
                            WriteFlash_Page(METER_CHECK_PAGEADDR,254,MeterCheck,2);//�ڲ�flashд����������У��
                            SPI_Flash_Write(EXT_METER_CHECK_PAGEADDR+255,&IndexCheck,1);//д���ⲿflash������У��
                            Right_Ack_Change_MeterAddr(Buf,Len);
                            break;
                        }
                    }
                    Count++;
                }
            }
            else if(Count==MeterNum)	//δ�ҵ�Ҫ���ĵı��
            {
                Err_Ack(Buf,NotFindMeter);
                return 0;
            }
            else break;
        }
    }
    else //InterFlash
    {
        ReadFlash_Byte(METER_NUM_ADDR,&MeterNum,1);
        ReadFlash_Byte(METER_CHECK_PAGEADDR,MeterCheck,256);
        for(i=0; i<255; i++) //�ж��Ƿ��Ѿ�����Ҫ�����ı��
        {
            if(Count<MeterNum)
            {
                if(MeterCheck[i]!=0xff)
                {
                    if(MeterCheck[i]==NewMeterCheck)
                    {
                        ReadFlash_Byte(METER_NUM_PAGEADDR+i*8,MeterAddr,8);
                        if(MeterAddr[0]==Buf[11])//���������ͬ
                        {
                            for(j=1; j<8; j++)
                            {
                                if(MeterAddr[j]!=Buf[j+20])break;
                            }
                            if(j==8)
                            {
                                //Copy_Inter_To_EXT();
                                Err_Ack(Buf,DoubleMeter); //����Ѿ�����
                                return 0;
                            }
                        }
                    }
                    Count++;
                }
            }
            else break;
        }
        Count=0;
        for(i=0; i<256; i++)
        {
            if(Count<MeterNum)
            {
                if(MeterCheck[i]!=0xff)
                {
                    if(MeterCheck[i]==OrgMeterCheck)
                    {
                        ReadFlash_Byte(METER_NUM_PAGEADDR+i*8,MeterAddr,8);
                        for(j=0; j<8; j++)
                        {
                            if(MeterAddr[j]!=Buf[j+11])break;
                        }
                        if(j==8)
                        {
                            for(j=0; j<255; j++)
                            {
                                MeterCheck[j]=0xff;
                            }
                            if(i%2)
                            {
                                ReadFlash_Byte(METER_CHECK_PAGEADDR+i-1,buf,2);
                                buf[1]=NewMeterCheck;
                                WriteFlash_Page(METER_CHECK_PAGEADDR,i-1,buf,2);
                            }
                            else
                            {
                                ReadFlash_Byte(METER_CHECK_PAGEADDR+i,buf,2);
                                buf[0]=NewMeterCheck;
                                WriteFlash_Page(METER_CHECK_PAGEADDR,i,buf,2);
                            }
                            buf[0]=Buf[11];//�������
                            for(j=0; j<7; j++)
                            {
                                buf[j+1]=Buf[j+21];
                            }
                            WriteFlash_Page(METER_NUM_PAGEADDR,i*8,buf,8); //д�������ͼ����
                            SPI_Flash_Write(EXT_METER_DATA_PAGEADDR+((i/2)*4096)+((i%2)*PAGESIZE/2),MeterCheck,256);
                            WriteFlash_Page(METER_DATA_PAGEADDR-((i/2)*2048)+((i%2)*PAGESIZE/2),0,MeterCheck,256);
                            ReadFlash_Byte(METER_CHECK_PAGEADDR,MeterCheck,255);
                            IndexCheck=CRC8(MeterCheck,255);
                            if(IndexCheck==0xff)
                            {
                                IndexCheck++;
                            }
                            ReadFlash_Byte(METER_CHECK_PAGEADDR+254,MeterCheck,2 );
                            MeterCheck[1]=IndexCheck;
                            WriteFlash_Page(METER_CHECK_PAGEADDR,254,MeterCheck,2);//�ڲ�flashд����������У��
                            Right_Ack_Change_MeterAddr(Buf,Len);
                            break;
                        }
                    }
                    Count++;
                }
            }
            else if(Count==MeterNum)	//δ�ҵ�Ҫ���ĵı��
            {
                //Copy_Inter_To_EXT();
                Err_Ack(Buf,NotFindMeter);
                return 0;
            }
            else break;
        }
        Copy_CJQ_Parameter_From_Inter_To_EXT();
        Copy_MeterCheck_And_MeterCheckCheck_From_Inter_To_EXT();
        Copy_Meter_Num_From_Inter_To_EXT();
    }
    return 0;
}
/*******************************************************************************
* Function Name  : Delete_One_Meter
* Description    : ɾ��һ̨�����ߣ���ţ�������������data��
* Input          : Buf����ɾ��һ̨�����ߡ�֡��Len��ָ���
* Output         : None
* Return         : None
*******************************************************************************/
u8  Delete_One_Meter(u8 *Buf,u16 Len)
{
    extern u8  G_UART_Num;
    u8 i,j,Count=0,m=0;
    u8 Check_Space;
    u8 uMeterCheck,IndexCheck;
    u8 MeterNum=0;
    u8 MeterAddr[8];
    u8 MeterCheck[256];
    uMeterCheck=CRC8(&Buf[12],7);
    if(Check_Index_Equ())
    {
        SPI_Flash_Read(EXT_METER_NUM_ADDR,&MeterNum,1);
        SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,MeterCheck,256);
        for(i=0; i<256; i++)
        {
            if(Count<MeterNum)
            {
                if(MeterCheck[i]!=0xff)
                {
                    if(uMeterCheck==MeterCheck[i])
                    {
                        SPI_Flash_Read(EXT_METER_NUM_PAGEADDR+i*8,MeterAddr,8);
                        for(j=0; j<8; j++)
                        {
                            if(MeterAddr[j]!=Buf[j+11])break;
                        }

                        if(j==8)
                        {
                            Check_Space=i;
                            for(j=0; j<255; j++)
                            {
                                MeterCheck[j]=0xff;
                            }

                            SPI_Flash_Write(EXT_METER_CHECK_PAGEADDR+i,MeterCheck,1);//д��flash
                            if(Check_Space%2==0)
                            {
                                ReadFlash_Byte(METER_CHECK_PAGEADDR+Check_Space,MeterCheck,2 );
                                MeterCheck[0]=0xff;
                                WriteFlash_Page(METER_CHECK_PAGEADDR,Check_Space,MeterCheck,2);//�ڲ�flashд����У����
                            }
                            else
                            {
                                ReadFlash_Byte(METER_CHECK_PAGEADDR+Check_Space-1,MeterCheck,2 );
                                MeterCheck[1]=0xff;
                                WriteFlash_Page(METER_CHECK_PAGEADDR,Check_Space-1,MeterCheck,2);//�ڲ�flashд����У����
                            }
                            SPI_Flash_Write(EXT_METER_NUM_PAGEADDR+i*8,MeterCheck,8);//д��flash
                            WriteFlash_Page(METER_NUM_PAGEADDR,i*8,MeterCheck,8);

                            SPI_Flash_Write(EXT_METER_DATA_PAGEADDR+((i/2)*4096)+((i%2)*PAGESIZE/2),MeterCheck,256);
                            WriteFlash_Page(METER_DATA_PAGEADDR-((i/2)*2048)+((i%2)*PAGESIZE/2),0,MeterCheck,256);

                            MeterNum--;
                            SPI_Flash_Write(EXT_METER_NUM_ADDR,&MeterNum,1);//д��������

                            ReadFlash_Byte(METER_NUM_ADDR-1,MeterCheck,2 );
                            MeterCheck[1]=MeterNum;
                            WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,16,MeterCheck,2);


                            SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,MeterCheck,256);
                            for(m=0; m<255; m++)
                            {
                                if((MeterCheck[m])!=0xff)
                                    break;
                            }
                            if(m==255)
                            {
                                IndexCheck=0xff;
                            }
                            else
                            {
                                IndexCheck=CRC8(MeterCheck,255);
                                if(IndexCheck==0xff)
                                    IndexCheck+=1;
                            }
                            SPI_Flash_Write(EXT_METER_CHECK_PAGEADDR+255,&IndexCheck,1);//�ⲿflashд����������У��
                            ReadFlash_Byte(METER_CHECK_PAGEADDR+254,MeterCheck,2 );
                            MeterCheck[1]=IndexCheck;
                            WriteFlash_Page(METER_CHECK_PAGEADDR,254,MeterCheck,2);//�ڲ�flashд����������У��
                            Right_Ack_Len0(Buf,Len);
                            break;
                        }
                    }
                    Count++;
                }
            }
            else if(Count==MeterNum)
            {
                Err_Ack(Buf,NotFindMeter);
                return 0;
            }
            else break;
        }
    }
    else
    {
        ReadFlash_Byte(METER_NUM_ADDR,&MeterNum,1);
        ReadFlash_Byte(METER_CHECK_PAGEADDR,MeterCheck,255);
        for(i=0; i<256; i++)
        {
            if(Count<MeterNum)
            {
                if(MeterCheck[i]!=0xff)
                {
                    if(uMeterCheck==MeterCheck[i])
                    {
                        ReadFlash_Byte(METER_NUM_PAGEADDR+i*8,MeterAddr,8);
                        for(j=0; j<8; j++)
                        {
                            if(MeterAddr[j]!=Buf[j+11])break;
                        }
                        if(j==8)
                        {
                            Check_Space=i;
                            for(j=0; j<255; j++)
                            {
                                MeterCheck[j]=0xff;
                            }
                            if(Check_Space%2==0)
                            {
                                ReadFlash_Byte(METER_CHECK_PAGEADDR+Check_Space,MeterCheck,2 );
                                MeterCheck[0]=0xff;
                                WriteFlash_Page(METER_CHECK_PAGEADDR,Check_Space,MeterCheck,2);//�ڲ�flashд��0xff
                            }
                            else
                            {
                                ReadFlash_Byte(METER_CHECK_PAGEADDR+Check_Space-1,MeterCheck,2 );
                                MeterCheck[1]=0xff;
                                WriteFlash_Page(METER_CHECK_PAGEADDR,Check_Space-1,MeterCheck,2);//�ڲ�flashд��0xff
                            }
                            WriteFlash_Page(METER_NUM_PAGEADDR,i*8,MeterCheck,8);

                            SPI_Flash_Write(EXT_METER_DATA_PAGEADDR+((i/2)*4096)+((i%2)*PAGESIZE/2),MeterCheck,256);
                            WriteFlash_Page(METER_DATA_PAGEADDR-((i/2)*2048)+((i%2)*PAGESIZE/2),0,MeterCheck,256);

                            MeterNum--;
                            ReadFlash_Byte(METER_NUM_ADDR-1,MeterCheck,2 );
                            MeterCheck[1]=MeterNum;
                            WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,16,MeterCheck,2);
                            ReadFlash_Byte(METER_CHECK_PAGEADDR,MeterCheck,256);
                            for(m=0; m<255; m++)
                            {
                                if((MeterCheck[m])!=0xff)
                                    break;
                            }
                            if(m==255)
                            {
                                IndexCheck=0xff;
                            }
                            else
                            {
                                IndexCheck=CRC8(MeterCheck,255);
                                if(IndexCheck==0xff)
                                    IndexCheck+=1;
                            }
                            ReadFlash_Byte(METER_CHECK_PAGEADDR+254,MeterCheck,2 );
                            MeterCheck[1]=IndexCheck;
                            WriteFlash_Page(METER_CHECK_PAGEADDR,254,MeterCheck,2);//�ڲ�flashд����������У��
                            Right_Ack_Len0(Buf,Len);
                            break;
                        }
                    }
                    Count++;
                }

            }
            else if(Count==MeterNum)
            {
                Err_Ack(Buf,NotFindMeter);
                return 0;
            }
            else break;

        }
        Copy_CJQ_Parameter_From_Inter_To_EXT();
        Copy_MeterCheck_And_MeterCheckCheck_From_Inter_To_EXT();
        Copy_Meter_Num_From_Inter_To_EXT();
    }
    return 0;
}
/**********************************************************************************
* Function Name  : Delete_All_Meter
* Description    : ɾ��ȫ�������ߣ���ţ�������������data��
* Input          : Buf����ɾ��ȫ�������ߡ�֡��Len��ָ���
* Output         : None
* Return         : return 1�������ɹ���return 0������ʧ�ܣ��ɼ�����ʼ������дʧ��
***********************************************************************************/
u8 Delete_All_Meter(u8 *Buf,u16 Len)
{
    u8 CJQ_Addr[8],i=0;
    u8 State1,State2;
    u8 CJQ_Check=0;
    u8 buf[21];
    ReadFlash_Byte(CJQ_PARAMETER_PAGEADDR,CJQ_Addr,8);
    for (i=0; i<132; i++)
    {
        FlashErase_Page(i*2048+0x0803E000);
    }
//		 for (i=0;i<132;i++)
//			  {
//			  	ReadFlash_Byte(i*2048+0x0803E000,G_PageTmp,2048 );
//			  }
    SPI_Flash_Erase_Chip();    	  //��Ƭ�ⲿflash����
//		 for (i=0;i<132;i++)
//			  {
//			  	SPI_Flash_Read(i*4096,G_PageTmp,2048 );
//			  }
    for(i=0; i<16; i++)
    {
        buf[i]=0xAA;
    }
    buf[16]=0x01;
    buf[17]=0x00;
    buf[18]=0x00;
    buf[19]=0x00;
    buf[20]=0x30;
    CJQ_Check=WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,0,buf,21);
    if(CJQ_Check==0)//�ڲ�flashд����
    {
        Err_Ack(Buf,WriteInterFlashErr);
        return 0;
    }
    CJQ_Check=SPI_Flash_Write(EXT_CJQ_PARAMETER_PAGEADDR,buf,21);
    if(CJQ_Check==0)   //�ⲿflashд����
    {
        Err_Ack(Buf,WriteEXTFlashErr);
        return 0;
    }
    else 		 //����Ӧ��
    {
        State1=SPI_Flash_Write(EXT_CJQ_PARAMETER_PAGEADDR,CJQ_Addr,8);//д��flash
        State2=WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,0,CJQ_Addr,8);
        if((State1==1)&&(State2==1))
        {
            Right_Ack_Len0(Buf,Len);
            return 1;
        }
        else
            return 0;

    }
}
/**********************************************************************************
* Function Name  : Read_Meter_Installation_Information
* Description    : ����߰�װ��Ϣ���Ѱ�װ��ߣ�δ��װ��ߣ���λ����
* Input          : Buf��������߰�װ��Ϣ��֡��Len��ָ���
* Output         : None
* Return         : return 1�������ɹ���return 0������ʧ�ܣ�
***********************************************************************************/
u8 Read_Meter_Installation_Information(u8 *Buf,u16 Len)
{
    u8 MeterNum=0;
    u16 Installed=0,InstalledTemp=0;
    u16 NotInstalled=0,NotInstalledTemp=0;
    extern u8 G_UART_Num;

    if(Check_Index_Equ()) 	// �����������У���Ƿ���ȷ
    {
        SPI_Flash_Read(EXT_METER_NUM_ADDR,&MeterNum,1);   //��ȡflash
    }
    else
    {
        ReadFlash_Byte(METER_NUM_ADDR,&MeterNum,1 );
        Copy_CJQ_Parameter_From_Inter_To_EXT();
        Copy_MeterCheck_And_MeterCheckCheck_From_Inter_To_EXT();
    }
    Installed=HEX2BCD(MeterNum);
    InstalledTemp=Installed;
    NotInstalled=HEX2BCD(255-MeterNum);
    NotInstalledTemp=NotInstalled;
    Buf[1]=0x80;
    Buf[20]=0x06;
    Buf[21]=(u8)((InstalledTemp)&(0x0ff));
    Buf[22]=(u8)(Installed>>8);
    Buf[23]=(u8)((NotInstalledTemp)&(0x0ff));
    Buf[24]=(u8)(NotInstalled>>8);
    Buf[25]=0x00;
    Buf[26]=0x00;
    Buf[27]=CRC8(Buf,27);
    Buf[28]=0x16;
    Send_UART(G_UART_Num,Buf,29);
    return 1;

}
/**********************************************************************************
* Function Name  : Read_Electronic_Meter_Addr
* Description    : ���Ǳ��ţ����ӱ�� 7λ��ַ
* Input          : Buf�������Ǳ��š�֡��Len��ָ���
* Output         : None
* Return         : return 1�������ɹ���return 0������ʧ�ܣ�
***********************************************************************************/
u8 Read_Electronic_Meter_Addr(u8 *Buf,u16 Len)
{
    extern u8 G_UART_Num;
    u8 MeterNum,Count=0,i=0,j=0,Num=0;
    u8 MeterCheck[256];
    u8 MeterAddr[140];//ÿ�ζ���20����Address
    if(Check_Index_Equ())
    {
        SPI_Flash_Read(EXT_METER_NUM_ADDR,&MeterNum,1);
        SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,MeterCheck,256);
        if(MeterNum==0)
        {
            Right_Ack_Len0(Buf,Len);
            return 0;
        }
        for(i=0; i<256; i++)
        {
            if(Count<MeterNum)
            {
                if(MeterCheck[i]!=0xff)
                {
                    if(Num==20)
                    {
                        Buf[1]=0x80;
                        Buf[20]=7*Num;
                        for(j=0; j<140; j++)
                        {
                            Buf[21+j]=MeterAddr[j];
                        }
                        Buf[21+j]=CRC8(Buf,21+j);
                        Buf[22+j]=0x16;
                        delay_ms(500);
                        Send_UART(G_UART_Num,Buf,j+23);
                        Num=0;
                    }
                    SPI_Flash_Read(EXT_METER_NUM_PAGEADDR+i*8+1,&MeterAddr[Num*7],7);
                    Num++;
                    Count++;
                }

            }
            else if((Count==MeterNum)&&(Num!=0))
            {
                Buf[1]=0x80;
                Buf[20]=7*Num;
                for(j=0; j<7*Num; j++)
                {
                    Buf[21+j]=MeterAddr[j];
                }
                Buf[21+j]=CRC8(Buf,21+j);
                Buf[22+j]=0x16;
                Send_UART(G_UART_Num,Buf,j+23);
                Count++;
                break;
            }
            else break;
        }
    }
    else
    {
        ReadFlash_Byte(METER_NUM_ADDR,&MeterNum,1 );
        ReadFlash_Byte(METER_CHECK_PAGEADDR,MeterCheck,256);
        if(MeterNum==0)
        {
            Right_Ack_Len0(Buf,Len);
            return 0;
        }
        for(i=0; i<256; i++)
        {
            if(Count<MeterNum)
            {
                if(MeterCheck[i]!=0xff)
                {
                    if(Num==20)
                    {
                        Buf[1]=0x80;
                        Buf[20]=7*Num;
                        for(j=0; j<140; j++)
                        {
                            Buf[21+j]=MeterAddr[j];
                        }
                        Buf[21+j]=CRC8(Buf,21+j);
                        Buf[22+j]=0x16;
       
                        delay_ms(500);
                        Send_UART(G_UART_Num,Buf,j+23);
                        Num=0;
                    }
                    ReadFlash_Byte(METER_NUM_PAGEADDR+i*8+1,&MeterAddr[Num*7],7);
                    Num++;
                    Count++;
                }

            }
            else if((Count==MeterNum)&&(Num!=0))
            {
                Buf[1]=0x80;
                Buf[20]=7*Num;
                for(j=0; j<7*Num; j++)
                {
                    Buf[21+j]=MeterAddr[j];
                }
                Buf[21+j]=CRC8(Buf,21+j);
                Buf[22+j]=0x16;
                Send_UART(G_UART_Num,Buf,j+23);
                break;
            }
            else break;
        }
        Copy_MeterCheck_And_MeterCheckCheck_From_Inter_To_EXT();
        Copy_Meter_Num_From_Inter_To_EXT();

    }
    return 0;
}
/**********************************************************************************
* Function Name  : Read_All_Installed_Meter_Number
* Description    : ���Ѱ�װ��ţ���� 7λ��ַ
* Input          : Buf�������Ѱ�װ��š�֡��Len��ָ���
* Output         : None
* Return         : return 1�������ɹ���return 0������ʧ�ܣ�
***********************************************************************************/
u8  Read_All_Installed_Meter_Number(u8 *Buf,u16 Len)
{
    extern u8 G_UART_Num;
    u8 MeterNum,Count=0,i=0,j=0,Num=0;
    u8 MeterCheck[256];
    u8 MeterAddr[42];//ÿ�ζ���6����Address
    if(Check_Index_Equ())
    {
        SPI_Flash_Read(EXT_METER_NUM_ADDR,&MeterNum,1);
        SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,MeterCheck,256);
        if(MeterNum==0)
        {
            Right_Ack_Len0(Buf,Len);
            return 0;
        }
        for(i=0; i<256; i++)
        {
            if(Count<MeterNum)
            {
                if(MeterCheck[i]!=0xff)
                {
                    if(Num==6)
                    {
                        Buf[1]=0x80;
                        Buf[20]=7*Num;
                        for(j=0; j<7*Num; j++)
                        {
                            Buf[21+j]=MeterAddr[j];
                        }
                        Buf[21+j]=CRC8(Buf,21+j);
                        Buf[22+j]=0x16;
				        delay_ms(500);
                        Send_UART(G_UART_Num,Buf,j+23);
                        Num=0;
                    }
                    SPI_Flash_Read(EXT_METER_NUM_PAGEADDR+i*8+1,&MeterAddr[Num*7],7);
                    Num++;
                    Count++;
                }

            }
            else if((Count==MeterNum)&&(Num!=0))
            {
         
                delay_ms(4000);
                Buf[1]=0x80;
                Buf[20]=7*Num;
                for(j=0; j<7*Num; j++)
                {
                    Buf[21+j]=MeterAddr[j];
                }
                Buf[21+j]=CRC8(Buf,21+j);
                Buf[22+j]=0x16;
                Send_UART(G_UART_Num,Buf,j+23);
                break;
            }
            else break;
        }
    }
    else
    {
        ReadFlash_Byte(METER_NUM_ADDR,&MeterNum,1 );
        ReadFlash_Byte(METER_CHECK_PAGEADDR,MeterCheck,256);
        if(MeterNum==0)
        {
            Right_Ack_Len0(Buf,Len);
            return 0;
        }
        for(i=0; i<256; i++)
        {
            if(Count<MeterNum)
            {
                if(MeterCheck[i]!=0xff)
                {
                    if(Num==20)
                    {
                        Buf[1]=0x80;
                        Buf[20]=7*Num;
                        for(j=0; j<7*Num; j++)
                        {
                            Buf[21+j]=MeterAddr[j];
                        }
                        Buf[21+j]=CRC8(Buf,21+j);
                        Buf[22+j]=0x16;
						delay_ms(500);
                        Send_UART(G_UART_Num,Buf,j+23);
                        Num=0;
                    }
                    ReadFlash_Byte(METER_NUM_PAGEADDR+i*8+1,&MeterAddr[Num*7],7);
                    Num++;
                    Count++;
                }

            }
            else if((Count==MeterNum)&&(Num!=0))
            {
                Buf[1]=0x80;
                Buf[20]=7*Num;
                for(j=0; j<7*Num; j++)
                {
                    Buf[21+j]=MeterAddr[j];
                }
                Buf[21+j]=CRC8(Buf,21+j);
                Buf[22+j]=0x16;
                Send_UART(G_UART_Num,Buf,j+23);
                break;
            }
            else break;
        }
        Copy_Inter_To_EXT();
    }
    return 0;
}
/**********************************************************************************
* Function Name  : Read_All_unstalled_Meter_Number
* Description    : ��δ��װ��ţ���� 7λ��ַ
* Input          : Buf�������Ѱ�װ��š�֡��Len��ָ���
* Output         : None
* Return         : return 1�������ɹ���return 0������ʧ�ܣ�
***********************************************************************************/
u8  Read_All_unstalled_Meter_Number(u8 *Buf,u16 Len)//?????������ְ�װ��δ��װ���ձ�
{
    extern u8 G_UART_Num;
    u8 MeterNum,Count=0,i=0,j=0,Num=0;
    u8 MeterCheck[256];
    u8 MeterAddr[42];//ÿ�ζ���6����Address
    if(Check_Index_Equ())
    {
        SPI_Flash_Read(EXT_METER_NUM_ADDR,&MeterNum,1);
        SPI_Flash_Read(EXT_METER_CHECK_PAGEADDR,MeterCheck,256);
        if(MeterNum==0)
        {
            Right_Ack_Len0(Buf,Len);
            return 0;
        }
        for(i=0; i<256; i++)
        {
            if(Count<MeterNum)
            {
                if(MeterCheck[i]!=0xff)
                {
                    if(Num==6)
                    {
                        Buf[1]=0x80;
                        Buf[20]=7*Num;
                        for(j=0; j<7*Num; j++)
                        {
                            Buf[21+j]=MeterAddr[j];
                        }
                        Buf[21+j]=CRC8(Buf,21+j);
                        Buf[22+j]=0x16;
				        delay_ms(500);
                        Send_UART(G_UART_Num,Buf,j+23);
                        Num=0;
                    }
                    SPI_Flash_Read(EXT_METER_NUM_PAGEADDR+i*8+1,&MeterAddr[Num*7],7);
                    Num++;
                    Count++;
                }

            }
            else if((Count==MeterNum)&&(Num!=0))
            {
         
                delay_ms(4000);
                Buf[1]=0x80;
                Buf[20]=7*Num;
                for(j=0; j<7*Num; j++)
                {
                    Buf[21+j]=MeterAddr[j];
                }
                Buf[21+j]=CRC8(Buf,21+j);
                Buf[22+j]=0x16;
                Send_UART(G_UART_Num,Buf,j+23);
                break;
            }
            else break;
        }
    }
    else
    {
        ReadFlash_Byte(METER_NUM_ADDR,&MeterNum,1 );
        ReadFlash_Byte(METER_CHECK_PAGEADDR,MeterCheck,256);
        if(MeterNum==0)
        {
            Right_Ack_Len0(Buf,Len);
            return 0;
        }
        for(i=0; i<256; i++)
        {
            if(Count<MeterNum)
            {
                if(MeterCheck[i]!=0xff)
                {
                    if(Num==20)
                    {
                        Buf[1]=0x80;
                        Buf[20]=7*Num;
                        for(j=0; j<7*Num; j++)
                        {
                            Buf[21+j]=MeterAddr[j];
                        }
                        Buf[21+j]=CRC8(Buf,21+j);
                        Buf[22+j]=0x16;
						delay_ms(500);
                        Send_UART(G_UART_Num,Buf,j+23);
                        Num=0;
                    }
                    ReadFlash_Byte(METER_NUM_PAGEADDR+i*8+1,&MeterAddr[Num*7],7);
                    Num++;
                    Count++;
                }

            }
            else if((Count==MeterNum)&&(Num!=0))
            {
                Buf[1]=0x80;
                Buf[20]=7*Num;
                for(j=0; j<7*Num; j++)
                {
                    Buf[21+j]=MeterAddr[j];
                }
                Buf[21+j]=CRC8(Buf,21+j);
                Buf[22+j]=0x16;
                Send_UART(G_UART_Num,Buf,j+23);
                break;
            }
            else break;
        }
        Copy_Inter_To_EXT();
    }
    return 0;
}
/*******************************************************************************
* Function Name  : Read_CJQ_Work_Mode
* Description    : ��ȡ�ɼ�������ģʽ
* Input          : *Buf:��ȡ�ɼ�������ģʽ����֡��
				   Len��֡����
* Output         : None
* Return         : return1�����ⲿflash����
				   return 0�����ڲ�flash����
*******************************************************************************/
u8  Read_CJQ_Work_Mode(u8 *Buf,u16 Len)
{
    u8 EXT_CJQ_Work_Mode[1];
    u8 Inter_CJQ_Work_Mode[1];
    extern u8 G_UART_Num;
    ReadFlash_Byte(CJQ_WORK_MODE,Inter_CJQ_Work_Mode,1 );
    SPI_Flash_Read(EXT_CJQ_WORK_MODE,EXT_CJQ_Work_Mode,1);   //��ȡ�ⲿflash�ɼ�������ģʽ
    if (Check_Index_Equ()) //EXT_Flash
    {
        SPI_Flash_Read(EXT_CJQ_WORK_MODE,EXT_CJQ_Work_Mode,1);   //��ȡ�ⲿflash�ɼ�������ģʽ
        Buf[1]=0x80;
        Buf[20]=0x01;
        Buf[21]=EXT_CJQ_Work_Mode[0];
        Buf[22]=CRC8(Buf,22);
        Buf[23]=0x16;
        Send_UART(G_UART_Num,Buf,24);
        return 1;
    }
    else  //Inter_Flash
    {
        ReadFlash_Byte(CJQ_WORK_MODE,Inter_CJQ_Work_Mode,1 );
        Buf[1]=0x80;
        Buf[20]=0x01;
        Buf[21]=Inter_CJQ_Work_Mode[0];
        Buf[22]=CRC8(Buf,22);
        Buf[23]=0x16;
        SPI_Flash_Write(EXT_CJQ_WORK_MODE,Inter_CJQ_Work_Mode,1);//д��flash
        Send_UART(G_UART_Num,Buf,24);
        Copy_CJQ_Parameter_From_Inter_To_EXT();
        Copy_MeterCheck_And_MeterCheckCheck_From_Inter_To_EXT();
        Copy_Meter_Num_From_Inter_To_EXT();
        return 0;
    }
}
/*******************************************************************************
* Function Name  : Read_CJQ_Time_Interval
* Description    : ��ȡ�ɼ���������ѯʱ����
* Input          : *Buf:��ȡ�ɼ���������ѯʱ��������֡��
				   Len��֡����
* Output         : None
* Return         : return1�����ⲿflash����
				   return 0�����ڲ�flash����
*******************************************************************************/
u8  Read_CJQ_Time_Interval(u8 *Buf,u16 Len)
{
    u8 EXT_CJQ_Time_Interval[3];
    u8 Inter_CJQ_Time_Interval[3];
    u8 i=0;
    extern u8 G_UART_Num;
    ReadFlash_Byte(TIME_INTERVAL,Inter_CJQ_Time_Interval,3 );
    SPI_Flash_Read(EXT_TIME_INTERVAL,EXT_CJQ_Time_Interval,3);   //��ȡ�ⲿflash��ѯʱ����
    if (Check_Index_Equ()) //EXT_Flash
    {
        SPI_Flash_Read(EXT_TIME_INTERVAL,EXT_CJQ_Time_Interval,3);   //��ȡ�ⲿflash��ѯʱ����
        Buf[1]=0x80;
        Buf[20]=0x03;
        for(i=0; i<3; i++)
        {
            Buf[i+21]=EXT_CJQ_Time_Interval[i];
        }
        Buf[24]=CRC8(Buf,24);
        Buf[25]=0x16;
        Send_UART(G_UART_Num,Buf,26);
        return 1;
    }
    else  //Inter_Flash
    {
        ReadFlash_Byte(TIME_INTERVAL,Inter_CJQ_Time_Interval,3 );
        Buf[1]=0x80;
        Buf[20]=0x03;
        for(i=0; i<3; i++)
        {
            Buf[i+21]=Inter_CJQ_Time_Interval[i];
        }
        Buf[24]=CRC8(Buf,24);
        Buf[25]=0x16;
        SPI_Flash_Write(EXT_TIME_INTERVAL,Inter_CJQ_Time_Interval,3);//д��flash
        Send_UART(G_UART_Num,Buf,26);
        Copy_CJQ_Parameter_From_Inter_To_EXT();
        Copy_MeterCheck_And_MeterCheckCheck_From_Inter_To_EXT();
        Copy_Meter_Num_From_Inter_To_EXT();
        return 0;
    }
}
/*******************************************************************************
* Function Name  : Change_CJQ_Work_Mode
* Description    : ���Ĳɼ�������ģʽ
* Input          : *Buf:���Ĳɼ�������ģʽ����֡��
			   Len��֡����
* Output         : None
* Return         : return1�����ⲿflash����
			   return 0�����ڲ�flash����
*******************************************************************************/
u8  Change_CJQ_Work_Mode(u8 *Buf,u16 Len)
{
    u8 Inter_CJQ_Work_Mode[2];
    extern u8 G_UART_Num;

    if (Check_Index_Equ()) //EXT_Flash
    {
        ReadFlash_Byte(CJQ_WORK_MODE,Inter_CJQ_Work_Mode,2 );
        Inter_CJQ_Work_Mode[0]=Buf[21];
        WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,0x10,Inter_CJQ_Work_Mode,2);
        ReadFlash_Byte(CJQ_WORK_MODE,Inter_CJQ_Work_Mode,2 );
        SPI_Flash_Write(EXT_CJQ_WORK_MODE,&Buf[21],1);
        SPI_Flash_Read(EXT_CJQ_WORK_MODE,Inter_CJQ_Work_Mode,1);
        Buf[1]=0x80;
        Buf[22]=CRC8(Buf,22);
        Buf[23]=0x16;
        Send_UART(G_UART_Num,Buf,24);
        return 1;
    }
    else  //Inter_Flash
    {
        ReadFlash_Byte(CJQ_WORK_MODE,Inter_CJQ_Work_Mode,2 );
        Inter_CJQ_Work_Mode[0]=Buf[21];
        WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,0x10,Inter_CJQ_Work_Mode,2);
        ReadFlash_Byte(CJQ_WORK_MODE,Inter_CJQ_Work_Mode,2 );
        Buf[1]=0x80;
        Buf[22]=CRC8(Buf,22);
        Buf[23]=0x16;
        Send_UART(G_UART_Num,Buf,24);
        Copy_CJQ_Parameter_From_Inter_To_EXT();
        Copy_MeterCheck_And_MeterCheckCheck_From_Inter_To_EXT();
        Copy_Meter_Num_From_Inter_To_EXT();
        return 0;
    }
}
/*******************************************************************************
* Function Name  : Change_CJQ_Time_Interval
* Description    : ���Ĳɼ���������ѯʱ����
* Input          : *Buf:���Ĳɼ���������ѯʱ��������֡��
				   Len��֡����
* Output         : None
* Return         : return1�����ⲿflash����
				   return 0�����ڲ�flash����
*******************************************************************************/
u8  Change_CJQ_Time_Interval(u8 *Buf,u16 Len)
{
    u8 Inter_CJQ_Time_Interval[4];
    u8 i;
    extern u8 G_UART_Num;

    if (Check_Index_Equ()) //EXT_Flash
    {
        ReadFlash_Byte(TIME_INTERVAL,Inter_CJQ_Time_Interval,4);
        for(i=0; i<3; i++)
        {
            Inter_CJQ_Time_Interval[i]=Buf[21+i];
        }

        WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,0x12,Inter_CJQ_Time_Interval,4);

        SPI_Flash_Write(EXT_TIME_INTERVAL,&Buf[21],3);
        Buf[1]=0x80;
        Buf[24]=CRC8(Buf,24);
        Buf[25]=0x16;
        Send_UART(G_UART_Num,Buf,26);
        return 1;
    }
    else  //Inter_Flash
    {
        ReadFlash_Byte(TIME_INTERVAL,Inter_CJQ_Time_Interval,4);
        for(i=0; i<3; i++)
        {
            Inter_CJQ_Time_Interval[i]=Buf[21+i];
        }
        WriteFlash_Page(CJQ_PARAMETER_PAGEADDR,0x12,Inter_CJQ_Time_Interval,4);
        Buf[1]=0x80;
        Buf[24]=CRC8(Buf,24);
        Buf[25]=0x16;
        ReadFlash_Byte(TIME_INTERVAL,Inter_CJQ_Time_Interval,4 );
        Send_UART(G_UART_Num,Buf,26);
        Copy_CJQ_Parameter_From_Inter_To_EXT();
        Copy_MeterCheck_And_MeterCheckCheck_From_Inter_To_EXT();
        Copy_Meter_Num_From_Inter_To_EXT();
        return 0;
    }
}




