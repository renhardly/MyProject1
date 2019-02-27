#include "platform_config.h"
/*******************************************************************
* Function Name  : TIM_Configuration
* Description    : 配置定时器：
				   TIM2；输出捕获比较中断，20ms中断一次
				   TIM4；输出捕获比较中断，20ms中断一次
				   TIM3：向上计数中断，15秒中断一次
* Input          : None	
* Output         : None
* Return         : None   
 ******************************************************************/
void TIM_Configuration(void)
	{
      
	  TIM_OCInitTypeDef  TIM_OCInitStructure;
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  /* Time base configuration */
	  TIM_TimeBaseStructure.TIM_Period = 65535;
	  TIM_TimeBaseStructure.TIM_Prescaler = 0;
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	  /* Prescaler configuration */
	  TIM_PrescalerConfig(TIM2, 0x8198, TIM_PSCReloadMode_Immediate);
	 TIM_PrescalerConfig(TIM4, 0x8198, TIM_PSCReloadMode_Immediate);
	  /* Output Compare Timing Mode configuration: Channel1 */
	  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;//???
	  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
	  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);
	  /* TIM IT enable */
	  TIM_ITConfig(TIM2, TIM_IT_CC1 , ENABLE);
	  TIM_ITConfig(TIM4, TIM_IT_CC1 , ENABLE);
	 /**************TIM3 15S中断一次*************************/
	  TIM_TimeBaseStructure.TIM_Period =29999;//设置了在下一个更新事件装入活动的自动重装载寄存器周期的值
	  TIM_TimeBaseStructure.TIM_Prescaler = 35999;//设置了用来作为 TIMx 时钟频率除数的预分频值
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0;//设置了时钟分割
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM 向上计数模式
	  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	  TIM_Cmd(TIM2, DISABLE);
	  TIM_Cmd(TIM3, DISABLE);
	  TIM_Cmd(TIM4, DISABLE);
	}
