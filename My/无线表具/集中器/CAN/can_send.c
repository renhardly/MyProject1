#include "stm32f10x_can.h"
#include "platform_config.h"

void can_send(void)
  {
  CanTxMsg TxMessage;
  /* transmit 1 message (生成一个信息)*/
  TxMessage.StdId=0x00;// 设定标准标识符
  TxMessage.ExtId=0x1234;// 设定扩展标识符
  TxMessage.IDE=CAN_ID_EXT;// 设定消息标识符的类型
  TxMessage.RTR=CAN_RTR_DATA;// 设定待传输消息的帧类型
  TxMessage.DLC=2;//设定待传输消息的帧长度
  TxMessage.Data[0]=0xDE;
  TxMessage.Data[1]=0xCA;
  CAN_Transmit(&TxMessage);//开始一个消息的传输

  

  /* 禁用中断处理 */
  CAN_ITConfig(CAN_IT_FMP0, DISABLE);
  }
