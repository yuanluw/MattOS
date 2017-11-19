#include "stm32f4xx.h"
#include "bsp_pwm.h"




/*
	���������GPIO��TIMͨ��:

	TIM1_CH1, PA8,	PE9,
	TIM1_CH2, PA9,	PE11
	TIM1_CH3, PA10,	PE13
	TIM1_CH4, PA11,	PE14

	TIM2_CH2, PA1,	PB3
	TIM2_CH3, PA2,	PB10
	TIM2_CH4, PA3,	PB11

	TIM3_CH1, PA6,  PB4, PC6
	TIM3_CH2, PA7,	PB5, PC7
	TIM3_CH3, PB0,	PC8
	TIM3_CH4, PB1,	PC9

	TIM4_CH1, PB6,  PD12
	TIM4_CH2, PB7,	PD13
	TIM4_CH3, PB8,	PD14
	TIM4_CH4, PB9,	PD15

	TIM5_CH1, PA0,  PH10
	TIM5_CH2, PA1,	PH11
	TIM5_CH3, PA2,	PH12
	TIM5_CH4, PA3,	PI10

	TIM8_CH1, PC6,  PI5
	TIM8_CH2, PC7,	PI6
	TIM8_CH3, PC8,	PI7
	TIM8_CH4, PC9,	PI2

	TIM9_CH1, PA2,  PE5
	TIM9_CH2, PA3,	PE6

	TIM10_CH1, PB8,  PF6

	TIM11_CH1, PB9,  PF7

	TIM12_CH1, PB14,  PH6
	TIM12_CH2, PB15,  PH9

	TIM13_CH1, PA6,  PF8
	TIM14_CH1, PA7,  PF9

	APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14 
	APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11
	

	APB1 ��ʱ��������ʱ�� TIMxCLK = SystemCoreClock / 2; 84M
	APB2 ��ʱ��������ʱ�� TIMxCLK = SystemCoreClock; 168M
*/


/**
 * @brief  
 * @param  
 * @retval ����GPIOx��Ӧ��ʱ��
 */
static uint32_t getRccCLK(GPIO_TypeDef* GPIOx)
{
    uint32_t rcc = 0;

	if (GPIOx == GPIOA)
	{
		rcc = RCC_AHB1Periph_GPIOA;
	}
	else if (GPIOx == GPIOB)
	{
		rcc = RCC_AHB1Periph_GPIOB;
	}
	else if (GPIOx == GPIOC)
	{
		rcc = RCC_AHB1Periph_GPIOC;
	}
	else if (GPIOx == GPIOD)
	{
		rcc = RCC_AHB1Periph_GPIOD;
	}
	else if (GPIOx == GPIOE)
	{
		rcc = RCC_AHB1Periph_GPIOE;
	}
	else if (GPIOx == GPIOF)
	{
		rcc = RCC_AHB1Periph_GPIOF;
	}
	else if (GPIOx == GPIOG)
	{
		rcc = RCC_AHB1Periph_GPIOG;
	}
	else if (GPIOx == GPIOH)
	{
		rcc = RCC_AHB1Periph_GPIOH;
	}
	else if (GPIOx == GPIOI)
	{
		rcc = RCC_AHB1Periph_GPIOI;
	}

	return rcc;
}


/**
 * @brief  
 * @param  
 * @retval ����TIMx��Ӧ��ʱ��
 */
