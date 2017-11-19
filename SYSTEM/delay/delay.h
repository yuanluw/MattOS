#ifndef __DELAY_H
#define __DELAY_H 			   
#include <sys.h>	



#define USE_FreeRTOS   0



#if USE_FreeRTOS == 1
#include "FreeRTOS.h"
#include "task.h"
#endif



/* ����ȫ���жϵĺ� */
#if USE_FreeRTOS == 1
#define ENABLE_INT()	taskENTER_CRITICAL()	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	taskEXIT_CRITICAL()	/* ��ֹȫ���ж� */
#else

#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */
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
    volatile uint32_t cnt;     //������
    volatile uint32_t preLoad; //������Ԥװ��ֵ
    volatile uint8_t mode;     //��ʱ��ģʽ
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

//����ֻ����һ�εĶ�ʱ��
#define createOnceSoftTimer(name,period)     createSoftTimer( (name),(period),SOFTTIM_ONCE_MODE);
//����ѭ�����еĶ�ʱ��
#define createCircleSoftTimer(name,period)   createSoftTimer( (name),(period),SOFTTIM_CIRCLE_MODE);


#endif





/**********************(C)  COPYRIGHT 2017 ��Զ�� *********************************************************/


























