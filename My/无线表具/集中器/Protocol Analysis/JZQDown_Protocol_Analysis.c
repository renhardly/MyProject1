
#include <stdio.h>
#include "platform_config.h"
#include <EXT_Flash.h>
#include "stm32f10x_flash.h"
#include "ErrCode.h"
#include "spi.h"

/*******************************************************************************
* Function Name  : JZQ_RTC_Set_1
* Description    : ���ü�����ʱ��
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void JZQ_RTC_Set_1(u8 *Buf,u16 Len)
{

    u8 buf[7]= {0};
    if(Len!=39)
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
    Buf[1]=0x80;
    Buf[Len-2]=CRC8(Buf,Len-2);
    Buf[Len-1]=0x16;
    Send_UART(G_UART_Num,Buf,Len);
}
/*******************************************************************************
* Function Name  : JZQ_RTC_Set_noret
* Description    : ��Уʱ���޷���
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void JZQ_RTC_Set_noret(u8 *Buf,u16 Len)
{

    u8 buf[7]= {0};
    u8 tmp[38];
    if(Len!=38)
    {
        return;
    }
    memcpy(tmp,Buf,Len);
    buf[0]=Buf[Len-3];//��
    buf[1]=Buf[Len-4];//��
    buf[2]=Buf[Len-5];//ʱ
    buf[3]=Buf[Len-6];//��
    buf[5]=Buf[Len-7];//��
    buf[6]=Buf[Len-8];//��
    P8563_settime(buf);
}
/*******************************************************************************
* Function Name  : JZQ_RTC_Set
* Description    : ��������ͬ����Уʱ��
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void JZQ_RTC_Set(u8 *Buf,u16 Len)
{

    u8 buf[7]= {0};
    u8 tmp[39];
    if(Len==39||Len==38)
    {
    }
    else
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    memcpy(tmp,Buf,Len);
    buf[0]=Buf[Len-3];//��
    buf[1]=Buf[Len-4];//��
    buf[2]=Buf[Len-5];//ʱ
    buf[3]=Buf[Len-6];//��
    buf[5]=Buf[Len-7];//��
    buf[6]=Buf[Len-8];//��
    P8563_settime(buf);
    tmp[1]=0x80;
    tmp[Len-2]=CRC8(tmp,Len-2);
    tmp[Len-1]=0x16;
    Send_UART(G_UART_Num,tmp,Len);
}
/*******************************************************************************
* Function Name  : JZQ_RTC_Read
* Description    : ��ȡ������ʵʱʱ��
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void JZQ_RTC_Read(u8 *Buf,u16 Len)
{

    u8 rtc_tmp[6];
    u8 buf[38];
    if(Len!=32)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    memcpy(buf,Buf,Len);
    P8563_gettime(rtc_tmp);
	exchange(rtc_tmp,6);
    buf[1]=0x80;
    buf[29]=0x06;
    buf[30]=rtc_tmp[0];
    buf[31]=rtc_tmp[1];
    buf[32]=rtc_tmp[2];
    buf[33]=rtc_tmp[3];
    buf[34]=rtc_tmp[4];
    buf[35]=rtc_tmp[5];
    buf[36]=CRC8(buf,36);
    buf[37]=0x16;
    Send_UART(G_UART_Num,buf,38);
}
/*******************************************************************************
* Function Name  : Set_Wireless_Parameter
* Description    : �������ڵ����
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void Set_Wireless_Parameter(u8* Buf,u16 Len)
{

    u8 pParameter[6];
    if(Len!=38)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    pParameter[5]=Buf[30];
    Set_WireLess_Parameter(TYPE_TRANSMIT_POWER,pParameter+5);
    pParameter[4]=Buf[33];
    Set_WireLess_Parameter(TYPE_CHANNEL_NUM,pParameter+4);
    Buf[1]=0x80;
    Buf[Len-2]=CRC8(Buf,Len-2);
    Buf[Len-1]=0x16;
    Send_UART(G_UART_Num,Buf,Len);
}
/*******************************************************************************
* Function Name  : JZQ_ADD_Subnot
* Description    : ����ӽڵ�
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void JZQ_ADD_Subnot(u8* Buf,u16 Len)
{

    u8 i,rfaddr[4];
    u8 Invalid_addr[4]= {0xff,0xff,0xff,0xff};
    if(Len!=36)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    memcpy(rfaddr,&Buf[30],4);
    for(i=0; i<20; i++)
    {
        u8 tmp[4];
        SPI_Flash_Read(CJQ_RFADDR+i*8,tmp,4);
        if(memcmp(tmp,Invalid_addr,4)==0)
            SPI_Flash_Write(CJQ_RFADDR+i*8,tmp,4);
    }
    if(i==20)
    {
        Err_Ack(Buf,StorageSpaceOverFlow);
        return;
    }
    Buf[1]=0x80;
    Buf[Len-2]=CRC8(Buf,Len-2);
    Buf[Len-1]=0x16;
    Send_UART(G_UART_Num,Buf,Len);
}
/*******************************************************************************
* Function Name  : JZQ_MainNotAddr_Write
* Description    : �������ڵ��ַ
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void JZQ_MainNotAddr_Write(u8* Buf,u16 Len)
{
    //����Ӳɼ������Ѿ���ɣ��˴����ظ�

    if(Len!=36)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    SPI_Flash_Write(JZQ_RFADDR,&Buf[30],4);
    if(Set_WireLess_Parameter(TYPE_ADDR,&Buf[30])!=1)
    {
        Err_Ack(Buf,Other);
        return;
    }
    Buf[1]=0x80;
    Buf[Len-2]=CRC8(Buf,Len-2);
    Buf[Len-1]=0x16;
    Send_UART(G_UART_Num,Buf,Len);
}
/*******************************************************************************
* Function Name  : JZQ_DEL_Subnot
* Description    : ɾ���ӽڵ�
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void JZQ_DEL_Subnot(u8* Buf,u16 Len)
{

    u8 i,rfaddr[4];
    u8 Invalid_addr[4]= {0xff,0xff,0xff,0xff};
    if(Len!=36)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    memcpy(rfaddr,&Buf[30],4);
    for(i=0; i<20; i++)
    {
        u8 tmp[4];
        SPI_Flash_Read(CJQ_RFADDR+i*8,tmp,4);
        if(memcmp(tmp,rfaddr,4)==0)
            SPI_Flash_Write(CJQ_RFADDR+i*8,Invalid_addr,4);
    }
    if(i==20)
    {
        Err_Ack(Buf,NotFindCJQ);
        return;
    }
    Buf[1]=0x80;
    Buf[Len-2]=CRC8(Buf,Len-2);
    Buf[Len-1]=0x16;
    Send_UART(G_UART_Num,Buf,Len);
}
/*******************************************************************************
* Function Name  : Back_SubNot_Status
* Description    : ���ӽڵ�״̬
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void Back_SubNot_Status(u8* Buf,u16 Len)
{

    u8 tmp[38],i,add[4],pRouting[35]= {0},cjq_num,cjq_check[20],check_num=0;
    if(Len!=36)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    SPI_Flash_Read(CJQ_NUM,&cjq_num,1);
    if(cjq_num==0)
        return;
    SPI_Flash_Read(CJQ_INDEX,cjq_check,20);
    memcpy(add,Buf+30,4);
    for(i=0; i<20; i++)
    {
        if(cjq_check[i]!=0xff)
        {
            u8 tmp_add[8];
            check_num++;
            if(check_num>cjq_num)
            {
                Err_Ack(Buf,CJQAddrErr);
                return;
            }
            SPI_Flash_Read(CJQ_ADDR+i*8,tmp_add,8);
            if(memcmp(tmp_add,add,4)==0)
            {
                if(Get_WireLess_Routing(add,pRouting)==0)
                {
                    Err_Ack(Buf,ReturnERR);
                    return;
                }
                memcpy(tmp,Buf,Len);
                tmp[1]=0x80;
                if(pRouting[0]==0)
                {
                    tmp[29]=0x01;
                    tmp[30]=0x00;
                }
                else
                {
                    tmp[29]=(pRouting[0]+1)*4+1;
                    tmp[30]=0x01;
                    memcpy(&tmp[31],&pRouting[1],tmp[29]-1);
                }
                tmp[30+tmp[29]]=CRC8(tmp,30+tmp[29]);
                tmp[31+tmp[29]]=0x16;
                Send_UART(G_UART_Num,tmp,32+tmp[29]);
            }
        }
    }
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

    u8 tmp[38],add[4],pParameter[6],pRouting[31];
    if(Len!=36)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    memcpy(tmp,Buf,Len);
    tmp[1]=0x80;
    memcpy(add,Buf+30,4);
    Get_WireLess_Parameter(pParameter);
    Get_WireLess_Routing(add,pRouting);

    tmp[29]=0x06;

    tmp[35]=0x07;//�򿵿��в�����20K��0x07��ʾ19200���Ʊ�ʾ
    tmp[34]=0x04;
    tmp[33]=pParameter[4];
    tmp[32]=0;
    tmp[31]=pRouting[7];
    tmp[30]=pParameter[5];

    tmp[36]=CRC8(tmp,36);
    tmp[37]=0x16;
    Send_UART(G_UART_Num,tmp,38);
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

    u8 tmp[36],pParameter[6];
    if(Len!=32)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    memcpy(tmp,Buf,Len);
    tmp[1]=0x80;
    if(Get_WireLess_Parameter(pParameter)!=6)
    {
        Err_Ack(Buf,Other);
        return;
    }
    tmp[29]=0x04;
    tmp[30]=pParameter[0];
    tmp[31]=pParameter[1];
    tmp[32]=pParameter[2];
    tmp[33]=pParameter[3];
    tmp[34]=CRC8(tmp,34);
    tmp[35]=0x16;
	_delay_ms(pParameter[0]); //ͨ���������ܳ�ͻ��lixiaowei
    Send_UART(G_UART_Num,tmp,36);
}
/*******************************************************************************
* Function Name  : Back_Wireless_Addr_ALL
* Description    : ��ȡ�����ӽڵ��ַ
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
void Back_Wireless_Addr_ALL(u8* Buf,u16 Len)
{

    u8 tt;
    u8 *tmp,cjq_num,i,cjq_check[20],check_num=0;
    if(Len!=32)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    SPI_Flash_Read(CJQ_NUM,&cjq_num,1);
    if(cjq_num==0)
    {
        Right_Ack_Len0(Buf,Len);
        return;
    }
    tmp=(u8 *)malloc((cjq_num*4+32));
    SPI_Flash_Read(CJQ_INDEX,cjq_check,20);
    for(i=0; i<20; i++)
    {
        if(cjq_check[i]!=0xff)
        {
            u8 tmp_add[8];
            check_num++;
            if(check_num>cjq_num)
            {
                free(tmp);
                Err_Ack(Buf,CJQAddrErr);
                return;
            }
            SPI_Flash_Read(CJQ_ADDR+i*8,tmp_add,8);
            memcpy(&tmp[(check_num-1)*4+30],tmp_add,4);

        }
    }
    memcpy(tmp,Buf,Len-2);
    tmp[1]=0x80;
    tmp[29]=4*cjq_num;
    tmp[30+4*cjq_num]=CRC8(tmp,30+4*cjq_num);
    tmp[30+4*cjq_num+1]=0x16;
    tt=30+(4*cjq_num)+2;
    Send_UART(G_UART_Num,tmp,tt);
    free(tmp);
}
/***********************************************************************************
* Function Name  : JZQ_NetAddr_Read
* Description    : ��ȡ�����������ַ
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None

************************************************************************************/
void JZQ_NetAddr_Read(u8* Buf,u16 Len)
{

    u8 buf[8],tmp[40],i;
    if(Len!=32)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    for(i=0; i<8; i++)
    {
        if(Buf[i+2]!=0xAA)break;
    }
    if(i!=8)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    SPI_Flash_Read(JZQ_ADDR,buf,8);
    memcpy(tmp,Buf,Len);
    tmp[1]=0x80;
    for(i=0; i<8; i++)
    {
        tmp[2+i]=buf[i];
    }
    tmp[29]=0x02;
    tmp[30]=buf[4];
    tmp[31]=buf[5];
    tmp[32]=CRC8(tmp,32);
    tmp[33]=0x16;
	_delay_ms(buf[0]); //ͨ���������ܳ�ͻ��lixiaowei
    Send_UART(G_UART_Num,tmp,34);
}

