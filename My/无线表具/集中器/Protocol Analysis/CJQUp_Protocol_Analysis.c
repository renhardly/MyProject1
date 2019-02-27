#include "platform_config.h"
//#include "I2C_Driver.h"
//#include"FILE_SYS.H"
#include <EXT_Flash.h>
#include "spi.h"
#include "ErrCode.h"
extern u8 G_UART_Num;
void Compose_Up_Frame_Format(u8 *pMeterDataRecv,u16 G_RecvDataLen) //组包
{

    u16 i;
    u8 JZQAddr[8];
    u8 SendUpFrameFormat[300];	//256+29+2+x  预留13个字节空间
    SPI_Flash_Read(JZQ_ADDR,JZQAddr,8);
    SendUpFrameFormat[0]=0x68;
    SendUpFrameFormat[1]=pMeterDataRecv[1];
    for(i=0; i<8; i++)
    {
        SendUpFrameFormat[i+2]=JZQAddr[i];
    }
    SendUpFrameFormat[i+2]=0x68;
    for(i=0; i<G_RecvDataLen-4; i++)
    {
        SendUpFrameFormat[i+11]=pMeterDataRecv[i+2];
    }
    SendUpFrameFormat[i+11]=CRC8(SendUpFrameFormat,i+11);
    SendUpFrameFormat[i+12]=0x16;
    Send_UART(G_UART_Num,SendUpFrameFormat,i+13);

}
void CJQUp_Protocol_Analysis(u8*pMeterDataRecv,u16 G_RecvDataLen)
{
   	if(memcmp(G_JZQ.ADDR,pMeterDataRecv+6,2)!=0)
		G_status.ST25=1;
    switch (pMeterDataRecv[1])
    {
    case 0x80:
    {
        switch(pMeterDataRecv[19])
        {
        case 0x01:
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
//		case 0x06: 不上传
		case 0x07:
		case 0x0A:
		case 0x33:
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }
		case 0x21:	   //开关阀
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }
		case 0x0b:	   
        {
            pMeterDataRecv[19]=0x0f;
			Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }
        case 0x22:	   //添加表具
		case 0x23:
		case 0x2A:
		case 0x2B:
		case 0x2c:
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }

        case 0x61:	   //查询采集器状态
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;

        }
        case 0x74:	   //上传子节点参数
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;

        }
        case 0x80:	   //更改子节点参数
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;

        }
        case 0x81:	   //更改子节点地址
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;

        }
        case 0x84:	 //应答信息
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }
		case 0x86:
        {
            pMeterDataRecv[19]=0x0a;
			Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }
		case 0x87://自定义协议,仅用于采集器和集中器间的通讯
        {
			JZQ_Flash_SD_Store(pMeterDataRecv,G_RecvDataLen);
            break;
        }
		 case 0x0E:	//hu 
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }

		 case 0x50:
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }      
	   case 0x51:
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }
		case 0x52:
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }
		 case 0x53 :
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }
		 case 0x54 :
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }
	    case 0x5A :
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }
        case 0x85:	 //设置网络同步（校时）
        {
            u16 i;
            u8 cjq_check[20];
            u8 cjq_add[8];
            memcpy(cjq_add,pMeterDataRecv+2,8);
            SPI_Flash_Read(CJQ_INDEX,cjq_check,20);	//还没有考虑到广播回复问题
            for(i=0; i<20; i++)
            {
                if(cjq_check[i]!=0xff)
                {
                    u8 tmp[8];
                    SPI_Flash_Read(CJQ_ADDR+i*8,tmp,8);
                    if(memcmp(cjq_add,tmp,8)==0)
                    {
                        G_CJQ_status[i].Counter=0;
                        return;
                    }

                }
            }
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }
        case 0x49:	 //机电参数设置
        {
            pMeterDataRecv[19]=0x59;
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }
        case 0xB5:	   //主动上传
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }
        }
        break;
    }
    case 0xC0: //错误号
    {
        Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
        break;
    }
    default:
        break;
    }
}
