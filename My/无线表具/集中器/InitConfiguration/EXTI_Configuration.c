#include "platform_config.h"
/******************************************************************************
* 函数名       EXTI_Configuration
* 描述           配置EXTI线
* 输入           无 
* 输出           无
* 返回值       无
******************************************************************************/
void EXTI_Configuration(void)  
	{
		EXTI_InitTypeDef EXTI_InitStructure;
       /*将EXTI线0连接到PB8*/
       GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);
       /*配置EXTI线8上出现下降沿，则产生中断*/
       EXTI_InitStructure.EXTI_Line = EXTI_Line8;
       EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断请求
       EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//设置输入线路下降沿为中断请求
       EXTI_InitStructure.EXTI_LineCmd = ENABLE;
       EXTI_Init(&EXTI_InitStructure);
       //EXTI_GenerateSWInterrupt(EXTI_Line8);
    }

//时钟芯片中断响应
int flag_arm=0;
void EXTI9_5_IRQHandler(void)
{	        
	if(EXTI_GetITStatus(EXTI_Line8)!= RESET)
	{
    	EXTI_ClearITPendingBit(EXTI_Line8);
	}
	flag_arm++; 
}
