#include "stm32f4xx.h"


#include "MattOS.h"
#include "bspMalloc.h"

taskHandler_t idleHandler_t;

taskHandler_t *currentTask = NULL;
taskHandler_t *nextTask = NULL;

/* 来自list.c */
extern  LIST_TypeDef  taskDelayLists;
/* 来自list.c */
extern  LIST_TypeDef  taskReadyLists[MAX_PRIORITIES]; 


#define NVIC_INT_CTRL       0xE000ED04      // 中断控制及状态寄存器
#define NVIC_PENDSVSET      0x10000000      // 触发软件中断的值
#define NVIC_SYSPRI2        0xE000ED22      // 系统优先级寄存器
#define NVIC_PENDSV_PRI     0x000000FF      // 配置优先级

#define MEM32(addr)         *(volatile unsigned long *)(addr)
#define MEM8(addr)          *(volatile unsigned char *)(addr)


/**
 * @brief  空闲任务
 * @param  none 
 * @retval none
 */
void idleTask(void *para)
{
    
    while(1)
    {
        __nop();
    }
    
}


/**
 * @brief  开始运行
 * @param  none 
 * @retval none
 */
static void taskRun(void)
{
    __set_PSP(0);
    
    MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;   // 向NVIC_SYSPRI2写NVIC_PENDSV_PRI，设置其为最低优先级
    
    MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;    // 向NVIC_INT_CTRL写NVIC_PENDSVSET，用于PendSV
    
    
}

/**
 * @brief  进行任务切换
 * @param  none 
 * @retval none
 */
static void taskDispatch(void)
{
    
    MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;    // 向NVIC_INT_CTRL写NVIC_PENDSVSET，用于PendSV
}



/**
 * @brief  任务堆栈初始化
 * @param  none 
 * @retval none
 */
static void taskInit (taskHandler_t * task, void (*entry)(void *), void *param, uint32_t * stack)
{
    

    *(--stack) = (unsigned long)(1<<24);                // XPSR, 设置了Thumb模式，恢复到Thumb状态而非ARM状态运行
    *(--stack) = (unsigned long)entry;                  // 程序的入口地址
    *(--stack) = (unsigned long)0x14;                   // R14(LR), 任务不会通过return xxx结束自己，所以未用
    *(--stack) = (unsigned long)0x12;                   // R12, 未用
    *(--stack) = (unsigned long)0x3;                    // R3, 未用
    *(--stack) = (unsigned long)0x2;                    // R2, 未用
    *(--stack) = (unsigned long)0x1;                    // R1, 未用
    *(--stack) = (unsigned long)param;                  // R0 = param, 传给任务的入口函数
    *(--stack) = (unsigned long)0x11;                   // R11, 未用
    *(--stack) = (unsigned long)0x10;                   // R10, 未用
    *(--stack) = (unsigned long)0x9;                    // R9, 未用
    *(--stack) = (unsigned long)0x8;                    // R8, 未用
    *(--stack) = (unsigned long)0x7;                    // R7, 未用
    *(--stack) = (unsigned long)0x6;                    // R6, 未用
    *(--stack) = (unsigned long)0x5;                    // R5, 未用
    *(--stack) = (unsigned long)0x4;                    // R4, 未用

    task->stack = stack;                                // 保存最终的值
    
}


/**
 * @brief  任务调度
 * @param  none 
 * @retval none
 */
void taskSched(void)
{
    uint32_t s = taskEnterCritical();
    
    taskHandler_t *handle_t = getHighReadyTask();
    
    if(handle_t != currentTask)
    {
       nextTask = handle_t;
       handle_t->state = RUN_STATE;
       if(currentTask->delayTicks > 0)
       {
          currentTask->state = DELAY_STATE;
       }
       else
       {
          currentTask->state = READY_STATE;
       }
      
       taskDispatch(); 
    }

    taskExitCritical(s);
}


/**
 * @brief  进入临界区
 * @param  none 
 * @retval 保存当前状态(退出临界区时使用)
 */
uint32_t taskEnterCritical(void)
{
    uint32_t s = __get_PRIMASK();
    
    __disable_irq(); 
    
    return s;
}


/**
 * @brief  退出临界区
 * @param  status 进入临界区获取的参数 
 * @retval none
 */
