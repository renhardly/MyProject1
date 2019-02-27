#include "platform_config.h"
#include "ErrCode.h"
/*******************************************************************************
* Function Name  : CRC8
* Description    : ������У��
* Input          : *ptr��У������ָ��,len�����ݳ���
* Output         : Null
* Return         : CRC��CRC8У����
*******************************************************************************/
u8 CRC8(u8 *ptr,u32 len)
{
    u8 crc;
    u8 i;
    crc = 0;
    while(len--)
    {
        crc ^= *ptr++;
        for(i = 0; i < 8; i++)
        {
            if(crc & 0x01)
            {
                crc = (crc >> 1) ^ 0x8C;
            }
            else crc >>= 1;
        }
    }
    return crc;
}
/*******************************************************************************
* Function Name  : RST
* Description    : ���ÿ��Ź���ι������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RstDog(void)
{
    u8 i=100;
    GPIO_ResetBits(GPIOE, GPIO_Pin_1);
    while(i--);
    GPIO_SetBits(GPIOE, GPIO_Pin_1);
}
/*******************************************************************************
* Function Name  : Add_Up
* Description    : ����У���
* Input          : AddUpBuf��ҪУ�������
                   AddUpLen��У������ĳ���
* Output         : None
* Return         : 8bit У���
*******************************************************************************/
u8 Add_Up (u8 *AddUpBuf,u16 AddUpLen)
{
    u8 Sum=0;
    u8 i;

    for(i = 0; i < AddUpLen; i++)
    {
        Sum+=AddUpBuf[i];
    }
    return Sum;
}

u8 GetWorkingMode(void)
{
    u8 WrkMode;
    SPI_Flash_Read(JZQ_WORKMODE,&WrkMode,1);//��ȡ�ⲿflash�й���ģʽλ
    return WrkMode;
}
u16 HEX2BCD(u8 Data)
{
    u16 DataTemp;
    u8  Units,Tens,Hundreds;
    Units=Data%10;
    Tens=(Data/10)%10;
    Hundreds=(Data/10)/10;
    DataTemp=Hundreds*0x100+Tens*0x10+Units;
    return DataTemp;
}
u8 BCD2HEX(u8 Data)
{
    u8 Units,Tens;
    u8 HexData;
    Units=Data%16;
    Tens=Data/16;
    HexData= Units+Tens*10;
    return HexData;
}
