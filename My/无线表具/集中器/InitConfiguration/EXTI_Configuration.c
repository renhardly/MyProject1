#include "platform_config.h"
/******************************************************************************
* ������       EXTI_Configuration
* ����           ����EXTI��
* ����           �� 
* ���           ��
* ����ֵ       ��
******************************************************************************/
void EXTI_Configuration(void)  
	{
		EXTI_InitTypeDef EXTI_InitStructure;
       /*��EXTI��0���ӵ�PB8*/
       GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);
       /*����EXTI��8�ϳ����½��أ�������ж�*/
       EXTI_InitStructure.EXTI_Line = EXTI_Line8;
       EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�����
       EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//����������·�½���Ϊ�ж�����
       EXTI_InitStructure.EXTI_LineCmd = ENABLE;
       EXTI_Init(&EXTI_InitStructure);
       //EXTI_GenerateSWInterrupt(EXTI_Line8);
    }

//ʱ��оƬ�ж���Ӧ
int flag_arm=0;
void EXTI9_5_IRQHandler(void)
{	        
	if(EXTI_GetITStatus(EXTI_Line8)!= RESET)
	{
    	EXTI_ClearITPendingBit(EXTI_Line8);
	}
	flag_arm++; 
}