static uint32_t getRcc_Tim(TIM_TypeDef* TIMx)
{
	uint32_t rcc = 0;

	/*
		APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14
		APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11
	*/
	if (TIMx == TIM1)
	{
		rcc = RCC_APB2Periph_TIM1;
	}
	else if (TIMx == TIM8)
	{
		rcc = RCC_APB2Periph_TIM8;
	}
	else if (TIMx == TIM9)
	{
		rcc = RCC_APB2Periph_TIM9;
	}
	else if (TIMx == TIM10)
	{
		rcc = RCC_APB2Periph_TIM10;
	}
	else if (TIMx == TIM11)
	{
		rcc = RCC_APB2Periph_TIM11;
	}
	/* ������ APB1ʱ�� */
	else if (TIMx == TIM2)
	{
		rcc = RCC_APB1Periph_TIM2;
	}
	else if (TIMx == TIM3)
	{
		rcc = RCC_APB1Periph_TIM3;
	}
	else if (TIMx == TIM4)
	{
		rcc = RCC_APB1Periph_TIM4;
	}
	else if (TIMx == TIM5)
	{
		rcc = RCC_APB1Periph_TIM5;
	}
	else if (TIMx == TIM6)
	{
		rcc = RCC_APB1Periph_TIM6;
	}
	else if (TIMx == TIM7)
	{
		rcc = RCC_APB1Periph_TIM7;
	}
	else if (TIMx == TIM12)
	{
		rcc = RCC_APB1Periph_TIM12;
	}
	else if (TIMx == TIM13)
	{
		rcc = RCC_APB1Periph_TIM13;
	}
	else if (TIMx == TIM14)
	{
		rcc = RCC_APB1Periph_TIM14;
	}

	return rcc;
} 


/**
 * @brief  
 * @param  
 * @retval ����AF��
 */
static uint16_t getPinSource(uint16_t gpio_pin)
{
    
   	uint16_t ret = 0;

	if (gpio_pin == GPIO_Pin_0)
	{
		ret = GPIO_PinSource0;
	}
	else if (gpio_pin == GPIO_Pin_1)
	{
		ret = GPIO_PinSource1;
	}
	else if (gpio_pin == GPIO_Pin_2)
	{
		ret = GPIO_PinSource2;
	}
	else if (gpio_pin == GPIO_Pin_3)
	{
		ret = GPIO_PinSource3;
	}
	else if (gpio_pin == GPIO_Pin_4)
	{
		ret = GPIO_PinSource4;
	}
	else if (gpio_pin == GPIO_Pin_5)
	{
		ret = GPIO_PinSource5;
	}
	else if (gpio_pin == GPIO_Pin_6)
	{
		ret = GPIO_PinSource6;
	}
	else if (gpio_pin == GPIO_Pin_7)
	{
		ret = GPIO_PinSource7;
	}
	else if (gpio_pin == GPIO_Pin_8)
	{
		ret = GPIO_PinSource8;
	}
	else if (gpio_pin == GPIO_Pin_9)
	{
		ret = GPIO_PinSource9;
	}
	else if (gpio_pin == GPIO_Pin_10)
	{
		ret = GPIO_PinSource10;
	}
	else if (gpio_pin == GPIO_Pin_11)
	{
		ret = GPIO_PinSource11;
	}
	else if (gpio_pin == GPIO_Pin_12)
	{
		ret = GPIO_PinSource12;
	}
	else if (gpio_pin == GPIO_Pin_13)
	{
		ret = GPIO_PinSource13;
	}
	else if (gpio_pin == GPIO_Pin_14)
	{
		ret = GPIO_PinSource14;
	}
	else if (gpio_pin == GPIO_Pin_15)
	{
		ret = GPIO_PinSource15;
	}

	return ret; 
    
}

/**
 * @brief  
 * @param  
 * @retval ����TIM��AF IO��
 */
static uint8_t getAFTim(TIM_TypeDef* TIMx)
{
   uint8_t ret = 0;

	/*
		APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14
		APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11
	*/
	if (TIMx == TIM1)
	{
		ret = GPIO_AF_TIM1;
	}
	else if (TIMx == TIM8)
	{
		ret = GPIO_AF_TIM8;
	}
	else if (TIMx == TIM9)
	{
		ret = GPIO_AF_TIM9;
	}
	else if (TIMx == TIM10)
	{
		ret = GPIO_AF_TIM10;
	}
	else if (TIMx == TIM11)
	{
		ret = GPIO_AF_TIM11;
	}
	/* ������ APB1ʱ�� */
	else if (TIMx == TIM2)
	{
		ret = GPIO_AF_TIM2;
	}
	else if (TIMx == TIM3)
	{
		ret = GPIO_AF_TIM3;
	}
	else if (TIMx == TIM4)
	{
		ret = GPIO_AF_TIM4;
	}
	else if (TIMx == TIM5)
	{
		ret = GPIO_AF_TIM5;
	}
	/* û�� TIM6 TIM7 */
	else if (TIMx == TIM8)
	{
		ret = GPIO_AF_TIM8;
	}
	else if (TIMx == TIM9)
	{
		ret = GPIO_AF_TIM9;
	}
	else if (TIMx == TIM12)
	{
		ret = GPIO_AF_TIM12;
	}
	else if (TIMx == TIM13)
	{
		ret = GPIO_AF_TIM13;
	}
	else if (TIMx == TIM14)
	{
		ret = GPIO_AF_TIM14;
	}

	return ret; 
}