void taskExitCritical(uint32_t status)
{
    __set_PRIMASK(status);
}




/**
 * @brief  创建任务
 * @param  handler_t  任务句柄
           fun        任务函数
           param      输入参数
           size       堆栈大小(字为单位)
           prio       任务优先级(0-MAX_PRIORITIES) 数字越小优先级越高
 * @retval True or False
 */
uint8_t createTask(taskHandler_t *handler_t,void(*fun)(void*),void *param,uint32_t size,uint8_t prio)
{
    //进入临界区
    uint32_t s = taskEnterCritical();
    uint32_t *stack;
    
    //空间太小
    if(size <= 20)
    {
        return False;
    }
    
    if(prio > MAX_PRIORITIES)
    {
        return False;
    }
    
    stack = myMalloc(size*4);
    if(stack == NULL)
    {
        return False;
    }
    
    taskInit(handler_t,fun,param,(stack+size));
    nodeInit(&handler_t->node);
    
    handler_t->prio = prio;
    handler_t->delayTicks = 0;
    handler_t->state = READY_STATE;
    
    //添加至就绪列表
    addTaskToReadyList(&handler_t->node);
    
    //进行一次任务调度
    if(currentTask != NULL)
    {
       taskSched(); 
    }
    
     //退出临界区
    taskExitCritical(s);
    
    return True;
    
}



/**
 * @brief  创建任务(任务堆栈自行分配)
 * @param  handler_t  任务句柄
           fun        任务函数
           param      输入参数
           stack      任务堆栈
           size       堆栈大小(字为单位)
           prio       任务优先级(0-MAX_PRIORITIES) 数字越小优先级越高
 * @retval True or False
 */
uint8_t createStaticTask(taskHandler_t *handler_t,void(*fun)(void*),void *param,uint32_t* stack,uint32_t size,uint8_t prio)
{
    //进入临界区
    uint32_t s = taskEnterCritical();


    if(prio > MAX_PRIORITIES)
    {
        return False;
    }
 
    taskInit(handler_t,fun,param,(stack+size));
    nodeInit(&handler_t->node);
    
    handler_t->prio = prio;
    handler_t->delayTicks = 0;
    handler_t->state = READY_STATE;
    
    //添加至就绪列表
    addTaskToReadyList(&handler_t->node);
    
    //进行一次任务调度
    if(currentTask != NULL)
    {
       taskSched(); 
    }
    
     //退出临界区
    taskExitCritical(s);
    
    return True;
    
}



/**
 * @brief  删除任务
 * @param  none 
 * @retval True or False
 */
uint8_t removeTask(taskHandler_t *handler_t)
{
    
    //进入临界区
    uint32_t s = taskEnterCritical();
    uint8_t flag = False;
    
    if(handler_t->state == DELAY_STATE)
    {
        //从延时列表中移除
        removeTaskToDelayList(&(handler_t->node)); 
 
        handler_t->delayTicks = 0;
    }
    else if(handler_t->state == SUSPEND_STATE)
    {
        removeTaskToSuspendList(&(handler_t->node)); 
    }
    else if(handler_t->state == READY_STATE || handler_t->state == RUN_STATE)
    {
        //从就绪列表中移除
        removeTaskToReadyList(&(handler_t->node)); 
        
        if(handler_t->state == RUN_STATE)
            flag = True;
    }

    
    if(flag == True)
    {
       //启动一次任务调度
        taskSched(); 
    }
    
    handler_t->state = NONE_STATE;
    
      //退出临界区
    taskExitCritical(s);
   
    return True;
}


/**
 * @brief  获取当前运行任务
 * @param  none 
 * @retval 任务句柄
 */
taskHandler_t* getCurrentTask(void)
{
    return currentTask;
}


/**
 * @brief  获取任务运行状态
 * @param  none 
 * @retval 运行状态
 */
TASKSTATE_ENUM getTaskState(taskHandler_t *handler_t)
{
    return handler_t->state;
}


/**
 * @brief  延时函数
 * @param  none 
 * @retval none
 */
