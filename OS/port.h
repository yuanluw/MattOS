#ifndef PORT_H
#define PORT_H



#include "stdint.h"

typedef enum
{
    READY_STATE,
    RUN_STATE,
    SUSPEND_STATE
}TASKSTATE_ENUM;



typedef uint32_t tTaskStack;

typedef struct{

    //堆栈地址
    tTaskStack *stack;
    //延时计数
    uint32_t  delayTicks;
    //优先级
    uint32_t prio;
    //运行状态
    TASKSTATE_ENUM state;

}tTaskHandler_t;





void        tTaskDispatch(void);
void        tTaskRun(void);
void        tTaskExitCritical(uint32_t status);
uint32_t    tTaskEnterCritical(void);

#endif



/**********************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/
