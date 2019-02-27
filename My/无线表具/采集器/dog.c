#include "stm32f10x.h"
void IWDG_init(void)
{
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); 	/* д��0x5555,�����������Ĵ���д�빦�� */
  IWDG_SetPrescaler(IWDG_Prescaler_256);            /* �ڲ�����ʱ��256��Ƶ 40K/256=156HZ(6.4ms) */ 
  /* ���Ź���ʱ�����ʱ�� */
  IWDG_SetReload(150);							    /* ι��ʱ�� 1s/6.4MS=150 .ע�ⲻ�ܴ���0xfff*/
  IWDG_ReloadCounter();								/* ι��*/
  IWDG_Enable(); 									/* ʹ�ܿ��Ź�*/
}