void taskDelay(uint32_t ticks)
{
    uint32_t s = taskEnterCritical();
    currentTask->delayTicks = ticks;
    currentTask->state = DELAY_STATE;
    //从就绪列表中移除
    removeTaskToReadyList(&(currentTask->node));
    //添加至延时列表
    addTaskToDelayList(&(currentTask->node));
    //启动一次任务调度
    taskSched();
    
    taskExitCritical(s);
}


/**
 * @brief  挂起一个任务
 * @param  none 
 * @retval True or False
 */
uint8_t taskSuspend(taskHandler_t *handler_t)
{
    
    uint32_t s = taskEnterCritical();
    
    if(handler_t->state == SUSPEND_STATE)
    {
        taskExitCritical(s);
        return False;
    }
    
    if(handler_t->state == READY_STATE || handler_t->state == RUN_STATE)
    {
        removeTaskToReadyList(&(handler_t->node));
    }
    else if(handler_t->state == DELAY_STATE)
    {
        removeTaskToDelayList(&(handler_t->node));
    }
    
    //添加至挂起队列
    addTaskToSuspendList(&(handler_t->node));
    
    if(handler_t->state == RUN_STATE)
    {
        //进行一次调度
        taskSched(); 
    }
    
    handler_t->state = SUSPEND_STATE;
    
    taskExitCritical(s);
    
    return True;
}

/**
 * @brief  恢复一个任务
 * @param  none 
 * @retval True or False
 */
uint8_t taskResume(taskHandler_t *handler_t)
{
    uint32_t s = taskEnterCritical();
    
    //此任务没有被挂起
    if(handler_t->state != SUSPEND_STATE)
    {
       taskExitCritical(s);
    
        return False; 
    }
    
    removeTaskToSuspendList(&(handler_t->node));
    
    if(handler_t->delayTicks > 0)
    {
        //添加至延时列表
        addTaskToDelayList(&(handler_t->node)); 
    }
    else
    {
       //添加至就绪列表
        addTaskToReadyList(&(handler_t->node));  
    }
    
    //进行一次调度
    taskSched(); 
    
    taskExitCritical(s);
    
    return True;
}



/**
 * @brief  延时处理
 * @param  none 
 * @retval none
 */
void taskSystemTickHandler(void)
{
    tNode *node;
    uint32_t s = taskEnterCritical();
    
    for(node=taskDelayLists.headNode.nextNode; node != &(taskDelayLists.headNode) ; node = node->nextNode)
    {
        taskHandler_t *task = (taskHandler_t*)((uint32_t)node - (uint32_t)(sizeof(taskHandler_t)-sizeof(tNode)));
        if(--task->delayTicks == 0)
        {
            task->state = READY_STATE;
             //从延时列表中移除
            removeTaskToDelayList(node);
            //添加至就绪列表列表
            addTaskToReadyList((node));
            break;
        }

    }
    
    taskExitCritical(s);
    //进行一次调度
    taskSched();
}



/**
 * @brief  开启任务调度
 * @param  none 
 * @retval none
 */
void startTaskSched(void)
{
   
    
    //创建空闲任务
    createTask(&idleHandler_t,idleTask,NULL,50,MAX_PRIORITIES-1);
    
    nextTask = (taskHandler_t*)getHighReadyTask();
    
    
    //此函数永远不会返回
    taskRun();
    
}





/**
 * @brief  PendSV_Handler(任务切换)
 * @param  none 
 * @retval none
 */
__asm  void  vPortPendSV_Handler()
{
    
    IMPORT currentTask
    IMPORT nextTask
    
    MRS R0, PSP
    CBZ R0, vPortPendSV_Handler_S
    
    STMDB R0!, {R4-R11}
    
    LDR  R1, = currentTask
    LDR  R1, [R1]
    STR  R0, [R1]
    
    
vPortPendSV_Handler_S
    
    LDR     R0,  = currentTask
    LDR     R1,  = nextTask
    LDR     R2,  [R1]
    STR     R2,  [R0]
    
    LDR     R0,  [R2]
    LDMIA   R0!, {R4-R11}
    
    MSR     PSP, R0
    ORR     LR,LR,#0x04
    BX      LR
        
 
}


/**
 * @brief  滴答计时器中断 
 * @param  无
 * @retval 无
 */
void vPortSystick_Handler(void)
{
	taskSystemTickHandler();
}


/**********************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/