/**
 * @brief  ����IO��Ϊ�������
 * @param  
 * @retval ��
 */
static void configGpioOut(GPIO_TypeDef* GPIOx,uint32_t GPIO_Pin)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    RCC_AHB1PeriphClockCmd(getRccCLK(GPIOx),ENABLE);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx,&GPIO_InitStruct);
    
}

/**
 * @brief  ����IO�ں͸��� ��ʹ��TIMx
 * @param  
 * @retval ��
 */
static void configGpioAndTim(GPIO_TypeDef* GPIOx,uint32_t GPIO_Pin,TIM_TypeDef* TIMx)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    //ʹ��GPIOʱ��
    RCC_AHB1PeriphClockCmd(getRccCLK(GPIOx),ENABLE);
    
    // ʹ��TIMʱ��
	if ((TIMx == TIM1) || (TIMx == TIM8) || (TIMx == TIM9) || (TIMx == TIM10) || (TIMx == TIM11))
	{
		RCC_APB2PeriphClockCmd(getRcc_Tim(TIMx), ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(getRcc_Tim(TIMx), ENABLE);
	}
    
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOx,&GPIO_InitStruct);
    
    /* ���ӵ�AF���� */
	GPIO_PinAFConfig(GPIOx, getPinSource(GPIO_Pin), getAFTim(TIMx));
}



/**
 * @brief  ����PWM��� ��Ƶ�ʿɴ�21M ռ�ձ�1%-99%��
 * @param   channel : 1,2,3,4 ��ʱ����CH1,CH2,CH3,CH4
           freq: �������Ƶ��
           dutyCycle :0-10000 ռ�ձ� 5000Ϊ50%
 * @retval ��
 */
