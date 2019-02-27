
#include "platform_config.h"
#include <EXT_Flash.h>
void Right_Ack_Len0(u8 *Buf,u16 Len);
#include "ErrCode.h"
 u8 Change_JZQAddr(u8 *Buf,u16 Len);
  u8 Add_One_CJQ(u8 *Buf,u16 Len);
/*******************************************************************************
* Function Name  : Computer_Concentrator_Protocol_Analysis
* Description    : ���������ֳֻ�������ָ�������Ӧ������
* Input          : Buf����λ�����͵����ݣ�Len�����ݳ���
* Output         : None
* Return         : None
*******************************************************************************/
s32  Computer_Concentrator_Protocol_Analysis(u8 *Buf,u16 Len)
{
    s32 JZQ_Check=0;
	if(memcmp(G_JZQ.ADDR,Buf+6,2)!=0)
		G_status.ST25=1;
    switch(Buf[28])	 //������
    {
    case 0x01: //����ֻ������ݣ��ɼ����У�
	case 0x02: //�����б������
	case 0x03://��ȡ��������ĳ̨�ɼ����еı�߰�װ��Ϣ
	case 0x04://��ȡ��������ĳ̨�ɼ����е��Ѱ�װ��ߵĵ�ַ�����Ϣ
	case 0x05://��ȡ��������ĳ̨�ɼ����е�δ��װ��ߵĵ�ַ�����Ϣ
	case 0x07://��ȡ��������ĳ̨�ɼ����е��Ѱ�װ��ߵĵ�ַ��ţ����ӱ�ţ���Ϣ
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
            Compose_Down_Frame_Format(Buf,Len);
        break;
    }
	case 0x06://��ȡ���������Ѱ�װ�ɼ����ĵ�ַ
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
	case 0x08://����ֻ���ʵʱ����
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
	case 0x0A://��ĳ���ɼ�������
	{
		JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
		else
		{
		  Buf[28]=0x86;	//?????������
		  Compose_Down_Frame_Format(Buf,Len);
		}			
		break;
	}
	case 0x0b://��ȡ��������ĳ̨�ɼ����е�һ̨���ָ�����ڵ���ʷ���ݼ�¼
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
	case 0x0C: //����������ַ 
	{
		Read_JZQAddr(Buf,Len);	
		break;				
	}
	case 0x0d: //��ȡ��������һ̨�ɼ����е�һ̨��ߵĵ�ǰ���������еģ�������Ϣ 
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
	case 0x0E:	 //hu ���۸��
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
//		 case 0x0F:	 //hu ��������
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
	case 0x0f: //��ȡ��������һ̨�ɼ����е�һ̨��ߵļ������ݼ۸� 
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
    case 0x22://���һ̨������
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
			Deal_One_Meter(Buf,Len,ADD_ONE);//?????Ҫ��Ҫ�·�
			Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
	case 0x23://��ɼ�����������Ӷ�̨�����ߵı��
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
	case 0x2A://���ļ�������һ̨�ɼ�����һ̨��ߵı�߱�ţ����������Ϣ����
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
			Deal_One_Meter(Buf,Len,ALT_ONE);//?????Ҫ��Ҫ�·�
			Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
	case 0x2B://ɾ����������һ̨�ɼ�����һ̨��ߵı�߱�ţ����������Ϣ����
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        else
        {
			Deal_One_Meter(Buf,Len,DEL_ONE);//?????Ҫ��Ҫ�·�
			Compose_Down_Frame_Format(Buf,Len);
        }
        break;
    }
	case 0x2c://ɾ����������һ̨�ɼ����ڵ�ȫ�������ߵı�ż���������Ϣ
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
	case 0x31://���ļ�������һ̨�ɼ����ĵ�ַ���
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
    case 0x32: //���ļ������������ȫ���ɼ����������ַ��ţ��������ڲɼ�������ͨѶ��ʽ��
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
	case 0x42://���ļ�������ַ
    {
        JZQ_Check=JZQ_Addr_Check(Buf);
        if(JZQ_Check==Failed)
            return Failed;
        Change_JZQAddr(Buf,Len);
        break;
    }
    case 0x43://	���һ̨�ɼ���
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
    case 0x44://	��Ӷ�̨�ɼ���
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
    case 0x45://	ɾ��һ̨�ɼ���
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
		 case 0x50:	 //hu д�۸��
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
		 case 0x51:	 //hu д������
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
	case 0x52:	 //hu д����
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
		case 0x53:	 //hu д������
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
		case 0x54:	 //hu д����Կ
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
	case 0x5A:	 //hu дʣ����
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
	    /*��ȡ������״̬*/
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
    /*��ȡ�ɼ���״̬*/
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
    /*����ģ�鲿��*/
    case 0x70:	  //������ģ�鳧�Һ�������Ϣ
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
    case 0x73:	//��ȡ���ڵ����
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
    case 0x74:	//��ȡ�ӽڵ����
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
    case 0x75:	// ��ȡ�����ӽڵ��ַ
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
    case 0x76:	// ���ӽڵ�״̬
    {
        Back_SubNot_Status(Buf,Len);
        break;
    }
    case 0x77:	// ��ȡ����������ģʽ
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
    case 0x78:	// ��ȡ���������ݽ�תʱ��
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
    case 0x7A:	// ������������	  ��������ģ�鳧�Һ�������Ϣ
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
    case 0x7B:	//����ӽڵ�
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
    case 0x7c:	//ɾ���ӽڵ�
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
    case 0x7e:	//�������ڵ��ַ
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
    case 0x7f:	//�������ڵ����
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
    case 0x80:	//�����ӽڵ����
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
    case 0x81:	//�����ӽڵ��ַ //������
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
	case 0x82:	//���ļ���������ģʽ
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
	case 0x83:	//���ļ���������ģʽ
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
    case 0x85:// ��������ͬ����Уʱ��
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
	case 0x90:	// �Կ�ʽ�ȱ����,ֱ���·�
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


