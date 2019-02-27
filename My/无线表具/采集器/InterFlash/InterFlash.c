/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : flash.c

*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include <InterFlash.h>
#include "ErrCode.h"


/* Private variables ---------------------------------------------------------*/  
volatile FLASH_Status FLASHStatus;
volatile TestStatus MemoryProgramStatus;  
//static u8 G_PageTmp[PAGESIZE]={0,1,2};
 
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : WriteFlash_Page
* Description    : 以字节为单位向flash中写数据,写入数据必须在一页之内，
                   开始地址使用偶地址，奇地址会出错按照字节读写也会出现问题。
* Input          : Page_Addr：32位页起始地址
                   Page Offset：32位页内偏移量
				   Write_Buf：待写入数据指针
				   Write_Len：待写入数据长度
* Output         : None
* Return         : 写结果，1为成功，0为失败
*******************************************************************************/
u8 WriteFlash_Page(u32 Page_Addr,u32 Page_Offset,u8 *Write_Buf,u16 Write_Len )
{ 
  u32 Addr_Tmp=0,i=0,j=0;
  FLASHStatus = FLASH_COMPLETE;
  MemoryProgramStatus = PASSED;

  /* 判断地址合法性 */
  if((Page_Addr <= CODE_END_PAGEADDR)||(Page_Addr > FLASH_END_PAGEADDR))
  {
  		return 0;
  }

   /* 判断长度合法性 */
  if((Page_Offset + Write_Len) > PAGESIZE)
  {
  		return 0;
  }
  /* Unlock the Flash Program Erase controller */
  
  Addr_Tmp =  Page_Addr;
  FLASH_Unlock();
  switch(Page_Addr)	  //分情况写入
  {
  		case WENKONG_PAGEADDR : //对温控页的操作。
			 {
		    	//暂未定义
			    break;
		     }
		case CJQ_PARAMETER_PAGEADDR : //采集器参数页，共18字节。
			 {
			  /* 写之前先读出原有数据到缓存 */
			  for(i=0;i<CJQ_PARAMETER_SIZE;i++)
			  {
			  	G_PageTmp[i] =  *((vu8*)Addr_Tmp);
				Addr_Tmp += 1;
			  }
			
			  /* 将数据写到缓存 */
			  for(i=Page_Offset,j=0;i<Page_Offset+Write_Len;i++)
			  {
			  	G_PageTmp[i] = Write_Buf[j]; 
				j++;
			  }
			
			  /* Clear All pending flags */
			  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
			
			  /* 一次只擦除一页 */
			  FLASHStatus = FLASH_ErasePage(Page_Addr);
			
			  /* 半字数据写入 */
			  i = 0;
			  j = 0;
			  while((i < CJQ_PARAMETER_SIZE) && (FLASHStatus == FLASH_COMPLETE))
			  {
			    FLASHStatus = FLASH_ProgramHalfWord(Page_Addr+i, *((u16*)(G_PageTmp+i)));
			    i += 2;	
			  }
			  break;
			}
		case METER_CHECK_PAGEADDR: //表号校验页，共256+1字节。
			{
			  /* 写之前先读出原有数据到缓存 */
			  for(i=0;i<METER_CHECK_SIZE;i++)
			  {
			  	G_PageTmp[i] =  *((vu8*)Addr_Tmp);
				Addr_Tmp += 1;
			  }
			
			  /* 将数据写到缓存 */
			  for(i=Page_Offset,j=0;i<Page_Offset+Write_Len;i++)
			  {
			  	G_PageTmp[i] = Write_Buf[j]; 
				j++;
			  }
			
			  /* Clear All pending flags */
			  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
			
			  /* 一次只擦除一页 */
				FLASHStatus = FLASH_ErasePage(Page_Addr);
			
			  /* 半字数据写入 */
			  i = 0;
			  j = 0;
			  while((i < METER_CHECK_SIZE) && (FLASHStatus == FLASH_COMPLETE))
			  {
			    FLASHStatus = FLASH_ProgramHalfWord(Page_Addr+i, *((u16*)(G_PageTmp+i)));
			    i += 2;	
			  }
			  break;
			}
   	   case METER_NUM_PAGEADDR : //表号页，共256+1字节。
			{
			  /* 写之前先读出原有数据到缓存 */
			  for(i=0;i<METER_NUM_SIZE;i++)
			  {
			  	G_PageTmp[i] =  *((vu8*)Addr_Tmp);
				Addr_Tmp += 1;
			  }
			
			  /* 将数据写到缓存 */
			  for(i=Page_Offset,j=0;i<Page_Offset+Write_Len;i++)
			  {
			  	G_PageTmp[i] = Write_Buf[j]; 
				j++;
			  }
			
			  /* Clear All pending flags */
			  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
			
			  /* 一次只擦除一页 */
				FLASHStatus = FLASH_ErasePage(Page_Addr);
			
			  /* 半字数据写入 */
			  i = 0;
			  j = 0;
			  while((i < METER_NUM_SIZE) && (FLASHStatus == FLASH_COMPLETE))
			  {
			    FLASHStatus = FLASH_ProgramHalfWord(Page_Addr+i, *((u16*)(G_PageTmp+i)));
			    i += 2;	
			  }

			  break;
			}
		default:	//写表数据
		    {
			  for(i=0;i< METER_DATA_SIZE+PAGESIZE/2;i++)
			  {
			  	G_PageTmp[i] =  *((vu8*)Addr_Tmp);
				Addr_Tmp += 1;
			  }
			
			  /* 将数据写到缓存 */
			  for(i=Page_Offset,j=0;i<Page_Offset+Write_Len;i++)
			  {
			  	G_PageTmp[i] = Write_Buf[j]; 
				j++;
			  }
			
			  /* Clear All pending flags */
			  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
			
			  /* 一次只擦除一页 */
			  FLASHStatus = FLASH_ErasePage(Page_Addr);
			
			  /* 半字数据写入 */
			  i = 0;
			  j = 0;
			  while((i < METER_DATA_SIZE) && (FLASHStatus == FLASH_COMPLETE))
			  {
			    FLASHStatus = FLASH_ProgramHalfWord(Page_Addr+i, *((u16*)(G_PageTmp+i)));
				FLASHStatus = FLASH_ProgramHalfWord(Page_Addr+PAGESIZE/2+i, *((u16*)(G_PageTmp+PAGESIZE/2+i)));
			    i += 2;	
			  }
			}
	}

	/* 检验数据是否出错  */
	Addr_Tmp = Page_Addr + Page_Offset;
	i=0;
	while((Addr_Tmp < Page_Addr + Page_Offset + Write_Len) && (MemoryProgramStatus != 0))
	{	
		if((*(vu8*) Addr_Tmp) != Write_Buf[i++])
		{	MemoryProgramStatus = FAILED;
			FLASH_Lock();
			return 0;
		}
		Addr_Tmp += 1;
	}
	FLASH_Lock();	
	return 1;
}

 


