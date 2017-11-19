#ifndef __DELAY_H
#define __DELAY_H 			   
#include <sys.h>	



#define USE_FreeRTOS   0



#if USE_FreeRTOS == 1
#include "FreeRTOS.h"
#include "task.h"
#endif



/* 开关全局中断的宏 */
#if USE_FreeRTOS == 1
#define ENABLE_INT()	taskENTER_CRITICAL()	/* 使能全局中断 */
#define DISABLE_INT()	taskEXIT_CRITICAL()	/* 禁止全局中断 */
#else

#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */
#define configTICK_RATE_HZ 1000     //1KHZ 
#endif


#define SOFTTIM_COUNT 4

#ifndef True
#define True 1
#endif

#ifndef False
#define False 0
#endif

#ifndef NULL
#define NULL 0
#endif



typedef enum
{
    SOFTTIM_ONCE_MODE  = 0,
    SOFTTIM_CIRCLE_MODE = 1
}SOFTTIM_MODE;


typedef struct SOFTTIM_T
{
    struct SOFTTIM_T *next;
    volatile uint32_t cnt;     //计数器
    volatile uint32_t preLoad; //计数器预装载值
    volatile uint8_t mode;     //定时器模式
    volatile uint8_t flag;   
    char name[15];
}SOFTTIM_STRUCT;



void delayInit(uint16_t sysclk);
void delay_ms(u16 nms);
void delay_us(u32 nus);



void systickISR(void);
int32_t getRunTime(void);
uint8_t createSoftTimer(const char *name,uint32_t period,SOFTTIM_MODE mode);
uint8_t removeSoftTimer(char *name);
uint8_t checkSoftTimerFlag(char *name);

//创建只运行一次的定时器
#define createOnceSoftTimer(name,period)     createSoftTimer( (name),(period),SOFTTIM_ONCE_MODE);
//创建循环运行的定时器
#define createCircleSoftTimer(name,period)   createSoftTimer( (name),(period),SOFTTIM_CIRCLE_MODE);


#endif





/**********************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/


























