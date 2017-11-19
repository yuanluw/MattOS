#include "bsp_key.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"
#include "bspMalloc.h"

#define KEY_UP_PORT       GPIOA
#define KEY_UP_PIN        GPIO_Pin_0
#define KEY_UP_CLK        RCC_AHB1Periph_GPIOA


#define KEY_LEFT_PORT     GPIOC
#define KEY_LEFT_PIN      GPIO_Pin_1
#define KEY_LEFT_CLK      RCC_AHB1Periph_GPIOC


#define KEY_DOWN_PORT     GPIOC
#define KEY_DOWN_PIN      GPIO_Pin_2
#define KEY_DOWN_CLK      RCC_AHB1Periph_GPIOC


#define KEY_RIGHT_PORT     GPIOC
#define KEY_RIGHT_PIN      GPIO_Pin_3
#define KEY_RIGHT_CLK      RCC_AHB1Periph_GPIOC


//按键事件指针
keyEventType * currentKeyEvent;


volatile uint8_t KEY_State;



/**
 * @brief  读取按键的状态 
 * @param  无
 * @retval 无
 */
static u8  Key_Read(void)
{
	
	
	if( (KEY_UP == 0  || KEY_DOWN == 0 || KEY_LEFT == 0 || KEY_RIGHT == 0))
	{
		
		delay_ms(10);
	
		if(KEY_UP == 0) return KEY_UP_PRES;
		else if(KEY_DOWN == 0) return KEY_DOWN_PRES;
		else if(KEY_LEFT == 0) return KEY_LEFT_PRES;
		else if(KEY_RIGHT == 0) return KEY_RIGHT_PRES;

	}
	
	
	return KEY_ALL_OFF;
	
}


/**
 * @brief  按键扫描 确定按键状态  
 * @param  无
 * @retval 无
 */
static u8 getKeyState(void)
{
	
	static uint8_t cnt = 0,read_key_state[KEY_LONG_CNT];
	uint8_t key_state  = 0 , i , key_cnt = 0;
	
	key_state = Key_Read();
	
	if((key_state!=KEY_ALL_OFF))   //有按键按下 开始记录按键 和 计数
	{
		read_key_state[cnt++] = key_state;
	}
	else if((key_state == KEY_ALL_OFF) && (cnt != 0))  //在未达到计数次数前按键被松开 则为短按 
	{
		cnt = 0;
		key_state =  read_key_state[0];
		for( i=0 ; i<KEY_LONG_CNT ; i++ )
		{
			read_key_state[i] = 0;
		}
		return key_state;
	}

	
	if(cnt == KEY_LONG_CNT )   //如果到了长按的计数次数 进行判断为哪个按键的长按
	{
		key_state = read_key_state[0];
		for( i=1 ; i<KEY_LONG_CNT ; i++)
		{
			if(key_state == read_key_state[i])
			{
				key_cnt++;
				read_key_state[i] = 0;
			}
		}
		
		cnt = 0;
		
		if(key_cnt > KEY_LONG_CNT -2 )        //最大扫描次数里有 KEY_LONG_CNT -2 次 跟初始按下情况一样 则为此按键的长按
		{
			
			while((KEY_UP == 0  || KEY_DOWN == 0 || KEY_LEFT == 0 || KEY_RIGHT == 0))  //等待此按键松开 
				      ;
			
			if(key_state == KEY_UP_PRES)
			{
				return KEY_UP_LONG_PRES;
			}
			else if(key_state == KEY_DOWN_PRES)
			{
				return KEY_DOWN_LONG_PRES;
			}
			else if(key_state == KEY_LEFT_PRES)
			{
				return KEY_LEFT_LONG_PRES;
			}
			else if(key_state == KEY_RIGHT_PRES)
			{
				return KEY_RIGHT_LONG_PRES;
			}
		}
		else
		{
			return key_state;
		}
	}
	
	
	return KEY_ALL_OFF;
}




/**
 * @brief  解析事件类型  
 * @param  无
 * @retval 无
 */
static keyEventEnum getEnum()
{
	KEY_State = getKeyState();
	
	switch(KEY_State)
	{
		case KEY_UP_PRES           : return UP;
		case KEY_DOWN_PRES         : return DOWN;
		case KEY_LEFT_PRES         : return LEFT;
		case KEY_ALL_OFF           : return NONE;
		case KEY_RIGHT_PRES        : return RIGHT;
		case KEY_UP_LONG_PRES      : return LONG_UP;
		case KEY_LEFT_LONG_PRES    : return LONG_LEFT;
		case KEY_DOWN_LONG_PRES    : return LONG_DOWN;
		case KEY_RIGHT_LONG_PRES   : return LONG_RIGHT;
		default                    : return NONE; 		
	}
}


