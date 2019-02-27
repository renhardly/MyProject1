#include "platform_config.h"
unsigned char G_8563_SetBuf[7]; /*时间交换区,全局变量声明*/
unsigned char G_8563_ReadBuf[7]; /*时间交换区,全局变量声明*/
void SDA_OUT(unsigned char level);
void SCL_OUT(unsigned char level);
#include "ErrCode.h"
/************************************************
** 功    能: IC总线初始化.在main()函数中应首先 **
**           调用本函数,然后再调用其它函数.    **
************************************************/
static void DELAY_5us(void)
{
    unsigned int i, j;

    for(i=5; i>0; i--)
        for(j=7; j>0; j--);
}

void SDA_OUT(unsigned char level)
{
    if( level == 0)
        GPIO_ResetBits(GPIOB, GPIO_Pin_7);
    else
        GPIO_SetBits(GPIOB, GPIO_Pin_7);
}

void SCL_OUT(unsigned char level)
{
    if( level == 0)
        GPIO_ResetBits(GPIOB, GPIO_Pin_6);
    else
        GPIO_SetBits(GPIOB, GPIO_Pin_6);
}

void SDA_DIR(unsigned char level)
{
    if( level == 0)
        GPIOB->CRL &= ~(0x03 << 28);
    else
        GPIOB->CRL |= (0x03 << 28);
}

unsigned char SDA_IN( void )
{
    return( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) ? 1 : 0);
}

void IC_start(void)
{
    SDA_DIR(1);
    SCL_OUT(1);
    SDA_OUT(1);
    DELAY_5us();
    SCL_OUT(1);
    SDA_OUT(0);
    DELAY_5us();
    SCL_OUT(0);
}

/**********************************
** 功    能: IC总线停止位.       **
**********************************/
void IC_stop(void)
{
    SDA_DIR(1);
    SCL_OUT(0);
    SDA_OUT(0);
    DELAY_5us();
    SCL_OUT(1);
    SDA_OUT(1);
}

/***************************************************
** 功    能: 向IC总线发送8位数据,并请求一个应答信 **
**           号ACK.如果收到ACK应答则返回1(TRUE),  **
**           否则返回0(FALSE).                    **
***************************************************/
unsigned char ack = 0;
unsigned char IC_WriteByte(unsigned char wdata)
{

    unsigned char i;
    SDA_DIR(1);
    for(i=0; i<8; i++)
    {

        if(wdata&0x80) SDA_OUT(1);
        else SDA_OUT(0);
        wdata<<=1;
        SCL_OUT(1);
        DELAY_5us();
        SCL_OUT(0);
        DELAY_5us();
    }
    SDA_DIR(0);
    DELAY_5us();
    SCL_OUT(1);
    DELAY_5us();
    ack = SDA_IN();
    if(ack == 1)
    {
        return 0;
    }
    SCL_OUT(0);
    DELAY_5us();
    return 1;

}
/***********************************************
** 功    能: 从IC总线上接收8位数据,并将接受到 **
**           8位数据作为一个字节返回,不回送应 **
**           答信号ACK.                       **
***********************************************/
unsigned char IC_ReadByte(void)
{
    unsigned char i,rdata;

    rdata=0;


    SDA_DIR(0);
    for(i=0; i<8; i++)
    {

        SCL_OUT(0);
        DELAY_5us();

        rdata = rdata<<1;
        if(SDA_IN()==1)
            rdata = rdata+1;
        SCL_OUT(1);
        DELAY_5us();
    }

    return(rdata);
}
void WriteACK(u8 ack)
{
    SDA_OUT(ack);
    DELAY_5us();
    SCL_OUT(1);
    DELAY_5us();
    SCL_OUT(0);
}

/********************************************
输出数据->pcf8563
********************************************/
void writeData(u8 address,u8 mdata)
{
    IC_start();
    IC_WriteByte(0xA2);/*写命令*/
    IC_WriteByte(address);/*写命令*/
    IC_WriteByte(mdata); /*写数据*/
    IC_stop();;
}
/********************************************
输入数据<-pcf8563
********************************************/
u8 ReadData(u8 address) /*单字节*/
{
    u8 rdata;

    IC_start();
    IC_WriteByte(0xa2);/*写命令*/
    IC_WriteByte(address);/*写命令*/
    IC_start();
    IC_WriteByte(0xa3);/*写命令*/
    rdata = IC_ReadByte();
    WriteACK(1);
    IC_stop();
    return(rdata);
}
/********************************************
内部函数,读入时间到内部缓冲区
********************************************/
void P8563_Read(u8 *Buf)
{
    Buf[0]= ReadData(0x02)&0x7f;//秒
    Buf[1]= ReadData(0x03)&0x7f;//分
    Buf[2]= ReadData(0x04)&0x3f;//时
    Buf[3]= ReadData(0x05)&0x3f;//日
    //Buf[4]= ReadData(0x06)&0x07;//星期
    Buf[4]= ReadData(0x07)&0x9f;//月
    Buf[5]= ReadData(0x08)&0xff;//年
	
    srand((unsigned int)((Buf[5]+Buf[4]+Buf[3]+Buf[2]+Buf[1]+Buf[0])%10000));
}
/********************************************
读入时间到内部缓冲区----外部调用
********************************************/
void P8563_gettime(u8 *Buf)
{
    P8563_Read(Buf);
    if(Buf[0]==0)
        P8563_Read(Buf); /*如果为秒=0，为防止时间变化，再读一次*/
}
/********************************************
写时间修改值
********************************************/
void P8563_settime(u8 *buf)
{
    u8 i;
    u8 tmp[6];
	memset(&G_status,0,sizeof(G_status));
    P8563_gettime(tmp);
    if((abs(buf[0]-tmp[0])>5&&abs(buf[0]-tmp[0])<55))	//秒
        G_status.ST31=1;
    else if((abs(buf[1]-tmp[1])>1&&abs(buf[1]-tmp[1])<59))//分
        G_status.ST31=1;
    else if((abs(buf[2]-tmp[2])>1&&abs(buf[2]-tmp[2])<23))//时
        G_status.ST31=1;
    else//此处对年月日未做判断，后续待加；
        G_status.ST31=0;
    for(i=2; i<=8; i++)
    {
        writeData(i,buf[i-2]);
    }
    G_RTC_Check_Flag=0;
}
void JZQ_RTC_Start(void)
{
    writeData(0x0,0x00);
    writeData(0xd,0x83);
	writeData(0x0,0x00);
	writeData(0x9,0x0); /*分为00报警*/
	writeData(0x1,0x12); /*报警有效*/
}
void exchange(u8 *p,u8 len)//倒序函数
{    
	u8 i,temp;    
	for(i=0;i<(len/2);i++)
		{
		temp=p[i];
		p[i]=p[len-1-i];
		p[len-1-i]=temp;    
		}        
}

