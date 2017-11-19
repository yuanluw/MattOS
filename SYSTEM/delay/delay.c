#include "bspMalloc.h"
#include "delay.h"
#include "sys.h"
#include "string.h"



//软件定时器头指针
SOFTTIM_STRUCT *softTimerPoint = NULL;

//CPU运行时间
volatile int32_t gRunTime = 0;
u8  fac_us=0;							//us延时倍乘数			   
u16 fac_ms=0;							//ms延时倍乘数,在os下,代表每个节拍的ms数

 
/**
 * @brief  软件定时器计数减1
 * @param  无
 * @retval 无
 */
static void softTmrDec(void)
{
    SOFTTIM_STRUCT *p;
    
    p = softTimerPoint;
    
    while(p->next != NULL)
    {
        p = p->next;
        if(p->cnt > 0)
        {
            if(--p->cnt == 0)
            {
                p->flag = True;
                
                //循环模式自动重装载计数值
                if(p->mode == SOFTTIM_CIRCLE_MODE)
                {
                    p->cnt = p->preLoad;  
                }
            }
        }
    }
    
    
    
    
}


/**
 * @brief  延时初始化，并初始化软件定时器 
 * @param  无
 * @retval 无
 */
void delayInit(uint16_t sysclk)
{
    u32 reload;
   
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
	fac_us=sysclk;							//不论是否使用OS,fac_us都需要使用
	reload=sysclk;							//每秒钟的计数次数 单位为M	   
	reload*=1000000/configTICK_RATE_HZ;		
											//reload为24位寄存器,最大值:16777216,在168M下,约合0.0998s左右	
	fac_ms=1000/configTICK_RATE_HZ;			//代表OS可以延时的最少单位	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//开启SYSTICK中断
	SysTick->LOAD=reload; 					//每1/configTICK_RATE_HZ断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //开启SYSTICK     
    
    softTimerPoint  = myMalloc(sizeof(softTimerPoint));
    softTimerPoint->next = NULL;
}

/**
 * @brief  systick中断处理函数
 * @param  无
 * @retval 无
 */
void systickISR(void)
{
    
    //1ms一次 对软件定时器计数器减一
    softTmrDec();

    gRunTime++;
    if(gRunTime == 0x7FFFFFFF) //溢出则自动清零
    {
        gRunTime =0 ;
    }
    
}

/**
 * @brief  获取程序运行时间 单位为1ms
 * @param  无
 * @retval 无
 */
int32_t getRunTime(void)
{
    int32_t runtime;
    
    DISABLE_INT();  	/* 关中断 */

	runtime = gRunTime;	

	ENABLE_INT();  		/* 开中断 */
    
    return runtime;
}



/**
 * @brief  创建软件定时器
 * @param  id 0 - SOFTTIM_COUNT-1
           period 计数值 单位为ms
           callback 回调函数
 * @retval True or False
 */
uint8_t createSoftTimer(const char *name,uint32_t period,SOFTTIM_MODE mode)
{
    SOFTTIM_STRUCT *newTmr;
    SOFTTIM_STRUCT *p;
    
    newTmr = myMalloc(sizeof(SOFTTIM_STRUCT));
        
    if(newTmr == NULL)
    {
        return False;
    }
    newTmr->cnt = period;
    newTmr->preLoad = period;
    newTmr->flag = False;
    newTmr->mode = mode;
    newTmr->next = NULL;
    strcpy(newTmr->name,name);
    p = softTimerPoint;
    
    while(p->next != NULL)
    {
        p = p->next;
    }
    
    DISABLE_INT();  			/* 关中断 */

    p->next = newTmr;
    
    ENABLE_INT();  				/* 开中断 */
    
    return True;
}




/**
 * @brief  检查软件定时器定时时间是否达到
 * @param  name 定时器名字
 * @retval True or False
 */
uint8_t checkSoftTimerFlag(char *name)
{
    
    SOFTTIM_STRUCT *p;
    
    p = softTimerPoint;
    
    while(p->next != NULL)
    {
         p = p->next;
        if(strcmp(p->name,name) == 0)
        {
            if(p->flag == True)
            {
                 //单次定时器 remove掉
                if(p->mode == SOFTTIM_ONCE_MODE)
                {
                    removeSoftTimer(name);
                }
                p->flag = False;
                return True;
            }
            return False;
        }
       
    }
   
    return False;

    
}





/**
 * @brief  停止运行一个软件定时器
 * @param  name 定时器名字
 * @retval none
 */
uint8_t removeSoftTimer(char *name)
{
    SOFTTIM_STRUCT *current,*previous;
    
    current = softTimerPoint->next;
    previous = softTimerPoint;
    
    while(current != NULL)
    {
       if(strcmp(current->name,name) == 0)  
       {
            DISABLE_INT();  			/* 关中断 */

            previous->next = current->next;
            myFree(current);
   
            ENABLE_INT();  				/* 开中断 */
            return True;
       }
       previous = current;
       current = current->next;
    }
    return False;
}


#if USE_FreeRTOS == 1

void delay_ms(uint16_t ms)
{
  
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
    {
        vTaskDelay(ms/fac_ms);
        ms %= fac_ms;
    }

    delay_us(ms*1000);
    
    
}


#else
/**
 * @brief  延时ms
 * @param  none
 * @retval none
 */
void delay_ms(uint16_t ms)
{
    uint16_t i;
    for(i=0 ; i<ms; i++)
    {
       delay_us(1000);
    }
}


#endif




/**
 * @brief  延时us
 * @param  none
 * @retval none
 */
void delay_us(uint32_t us)
{
    uint32_t ticks = 0;
    uint32_t told = 0;
    uint32_t tnow = 0;
    uint32_t tcnt = 0;
    uint32_t reload = 0;
    
    
    reload = SysTick->LOAD;
    ticks = us * fac_us;
    
    told = SysTick->VAL;
    
    while(1)
    {
        tnow = SysTick->VAL;
        
        if(tnow != told)
        {
            if(tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            
            
            if(tcnt >= ticks) //延时完成
            {
                break;
            }
        }
    }
}

/**********************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/





















