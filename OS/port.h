#ifndef PORT_H
#define PORT_H



#include "MattOS.h"






void             taskExitCritical(uint32_t status);
uint32_t         taskEnterCritical(void);
uint8_t          createTask(taskHandler_t *handler_t,void(*fun)(void*),void *param,uint32_t size,uint8_t prio);
uint8_t          createStaticTask(taskHandler_t *handler_t,void(*fun)(void*),void *param,uint32_t* stack,uint32_t size,uint8_t prio);
void             startTaskSched(void);
void             taskDelay(uint32_t ticks);
uint8_t          removeTask(taskHandler_t *handler_t);
taskHandler_t*   getCurrentTask(void);
uint8_t          taskSuspend(taskHandler_t *handler_t);
uint8_t          taskResume(taskHandler_t *handler_t);
#endif



/**********************(C)  COPYRIGHT 2017 Œ‚‘∂„Ú *********************************************************/
