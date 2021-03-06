
#include <InterFlash.h>
#include "spi.h"
#include "ErrCode.h"


  
/********************************************************************************
* 4Kbytes为一个Sector
* 16个扇区为1个Block
* W25X128
* 容量为16M字节,共有256个Block,4096个Sector 
* 可以按Sector擦除、可按Block擦除 且可以整片擦除
********************************************************************************/	 

void Delay(u16 i)
{
   while(i--);
}
u16 SPI_FLASH_TYPE=W25Q64;//默认就是25Q64

/*******************************************************************************
* Function Name  : SPI_Flash_Init
* Description    : 初始化SPI flash 速率，25Q128速度可达104MHz以上
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void SPI_Flash_Init(void)
{	
	SPI1_Init();		   	//初始化SPI
	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);//设置为18M时钟,高速模式
	SPI_FLASH_TYPE=SPI_Flash_ReadID();//读取FLASH ID.  

}  
 /*******************************************************************************
* Function Name  : SPI_Flash_ReadSR
* Description    : 读取SPI_FLASH的状态寄存器
* Input          : None
* Output         : None
* Return         : 状态值
				   BIT7  6   5   4   3   2   1   0
                   SPR   RV  TB BP2 BP1 BP0 WEL BUSY
                   SPR:默认0,状态寄存器保护位,配合WP使用
                   TB,BP2,BP1,BP0:FLASH区域写保护设置
                   WEL:写使能锁定
                   BUSY:忙标记位(1,忙;0,空闲)
                   默认:0x00
*******************************************************************************/
u8 SPI_Flash_ReadSR(void)   
{  
	u8 byte=0; 
	//TIM_Cmd(TIM3, DISABLE);   
	SPI_FLASH_CS_Low();                            //使能器件   
	SPI1_ReadWriteByte(W25X_ReadStatusReg);    //发送读取状态寄存器命令    
	byte=SPI1_ReadWriteByte(0Xff);             //读取一个字节  
	SPI_FLASH_CS_HIGH( );					   //取消片选   
//	TIM_Cmd(TIM3, ENABLE);                              
	return byte;   
}
 /*******************************************************************************
* Function Name  : SPI_FLASH_Write_SR
* Description    : 写SPI_FLASH状态寄存器
* Input          : 状态值 ，只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写
* Output         : None
* Return         : None
				   BIT7  6   5   4   3   2   1   0
                   SPR   RV  TB BP2 BP1 BP0 WEL BUSY
                   SPR:默认0,状态寄存器保护位,配合WP使用
                   TB,BP2,BP1,BP0:FLASH区域写保护设置
                   WEL:写使能锁定
                   BUSY:忙标记位(1,忙;0,空闲)
                   默认:0x00
*******************************************************************************/ 
void SPI_FLASH_Write_SR(u8 sr)   
{   
   // TIM_Cmd(TIM3, DISABLE);
	SPI_FLASH_CS_Low();                            //使能器件   
	SPI1_ReadWriteByte(W25X_WriteStatusReg);   //发送写状态寄存器命令    
	SPI1_ReadWriteByte(sr);               //写入一个字节  
	SPI_FLASH_CS_HIGH();                            //取消片选 
	//TIM_Cmd(TIM3, ENABLE);    	      
}  
  /*******************************************************************************
* Function Name  : SPI_FLASH_Write_Enable
* Description    : SPI_FLASH写使能 ，将WEL置位
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void SPI_FLASH_Write_Enable(void)   
{
	//TIM_Cmd(TIM3, DISABLE);
	SPI_FLASH_CS_Low();                            //使能器件   
    SPI1_ReadWriteByte(W25X_WriteEnable);      //发送写使能  
	SPI_FLASH_CS_HIGH();                            //取消片选   
	//TIM_Cmd(TIM3, ENABLE);  	      
} 
  /*******************************************************************************
* Function Name  : SPI_FLASH_Write_Disable
* Description    : SPI_FLASH写使能 ，将WEL清零
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void SPI_FLASH_Write_Disable(void)   
{  
//	TIM_Cmd(TIM3, DISABLE);
	SPI_FLASH_CS_Low();                            //使能器件   
    SPI1_ReadWriteByte(W25X_WriteDisable);     //发送写禁止指令    
	SPI_FLASH_CS_HIGH();                            //取消片选
//	TIM_Cmd(TIM3, ENABLE);     	      
} 
  /*******************************************************************************
* Function Name  : SPI_Flash_ReadID
* Description    : 读取芯片ID
* Input          : None
* Output         : None
* Return         : 16位的芯片ID
*******************************************************************************/  			    
u16 SPI_Flash_ReadID(void)
{
	u16 Temp = 0;
//	TIM_Cmd(TIM3, DISABLE); 	  
	SPI_FLASH_CS_Low();				    
	SPI1_ReadWriteByte(0x90);//发送读取ID命令	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	SPI_FLASH_CS_HIGH();
//	TIM_Cmd(TIM3, ENABLE); 				    
	return Temp;
}
/*******************************************************************************
* Function Name  : SPI_Flash_Read
* Description    : 读取SPI FLASH ，在指定地址开始读取指定长度的数据
* Input          : pBuffer:数据存储区，
                   ReadAddr:开始读取的地址(24bit)，
                   NumByteToRead:要读取的字节数(最大65535)
* Output         : None
* Return         : None
*******************************************************************************/    		    
void SPI_Flash_Read(u32 ReadAddr,u8* pBuffer,u16 NumByteToRead)   
{ 
 	u16 i;
	//TIM_Cmd(TIM3, DISABLE);     												    
	SPI_FLASH_CS_Low();                        //使能器件   
    SPI1_ReadWriteByte(W25X_ReadData);         //发送读取命令   
    SPI1_ReadWriteByte((u8)((ReadAddr)>>16));  //发送24bit地址    
    SPI1_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPI1_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //循环读数  
    }
	SPI_FLASH_CS_HIGH();                       //取消片选  
