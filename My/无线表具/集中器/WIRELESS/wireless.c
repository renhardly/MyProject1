/* ������������ģ��Ĳ����������ѯ���� */
#include "hal.h"
#include <EXT_Flash.h>
#include "platform_config.h"
extern u8 G_RecvData[80];
static void _delay_us_w(u16 nUs)
{
    u16 i,j;
    for (j=0; j<nUs; j++)
    {
        i = 8;

        while(i--);
    }
}
static void _delay_ms_w(u16 nMs)
{
    u16 i;
    for (i=0; i<nMs; i++)
    {
        _delay_us_w(1000);
    }
}

u8 G_WireLess_Back = 0;
static u8 G_WireLess_Cmd[16]=
{0XFD,0XFD,0XFD,0XFD,0X68,0X01,0X07,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X16};
#define  CJQ_OR_JZQ 	JZQ_UART
/*****************************************************************************
* Function Name  : Get_WireLess_Parameter
* Description    : ��ȡ����ģ��Ĳ���������Ϊ��ַ4B���ŵ���1B�����书��1B��
					���ص����ݸ�ʽΪ��||----4�ֽڵ�ַ----||--1�ֽ��ŵ���--||--1�ֽڷ��书��--||
* C              : 0x01
* Input          : ���ģ�����������ָ�룬���鳤��6�ֽ�.
* Output         : None
* Return         : ���ݳ���
******************************************************************************
*/
u8 Get_WireLess_Parameter(u8* pParameter)
{
    u8 i=0;
    u8 cs = 0;

    G_WireLess_Cmd[5] = 0x01;
    G_WireLess_Cmd[6] = 0x07;
    memset(G_WireLess_Cmd+7,0,7);
    for(i=0; i<10; i++)
    {
        cs += G_WireLess_Cmd[4+i];
    }
    G_WireLess_Cmd[14] = cs;
    cs = 0;

#if CJQ_OR_JZQ
    Send_UART(3,G_WireLess_Cmd,16)	;//����3�Ӳɼ�������ģ��
#else
    Send_UART(2,G_WireLess_Cmd,16)	;//����2�Ӽ���������ģ��
#endif


    _delay_ms_w(200);
    if(G_WireLess_Back==1)
    {
        if((G_RecvData[4]==0x68) && (G_RecvData[5]==0x81))
        {
            for(i=0; i<10; i++)
            {
                cs += G_RecvData[4+i];
            }
            if(cs==G_RecvData[14])
            {
                memcpy(pParameter,G_RecvData+7,6);
                G_WireLess_Back = 0;
                return 6;
            }
            else
            {
                G_WireLess_Back = 0;
                return 0;
            }
        }
    }

    return 0;
}

