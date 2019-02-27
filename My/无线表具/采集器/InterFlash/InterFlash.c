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
* Description    : ���ֽ�Ϊ��λ��flash��д����,д�����ݱ�����һҳ֮�ڣ�
                   ��ʼ��ַʹ��ż��ַ�����ַ��������ֽڶ�дҲ��������⡣
* Input          : Page_Addr��32λҳ��ʼ��ַ
                   Page Offset��32λҳ��ƫ����
				   Write_Buf����д������ָ��
				   Write_Len����д�����ݳ���
* Output         : None
* Return         : д�����1Ϊ�ɹ���0Ϊʧ��
*******************************************************************************/
u8 WriteFlash_Page(u32 Page_Addr,u32 Page_Offset,u8 *Write_Buf,u16 Write_Len )
{ 
  u32 Addr_Tmp=0,i=0,j=0;
  FLASHStatus = FLASH_COMPLETE;
  MemoryProgramStatus = PASSED;

  /* �жϵ�ַ�Ϸ��� */
  if((Page_Addr <= CODE_END_PAGEADDR)||(Page_Addr > FLASH_END_PAGEADDR))
  {
  		return 0;
  }

   /* �жϳ��ȺϷ��� */
  if((Page_Offset + Write_Len) > PAGESIZE)
  {
  		return 0;
  }
  /* Unlock the Flash Program Erase controller */
  
  Addr_Tmp =  Page_Addr;
  FLASH_Unlock();
  switch(Page_Addr)	  //�����д��
  {
  		case WENKONG_PAGEADDR : //���¿�ҳ�Ĳ�����
			 {
		    	//��δ����
			    break;
		     }
		case CJQ_PARAMETER_PAGEADDR : //�ɼ�������ҳ����18�ֽڡ�
			 {
			  /* д֮ǰ�ȶ���ԭ�����ݵ����� */
			  for(i=0;i<CJQ_PARAMETER_SIZE;i++)
			  {
			  	G_PageTmp[i] =  *((vu8*)Addr_Tmp);
				Addr_Tmp += 1;
			  }
			
			  /* ������д������ */
			  for(i=Page_Offset,j=0;i<Page_Offset+Write_Len;i++)
			  {
			  	G_PageTmp[i] = Write_Buf[j]; 
				j++;
			  }
			
			  /* Clear All pending flags */
			  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
			
			  /* һ��ֻ����һҳ */
			  FLASHStatus = FLASH_ErasePage(Page_Addr);
			
			  /* ��������д�� */
			  i = 0;
			  j = 0;
			  while((i < CJQ_PARAMETER_SIZE) && (FLASHStatus == FLASH_COMPLETE))
			  {
			    FLASHStatus = FLASH_ProgramHalfWord(Page_Addr+i, *((u16*)(G_PageTmp+i)));
			    i += 2;	
			  }
			  break;
			}
		case METER_CHECK_PAGEADDR: //���У��ҳ����256+1�ֽڡ�
			{
			  /* д֮ǰ�ȶ���ԭ�����ݵ����� */
			  for(i=0;i<METER_CHECK_SIZE;i++)
			  {
			  	G_PageTmp[i] =  *((vu8*)Addr_Tmp);
				Addr_Tmp += 1;
			  }
			
			  /* ������д������ */
			  for(i=Page_Offset,j=0;i<Page_Offset+Write_Len;i++)
			  {
			  	G_PageTmp[i] = Write_Buf[j]; 
				j++;
			  }
			
			  /* Clear All pending flags */
			  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
			
			  /* һ��ֻ����һҳ */
				FLASHStatus = FLASH_ErasePage(Page_Addr);
			
			  /* ��������д�� */
			  i = 0;
			  j = 0;
			  while((i < METER_CHECK_SIZE) && (FLASHStatus == FLASH_COMPLETE))
			  {
			    FLASHStatus = FLASH_ProgramHalfWord(Page_Addr+i, *((u16*)(G_PageTmp+i)));
			    i += 2;	
			  }
			  break;
			}
   	   case METER_NUM_PAGEADDR : //���ҳ����256+1�ֽڡ�
			{
			  /* д֮ǰ�ȶ���ԭ�����ݵ����� */
			  for(i=0;i<METER_NUM_SIZE;i++)
			  {
			  	G_PageTmp[i] =  *((vu8*)Addr_Tmp);
				Addr_Tmp += 1;
			  }
			
			  /* ������д������ */
			  for(i=Page_Offset,j=0;i<Page_Offset+Write_Len;i++)
			  {
			  	G_PageTmp[i] = Write_Buf[j]; 
				j++;
			  }
			
			  /* Clear All pending flags */
			  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
			
			  /* һ��ֻ����һҳ */
				FLASHStatus = FLASH_ErasePage(Page_Addr);
			
			  /* ��������д�� */
			  i = 0;
			  j = 0;
			  while((i < METER_NUM_SIZE) && (FLASHStatus == FLASH_COMPLETE))
			  {
			    FLASHStatus = FLASH_ProgramHalfWord(Page_Addr+i, *((u16*)(G_PageTmp+i)));
			    i += 2;	
			  }

			  break;
			}
		default:	//д������
		    {
			  for(i=0;i< METER_DATA_SIZE+PAGESIZE/2;i++)
			  {
			  	G_PageTmp[i] =  *((vu8*)Addr_Tmp);
				Addr_Tmp += 1;
			  }
			
			  /* ������д������ */
			  for(i=Page_Offset,j=0;i<Page_Offset+Write_Len;i++)
			  {
			  	G_PageTmp[i] = Write_Buf[j]; 
				j++;
			  }
			
			  /* Clear All pending flags */
			  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
			
			  /* һ��ֻ����һҳ */
			  FLASHStatus = FLASH_ErasePage(Page_Addr);
			
			  /* ��������д�� */
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

	/* ���������Ƿ����  */
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
* Description    : ���ֽ�Ϊ��λ��flash�ж����ݡ�
* Input          : Start_Addr��32λ��ʼ��ַ
                   Read_Buf��8λҪ��ȡ��������buf��ַ
				   Read_Len����ȡ���ݳ���
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
    	*Read_Buf=*((vu8*)Start_Addr);//����ֵ���ͱ���ת����ָ������
		Start_Addr=Start_Addr+1;
		Read_Buf++;
  	}
	FLASH_Lock();
}

/*******************************************************************************
* Function Name  : FlashErase_Page
* Description    : ����flash�е�һҳ��
* Input          : Start_Addr��32λ��ַ
* Output         : None
* Return         : None
*******************************************************************************/
void FlashErase_Page(u32 Start_Addr )
{
	/* �жϵ�ַ�Ϸ��� */
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



