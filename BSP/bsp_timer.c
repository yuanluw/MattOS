#include "bsp_timer.h"
#include "stm32f4xx.h"




/**
 * @brief  ���ڴ���ADC1����
 * @param  speed ��������
 * @retval ��
 */
void timer2Init(uint32_t speed)
{
    
    uint32_t period = 42000000/speed;
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

    TIM_DeInit(TIM2);
    //��������ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    
    TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
    TIM_TimeBaseInitStruct.TIM_Period = period - 1;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 2-1;   
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct); 
    
    TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_Update);
    
    TIM_Cmd(TIM2, ENABLE);
    
    
}


/**
 * @brief  ���ڴ���DAC���
 * @param  speed = HZ * length Ҫ���õ�Ƶ�ʳ�����������ĳ��� 
 * @retval ��
 */
void timer6Init(uint32_t speed)
{
    
    uint32_t period = 84000000/speed;
    
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
    
    TIM_TimeBaseInitStruct.TIM_Period = period + 1;
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);
    
    TIM_SelectOutputTrigger(TIM6,TIM_TRGOSource_Update);
    
    TIM_Cmd(TIM6,ENABLE);
    
}

/**********************(C)  COPYRIGHT 2017 ��Զ�� *********************************************************/
