#include "stm32f10x.h"
void IWDG_init(void)
{
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); 	/* 写入0x5555,用于允许狗狗寄存器写入功能 */
  IWDG_SetPrescaler(IWDG_Prescaler_256);            /* 内部低速时钟256分频 40K/256=156HZ(6.4ms) */ 
  /* 看门狗定时器溢出时间 */
  IWDG_SetReload(150);							    /* 喂狗时间 1s/6.4MS=150 .注意不能大于0xfff*/
  IWDG_ReloadCounter();								/* 喂狗*/
  IWDG_Enable(); 									/* 使能看门狗*/
}


