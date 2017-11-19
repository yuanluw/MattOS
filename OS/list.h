#ifndef LIST_H
#define LIST_H


#include "port.h"
#include "MattOS.h"


typedef struct _tNode
{
    struct _tNode *preNode;
    struct _tNode *nextNode;
    tTaskHandler_t *owner;
}tNode;


typedef struct LIST_T
{
    tNode headNode;
    //Ω·µ„ ˝
    uint32_t nodeCount;
    
    uint8_t index;
}LIST_TypeDef;





uint8_t addTaskToReadyList(tTaskHandler_t *task);
uint8_t removeTaskToReadyList(tTaskHandler_t *task);
tTaskHandler_t* getHighReadyTask(void);
void taskReadyListInit(void);






#endif


/**********************(C)  COPYRIGHT 2017 Œ‚‘∂„Ú *********************************************************/
