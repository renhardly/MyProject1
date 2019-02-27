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

    DMA_InitStructure.DMA_BufferSize = 7;//BufferSize=2����ΪADCת��������7��ͨ��
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ������Bufferд�����Զ��ص���ʼ��ַ��ʼ����
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel1, ENABLE);//������ɺ�����DMAͨ��
//########################################################################
//  ��Ӧ����GPIO��ʼ
//########################################################################

//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
//	GPIO_Init(GPIOC,&GPIO_InitStruct); 			   //�����ѹ��⣬14ͨ����


    ADC_DeInit(ADC1);
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent; //����ADC�Ĺ���ģʽ���˴�����Ϊ��������ģʽ
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//ADC�����Ҷ���
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;//����ΪDISABLE,ADC�����ڵ���ģʽ��ENABLE������������ģʽ
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//���崥����ʽ���˴�Ϊ�������
    ADC_InitStruct.ADC_NbrOfChannel = 7;         //���ý��й���ת����ADCͨ����Ŀ���˴�Ϊ7��ͨ����
    ADC_InitStruct.ADC_ScanConvMode = ENABLE;//ADC�����ڶ�ͨ��ģʽ���ǵ�ͨ��ģʽ
    ADC_Init(ADC1, &ADC_InitStruct);
//########################################################################
//  ����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
//########################################################################

    //ADC_RegularChannelConfig(ADC1,ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1,ADC_Channel_16,2, ADC_SampleTime_55Cycles5);//�¶�



    ADC_TempSensorVrefintCmd(ENABLE);
    //ADC1 ʹ��
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1,ENABLE);
    //����ָ����ADC1��У׼�Ĵ���
    ADC_ResetCalibration(ADC1);
    //��ȡADC1����У׼�Ĵ�����״̬��ֻ��У׼�Ĵ���������ɡ�
    while(ADC_GetResetCalibrationStatus(ADC1));
    //��ʼָ��ADC��У׼״̬
    ADC_StartCalibration(ADC1);
    //��ȡָ��ADC��У׼����ֻ��У׼��ɡ�
    while(ADC_GetCalibrationStatus(ADC1));
    ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ʹ��ADC1�����ת����������

}
