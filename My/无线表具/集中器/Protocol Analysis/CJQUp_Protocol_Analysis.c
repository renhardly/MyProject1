#include "platform_config.h"
//#include "I2C_Driver.h"
//#include"FILE_SYS.H"
#include <EXT_Flash.h>
#include "spi.h"
#include "ErrCode.h"
extern u8 G_UART_Num;
void Compose_Up_Frame_Format(u8 *pMeterDataRecv,u16 G_RecvDataLen) //���
{

    u16 i;
    u8 JZQAddr[8];
    u8 SendUpFrameFormat[300];	//256+29+2+x  Ԥ��13���ֽڿռ�
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
//		case 0x06: ���ϴ�
		case 0x07:
		case 0x0A:
		case 0x33:
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }
		case 0x21:	   //���ط�
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
        case 0x22:	   //��ӱ��
		case 0x23:
		case 0x2A:
		case 0x2B:
		case 0x2c:
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }

        case 0x61:	   //��ѯ�ɼ���״̬
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;

        }
        case 0x74:	   //�ϴ��ӽڵ����
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;

        }
        case 0x80:	   //�����ӽڵ����
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;

        }
        case 0x81:	   //�����ӽڵ��ַ
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;

        }
        case 0x84:	 //Ӧ����Ϣ
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
		case 0x87://�Զ���Э��,�����ڲɼ����ͼ��������ͨѶ
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
        case 0x85:	 //��������ͬ����Уʱ��
        {
            u16 i;
            u8 cjq_check[20];
            u8 cjq_add[8];
            memcpy(cjq_add,pMeterDataRecv+2,8);
            SPI_Flash_Read(CJQ_INDEX,cjq_check,20);	//��û�п��ǵ��㲥�ظ�����
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
        case 0x49:	 //�����������
        {
            pMeterDataRecv[19]=0x59;
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }
        case 0xB5:	   //�����ϴ�
        {
            Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
            break;
        }
        }
        break;
    }
    case 0xC0: //�����
    {
        Compose_Up_Frame_Format(pMeterDataRecv,G_RecvDataLen);
        break;
    }
    default:
        break;
    }
}
