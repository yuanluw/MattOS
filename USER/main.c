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
        ledControl();
        
        taskDelay(100);
    }
}

void task2(void *para)
{
     uint32_t s = 0;
    while(1)
    {
        s = taskEnterCritical();
        printf("%s","task2\r\n");
        taskExitCritical(s);
        taskDelay(100);
    }
}


void task3(void *para)
{
    uint32_t s = 0;
    while(1)
    {
        s = taskEnterCritical();
        printf("%s\r\n","task3");
        taskExitCritical(s);

        taskDelay(100);  
    }
}



taskHandler_t tTask1;
taskHandler_t tTask2;
taskHandler_t tTask3;
taskHandler_t tTask4;



int main()
{
   
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delayInit(168);
	ledInit();
	keyInit();
    uartInit(115200);
    
    createTask(&tTask1,task1,NULL,200,3);
    createTask(&tTask2,task2,NULL,200,3);
    createTask(&tTask3,task3,NULL,200,3);

    startTaskSched();

   
    return 0;
  
}





/******************************************(C)  COPYRIGHT 2017 Œ‚‘∂„Ú *********************************************************/



