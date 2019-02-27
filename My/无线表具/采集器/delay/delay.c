#include "stm32f10x.h"
void delay_us(u16 nUs)
{
	u16 i,j;
	for (j=0;j<nUs;j++)
	{
		i = 10;

		while(i--);
	}
}
void delay_ms(u16 nMs)
{
	u16 i;
	for (i=0;i<nMs;i++)
	{
		delay_us(1000);
	}	
}



































