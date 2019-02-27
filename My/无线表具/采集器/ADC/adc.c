#include "platform_config.h"
#define ADC1_DR_Address  ((u32)0x4001244C)
u16 G_AD_Value[7];
void ADC_Status_Init(void)
{

    ADC_InitTypeDef ADC_InitStruct;
//    GPIO_InitTypeDef GPIO_InitStruct;
    DMA_InitTypeDef DMA_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned long)G_AD_Value;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;

    DMA_InitStructure.DMA_BufferSize = 7;//BufferSize=2，因为ADC转换序列有7个通道
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环模式开启，Buffer写满后，自动回到初始地址开始传输
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel1, ENABLE);//配置完成后，启动DMA通道
//########################################################################
//  对应引脚GPIO初始
//########################################################################

//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
//	GPIO_Init(GPIOC,&GPIO_InitStruct); 			   //输入电压检测，14通道；


    ADC_DeInit(ADC1);
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent; //设置ADC的工作模式，此处设置为独立工作模式
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//ADC数据右对齐
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;//设置为DISABLE,ADC工作在单次模式。ENABLE，工作在连续模式
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//定义触发方式，此处为软件触发
    ADC_InitStruct.ADC_NbrOfChannel = 7;         //设置进行规则转换的ADC通道数目。此处为7个通道。
    ADC_InitStruct.ADC_ScanConvMode = ENABLE;//ADC工作在多通道模式还是单通道模式
    ADC_Init(ADC1, &ADC_InitStruct);
//########################################################################
//  设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
//########################################################################

    //ADC_RegularChannelConfig(ADC1,ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1,ADC_Channel_16,2, ADC_SampleTime_55Cycles5);//温度



    ADC_TempSensorVrefintCmd(ENABLE);
    //ADC1 使能
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1,ENABLE);
    //重置指定的ADC1的校准寄存器
    ADC_ResetCalibration(ADC1);
    //获取ADC1重置校准寄存器的状态，只到校准寄存器重设完成。
    while(ADC_GetResetCalibrationStatus(ADC1));
    //开始指定ADC的校准状态
    ADC_StartCalibration(ADC1);
    //获取指定ADC的校准程序，只到校准完成。
    while(ADC_GetCalibrationStatus(ADC1));
    ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //使能ADC1的软件转换启动功能

}
