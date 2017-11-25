#include "app.h"

#include "stdio.h"

#include "Mattos.h"
#include "bsp_Led.h"
#include "bsp_key.h"

taskHandler_t ledTaskHandler_t;
taskHandler_t keyTaskHandler_t;


/**
 * @brief  led运行任务
 * @param  none 
 * @retval none
 */
void ledTask(void *para)
{
    while(1)
    {
        ledControl();
        
        taskDelay(100);
    }
}

void upEvent(void*para)
{
    printf("%s","upEvent\r\n");
    taskResume(&ledTaskHandler_t);
}


void downEvent(void*para)
{
    printf("%s","downEvent\r\n");
    taskSuspend(&ledTaskHandler_t);
}
/**
 * @brief  按键任务
 * @param  none 
 * @retval none
 */
void keyTask(void *para)
{
    addKeyEvent("up",UP,upEvent,NULL);
    addKeyEvent("down",DOWN,downEvent,NULL);
    
    while(1)
    {
        runKeyEvent();
        taskDelay(100);
    }
}






/**
 * @brief  启动任务(此函数不会返回)
 * @param  none 
 * @retval none
 */
void appRun(void)
{
    //创建任务
    createTask(&ledTaskHandler_t,ledTask,NULL,200,2);
    createTask(&keyTaskHandler_t,keyTask,NULL,200,3);

    //开启调度 
    startTaskSched();
}













/******************************************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/