/*******************************************************************************
* Function Name  : ReadFlash_Byte
* Description    : 以字节为单位从flash中读数据。
* Input          : Start_Addr：32位开始地址
                   Read_Buf：8位要读取数据数组buf地址
				   Read_Len：读取数据长度
* Output         : None
* Return         : None
*******************************************************************************/
void ReadFlash_Byte(u32 Start_Addr,u8 *Read_Buf,u16 Read_Len )
{
	u16 i=0;
	  /* Unlock the Flash Program Erase controller */
  	FLASH_Unlock();
	for(i=0;i<Read_Len;i++)
	{
    	*Read_Buf=*((vu8*)Start_Addr);//将数值类型变量转换成指针类型
		Start_Addr=Start_Addr+1;
		Read_Buf++;
  	}
	FLASH_Lock();
}

/*******************************************************************************
* Function Name  : FlashErase_Page
* Description    : 擦除flash中的一页。
* Input          : Start_Addr：32位地址
* Output         : None
* Return         : None
*******************************************************************************/
void FlashErase_Page(u32 Start_Addr )
{
	/* 判断地址合法性 */
    if((Start_Addr < CODE_END_PAGEADDR)||(Start_Addr > FLASH_END_PAGEADDR))
    {
  		return;
    }
	/* Unlock the Flash Program Erase controller */
  	FLASH_Unlock();
	/* Clear All pending flags */
  	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASHStatus = FLASH_ErasePage(Start_Addr);
	FLASH_Lock();
}



