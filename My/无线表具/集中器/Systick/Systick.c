#include "stm32f10x_lib.h"
void Systick_config(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	SysTick_SetReload(4000000);	//500ms÷–∂œ“ª¥Œ
	SysTick_ITConfig(ENABLE);
	SysTick_CounterCmd(ENABLE);
}