/**
 * @brief  按键IO口配置  
 * @param  无
 * @retval 无
 */
void keyInit(void)
{
	
	
	GPIO_InitTypeDef   GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(KEY_UP_CLK|KEY_LEFT_CLK|KEY_DOWN_CLK|KEY_RIGHT_CLK,ENABLE);
	
	
	GPIO_InitStruct.GPIO_Mode    = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType   = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd    = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed   = GPIO_Speed_50MHz;
	
	
    GPIO_InitStruct.GPIO_Pin     = KEY_LEFT_PIN;
	GPIO_Init(KEY_LEFT_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin     = KEY_DOWN_PIN;
	GPIO_Init(KEY_DOWN_PORT,&GPIO_InitStruct);
	
	
	GPIO_InitStruct.GPIO_Pin     = KEY_RIGHT_PIN;
	GPIO_Init(KEY_RIGHT_PORT,&GPIO_InitStruct);
    
    
	GPIO_InitStruct.GPIO_PuPd    = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Pin     = KEY_UP_PIN;
	GPIO_Init(KEY_UP_PORT,&GPIO_InitStruct);
    
    currentKeyEvent = myMalloc(sizeof(keyEventType));
	//指向空
	currentKeyEvent->next = NULL;
	
}



/**
 * @brief  添加按键事件  
 * @param  state: 某个按键和按键的状态
           fun(): 按键状态发生时触发的事件 
           name 事件名称
           arg 额外参数
 * @retval 无
 */

uint8_t addKeyEvent(char const *name,keyEventEnum state,void (*fun)(void *arg),void *arg)
{
	
    keyEventType *newKeyEvent = NULL;
    keyEventType *q = currentKeyEvent;
    
    #if KEY_UNIQUE_EVENT == 1
    while(q->next != NULL)
    {
        //已经存在此按键状态的事件
        if(q->next->keyState == state)
        {
            return False;
        }     
        q = q->next;
    }
    q = currentKeyEvent;
    #endif
    
    newKeyEvent = myMalloc(sizeof(keyEventType));
    
    if(newKeyEvent == NULL)
    {
        return False;
    }
    
    newKeyEvent->arg = arg;
    newKeyEvent->fun = fun;
    newKeyEvent->keyState = state;
    newKeyEvent->next = NULL;
    strcpy(newKeyEvent->name,name);
    //添加新的按键事件
    while(q->next != NULL)
    {
        q = q->next;
    }
    q->next = newKeyEvent;
	
	return True;
}



#if KEY_UNIQUE_EVENT == 1
/**
 * @brief  删除 按键事件  
 * @param  state 按键状态
 * @retval 无
 */
u8 removeKeyEvent(keyEventEnum state)
{
	keyEventType *q = currentKeyEvent;
    keyEventType *temp;
    while(q->next != NULL)
    {
       
        //删除事件，回收内存
        if(q->next->keyState == state)
        {
            temp = q->next;
            q->next = q->next->next;
            myFree(temp);
            return True;
        }
        q = q->next;
    }
    return False;
}


#else
/**
 * @brief  删除 按键事件  
 * @param  name 事件名称
 * @retval 无
 */
u8 removeKeyEvent(char const *name)
{
	keyEventType *q = currentKeyEvent;
    keyEventType *temp;
    while(q->next != NULL)
    {
       
        //删除事件，回收内存
        if(strcmp(q->next->name,(char*)name) == 0)
        {
            temp = q->next;
            q->next = q->next->next;
            myFree(temp);
            return True;
        }
        q = q->next;
    }
    return False;
}

#endif



/**
 * @brief  按键事件运行  
 * @param  无
 * @retval 无
 */
void runKeyEvent()
{
	
	keyEventEnum state = getEnum();
	keyEventType *q = currentKeyEvent;
    
    
	while(q->next != NULL)
    {
        if(q->next->keyState == state)
        {
            q->next->fun(q->next->arg);
            #if KEY_UNIQUE_EVENT == 1
                break;
            #endif 
        }
        q = q->next;
    }
}

/**********************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/