void setPWMOut(GPIO_TypeDef* GPIOx,uint32_t GPIO_Pin,TIM_TypeDef* TIMx,uint8_t channel,uint32_t freq,uint32_t ducyCycle)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    uint16_t usPeriod;
    uint16_t usPrescaler;
    uint32_t uiTIMClk;
    
    if(ducyCycle == 0)
    {
        //ʧ�ܶ�ʱ��
        TIM_Cmd(TIMx,DISABLE);
        //����IOΪ�������
        configGpioOut(GPIOx,GPIO_Pin);
        //���0
        GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_RESET);
        return ;
    }
    else if(ducyCycle == 10000)
    {
       //ʧ�ܶ�ʱ��
        TIM_Cmd(TIMx,DISABLE);
        //����IOΪ�������
        configGpioOut(GPIOx,GPIO_Pin);
        //���1
        GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_SET); 
        return ;
    }
    
    //����IO��
    configGpioAndTim(GPIOx,GPIO_Pin,TIMx);
    
    if ((TIMx == TIM1) || (TIMx == TIM8) || (TIMx == TIM9) || (TIMx == TIM10) || (TIMx == TIM11))
	{
		/* APB2 ��ʱ�� */
		uiTIMClk = SystemCoreClock; //180M
	}
	else	/* APB1 ��ʱ�� */
	{
		uiTIMClk = SystemCoreClock / 2;
	}
    
    
    if (freq < 100)
	{
        //��Ƶ�� = 10000 
		usPrescaler = 10000 - 1;			
        // �Զ���װ��ֵ         
		usPeriod =  (uiTIMClk / 10000) / freq  - 1;		
	}
	else if (freq < 3000)
	{
		usPrescaler = 100 - 1;					      
		usPeriod =  (uiTIMClk / 100) / freq  - 1;		
	}
	else	/* ����3K��Ƶ�ʣ������Ƶ */
	{
		usPrescaler = 0;					
		usPeriod = uiTIMClk / freq - 1;	
	}

    //���û�����ʱ
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period            = usPeriod;
    TIM_TimeBaseStructure.TIM_Prescaler         = usPrescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision     = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
    
    
    //�������
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = (ducyCycle * usPeriod) / 10000;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;	
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;					
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;		
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;		
    
    if (channel == 1)
	{
		TIM_OC1Init(TIMx, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}
	else if (channel == 2)
	{
		TIM_OC2Init(TIMx, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}
	else if (channel == 3)
	{
		TIM_OC3Init(TIMx, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}
	else if (channel == 4)
	{
		TIM_OC4Init(TIMx, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}

	TIM_ARRPreloadConfig(TIMx, ENABLE);

	//��ʱ�����
	TIM_Cmd(TIMx, ENABLE);

	if ((TIMx == TIM1) || (TIMx == TIM8))
	{
		TIM_CtrlPWMOutputs(TIMx, ENABLE);
	}
}


/**
 * @brief  ����PWM������� 
 * @param   channel : 1,2,3,4 ��ʱ����CH1,CH2,CH3,CH4
           freq: �������Ƶ��
           dutyCycle :0-10000 ռ�ձ� 5000Ϊ50%
 * @retval ��
 */
void setPWMOutN(GPIO_TypeDef* GPIOx,uint32_t GPIO_Pin,TIM_TypeDef* TIMx,uint8_t channel,uint32_t freq,uint32_t ducyCycle)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    uint16_t usPeriod;
    uint16_t usPrescaler;
    uint32_t uiTIMClk;
    
    if(ducyCycle == 0)
    {
        //ʧ�ܶ�ʱ��
        TIM_Cmd(TIMx,DISABLE);
        //����IOΪ�������
        configGpioOut(GPIOx,GPIO_Pin);
        //���0
        GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_RESET);
        return ;
    }
    else if(ducyCycle == 10000)
    {
       //ʧ�ܶ�ʱ��
        TIM_Cmd(TIMx,DISABLE);
        //����IOΪ�������
        configGpioOut(GPIOx,GPIO_Pin);
        //���1
        GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_SET); 
        return ;
    }
    
    //����IO��
    configGpioAndTim(GPIOx,GPIO_Pin,TIMx);
    
    if ((TIMx == TIM1) || (TIMx == TIM8) || (TIMx == TIM9) || (TIMx == TIM10) || (TIMx == TIM11))
	{
		/* APB2 ��ʱ�� */
		uiTIMClk = SystemCoreClock; //180M
	}
	else	/* APB1 ��ʱ�� */
	{
		uiTIMClk = SystemCoreClock / 2;
	}
    
    
    if (freq < 100)
	{
        //��Ƶ�� = 10000 
		usPrescaler = 10000 - 1;			
        // �Զ���װ��ֵ         
		usPeriod =  (uiTIMClk / 10000) / freq  - 1;		
	}
	else if (freq < 3000)
	{
		usPrescaler = 100 - 1;					      
		usPeriod =  (uiTIMClk / 100) / freq  - 1;		
	}
	else	/* ����3K��Ƶ�ʣ������Ƶ */
	{
		usPrescaler = 0;					
		usPeriod = uiTIMClk / freq - 1;	
	}

    //���û�����ʱ
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period            = usPeriod;
    TIM_TimeBaseStructure.TIM_Prescaler         = usPrescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision     = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
    
    
    //�������
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = (ducyCycle * usPeriod) / 10000;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;	
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;					
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;		
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;		
    
    if (channel == 1)
	{
		TIM_OC1Init(TIMx, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}
	else if (channel == 2)
	{
		TIM_OC2Init(TIMx, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}
	else if (channel == 3)
	{
		TIM_OC3Init(TIMx, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}
	else if (channel == 4)
	{
		TIM_OC4Init(TIMx, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}

	TIM_ARRPreloadConfig(TIMx, ENABLE);

	//��ʱ�����
	TIM_Cmd(TIMx, ENABLE);

	if ((TIMx == TIM1) || (TIMx == TIM8))
	{
		TIM_CtrlPWMOutputs(TIMx, ENABLE);
	}
}




/**
 * @brief   ������ͨ��ʱ������ж�(�жϷ���������б�д)
* @param    freq:��ʱƵ��
            _PreemptionPriority �жϷ���
            _SubPriority  �����ȼ�
 * @retval ��
 */
void setTimNvic(TIM_TypeDef* TIMx, uint32_t freq, uint8_t _PreemptionPriority, uint8_t _SubPriority)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;
    uint8_t irq; //�жϺ�
	uint16_t usPeriod;
	uint16_t usPrescaler;
	uint32_t uiTIMCLK;
    
    /* ʹ��TIMʱ�� */
	if ((TIMx == TIM1) || (TIMx == TIM8) || (TIMx == TIM9) || (TIMx == TIM10) || (TIMx == TIM11))
	{
		RCC_APB2PeriphClockCmd(getRcc_Tim(TIMx), ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(getRcc_Tim(TIMx), ENABLE);
	}
    
    if (freq == 0)
	{
        // �رն�ʱ��� 
		TIM_Cmd(TIMx, DISABLE);		
		return;
	}
    
     if ((TIMx == TIM1) || (TIMx == TIM8) || (TIMx == TIM9) || (TIMx == TIM10) || (TIMx == TIM11))
	{
		/* APB2 ��ʱ�� */
		uiTIMCLK = SystemCoreClock; //180M
	}
	else	/* APB1 ��ʱ�� */
	{
		uiTIMCLK = SystemCoreClock / 2;
	}
    
      if (freq < 100)
	{
        //��Ƶ�� = 10000 
		usPrescaler = 10000 - 1;			
        // �Զ���װ��ֵ         
		usPeriod =  (uiTIMCLK / 10000) / freq  - 1;		
	}
	else if (freq < 3000)
	{
		usPrescaler = 100 - 1;					      
		usPeriod =  (uiTIMCLK / 100) / freq  - 1;		
	}
	else	/* ����3K��Ƶ�ʣ������Ƶ */
	{
		usPrescaler = 0;					
		usPeriod = uiTIMCLK / freq - 1;	
	}
    
    /* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = usPeriod;
	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;		/* TIM1 �� TIM8 �������� */

	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

	TIM_ARRPreloadConfig(TIMx, ENABLE);
 
    /* TIM Interrupts enable */
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
    
    /* TIMx enable counter */
	TIM_Cmd(TIMx, ENABLE);
    
    
    if ((TIMx == TIM1) || (TIMx == TIM10))
        irq = TIM1_UP_TIM10_IRQn;
    else if (TIMx == TIM2)
        irq = TIM2_IRQn;
    else if (TIMx == TIM3)
        irq = TIM3_IRQn;
    else if (TIMx == TIM4)
        irq = TIM4_IRQn;
    else if (TIMx == TIM5)
        irq = TIM5_IRQn;
    else if (TIMx == TIM6)
        irq = TIM6_DAC_IRQn;
    else if (TIMx == TIM7)
        irq = TIM7_IRQn;
    else if (TIMx == TIM7)
        irq = TIM7_IRQn;
    else if (TIMx == TIM7)
        irq = TIM7_IRQn;
    else if ((TIMx == TIM8) || (TIMx == TIM13))
        irq = TIM8_UP_TIM13_IRQn;
    else if (TIMx == TIM9)
        irq = TIM1_BRK_TIM9_IRQn;
    else if (TIMx == TIM11)
        irq = TIM1_TRG_COM_TIM11_IRQn;
    else if (TIMx == TIM12)
        irq = TIM8_BRK_TIM12_IRQn;
    else if (TIMx == TIM12)
        irq = TIM8_TRG_COM_TIM14_IRQn;
    
    NVIC_InitStructure.NVIC_IRQChannel = irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _PreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = _SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
}







/**********************(C)  COPYRIGHT 2017 ��Զ�� *********************************************************/

