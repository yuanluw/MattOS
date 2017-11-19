#include "stm32f4xx.h"

#include "bsp_interrupt.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "TFT_LCD.h"
#include "delay.h"

#include "port.h"
/**
 * @brief  定时器7中断
 * @param  无
 * @retval 无
 */
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)!=RESET)
	{
      
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
		
	}
	
	
}



/**
 * @brief  ADC1采样完成中断 
 * @param  无
 * @retval 无
 */
void DMA2_Stream0_IRQHandler(void)
{

    
    DMA_ClearFlag(DMA2_Stream0,DMA_FLAG_TCIF0);
    
}





/**
 * @brief  ADC2采样完成中断 
 * @param  无
 * @retval 无
 */
void DMA2_Stream2_IRQHandler(void)
{
    
   DMA_ClearFlag(DMA2_Stream2,DMA_FLAG_TCIF2); 
}


/**
 * @brief  串口1接收中断
 * @param  无
 * @retval 无
 */
void USART1_IRQHandler(void)
{
    uint16_t res;
    
    res = USART_ReceiveData(USART1);
    
    
    USART_ClearFlag(USART1, USART_FLAG_TC);;
    
    (void)res;
}


extern void tTaskTickDECHandler(void);
/**
 * @brief  滴答计时器中断 
 * @param  无
 * @retval 无
 */
void SysTick_Handler(void)
{
	//systickISR();
   // tTaskTickDECHandler();
}








/**********************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/
