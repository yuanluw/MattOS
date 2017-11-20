#ifndef PORT_H
#define PORT_H



#include "MattOS.h"






void        taskExitCritical(uint32_t status);
uint32_t    taskEnterCritical(void);
uint8_t     createTask(taskHandler_t *handler_t,void(*fun)(void*),void *param,uint32_t size,uint8_t prio);
uint8_t     createStaticTask(taskHandler_t *handler_t,void(*fun)(void*),void *param,uint32_t* stack,uint32_t size,uint8_t prio);
void        startTaskSched(void);
void        taskDelay(uint32_t ticks);
#endif



/**********************(C)  COPYRIGHT 2017 Œ‚‘∂„Ú *********************************************************/
