#ifndef _ONENET_H_
#define _ONENET_H_

#include "sys.h"

_Bool OneNet_Ping(void);

_Bool OneNet_DevLink(void);

void OneNet_SendData(void);

void OneNet_RevPro(unsigned char *cmd);

#endif