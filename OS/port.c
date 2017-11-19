#include "stm32f4xx.h"

#include "port.h"

#include "MattOS.h"
#include "bspMalloc.h"
#include "list.h"



tTaskHandler_t *currentTask = NULL;
tTaskHandler_t *nextTask = NULL;


#define NVIC_INT_CTRL       0xE000ED04      // �жϿ��Ƽ�״̬�Ĵ���
#define NVIC_PENDSVSET      0x10000000      // ��������жϵ�ֵ
#define NVIC_SYSPRI2        0xE000ED22      // ϵͳ���ȼ��Ĵ���
#define NVIC_PENDSV_PRI     0x000000FF      // �������ȼ�

#define MEM32(addr)         *(volatile unsigned long *)(addr)
#define MEM8(addr)          *(volatile unsigned char *)(addr)



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
 * @brief  ��ʼ����
 * @param  none 
 * @retval none
 */
void tTaskRun(void)
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
void tTaskDispatch(void)
{
    
    MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;    // ��NVIC_INT_CTRLдNVIC_PENDSVSET������PendSV
}



/**
 * @brief  �����ջ��ʼ��
 * @param  none 
 * @retval none
 */
static void tTaskInit (tTaskHandler_t * task, void (*entry)(void *), void *param, uint32_t * stack)
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
 * @brief  �����ٽ���
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
 * @brief  �˳��ٽ���
 * @param  none 
 * @retval none
 */
void tTaskExitCritical(uint32_t status)
{
    __set_PRIMASK(status);
}








/**
 * @brief  ��������
 * @param  none 
 * @retval none
 */
uint8_t createTask(tTaskHandler_t *handler_t,void(*fun)(void*),void *param,uint32_t size,uint8_t prio)
{
    //�����ٽ���
    uint32_t s = tTaskEnterCritical();
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
    
    stack = myMalloc(size);
    if(stack == NULL)
    {
        return False;
    }
    
    tTaskInit(handler_t,fun,param,stack);
    
    handler_t->prio = prio;
    handler_t->delayTicks = 0;
    handler_t->state = READY_STATE;
    
    //����������б�
    addTaskToReadyList(handler_t);
    
    
     //�˳��ٽ���
    tTaskExitCritical(s);
    
}




void tTaskDelay(uint32_t ticks)
{
    uint32_t s = tTaskEnterCritical();
    currentTask->delayTicks = ticks;
    tTaskExitCritical(s);
    
    
}


/**********************(C)  COPYRIGHT 2017 ��Զ�� *********************************************************/
