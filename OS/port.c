#include "stm32f4xx.h"


#include "MattOS.h"
#include "bspMalloc.h"

taskHandler_t idleHandler_t;

taskHandler_t *currentTask = NULL;
taskHandler_t *nextTask = NULL;

/* ����list.c */
extern  LIST_TypeDef  taskDelayLists;
/* ����list.c */
extern  LIST_TypeDef  taskReadyLists[MAX_PRIORITIES]; 


#define NVIC_INT_CTRL       0xE000ED04      // �жϿ��Ƽ�״̬�Ĵ���
#define NVIC_PENDSVSET      0x10000000      // ��������жϵ�ֵ
#define NVIC_SYSPRI2        0xE000ED22      // ϵͳ���ȼ��Ĵ���
#define NVIC_PENDSV_PRI     0x000000FF      // �������ȼ�

#define MEM32(addr)         *(volatile unsigned long *)(addr)
#define MEM8(addr)          *(volatile unsigned char *)(addr)


/**
 * @brief  ��������
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
 * @brief  ��ʼ����
 * @param  none 
 * @retval none
 */
static void taskRun(void)
{
    __set_PSP(0);
    
    MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;   // ��NVIC_SYSPRI2дNVIC_PENDSV_PRI��������Ϊ������ȼ�
    
    MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;    // ��NVIC_INT_CTRLдNVIC_PENDSVSET������PendSV
    
    
}

/**
 * @brief  ���������л�
 * @param  none 
 * @retval none
 */
static void taskDispatch(void)
{
    
    MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;    // ��NVIC_INT_CTRLдNVIC_PENDSVSET������PendSV
}



/**
 * @brief  �����ջ��ʼ��
 * @param  none 
 * @retval none
 */
static void taskInit (taskHandler_t * task, void (*entry)(void *), void *param, uint32_t * stack)
{
    

    *(--stack) = (unsigned long)(1<<24);                // XPSR, ������Thumbģʽ���ָ���Thumb״̬����ARM״̬����
    *(--stack) = (unsigned long)entry;                  // �������ڵ�ַ
    *(--stack) = (unsigned long)0x14;                   // R14(LR), ���񲻻�ͨ��return xxx�����Լ�������δ��
    *(--stack) = (unsigned long)0x12;                   // R12, δ��
    *(--stack) = (unsigned long)0x3;                    // R3, δ��
    *(--stack) = (unsigned long)0x2;                    // R2, δ��
    *(--stack) = (unsigned long)0x1;                    // R1, δ��
    *(--stack) = (unsigned long)param;                  // R0 = param, �����������ں���
    *(--stack) = (unsigned long)0x11;                   // R11, δ��
    *(--stack) = (unsigned long)0x10;                   // R10, δ��
    *(--stack) = (unsigned long)0x9;                    // R9, δ��
    *(--stack) = (unsigned long)0x8;                    // R8, δ��
    *(--stack) = (unsigned long)0x7;                    // R7, δ��
    *(--stack) = (unsigned long)0x6;                    // R6, δ��
    *(--stack) = (unsigned long)0x5;                    // R5, δ��
    *(--stack) = (unsigned long)0x4;                    // R4, δ��

    task->stack = stack;                                // �������յ�ֵ
    
}


/**
 * @brief  �������
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
       taskDispatch(); 
    }

    taskExitCritical(s);
}


/**
 * @brief  �����ٽ���
 * @param  none 
 * @retval none
 */
uint32_t taskEnterCritical(void)
{
    uint32_t s = __get_PRIMASK();
    
    __disable_irq(); 
    
    return s;
}


/**
 * @brief  �˳��ٽ���
 * @param  none 
 * @retval none
 */
void taskExitCritical(uint32_t status)
{
    __set_PRIMASK(status);
}




/**
 * @brief  ��������
 * @param  none 
 * @retval none
 */
uint8_t createTask(taskHandler_t *handler_t,void(*fun)(void*),void *param,uint32_t size,uint8_t prio)
{
    //�����ٽ���
    uint32_t s = taskEnterCritical();
    uint32_t *stack;
    
    //�ռ�̫С
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
    
    //����������б�
    addTaskToReadyList(&handler_t->node);
    
    //����һ���������
    if(currentTask != NULL)
    {
       taskSched(); 
    }
    
     //�˳��ٽ���
    taskExitCritical(s);
    
    return True;
    
}



/**
 * @brief  ��������(�����ջ���з���)
 * @param  none 
 * @retval none
 */
uint8_t createStaticTask(taskHandler_t *handler_t,void(*fun)(void*),void *param,uint32_t* stack,uint32_t size,uint8_t prio)
{
    //�����ٽ���
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
    
    //����������б�
    addTaskToReadyList(&handler_t->node);
    
    //����һ���������
    if(currentTask != NULL)
    {
       taskSched(); 
    }
    
     //�˳��ٽ���
    taskExitCritical(s);
    
    return True;
    
}


/**
 * @brief  ��ʱ����
 * @param  none 
 * @retval none
 */
void taskDelay(uint32_t ticks)
{
    uint32_t s = taskEnterCritical();
    currentTask->delayTicks = ticks;
    currentTask->state = SUSPEND_STATE;
    //�Ӿ����б����Ƴ�
    removeTaskToReadyList(&(currentTask->node));
    //�������ʱ�б�
    addTaskToDelayList(&(currentTask->node));
    //����һ���������
    taskSched();
    
    taskExitCritical(s);
}


/**
 * @brief  ��ʱ����
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
             //����ʱ�б����Ƴ�
            removeTaskToDelayList(node);
            //����������б��б�
            addTaskToReadyList((node));
            break;
        }

    }

    
    taskExitCritical(s);
    //����һ�ε���
    taskSched();
}


extern taskHandler_t tTask1;
/**
 * @brief  �����������
 * @param  none 
 * @retval none
 */
void startTaskSched(void)
{
   
    
    //������������
    createTask(&idleHandler_t,idleTask,NULL,50,MAX_PRIORITIES-1);
    
    nextTask = (taskHandler_t*)getHighReadyTask();
    
    
    //�˺�����Զ���᷵��
    taskRun();
    
}


/**
 * @brief  PendSV_Handler(�����л�)
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
 * @brief  �δ��ʱ���ж� 
 * @param  ��
 * @retval ��
 */
void vPortSystick_Handler(void)
{
	taskSystemTickHandler();
}


/**********************(C)  COPYRIGHT 2017 ��Զ�� *********************************************************/
