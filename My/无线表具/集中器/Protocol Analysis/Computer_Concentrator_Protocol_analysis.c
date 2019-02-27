
#include "platform_config.h"
#include <EXT_Flash.h>
void Right_Ack_Len0(u8 *Buf,u16 Len);
#include "ErrCode.h"
 u8 Change_JZQAddr(u8 *Buf,u16 Len);
  u8 Add_One_CJQ(u8 *Buf,u16 Len);
/*******************************************************************************
* Function Name  : Computer_Concentrator_Protocol_Analysis
* Description    : 集中器、手持机发来的指令，解析相应的命令
* Input          : Buf：上位机发送的数据，Len：数据长度
* Output         : None
* Return         : None
*******************************************************************************/
s32  Computer_Concentrator_Protocol_Analysis(u8 *Buf,u16 Len)
{
    s32 JZQ_Check=0;
	if(memcmp(G_JZQ.ADDR,Buf+6,2)!=0)
		G_status.ST25=1;
    switch(Buf[28])	 //控制码
    {
    case 0x01: //读单只表具数据（采集器中）
	case 0x02: //读所有表具数据
	case 0x03://读取集中器中某台采集器中的表具安装信息
	case 0x04://读取集中器中某台采集器中的已安装表具的地址编号信息
	case 0x05://读取集中器中某台采集器中的未安装表具的地址编号信息
	case 0x07://读取集中器中某台采集器中的已安装表具的地址编号（电子表号）信息
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
            Compose_Down_Frame_Format(Buf,Len);
        break;
    }
	case 0x06://读取集中器中已安装采集器的地址
	{
		JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
		else
		{
		  Read_Installed_CJQAddr(Buf,Len);
		}			
		break;
	}
	case 0x08://读单只表具实时数据
	{
		JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
		else
		{
		  Compose_Down_Frame_Format(Buf,Len);
		}			
		break;
	}
	case 0x0A://读某个采集器数据
	{
		JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
		else
		{
		  Buf[28]=0x86;	//?????需讨论
		  Compose_Down_Frame_Format(Buf,Len);
		}			
		break;
	}
	case 0x0b://读取集中器中某台采集器中的一台表具指定日期的历史数据记录
	{
		JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
		else
		{	
			Return_Meter_History_Data(Buf,Len);
		}			
		break;
	}
	case 0x0C: //读集中器地址 
	{
		Read_JZQAddr(Buf,Len);	
		break;				
	}
	case 0x0d: //读取集中器中一台采集器中的一台表具的当前（集中器中的）数据信息 
	{
		JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
		else
		{
		  Read_JZQ_Flash_Data(Buf,Len);
		}
		break;			
	}
	case 0x0E:	 //hu 读价格表
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Buf[28]=0x0A;
            Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
//		 case 0x0F:	 //hu 读购入金额
//    {
//        JZQ_Check=JZQ_Addr_Check(Buf);
//        if(JZQ_Check==Failed)
//		  	return Failed;
//        else
//        {
//            Buf[28]=0x0A;
//            Compose_Down_Frame_Format(Buf,Len);
//        }
//        break;
//    }
	case 0x0f: //读取集中器中一台采集器中的一台表具的计量数据价格。 
	{
		JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
		else
		{
		  Buf[28]=0x0b;
		  Compose_Down_Frame_Format(Buf,Len);
		}
		break;			
	}
	case 0x10:
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Compose_Down_Frame_Format(Buf,Len);
            JZQ_RTC_Set(Buf,Len);
        }
        break;
    }
	case 0x11:
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {          
            if(DEL_JZQ_Meter_Data(Buf,Len)!=-1)
				Compose_Down_Frame_Format(Buf,Len);
			else
				Err_Ack(Buf,WriteEXTFlashErr);
        }
        break;
    }
    case 0x21:
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }	
    case 0x22://添加一台管理表具
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
			Deal_One_Meter(Buf,Len,ADD_ONE);//?????要不要下发
			Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
	case 0x23://向采集器内批量添加多台管理表具的编号
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
				Add_Multi_Meter(Buf,Len);
				Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
	case 0x2A://更改集中器中一台采集器内一台表具的表具编号，表具数据信息不变
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
			Deal_One_Meter(Buf,Len,ALT_ONE);//?????要不要下发
			Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
	case 0x2B://删除集中器中一台采集器内一台表具的表具编号，表具数据信息不变
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
			Deal_One_Meter(Buf,Len,DEL_ONE);//?????要不要下发
			Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
	case 0x2c://删除集中器中一台采集器内的全部管理表具的编号及其数据信息
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
			DEL_CJQ_Meters(Buf,Len);
			Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
	case 0x31://更改集中器中一台采集器的地址编号
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
			Deal_One_CJQ(Buf,Len,ALT_ONE);
			Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
    case 0x32: //更改集中器及其管理全部采集器的网络地址编号，仅适用于采集器组网通讯方式。
    {
		JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        JZQ_NetAddr_Alter(Buf,Len);
        Compose_Down_Frame_Format(Buf,Len);
        break;
    }
    case 0x33: 
    {
		JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;

		Deal_One_CJQ(Buf,Len,INI_ONE);
        Compose_Down_Frame_Format(Buf,Len);
        break;
    }
	case 0x41:
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
		  	return Failed;
        Init_JZQ(Buf,Len);
        break;
    }
	case 0x42://更改集中器地址
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        Change_JZQAddr(Buf,Len);
        break;
    }
    case 0x43://	添加一台采集器
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Deal_One_CJQ(Buf,Len,ADD_ONE);
        }
        break;
    }
    case 0x44://	添加多台采集器
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Add_Multi_CJQ(Buf,Len);
        }
        break;
    }
    case 0x45://	删除一台采集器
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Deal_One_CJQ(Buf,Len,DEL_ONE);
        }
        break;
    }
		 case 0x50:	 //hu 写价格表
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Buf[28]=0x40;
            Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
		 case 0x51:	 //hu 写结算日
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Buf[28]=0x41;
            Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
	case 0x52:	 //hu 写抄表
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Buf[28]=0x42;
            Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
		case 0x53:	 //hu 写购入金额
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Buf[28]=0x43;
            Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
		case 0x54:	 //hu 写新密钥
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Buf[28]=0x44;
            Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
	    case 0x55:
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Buf[28]=0x45;
            Compose_Down_Frame_Format(Buf,Len);

        }
        break;
    }
	    case 0x59:
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Buf[28]=0x49;
            Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
	case 0x5A:	 //hu 写剩余金额
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Buf[28]=0x4A;
            Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
	    /*读取集中器状态*/
    case 0x60:
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            JZQ_Status_Read(Buf,Len);
        }
        break;
    }
    /*读取采集器状态*/
    case 0x61:
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
    /*无线模块部分*/
    case 0x70:	  //读无线模块厂家和组网信息
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            JZQ_Wireless_NetType_Read(Buf,Len);
        }
        break;
    }
    case 0x71:
    {
        JZQ_NetAddr_Read(Buf,Len);
        break;
    }
    case 0x72:
    {
        Back_Wireless_Addr(Buf,Len);
        break;
    }
    case 0x73:	//读取主节点参数
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Back_Wireless_Parameter(Buf,Len);
        }
        break;
    }
    case 0x74:	//读取子节点参数
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
    case 0x75:	// 读取所有子节点地址
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Back_Wireless_Addr_ALL(Buf,Len);
        }
        break;
    }
    case 0x76:	// 读子节点状态
    {
        Back_SubNot_Status(Buf,Len);
        break;
    }
    case 0x77:	// 读取集中器工作模式
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Back_JZQ_Work_Mode(Buf,Len);
        }
        break;
    }
    case 0x78:	// 读取集中器数据结转时间
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Back_JZQ_JieZhuan_Time(Buf,Len);
        }
        break;
    }
	case 0x79:
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            JZQ_RTC_Read(Buf,Len);
        }
        break;
    }
    case 0x7A:	// 更改网络类型	  更改无线模块厂家和组网信息
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            JZQ_Wireless_NetType_Write(Buf,Len);
        }
        break;
    }
    case 0x7B:	//添加子节点
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            JZQ_ADD_Subnot(Buf,Len);
        }
        break;
    }
    case 0x7c:	//删除子节点
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            JZQ_DEL_Subnot(Buf,Len);
        }
        break;
    }
    case 0x7d:
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
		  	return Failed;
        Change_JZQAddr(Buf,Len);
        break;
    }
    case 0x7e:	//更改主节点地址
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            JZQ_MainNotAddr_Write(Buf,Len);
        }
        break;
    }
    case 0x7f:	//更改主节点参数
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Set_Wireless_Parameter(Buf,Len);
        }
        break;
    }
    case 0x80:	//更改子节点参数
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
    case 0x81:	//更改子节点地址 //需讨论
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
	case 0x82:	//更改集中器工作模式
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            ChangeWorkingMode(Buf,Len);
        }
        break;
    }
	case 0x83:	//更改集中器工作模式
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            ChangeJieZhuan_Time(Buf,Len);
        }
        break;
    }
    case 0x84:
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            JZQ_RTC_Set(Buf,Len);
        }
        break;

    }
    case 0x85:// 设置网络同步（校时）
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
            JZQ_RTC_Set_noret(Buf,Len);
            Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
	case 0x90:	// 对卡式热表操作,直接下发
	case 0x91:
	case 0x92:
	case 0x93:
	case 0x94:
	{
		JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
		else
		{
		  Compose_Down_Frame_Format(Buf,Len);
		}			
		break;		
	}
    default:
        break;
    }
    return Success;
}


