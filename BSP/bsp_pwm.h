#ifndef BSP_PWM_H
#define BSP_PWM_H
#include "stm32f4xx.h"



void setTimNvic(TIM_TypeDef* TIMx, uint32_t freq, uint8_t _PreemptionPriority, uint8_t _SubPriority);
void setPWMOutN(GPIO_TypeDef* GPIOx,uint32_t GPIO_Pin,TIM_TypeDef* TIMx,uint8_t channel,uint32_t freq,uint32_t ducyCycle);
void setPWMOut(GPIO_TypeDef* GPIOx,uint32_t GPIO_Pin,TIM_TypeDef* TIMx,uint8_t channel,uint32_t freq,uint32_t ducyCycle);


#endif

