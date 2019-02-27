/* 万康自组网无线模块的参数设置与查询函数 */
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
* Description    : 读取无线模块的参数（依次为地址4B、信道号1B、发射功率1B）
					返回的数据格式为：||----4字节地址----||--1字节信道号--||--1字节发射功率--||
* C              : 0x01
* Input          : 存放模块参数的数组指针，数组长度6字节.
* Output         : None
* Return         : 数据长度
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
    Send_UART(3,G_WireLess_Cmd,16)	;//串口3接采集器上行模块
#else
    Send_UART(2,G_WireLess_Cmd,16)	;//串口2接集中器下行模块
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
* Description    : 设置无线模块的参数（可以设置地址、信道号、发射功率）
* C              : 0x02
* Input
			type : 设置参数的类型，由头文件中的宏定义决定
			pAddr：待设置参数的指针
* Output         : None
* Return         : 1 设置成功，0设置失败
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
    Send_UART(3,G_WireLess_Cmd,16)	;//串口3接采集器上行模块
#else
    Send_UART(2,G_WireLess_Cmd,16)	;//串口2接集中器下行模块
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
    case TYPE_ADDR:	//设置模块地址
    {
        memcpy(G_WireLess_Cmd+7,pAddr,4);
        break;
    }
    case TYPE_CHANNEL_NUM:	//设置信道号
    {
        G_WireLess_Cmd[11] = *pAddr;
        break;
    }
    case TYPE_TRANSMIT_POWER: //设置发射功率
    {
        G_WireLess_Cmd[12] = *pAddr;
        break;
    }
    case TYPE_ALL_PARAMETER: //设置所有参数
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
    Send_UART(3,G_WireLess_Cmd,16)	;//串口3接采集器上行模块
#else
    Send_UART(2,G_WireLess_Cmd,16)	;//串口2接集中器下行模块
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
* Description    : 读取无线模块的路由信息
* C              : 0x03
* Input
			pAddr: 模块地址
		 pRouting：存放路由信息的数组指针，最长32+1字节（8跳）

* Output         : None
* Return         : 数据长度
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
    Send_UART(3,G_WireLess_Cmd,16)	;//串口3接采集器上行模块
#else
    Send_UART(2,G_WireLess_Cmd,16)	;//串口2接集中器下行模块
#endif

    _delay_ms_w(400);
    if(G_WireLess_Back==1)
    {
        nL = G_RecvData[6];//数据域长度
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
* Description    : 主模块遍历采集器地址发送数据以组网 
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





