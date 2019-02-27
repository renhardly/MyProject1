#include "stm32f10x_can.h"
#include "platform_config.h"

void can_send(void)
  {
  CanTxMsg TxMessage;
  /* transmit 1 message (����һ����Ϣ)*/
  TxMessage.StdId=0x00;// �趨��׼��ʶ��
  TxMessage.ExtId=0x1234;// �趨��չ��ʶ��
  TxMessage.IDE=CAN_ID_EXT;// �趨��Ϣ��ʶ��������
  TxMessage.RTR=CAN_RTR_DATA;// �趨��������Ϣ��֡����
  TxMessage.DLC=2;//�趨��������Ϣ��֡����
  TxMessage.Data[0]=0xDE;
  TxMessage.Data[1]=0xCA;
  CAN_Transmit(&TxMessage);//��ʼһ����Ϣ�Ĵ���

  

  /* �����жϴ��� */
  CAN_ITConfig(CAN_IT_FMP0, DISABLE);
  }
