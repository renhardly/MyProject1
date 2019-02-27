#include "I2C_Driver.h"


/********************************************************/              

void I2C_delay(void)
{	
   u8 i=100; //��������Ż��ٶ�	����������͵�5����д��
   while(i) 
   { 
     i--; 
   } 
}

bool I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_read)return FALSE;	//SDA��Ϊ�͵�ƽ������æ,�˳�
	SDA_L;
	I2C_delay();
	if(SDA_read) return FALSE;	//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
	SDA_L;
	I2C_delay();
	return TRUE;
}

void I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
}

void I2C_Ack(void)
{	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

void I2C_NoAck(void)
{	
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

bool I2C_WaitAck(void) 	 //����Ϊ:=1��ACK,=0��ACK
{
	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
      SCL_L;
      return FALSE;
	}
	SCL_L;
	return TRUE;
}

void I2C_SendByte(u8 SendByte) //���ݴӸ�λ����λ//
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
		SCL_H;
        I2C_delay();
    }
    SCL_L;
}

u8 I2C_ReceiveByte(void)  //���ݴӸ�λ����λ//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
	  SCL_H;
      I2C_delay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
}

//д��1�ֽ�����       ��д������    ��д���ַ       ��������(24c16��SD2403)
bool I2C_WriteByte(u8 SendByte, u16 WriteAddress, u8 DeviceAddress)
{		
    if(!I2C_Start())return FALSE;
    I2C_SendByte(((WriteAddress & 0x0700) >>7) | DeviceAddress & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte((u8)(WriteAddress & 0x00FF));   //���õ���ʼ��ַ      
    I2C_WaitAck();	
    I2C_SendByte(SendByte);
    I2C_WaitAck();   
    I2C_Stop(); 
	//ע�⣺��Ϊ����Ҫ�ȴ�EEPROMд�꣬���Բ��ò�ѯ����ʱ��ʽ(10ms)
    //Systick_Delay_1ms(10);
    return TRUE;
}

//ע�ⲻ�ܿ�ҳд
//д��1������      ��д�������ַ    ��д�볤��      ��д���ַ       ��������(24c16��SD2403)
bool I2C_BufferWrite(u8* pBuffer, u8 length,     u16 WriteAddress, u8 DeviceAddress)
{
    if(!I2C_Start())return FALSE;
    I2C_SendByte(((WriteAddress & 0x0700) >>7) | DeviceAddress & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte((u8)(WriteAddress & 0x00FF));   //���õ���ʼ��ַ      
	  I2C_WaitAck();	
	  
		while(length--)
		{
		  I2C_SendByte(* pBuffer);
		  I2C_WaitAck();
          pBuffer++;
		}
	  I2C_Stop();
	  //ע�⣺��Ϊ����Ҫ�ȴ�EEPROMд�꣬���Բ��ò�ѯ����ʱ��ʽ(10ms)
	  //Systick_Delay_1ms(10);
	  return TRUE;
}


//��ҳд��1������  ��д�������ַ    ��д�볤��      ��д���ַ       ��������(24c16��SD2403)
void I2C_PageWrite(  u8* pBuffer, u8 length,     u16 WriteAddress, u8 DeviceAddress)
{
    u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
    Addr  = WriteAddress % I2C_PageSize;      //д���ַ�ǿ�ʼҳ�ĵڼ�λ
    count = I2C_PageSize - Addr;					    //�ڿ�ʼҳҪд��ĸ���
    NumOfPage   =  length / I2C_PageSize;     //Ҫд���ҳ��
    NumOfSingle =  length % I2C_PageSize;     //����һҳ�ĸ���

    if(Addr == 0)         //д���ַ��ҳ�Ŀ�ʼ 
    {
      if(NumOfPage == 0)  //����С��һҳ 
      {
        I2C_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress);   //д����һҳ������
      }
      else		            //���ݴ��ڵ���һҳ  
      {
        while(NumOfPage)//Ҫд���ҳ��
        {
          I2C_BufferWrite(pBuffer,I2C_PageSize,WriteAddress,DeviceAddress);//дһҳ������
          WriteAddress +=  I2C_PageSize;
          pBuffer      +=  I2C_PageSize;
					NumOfPage--;
				//	Systick_Delay_1ms(10);
         }
        if(NumOfSingle!=0)//ʣ������С��һҳ
        {
          I2C_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress); //д����һҳ������
				//	Systick_Delay_1ms(10);
        }
       }
    }

    else                  //д���ַ����ҳ�Ŀ�ʼ 
    {
      if(NumOfPage== 0)   //����С��һҳ 
      {
        I2C_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress);   //д����һҳ������
      }
      else                //���ݴ��ڵ���һҳ
      {
        length       -= count;
        NumOfPage     = length / I2C_PageSize;  //���¼���Ҫд���ҳ��
        NumOfSingle   = length % I2C_PageSize;  //���¼��㲻��һҳ�ĸ���	
      
        if(count != 0)
        {  
				  I2C_BufferWrite(pBuffer,count,WriteAddress,DeviceAddress);      //����ʼ�Ŀռ�д��һҳ
          WriteAddress += count;
          pBuffer      += count;
         } 
      
        while(NumOfPage--)  //Ҫд���ҳ��
        {
          I2C_BufferWrite(pBuffer,I2C_PageSize,WriteAddress,DeviceAddress);//дһҳ������
          WriteAddress +=  I2C_PageSize;
          pBuffer      +=  I2C_PageSize; 
        }
        if(NumOfSingle != 0)//ʣ������С��һҳ
        {
          I2C_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress); //д����һҳ������ 
        }
      }
    } 
}

//����1������         ��Ŷ�������  ����������      ��������ַ       ��������(24c16��SD2403)	
bool I2C_ReadByte(u8* pBuffer,   u8 length,     u16 ReadAddress,  u8 DeviceAddress)
{		
    if(!I2C_Start())return FALSE;
    I2C_SendByte(((ReadAddress & 0x0700) >>7) | DeviceAddress & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte((u8)(ReadAddress & 0x00FF));   //���õ���ʼ��ַ      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(((ReadAddress & 0x0700) >>7) | DeviceAddress | 0x0001);
    I2C_WaitAck();
    while(length)
    {
      *pBuffer = I2C_ReceiveByte();
      if(length == 1)I2C_NoAck();
      else I2C_Ack(); 
      pBuffer++;
      length--;
    }
    I2C_Stop();
    return TRUE;
}



