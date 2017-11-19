#include "stm32f4xx.h"

#include "port.h"

#include "MattOS.h"
#include "bspMalloc.h"
#include "list.h"



tTaskHandler_t *currentTask = NULL;
tTaskHandler_t *nextTask = NULL;


#define NVIC_INT_CTRL       0xE000ED04      // 中断控制及状态寄存器
#define NVIC_PENDSVSET      0x10000000      // 触发软件中断的值
#define NVIC_SYSPRI2        0xE000ED22      // 系统优先级寄存器
#define NVIC_PENDSV_PRI     0x000000FF      // 配置优先级

#define MEM32(addr)         *(volatile unsigned long *)(addr)
#define MEM8(addr)          *(volatile unsigned char *)(addr)



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
 * @brief  开始运行
 * @param  none 
 * @retval none
 */
void tTaskRun(void)
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
void tTaskDispatch(void)
{
    
    MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;    // 向NVIC_INT_CTRL写NVIC_PENDSVSET，用于PendSV
}



/**
 * @brief  任务堆栈初始化
 * @param  none 
 * @retval none
 */
static void tTaskInit (tTaskHandler_t * task, void (*entry)(void *), void *param, uint32_t * stack)
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
 * @brief  进入临界区
 * @param  none 
 * @retval none
 */
uint32_t tTaskEnterCritical(void)
{
    uint32_t s = __get_PRIMASK();
    
    __disable_irq(); 
    
    return s;
}


/**
 * @brief  退出临界区
 * @param  none 
 * @retval none
 */
void tTaskExitCritical(uint32_t status)
{
    __set_PRIMASK(status);
}








/**
 * @brief  创建任务
 * @param  none 
 * @retval none
 */
uint8_t createTask(tTaskHandler_t *handler_t,void(*fun)(void*),void *param,uint32_t size,uint8_t prio)
{
    //进入临界区
    uint32_t s = tTaskEnterCritical();
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
    
    stack = myMalloc(size);
    if(stack == NULL)
    {
        return False;
    }
    
    tTaskInit(handler_t,fun,param,stack);
    
    handler_t->prio = prio;
    handler_t->delayTicks = 0;
    handler_t->state = READY_STATE;
    
    //添加至就绪列表
    addTaskToReadyList(handler_t);
    
    
     //退出临界区
    tTaskExitCritical(s);
    
}




void tTaskDelay(uint32_t ticks)
{
    uint32_t s = tTaskEnterCritical();
    currentTask->delayTicks = ticks;
    tTaskExitCritical(s);
    
    
}


/**********************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/