/***********************************************************************************
* Function Name  : JZQ_Wireless_NetType_Read
* Description    : ��ȡ����������ģ����������
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None

************************************************************************************/

void JZQ_Wireless_NetType_Read(u8* Buf,u16 Len)
{

    u8 tmp[33];
    if(Len!=32)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    SPI_Flash_Read(JZQ_WIRELESS_NetType,&tmp[30],1);
	memcpy(tmp,Buf,Len);
    tmp[1]=0x80;
    tmp[29]=0x01;
    tmp[31]=CRC8(tmp,31);
    tmp[32]=0x16;
    Send_UART(G_UART_Num,tmp,33);
}
/***********************************************************************************
* Function Name  : JZQ_Wireless_NetType_Write
* Description    : ���ļ���������ģ����������
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None

************************************************************************************/

void JZQ_Wireless_NetType_Write(u8* Buf,u16 Len)
{

    if(Len!=33)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    SPI_Flash_Write(JZQ_WIRELESS_NetType,&Buf[30],1);
    Buf[1]=0x80;
    Buf[Len-2]=CRC8(Buf,Len-2);
    Buf[Len-1]=0x16;
    Send_UART(G_UART_Num,Buf,Len);
}
/***********************************************************************************
* Function Name  : JZQ_Wireless_NetType_Set
* Description    : ���ļ���������ģ����������
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None

************************************************************************************/

