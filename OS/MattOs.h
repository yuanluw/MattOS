#ifndef MATT_OS_H
#define MATT_OS_H

#include "stdint.h"
#include "list.h"
#include "port.h"




#ifndef NULL
#define NULL 0
#endif

#ifndef True
#define True 1
#endif

#ifndef False
#define False 0
#endif


#define MAX_PRIORITIES     10
#define HEAP_SIZE          30*1024





#define vPortPendSV_Handler  PendSV_Handler
#define vPortSystick_Handler SysTick_Handler





#endif

/**********************(C)  COPYRIGHT 2017 ��Զ�� *********************************************************/
