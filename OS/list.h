#ifndef LIST_H
#define LIST_H


#include "MattOS.h"

typedef enum
{
    NONE_STATE,
    READY_STATE,
    RUN_STATE,
    DELAY_STATE,
    SUSPEND_STATE
}TASKSTATE_ENUM;



typedef uint32_t tTaskStack;



typedef struct _tNode
{
    struct _tNode *preNode;
    struct _tNode *nextNode;
}tNode;


typedef struct{

    //堆栈地址
    tTaskStack *stack;
    //延时计数
    uint32_t  delayTicks;
    //优先级
    uint32_t prio;
    //运行状态
    TASKSTATE_ENUM state;
    //任务节点
    tNode node;
}taskHandler_t;


typedef struct LIST_T
{
    tNode headNode;
    //结点数
    uint32_t nodeCount;
    
    uint8_t index;
}LIST_TypeDef;





void nodeInit(tNode *node);
void listInit(LIST_TypeDef *list);
uint8_t addTaskToReadyList(tNode *node);
uint8_t removeTaskToReadyList(tNode *node);
taskHandler_t* getHighReadyTask(void);
void taskReadyListInit(void);

uint8_t addTaskToDelayList(tNode *node);
uint8_t removeTaskToDelayList(tNode *node);

uint8_t addTaskToSuspendList(tNode *node);
uint8_t removeTaskToSuspendList(tNode *node);



#endif


/**********************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/