void JZQ_Wireless_NetType_Set(u8* Buf,u16 Len)
{

    u8 tmp;
    if(Len!=33)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    tmp=Buf[Len-3];
    SPI_Flash_Write(JZQ_WIRELESS_NetType,&tmp,1);
    Buf[1]=0x80;
    Buf[Len-2]=CRC8(Buf,Len-2);
    Buf[Len-1]=0x16;
    Send_UART(G_UART_Num,Buf,Len);
}
/***********************************************************************************
* Function Name  : JZQ_Status_Read
* Description    : ���ļ�����״̬
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
************************************************************************************/


void JZQ_Status_Read(u8* Buf,u16 Len)
{


    u8 tmp[257],tmp1[257];
	u8 tmp_f[5];
    u8 param[6]; //����ģ����
    u8 i,j,cjq_check[20],tmp_add1[8],tmp_add2[8];  //���ɼ������ظ���
    if(Len!=32)
    {
        Err_Ack(Buf,CmdErr);
        return;
    }
    memcpy(tmp,Buf,Len);
    G_status.ST00=0;
    G_status.ST01=0;
    G_status.ST02=0;
    if(G_AD_Value[0]<(u16)(1354*0.8)) //��Դ��ѹ	?????��·û��
        G_status.ST03=1;
    else
        G_status.ST03=0;
    G_status.ST04=0;
    G_status.ST05=0;
    G_status.ST06=0;
    G_status.ST07=0;

    G_status.ST10=0;
    G_status.ST11=0;
    for(i=0; i<5; i++) //���flash�Ƿ���������
    {
        tmp_f[i]=rand()%256;
    }
    SPI_Flash_Write(EXT_FLASH_CHECK, tmp_f, 5);
    SPI_Flash_Read(EXT_FLASH_CHECK,tmp1,5);

    if(memcmp(tmp_f,tmp1,5)!=0)
        G_status.ST12=1;
    else
        G_status.ST12=0;
    G_status.ST13=0;
//	G_status.ST14=0;

    if(((1.42 - G_AD_Value[1]*3.3/4096)*1000/4.35 + 25)>65)//MCU�ڲ��¶�?????�¶�û�еõ�
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
    G_status.ST24=0;
    //G_status.ST25=0;
    G_status.ST26=0;
    G_status.ST27=0;

//	G_status.ST30=0;
//	G_status.ST31=0;
    G_status.ST32=0;
    G_status.ST33=0;
    G_status.ST34=0;

    G_status.ST35=0;	//�ɼ�����
    SPI_Flash_Read(CJQ_INDEX,cjq_check,20);
    for(i=0; i<20; i++)
    {
        for(j=i+1; j<20; j++)
        {
            if(cjq_check[i]==cjq_check[j])
            {
                if(cjq_check[i]==0xFF)	 break;

                SPI_Flash_Read(CJQ_ADDR+i*8,tmp_add1,8);
                SPI_Flash_Read(CJQ_ADDR+j*8,tmp_add2,8);
                if(memcmp(tmp_add1,tmp_add2,8)==0)
                {
                    G_status.ST35=1; //�ɼ������ظ�
                    break;
                }
            }
        }
        if(G_status.ST35==1) break;
    }


//	G_status.ST36=0;
    G_status.ST37=0;

    G_status.ST40=0;
    G_status.ST41=0;
    G_status.ST42=0;
    G_status.ST43=0;
    G_status.ST44=0;
    G_status.ST45=0;
    G_status.ST46=0;
    G_status.ST47=0;

    tmp[1]=0x80;
    tmp[29]=0x05;
    memcpy(tmp+30,&G_status,sizeof(G_status));
    tmp[35]=CRC8(tmp,35);
    tmp[36]=0x16;
    Send_UART(G_UART_Num,tmp,37);

}
/***********************************************************************************
* Function Name  : JZQ_NetAddr_Alter
* Description    : ���Ĳɼ��������ַ
* Input          : Buf���������飬Len�����ݳ���
* Output         : None
* Return         : None
************************************************************************************/
void JZQ_NetAddr_Alter(u8* Buf,u16 Len)
{
	u8 i,buf[8];
	Change_JZQAddr(Buf,Len);
	for(i=0; i<CJQ_NUM_SIZE; i++)
	{
	    if(G_PageTmp[i]!=0xff)
	    {
	       	u8 tmp[40];
			SPI_Flash_Read(CJQ_ADDR+i*8, buf,8);
			memcpy(tmp+11,buf,8);
			memcpy(buf+4,Buf+34,2);
			memcpy(tmp+30,buf,8);
			Deal_One_CJQ(tmp,40,ALT_ONE);
	    }
	}
}
/*******************************************************************************
* Function Name  : Err_Ack
* Description    : �쳣Ӧ��
* Input          : Buf������֡���飬ErrCode:�������
* Output         : None
* Return         : None
*******************************************************************************/
void Err_Ack(u8 *Buf,u8 ErrCode)
{

    Buf[1]=0xC0;
    Buf[29]=0x01;
    Buf[30]=ErrCode;//У�������
    Buf[31]=CRC8(Buf,31);
    Buf[32]=0x16;
    Send_UART(G_UART_Num,Buf,33);
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

	u8 u100[100];
	memcpy(u100,Buf,Len);
    Buf[1]=0x80;
    Buf[Len-2]=CRC8(Buf,Len-2);
    Send_UART(G_UART_Num,Buf,Len);
}
/***********************************************************************************
* Function Name  : JZQ_Addr_Check
* Description    : ��鼯������ַ�Ƿ���ȷ
* Input          : *Buf����λ�����͵�����֡
* Output         : None
* Return         : return 0���ɼ�����ַ����ȷ
				   return 1���ⲿflash�вɼ�����ַ��ȷ
				   return 2���ⲿflash�вɼ�����ַ����ȷ���ڲ�flash�вɼ�����ַ��ȷ
************************************************************************************/
s32 JZQ_Addr_Check(u8*Buf)
{
	if(memcmp(G_JZQ.ADDR,Buf+2,8)==0)
		return Success;
	return Failed;
}
void Compose_Down_Frame_Format(u8* Buf, u16 Len)//���
{
    u8 DownFrameFormat[100]; //21+2+ Ԥ��77���ֽ�data��
    u16 i=0;
    DownFrameFormat[0]=0x68;
    DownFrameFormat[1]=0x00;
    for(i=0; i<Len-13; i++)
    {
        DownFrameFormat[i+2]=Buf[i+11];
    }
    DownFrameFormat[i+2]=CRC8(DownFrameFormat,i+2);
    DownFrameFormat[i+3]=0x16;
    Auto_Sent(DownFrameFormat,i+4);
}
