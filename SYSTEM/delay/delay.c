#include "bspMalloc.h"
#include "delay.h"
#include "sys.h"
#include "string.h"



//�����ʱ��ͷָ��
SOFTTIM_STRUCT *softTimerPoint = NULL;

//CPU����ʱ��
volatile int32_t gRunTime = 0;
u8  fac_us=0;							//us��ʱ������			   
u16 fac_ms=0;							//ms��ʱ������,��os��,����ÿ�����ĵ�ms��

 
/**
 * @brief  �����ʱ��������1
 * @param  ��
 * @retval ��
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
                
                //ѭ��ģʽ�Զ���װ�ؼ���ֵ
                if(p->mode == SOFTTIM_CIRCLE_MODE)
                {
                    p->cnt = p->preLoad;  
                }
            }
        }
    }
    
    
    
    
}


/**
 * @brief  ��ʱ��ʼ��������ʼ�������ʱ�� 
 * @param  ��
 * @retval ��
 */
void delayInit(uint16_t sysclk)
{
    u32 reload;
   
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
	fac_us=sysclk;							//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
	reload=sysclk;							//ÿ���ӵļ������� ��λΪM	   
	reload*=1000000/configTICK_RATE_HZ;		
											//reloadΪ24λ�Ĵ���,���ֵ:16777216,��168M��,Լ��0.0998s����	
	fac_ms=1000/configTICK_RATE_HZ;			//����OS������ʱ�����ٵ�λ	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//����SYSTICK�ж�
	SysTick->LOAD=reload; 					//ÿ1/configTICK_RATE_HZ��һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //����SYSTICK     
    
    softTimerPoint  = myMalloc(sizeof(softTimerPoint));
    softTimerPoint->next = NULL;
}

/**
 * @brief  systick�жϴ�����
 * @param  ��
 * @retval ��
 */
void systickISR(void)
{
    
    //1msһ�� �������ʱ����������һ
    softTmrDec();

    gRunTime++;
    if(gRunTime == 0x7FFFFFFF) //������Զ�����
    {
        gRunTime =0 ;
    }
    
}

/**
 * @brief  ��ȡ��������ʱ�� ��λΪ1ms
 * @param  ��
 * @retval ��
 */
int32_t getRunTime(void)
{
    int32_t runtime;
    
    DISABLE_INT();  	/* ���ж� */

	runtime = gRunTime;	

	ENABLE_INT();  		/* ���ж� */
    
    return runtime;
}



/**
 * @brief  ���������ʱ��
 * @param  id 0 - SOFTTIM_COUNT-1
           period ����ֵ ��λΪms
           callback �ص�����
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
    
    DISABLE_INT();  			/* ���ж� */

    p->next = newTmr;
    
    ENABLE_INT();  				/* ���ж� */
    
    return True;
}




/**
 * @brief  ��������ʱ����ʱʱ���Ƿ�ﵽ
 * @param  name ��ʱ������
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
                 //���ζ�ʱ�� remove��
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
 * @brief  ֹͣ����һ�������ʱ��
 * @param  name ��ʱ������
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
            DISABLE_INT();  			/* ���ж� */

            previous->next = current->next;
            myFree(current);
   
            ENABLE_INT();  				/* ���ж� */
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
  
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
    {
        vTaskDelay(ms/fac_ms);
        ms %= fac_ms;
    }

    delay_us(ms*1000);
    
    
}


#else
/**
 * @brief  ��ʱms
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
 * @brief  ��ʱus
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
            
            
            if(tcnt >= ticks) //��ʱ���
            {
                break;
            }
        }
    }
}

/**********************(C)  COPYRIGHT 2017 ��Զ�� *********************************************************/





















