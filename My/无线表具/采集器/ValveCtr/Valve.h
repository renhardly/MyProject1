/******************** 山东三龙采集器项目 ********************
* File Name          : 
* Author             :胡海强
* Date First Issued  : 
* Description        : 开关阀，机电同步数据
********************************************************************************/
#ifndef __VALVE_H
#define __VALVE_H
void Switch_Valve(u8 *Buf,u16 Len);
void Write_Ele_and_Mechan_Syn_Data(u8 *Buf,u16 Len);
//void Up_Meter_Data(u8 *DataBuf,u16 DataLen);
void Up_Meter_Auto_Data(u8 *DataBuf,u16 DataLen);
#endif

