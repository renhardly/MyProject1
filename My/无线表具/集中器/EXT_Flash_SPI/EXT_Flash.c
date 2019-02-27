//#include "stm32f10x_lib.h"
#include <EXT_Flash.h>
#include "spi.h"
#include "ErrCode.h"
/********************************************************************************
* 4KbytesΪһ��Sector
* 16������Ϊ1��Block
* W25X128
* ����Ϊ16M�ֽ�,����256��Block,4096��Sector
* ���԰�Sector�������ɰ�Block���� �ҿ�����Ƭ����
********************************************************************************/

void Delay(u16 i)
{
    while(i--);
}
u16 SPI_FLASH_TYPE=W25Q64;//Ĭ�Ͼ���25Q64
/*******************************************************************************
* Function Name  : SPI_Flash_Init
* Description    : ��ʼ��SPI flash ���ʣ�25Q128�ٶȿɴ�104MHz����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void SPI_Flash_Init(void)
{
    SPI1_Init();		   	//��ʼ��SPI
    SPI1_SetSpeed(SPI_BaudRatePrescaler_2);//����Ϊ18Mʱ��,����ģʽ
    SPI_FLASH_TYPE=SPI_Flash_ReadID();//��ȡFLASH ID.

}
/*******************************************************************************
* Function Name  : SPI_Flash_ReadSR
* Description    : ��ȡSPI_FLASH��״̬�Ĵ���
* Input          : None
* Output         : None
* Return         : ״ֵ̬
			   BIT7  6   5   4   3   2   1   0
                  SPR   RV  TB BP2 BP1 BP0 WEL BUSY
                  SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
                  TB,BP2,BP1,BP0:FLASH����д��������
                  WEL:дʹ������
                  BUSY:æ���λ(1,æ;0,����)
                  Ĭ��:0x00
*******************************************************************************/
u8 SPI_Flash_ReadSR(void)
{
    u8 byte=0;
    SPI_FLASH_CS_Low();                            //ʹ������
    SPI1_ReadWriteByte(W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������
    byte=SPI1_ReadWriteByte(0Xff);             //��ȡһ���ֽ�
    SPI_FLASH_CS_HIGH( );                            //ȡ��Ƭѡ
    return byte;
}
/*******************************************************************************
* Function Name  : SPI_FLASH_Write_SR
* Description    : дSPI_FLASH״̬�Ĵ���
* Input          : ״ֵ̬ ��ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д
* Output         : None
* Return         : None
			   BIT7  6   5   4   3   2   1   0
                  SPR   RV  TB BP2 BP1 BP0 WEL BUSY
                  SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
                  TB,BP2,BP1,BP0:FLASH����д��������
                  WEL:дʹ������
                  BUSY:æ���λ(1,æ;0,����)
                  Ĭ��:0x00
*******************************************************************************/
void SPI_FLASH_Write_SR(u8 sr)
{
    SPI_FLASH_CS_Low();                            //ʹ������
    SPI1_ReadWriteByte(W25X_WriteStatusReg);   //����д״̬�Ĵ�������
    SPI1_ReadWriteByte(sr);               //д��һ���ֽ�
    SPI_FLASH_CS_HIGH();                            //ȡ��Ƭѡ
}
/*******************************************************************************
* Function Name  : SPI_FLASH_Write_Enable
* Description    : SPI_FLASHдʹ�� ����WEL��λ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_Write_Enable(void)
{
    SPI_FLASH_CS_Low();                            //ʹ������
    SPI1_ReadWriteByte(W25X_WriteEnable);      //����дʹ��
    SPI_FLASH_CS_HIGH();                            //ȡ��Ƭѡ
}
/*******************************************************************************
* Function Name  : SPI_FLASH_Write_Disable
* Description    : SPI_FLASHдʹ�� ����WEL����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_Write_Disable(void)
{
    SPI_FLASH_CS_Low();                            //ʹ������
    SPI1_ReadWriteByte(W25X_WriteDisable);     //����д��ָֹ��
    SPI_FLASH_CS_HIGH();                            //ȡ��Ƭѡ
}
/*******************************************************************************
* Function Name  : SPI_Flash_ReadID
* Description    : ��ȡоƬID
* Input          : None
* Output         : None
* Return         : 16λ��оƬID
*******************************************************************************/
u16 SPI_Flash_ReadID(void)
{
    u16 Temp = 0;
    SPI_FLASH_CS_Low();
    SPI1_ReadWriteByte(0x90);//���Ͷ�ȡID����
    SPI1_ReadWriteByte(0x00);
    SPI1_ReadWriteByte(0x00);
    SPI1_ReadWriteByte(0x00);
    Temp|=SPI1_ReadWriteByte(0xFF)<<8;
    Temp|=SPI1_ReadWriteByte(0xFF);
    SPI_FLASH_CS_HIGH();
    return Temp;
}
/*******************************************************************************
* Function Name  : SPI_Flash_Read
* Description    : ��ȡSPI FLASH ����ָ����ַ��ʼ��ȡָ�����ȵ�����
* Input          : pBuffer:���ݴ洢����
                   ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)��
                   NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_Flash_Read(u32 ReadAddr,u8* pBuffer,u16 NumByteToRead)
{
    u16 i;
    SPI_FLASH_CS_Low();                        //ʹ������
    SPI1_ReadWriteByte(W25X_ReadData);         //���Ͷ�ȡ����
    SPI1_ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ
    SPI1_ReadWriteByte((u8)((ReadAddr)>>8));
    SPI1_ReadWriteByte((u8)ReadAddr);
    for(i=0; i<NumByteToRead; i++)
    {
        pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //ѭ������
    }
    SPI_FLASH_CS_HIGH();                       //ȡ��Ƭѡ
}
/*******************************************************************************
* Function Name  : SPI_Flash_Write_Page
* Description    : SPI��һҳ(0~65535)��д������256���ֽڵ����ݣ���ָ����ַ��ʼд�����256�ֽڵ�����
* Input          : pBuffer:���ݴ洢����
                   WriteAddr:��ʼд��ĵ�ַ(24bit)��
                   NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_Flash_Write_Page(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)
{
    u16 i;
    SPI_FLASH_Write_Enable();                  //SET WEL
    SPI_FLASH_CS_Low();                            //ʹ������
    SPI1_ReadWriteByte(W25X_PageProgram);      //����дҳ����
    SPI1_ReadWriteByte((u8)((WriteAddr)>>16)); //����24bit��ַ
    SPI1_ReadWriteByte((u8)((WriteAddr)>>8));
    SPI1_ReadWriteByte((u8)WriteAddr);
    for(i=0; i<NumByteToWrite; i++)SPI1_ReadWriteByte(pBuffer[i]); //ѭ��д��
    SPI_FLASH_CS_HIGH();                            //ȡ��Ƭѡ
    SPI_Flash_Wait_Busy();					   //�ȴ�д�����
}
/*******************************************************************************
* Function Name  : SPI_Flash_Write_NoCheck
* Description    : �޼���дSPI FLASH ������ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,
                   �����ڷ�0XFF��д������ݽ�ʧ��!
				   �����Զ���ҳ���� ��
				   ��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
* Input          : pBuffer:���ݴ洢��
                   WriteAddr:��ʼд��ĵ�ַ(24bit)
                   NumByteToWrite:Ҫд����ֽ���(���65535)
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_Flash_Write_NoCheck(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)
{
    u16 pageremain;
    pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���
    if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
    while(1)
    {
        SPI_Flash_Write_Page(WriteAddr,pBuffer,pageremain);
        if(NumByteToWrite==pageremain)break;//д�������
        else //NumByteToWrite>pageremain
        {
            pBuffer+=pageremain;
            WriteAddr+=pageremain;

            NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
            if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
            else pageremain=NumByteToWrite; 	  //����256���ֽ���
        }
    };
}
/*******************************************************************************
* Function Name  : SPI_Flash_Write
* Description    : ��ָ����ַ��ʼд��ָ�����ȵ����ݣ�
                   �ú�������������!
* Input          : pBuffer:���ݴ洢��
                   WriteAddr:��ʼд��ĵ�ַ(24bit)
                   NumByteToWrite:Ҫд����ֽ���(���65535)
* Output         : None
* Return         : return=1,д����ȷ��return=0��д���ַ���Ϸ���д�����ݲ���ȷ��
*******************************************************************************/
s32 SPI_Flash_Write(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)
{
    u32 secpos;
    u16 secoff;
    u16 secremain;
    u16 i;
    secpos=WriteAddr/4096;//������ַ 0~4096 for w25Q128
    secoff=WriteAddr%4096;//�������ڵ�ƫ��
    secremain=4096-secoff;//����ʣ��ռ��С
    secremain=2048-secoff;//��2K�ռ䣨�Ͱ�������ʣ��ռ�
    /*���ݾ��洢�ڵ�2K�ռ��ڣ��Ը�2K�ռ䲻�������*/
    if((0<secremain)&&(secremain<=2048))
    {
        if(NumByteToWrite<=secremain)
        {
            secremain=NumByteToWrite;//������2048���ֽ�
            SPI_Flash_Read(secpos*4096,G_PageTmp,2048);//�����Ͱ�����������
//            for(i=0; i<secremain; i++) //У������
//            {
//                if(G_PageTmp[secoff+i]!=0XFF)break;//��Ҫ����
//            }
//            if(i<secremain)//��Ҫ����
//            {
                SPI_Flash_Erase_Sector(secpos);//�����������
                for(i=0; i<secremain; i++)	 //����
                {
                    G_PageTmp[i+secoff]=pBuffer[i];
                }
                SPI_Flash_Write_NoCheck(secpos*4096,G_PageTmp,2048);//д��Ͱ�����

//            }
//            else
//                SPI_Flash_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.

            SPI_Flash_Read(secpos*4096,G_PageTmp,NumByteToWrite);// ����д��������Ƿ���ȷ
            for(i=0; i<NumByteToWrite; i++)
            {
                if((G_PageTmp[i+secoff])!=(pBuffer[i])) break;
            }
            if(i==NumByteToWrite)
                return Success;
            else
                return Failed;
        }
        else //Ҫд����ֽ�����������ʣ��ռ䣬����
            return Failed ;
    }
    else
        return Failed;

}

/*******************************************************************************
* Function Name  : SPI_Flash_Erase_Chip
* Description    : ��������оƬ
                   //��Ƭ����ʱ��:
                   //W25X16:25s
                   //W25X32:40s
                   //W25X64:40s
                   //�ȴ�ʱ�䳬��...
* Input          : pBuffer:���ݴ洢��
                   WriteAddr:��ʼд��ĵ�ַ(24bit)
                   NumByteToWrite:Ҫд����ֽ���(���65535)
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_Flash_Erase_Chip(void)
{
    SPI_FLASH_Write_Enable();                  //SET WEL
    SPI_Flash_Wait_Busy();
    SPI_FLASH_CS_Low();                            //ʹ������
    SPI1_ReadWriteByte(W25X_ChipErase);        //����Ƭ��������
    SPI_FLASH_CS_HIGH();                            //ȡ��Ƭѡ
    SPI_Flash_Wait_Busy();   				   //�ȴ�оƬ��������
}
/*******************************************************************************
* Function Name  : SPI_Flash_Erase_Sector
* Description    : ����һ������
                  ����һ������������ʱ��:150ms
* Input          : Dst_Addr:������ַ 0~4096 for w25x128
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_Flash_Erase_Sector(u32 Dst_Addr)
{
    Dst_Addr*=4096;
    SPI_FLASH_Write_Enable();                  //SET WEL
    SPI_Flash_Wait_Busy();
    SPI_FLASH_CS_Low();                            //ʹ������
    SPI1_ReadWriteByte(W25X_SectorErase);      //������������ָ��
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  //����24bit��ַ
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));
    SPI1_ReadWriteByte((u8)Dst_Addr);
    SPI_FLASH_CS_HIGH();                            //ȡ��Ƭѡ
    SPI_Flash_Wait_Busy();   				   //�ȴ��������
}
/*******************************************************************************
* Function Name  : SPI_Flash_Wait_Busy
* Description    : �ȴ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_Flash_Wait_Busy(void)
{
    while ((SPI_Flash_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}
/*******************************************************************************
* Function Name  : SPI_Flash_PowerDown
* Description    : �������ģʽ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_Flash_PowerDown(void)
{
    SPI_FLASH_CS_Low();                            //ʹ������
    SPI1_ReadWriteByte(W25X_PowerDown);        //���͵�������
    SPI_FLASH_CS_HIGH();                            //ȡ��Ƭѡ
    Delay(20); //�ȴ�TPD
}
/*******************************************************************************
* Function Name  : SPI_Flash_WAKEUP
* Description    : ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_Flash_WAKEUP(void)
{
    SPI_FLASH_CS_Low();                            //ʹ������
    SPI1_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB
    SPI_FLASH_CS_HIGH();                            //ȡ��Ƭѡ
    Delay(20);//�ȴ�TRES1
}

