/*****************************************************************************
* Function Name  : Set_WireLess_Parameter
* Description    : ��������ģ��Ĳ������������õ�ַ���ŵ��š����书�ʣ�
* C              : 0x02
* Input
			type : ���ò��������ͣ���ͷ�ļ��еĺ궨�����
			pAddr�������ò�����ָ��
* Output         : None
* Return         : 1 ���óɹ���0����ʧ��
******************************************************************************
*/
u8 Set_WireLess_Parameter(u8 type,u8* pAddr)
{
    u8 i=0;
    u8 cs = 0;

    G_WireLess_Cmd[5] = 0x01;
    G_WireLess_Cmd[6] = 0x07;
    memset(G_WireLess_Cmd+7,0,7);
    for(i=0; i<10; i++)
    {
        cs += G_WireLess_Cmd[4+i];
    }
    G_WireLess_Cmd[14] = cs;
    cs = 0;

#if CJQ_OR_JZQ
    Send_UART(3,G_WireLess_Cmd,16)	;//����3�Ӳɼ�������ģ��
#else
    Send_UART(2,G_WireLess_Cmd,16)	;//����2�Ӽ���������ģ��
#endif

    _delay_ms_w(200);
    if(G_WireLess_Back==1)
    {
        if((G_RecvData[4]==0x68) && (G_RecvData[5]==0x81))
        {
            for(i=0; i<10; i++)
            {
                cs += G_RecvData[4+i];
            }
            if(cs==G_RecvData[14])
            {
                memcpy(G_WireLess_Cmd+7,G_RecvData+7,6);
                G_WireLess_Back = 0;
            }
            else
            {
                G_WireLess_Back = 0;
                return 0;
            }
        }
    }
    else return 0;

    G_WireLess_Cmd[5] = 0x02;
    G_WireLess_Cmd[6] = 0x07;

    switch(type)
    {
    case TYPE_ADDR:	//����ģ���ַ
    {
        memcpy(G_WireLess_Cmd+7,pAddr,4);
        break;
    }
    case TYPE_CHANNEL_NUM:	//�����ŵ���
    {
        G_WireLess_Cmd[11] = *pAddr;
        break;
    }
    case TYPE_TRANSMIT_POWER: //���÷��书��
    {
        G_WireLess_Cmd[12] = *pAddr;
        break;
    }
    case TYPE_ALL_PARAMETER: //�������в���
    {
        memcpy(G_WireLess_Cmd+7,pAddr,6);
        break;
    }
    default:
        return 0;
    }

    cs = 0;
    for(i=0; i<10; i++)
    {
        cs += G_WireLess_Cmd[4+i];
    }
    G_WireLess_Cmd[14] = cs;

#if CJQ_OR_JZQ
    Send_UART(3,G_WireLess_Cmd,16)	;//����3�Ӳɼ�������ģ��
#else
    Send_UART(2,G_WireLess_Cmd,16)	;//����2�Ӽ���������ģ��
#endif

    _delay_ms_w(200);
    if(G_WireLess_Back==1)
    {
        if((G_RecvData[4]==0x68) && (G_RecvData[5]==0x82))
        {
            cs = 0;
            for(i=0; i<10; i++)
            {
                cs += G_RecvData[4+i];
            }
            if(cs==G_RecvData[14])
            {
                G_WireLess_Back = 0;
                return 1;
            }
            else
            {
                G_WireLess_Back = 0;
                return 0;
            }
        }
    }
    return 0;
}

/*****************************************************************************
* Function Name  : Get_WireLess_Routing
* Description    : ��ȡ����ģ���·����Ϣ
* C              : 0x03
* Input
			pAddr: ģ���ַ
		 pRouting�����·����Ϣ������ָ�룬�32+1�ֽڣ�8����

* Output         : None
* Return         : ���ݳ���
******************************************************************************
*/
u8 Get_WireLess_Routing(u8* pAddr,u8* pRouting)
{
    u8 i = 0;
    u8 cs = 0;
    u8 nL = 0;

    G_WireLess_Cmd[5] = 0x03;
    G_WireLess_Cmd[6] = 0x07;
    for(i=0; i<4; i++)
    {
        G_WireLess_Cmd[7+i] =  pAddr[i];
    }
    for(i=0; i<10; i++)
    {
        cs += G_WireLess_Cmd[4+i];
    }
    G_WireLess_Cmd[14] = cs;


#if CJQ_OR_JZQ
    Send_UART(3,G_WireLess_Cmd,16)	;//����3�Ӳɼ�������ģ��
#else
    Send_UART(2,G_WireLess_Cmd,16)	;//����2�Ӽ���������ģ��
#endif

    _delay_ms_w(400);
    if(G_WireLess_Back==1)
    {
        nL = G_RecvData[6];//�����򳤶�
        if((G_RecvData[4]==0x68) && (G_RecvData[5]==0x83) && (nL!=0))
        {
            memcpy(pRouting,G_RecvData+7,nL);
            G_WireLess_Back = 0;
            return nL;
        }
        G_WireLess_Back=0;
    }
    return 0;
}
/*****************************************************************************
* Function Name  : WireLess_Networking
* Description    : ��ģ������ɼ�����ַ�������������� 
* Input          : None
* Output         : None
* Return         : None
******************************************************************************
*/
void WireLess_Networking(void)	  
{
	u8 i = 0,cjq_index[20],addr[10];

	addr[0] = 0x68;
	addr[1] = 0x10;
	SPI_Flash_Read(CJQ_INDEX,cjq_index,20);
	for(i=0;i<20;i++)
	{
		
		if(cjq_index[i]!=0xFF)
		{
			SPI_Flash_Read(CJQ_ADDR,&addr[2],8);
			addr[6] = 0x00;
			addr[7] = 0x00;
			Send_UART(2,addr,8);
			_delay_ms_w(50);				
		}	
	}
}





