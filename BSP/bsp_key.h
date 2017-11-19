#ifndef _BSP_KEY_H 
#define _BSP_KEY_H

#include "stdint.h"

#ifndef True
#define True 1
#endif

#ifndef False
#define False 0
#endif

#define KEYEVENTMAX   8

#define KEY_UP         PAin(0)
#define KEY_DOWN       PCin(2)
#define KEY_LEFT       PCin(1)
#define KEY_RIGHT      PCin(3)

#define KEY_ALL_OFF         0
#define KEY_UP_PRES         1
#define KEY_LEFT_PRES       2
#define KEY_DOWN_PRES       3
#define KEY_RIGHT_PRES      4

#define KEY_UP_LONG_PRES         5
#define KEY_LEFT_LONG_PRES       6
#define KEY_DOWN_LONG_PRES       7
#define KEY_RIGHT_LONG_PRES      8

#define KEY_LONG_CNT        10

#define KEY_EVENT_NAME_LENGTH 15

//一个按键状态只支持一个事件
#define KEY_UNIQUE_EVENT   0

typedef enum
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	LONG_UP,
	LONG_DOWN,
	LONG_LEFT,
	LONG_RIGHT
}keyEventEnum;


typedef struct keyEventType
{
    struct keyEventType *next;
	keyEventEnum keyState; //事件
	void (*fun)(void *arg);         //处理函数
    void *arg;
    char name[KEY_EVENT_NAME_LENGTH];
}keyEventType;






void keyInit(void);
void runKeyEvent(void);
u8 addKeyEvent(char const *name,keyEventEnum state,void (*fun)(),void *arg);
#if KEY_UNIQUE_EVENT == 1
u8 removeKeyEvent(keyEventEnum state);
#else
u8 removeKeyEvent(char const *name);
#endif


#endif



/*******************************(C) COPYRIGHT 2017（吴远泸）*********************************/

