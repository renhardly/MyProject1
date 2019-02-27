//#include "includes.h"
#include <STM32F10X_SPI.h>
#include <STM32F10X_GPIO.h>
#include <STM32F10X_RCC.h>

void SPI2_ENC28J60_init(void)
  {
  SPI_InitTypeDef  SPI_InitStructure;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);
  /* Enable SPI1  */
  SPI_Cmd(SPI2, ENABLE); 
  }
//SPI1读写一字节数据
unsigned char	SPI1_ReadWrite(unsigned char writedat)
	{
	/* Loop while DR register in not emplty */
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI2, writedat);
	
	/* Wait to receive a byte */
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI2);
	}