//	TIM_Cmd(TIM3, ENABLE);    	      
} 
/*******************************************************************************
* Function Name  : SPI_Flash_Write_Page
* Description    : SPI在一页(0~65535)内写入少于256个字节的数据，在指定地址开始写入最大256字节的数据
* Input          : pBuffer:数据存储区，
                   WriteAddr:开始写入的地址(24bit)，
                   NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
* Output         : None
* Return         : None
*******************************************************************************/    
void SPI_Flash_Write_Page(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)
{
 	u16 i;
//	TIM_Cmd(TIM3, DISABLE);   
    SPI_FLASH_Write_Enable();                  //SET WEL 
	SPI_FLASH_CS_Low();                            //使能器件   
    SPI1_ReadWriteByte(W25X_PageProgram);      //发送写页命令   
    SPI1_ReadWriteByte((u8)((WriteAddr)>>16)); //发送24bit地址    
    SPI1_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPI1_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI1_ReadWriteByte(pBuffer[i]);//循环写数  
	SPI_FLASH_CS_HIGH();                            //取消片选 
	SPI_Flash_Wait_Busy();					   //等待写入结束
//	TIM_Cmd(TIM3, ENABLE); 
} 
/*******************************************************************************
* Function Name  : SPI_Flash_Write_NoCheck
* Description    : 无检验写SPI FLASH ，必须确保所写的地址范围内的数据全部为0XFF,
                   否则在非0XFF处写入的数据将失败!
				   具有自动换页功能 ，
				   在指定地址开始写入指定长度的数据,但是要确保地址不越界!
* Input          : pBuffer:数据存储区
                   WriteAddr:开始写入的地址(24bit)
                   NumByteToWrite:要写入的字节数(最大65535)
* Output         : None
* Return         : None
*******************************************************************************/ 
void SPI_Flash_Write_NoCheck(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	 
	//TIM_Cmd(TIM3, DISABLE);   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		SPI_Flash_Write_Page(WriteAddr,pBuffer,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};
//	TIM_Cmd(TIM3, ENABLE); 	    
}
/*******************************************************************************
* Function Name  : SPI_Flash_Write
* Description    : 在指定地址开始写入指定长度的数据，
                   该函数带擦除操作!
* Input          : pBuffer:数据存储区
                   WriteAddr:开始写入的地址(24bit)
                   NumByteToWrite:要写入的字节数(最大65535)
* Output         : None
* Return         : return=1,写入正确；return=0：写入地址不合法、写入数据不正确；
*******************************************************************************/	   
u8 SPI_Flash_Write(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;
//	TIM_Cmd(TIM3, DISABLE); 
	secpos=WriteAddr/4096;//扇区地址 0~4096 for w25Q128
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小
	secremain=2048-secoff;//低2K空间（低半扇区）剩余空间   
   /*数据均存储在低2K空间内，对高2K空间不允许操作*/
	if((0<secremain)&&(secremain<=2048))  
	{	    
	    if(NumByteToWrite<=secremain)
		{
			secremain=NumByteToWrite;//不大于2048个字节
			switch (secpos*4096)
			{
				case EXT_CJQ_PARAMETER_PAGEADDR://采集器参数、温控器页
				 	{
						SPI_Flash_Read(secpos*4096,G_PageTmp,CJQ_PARAMETER_SIZE);//读出采集器参数
					
						for(i=0;i<secremain;i++)	   //复制
						{
						   G_PageTmp[i+secoff]=pBuffer[i];	  
						}
						SPI_Flash_Erase_Sector(secpos);//擦除这个扇区
						SPI_Flash_Write_NoCheck(secpos*4096,G_PageTmp,CJQ_PARAMETER_SIZE);//写入低半扇区 
						break;
					}
				case EXT_METER_CHECK_PAGEADDR://表具索引页 256+1字节
				 	{
						SPI_Flash_Read(secpos*4096,G_PageTmp,METER_CHECK_SIZE);//表具索引
					
						for(i=0;i<secremain;i++)	   //复制
						{
						   G_PageTmp[i+secoff]=pBuffer[i];	  
						}
						SPI_Flash_Erase_Sector(secpos);//擦除这个扇区
						SPI_Flash_Write_NoCheck(secpos*4096,G_PageTmp,METER_CHECK_SIZE);//写入低半扇区 
						break;
					}
				case EXT_METER_NUM_PAGEADDR://表号页 256*8字节
				 	{
						SPI_Flash_Read(secpos*4096,G_PageTmp,METER_NUM_SIZE);//
					
						for(i=0;i<secremain;i++)	   //复制
						{
						   G_PageTmp[i+secoff]=pBuffer[i];	  
						}
						SPI_Flash_Erase_Sector(secpos);//擦除这个扇区
						SPI_Flash_Write_NoCheck(secpos*4096,G_PageTmp,METER_NUM_SIZE);//写入低半扇区 
						break;
					}
					default ://表号页 256字节
					  {
						SPI_Flash_Read(secpos*4096,G_PageTmp,METER_DATA_SIZE+PAGESIZE/2);//
					
						for(i=0;i<secremain;i++)	   //复制
						{
						   G_PageTmp[i+secoff]=pBuffer[i];	  
						}
						SPI_Flash_Erase_Sector(secpos);//擦除这个扇区
						SPI_Flash_Write_NoCheck(secpos*4096,G_PageTmp,METER_DATA_SIZE+PAGESIZE/2);// 
						break;
					   }

			}
			SPI_Flash_Read(secpos*4096,G_PageTmp,NumByteToWrite);// 检验写入的数据是否正确
			for(i=0;i<NumByteToWrite;i++)
			{
				if((G_PageTmp[i+secoff])!=(pBuffer[i])) break;
			}
			if(i==NumByteToWrite)
			{
			//	TIM_Cmd(TIM3, ENABLE); 	
				return (1);
			}
			else 
			{
			// TIM_Cmd(TIM3, ENABLE); 	
			 return 0 ;	
			}
		 }
		else //要写入的字节数大于扇区剩余空间，报错	
		{
		   //	TIM_Cmd(TIM3, ENABLE); 	
			return 0 ;	
		}	
		    							  
	}
	else 
	{
		//TIM_Cmd(TIM3, ENABLE); 	
		return 0;
	}
	 	 
}

