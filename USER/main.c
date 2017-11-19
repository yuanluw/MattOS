#include "stm32f4xx.h"
#include "stdio.h"

#include "main.h"
#include "bsp_led.h"
#include "delay.h"
#include "bsp_key.h"
#include "bsp_pwm.h"
#include "bsp_adc.h"
#include "bsp_ui.h"
#include "bspUart.h"
#include "port.h"



void task1(void *para)
{
    while(1)
    {
        printf("%d",1);

    }
}

void task2(void *para)
{
    while(1)
    {
       ledControl();

    }
}

void _idleTask(void *para)
{
    while(1)
    {
        
    }
}

tTaskHandler_t tTask1;
tTaskHandler_t tTask2;
tTaskHandler_t tTaskIdle;
tTaskStack task1Stack[1024];
tTaskStack task2Stack[1024];
tTaskStack idleStack[1024];
tTaskHandler_t * taskTable[2];
tTaskHandler_t *idleTask;









/**
 * @brief  基础外设初始化 
 * @param  无
 * @retval 无
 */
static void Bsp_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delayInit(168);
	ledInit();
	keyInit();
    uartInit(115200);
 
    
    taskTable[0] = &tTask1;
    taskTable[1] = &tTask2;
    idleTask = &tTaskIdle;
    
    
    tTaskRun();
    
    
}




int main()
{
   
    Bsp_Init();

   
    return 0;
  
}





/******************************************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/



