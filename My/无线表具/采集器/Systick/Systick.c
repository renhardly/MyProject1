#include "stm32f10x_lib.h"
#include <stdio.h>
#include "platform_config.h"
#include <InterFlash.h>
#include "stm32f10x_flash.h"
#include "ErrCode.h"
#include "spi.h"
void Systick_config(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	SysTick_SetReload(4000000);	//500ms÷–∂œ“ª¥Œ
	SysTick_ITConfig(ENABLE);
	SysTick_CounterCmd(ENABLE);
}

