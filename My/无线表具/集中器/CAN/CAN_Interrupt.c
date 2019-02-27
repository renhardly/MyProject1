#include "stm32f10x_lib.h"
#include "platform_config.h"

/*******************************************************************************
* Function Name  : CAN_Interrupt
* Description    : Configures the CAN, transmit and receive using interrupt.
* Input          : None
* Output         : None
* Return         : PASSED if the reception is well done, FAILED in other case
*******************************************************************************/
void CAN_Interrupt(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;

  /* CAN register init */
  CAN_DeInit();
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM=DISABLE;//��ֹʱ�䴥��ͨ��ģʽ
  CAN_InitStructure.CAN_ABOM=DISABLE;//�����CAN_MCR�Ĵ�����INRQλ������1�����0��һ��Ӳ����� 
                                     //��128��11λ����������λ�����˳�����״̬
  CAN_InitStructure.CAN_AWUM=DISABLE;//˯��ģʽͨ�����CAN_MCR�Ĵ�����SLEEPλ����������� 
  CAN_InitStructure.CAN_NART=DISABLE;//DISABLE;CAN����ֻ������1�Σ����ܷ��͵Ľ����Σ��ɹ���������ٲö�ʧ��
  CAN_InitStructure.CAN_RFLM=DISABLE;//�ڽ������ʱFIFOδ��������������FIFO�ı���δ����������һ���յ��ı��ĻḲ��ԭ�� 
                                                            //�ı��� 
  CAN_InitStructure.CAN_TXFP=DISABLE;//����FIFO���ȼ��ɱ��ĵı�ʶ�������� 
  CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;//CANӲ�������ڻ���ģʽ
// CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;//CANӲ������������ģʽ
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;//����ͬ����Ծ���1��ʱ�䵥λ
  CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;//ʱ���1Ϊ8��ʱ�䵥λ
  CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;//ʱ���2Ϊ7��ʱ�䵥λ
  CAN_InitStructure.CAN_Prescaler=4;//(pclk1/((1+8+7)*49)) = 36Mhz/16/4 = ?Kbits�趨��һ��ʱ�䵥λ�ĳ���9
  CAN_Init(&CAN_InitStructure);

 /* CAN filter init ��������ʼ��*/ 
  CAN_FilterInitStructure.CAN_FilterNumber=1;//ָ���˴���ʼ���Ĺ�����1
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//ָ���˹�����������ʼ������ģʽΪ��ʶ������λģʽ 
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//�����˹�����λ��1��32λ������
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;//�����趨��������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ���� 
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;//�����趨��������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ���
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ��
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;//�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ���
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;//�趨��ָ���������FIFO0
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//ʹ�ܹ�����
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* CAN FIFO0 message pending interrupt enable */ 
  CAN_ITConfig(CAN_IT_FMP0, ENABLE);//ʹ��ָ����CAN�ж�
}

void can_send(void)
  {
  CanTxMsg TxMessage;
  CAN_LED_ON();
  /* transmit 1 message (����һ����Ϣ)*/
  TxMessage.StdId=0x00;// �趨��׼��ʶ��
  TxMessage.ExtId=0x1234;// �趨��չ��ʶ��
  TxMessage.IDE=CAN_ID_EXT;// �趨��Ϣ��ʶ��������
  TxMessage.RTR=CAN_RTR_DATA;// �趨��������Ϣ��֡����
  TxMessage.DLC=2;//�趨��������Ϣ��֡����
  TxMessage.Data[0]=0xDE;
  TxMessage.Data[1]=0xCA;
  CAN_Transmit(&TxMessage);//��ʼһ����Ϣ�Ĵ���
	CAN_LED_OFF();
  

  /* �����жϴ��� */
//  CAN_ITConfig(CAN_IT_FMP0, DISABLE);
  }

