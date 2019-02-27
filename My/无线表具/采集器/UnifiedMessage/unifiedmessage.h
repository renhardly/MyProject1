#include "EXT_Flash.h"
#ifndef PAGESIZE
#define PAGESIZE  ((u16)0x800)
extern u8 G_PageTmp[PAGESIZE];
#endif







void  CopyInterTOEXT(void);
u8  UnifiedMessage(void);