/*******************************************************************************
* Function Name  : SPI_Flash_Erase_Chip
* Description    : 擦除整个芯片
                   //整片擦除时间:
                   //W25X16:25s 
                   //W25X32:40s 
                   //W25X64:40s 
                   //等待时间超长...
* Input          : pBuffer:数据存储区
                   WriteAddr:开始写入的地址(24bit)
                   NumByteToWrite:要写入的字节数(最大65535)
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_Flash_Erase_Chip(void)   
{   
    TIM_Cmd(TIM3, DISABLE);                                      
    SPI_FLASH_Write_Enable();                  //SET WEL 
    SPI_Flash_Wait_Busy();   
  	SPI_FLASH_CS_Low();                            //使能器件   
    SPI1_ReadWriteByte(W25X_ChipErase);        //发送片擦除命令  
	SPI_FLASH_CS_HIGH();                            //取消片选     	      
	SPI_Flash_Wait_Busy();   				   //等待芯片擦除结束
	TIM_Cmd(TIM3, ENABLE);
} 
 /*******************************************************************************
* Function Name  : SPI_Flash_Erase_Sector
* Description    : 擦除一个扇区
                   擦除一个扇区的最少时间:150ms
* Input          : Dst_Addr:扇区地址 0~4096 for w25x128
* Output         : None
* Return         : None
*******************************************************************************/ 
void SPI_Flash_Erase_Sector(u32 Dst_Addr)   
{   
	//TIM_Cmd(TIM3, DISABLE); 
	Dst_Addr*=4096;
    SPI_FLASH_Write_Enable();                  //SET WEL 	 
    SPI_Flash_Wait_Busy();   
  	SPI_FLASH_CS_Low();                            //使能器件   
    SPI1_ReadWriteByte(W25X_SectorErase);      //发送扇区擦除指令 
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  //发送24bit地址    
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI1_ReadWriteByte((u8)Dst_Addr);  
	SPI_FLASH_CS_HIGH();                            //取消片选     	      
    SPI_Flash_Wait_Busy();   				   //等待擦除完成
	//TIM_Cmd(TIM3, ENABLE); 
} 
 /*******************************************************************************
* Function Name  : SPI_Flash_Wait_Busy
* Description    : 等待空闲
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void SPI_Flash_Wait_Busy(void)   
{   
	while ((SPI_Flash_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
} 
 /*******************************************************************************
* Function Name  : SPI_Flash_PowerDown
* Description    : 进入掉电模式
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/   
void SPI_Flash_PowerDown(void)   
{ 
//TIM_Cmd(TIM3, DISABLE); 
  	SPI_FLASH_CS_Low();                            //使能器件   
    SPI1_ReadWriteByte(W25X_PowerDown);        //发送掉电命令  
	SPI_FLASH_CS_HIGH();                            //取消片选     	                               
    Delay(200); //等待TPD  
	//TIM_Cmd(TIM3, ENABLE); 
}  
 /*******************************************************************************
* Function Name  : SPI_Flash_WAKEUP
* Description    : 唤醒
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void SPI_Flash_WAKEUP(void)   
{  
    //TIM_Cmd(TIM3, DISABLE); 
  	SPI_FLASH_CS_Low();                            //使能器件   
    SPI1_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	SPI_FLASH_CS_HIGH();                            //取消片选                                
    Delay(200);//等待TRES1
	//TIM_Cmd(TIM3, ENABLE); 
} 

























