
#include "platform_config.h"
#include <interflash.h>
#define Success     			    	((u8)0x00)
#define Failed			     			    	-1
#define NotFindMeter     				((u8)0x01)
#define DoubleMeter     				((u8)0x02)
#define DoubleElectronicMeterAddr 	    ((u8)0x03)
#define MeterCodeIsAllZero              ((u8)0x04)
#define WriteInterFlashErr              ((u8)0x05)
#define WriteEXTFlashErr  				((u8)0x06)
#define IllegalCJQAddr  				((u8)0x07)
#define IllegalDataFormat			    ((u8)0x08)
#define StorageSpaceOverFlow  			((u8)0x09)
#define ReadEEPROMFailure  				((u8)0x0A)
#define WriteEEPROMFailure  			((u8)0x0B)



#define NotFindCJQ  					((u8)0x10)
#define DoubleCJQ   					((u8)0x11)
#define CJQAddrErr  					((u8)0x12)
#define CJQNetworkAddrErr  				((u8)0x13)


#define NotFindJZQ    					((u8)0x16)
#define DoubleJZQ   				    ((u8)0x17)
#define JZQAddrErr                      ((u8)0x18)
#define JZQNetworkAddrErr  				((u8)0x19)


#define IllegalControlCode              ((u8)0x20)
#define DataErr						    ((u8)0x21)
#define MeterTypeErr				    ((u8)0x22)
#define CheckCodeErr                    ((u8)0x23)
#define MallocERR                       ((u8)0x24)
#define Radio_Read_Err                    ((u8)0x25)
#define CmdErr                 			   ((u8)0x26)
