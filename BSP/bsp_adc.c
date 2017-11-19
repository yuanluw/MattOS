#include "stm32f4xx.h"
#include "bsp_adc.h"

uint16_t adc1Buffer[ADC1_LENGTH] = {0};
uint16_t adc2Buffer[ADC2_LENGTH] = {0};
uint16_t adc3Buffer[ADC3_LENGTH] = {0};


/**
 * @brief  ADC1����ΪDMA TIM2����
 * @param  none
 * @retval ��
 */
void adc1Init(void)
{
    
    GPIO_InitTypeDef GPIO_InitStruct;
    ADC_InitTypeDef  ADC_InitStruct;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    DMA_InitTypeDef  DMA_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    
    //����������ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
    
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA,&GPIO_InitStruct); 
    
    
    ADC_CommonInitStructure.ADC_DMAAccessMode      =  ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_Mode               =  ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler          =  ADC_Prescaler_Div2;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay   =  ADC_TwoSamplingDelay_5Cycles; //�������
    ADC_CommonInit(&ADC_CommonInitStructure);
    
    
    
    ADC_InitStruct.ADC_Resolution            =  ADC_Resolution_12b;
     // ��ֹɨ��ģʽ����ͨ���ɼ�����Ҫ	
    ADC_InitStruct.ADC_ScanConvMode          =  DISABLE;
    //����ת��
    ADC_InitStruct.ADC_ContinuousConvMode    =  DISABLE;

    ADC_InitStruct.ADC_ExternalTrigConvEdge  =  ADC_ExternalTrigConvEdge_Rising;

    ADC_InitStruct.ADC_ExternalTrigConv      =  ADC_ExternalTrigConv_T2_TRGO;

    ADC_InitStruct.ADC_DataAlign             =  ADC_DataAlign_Right;
    //ת��ͨ����
    ADC_InitStruct.ADC_NbrOfConversion       =  1;

    ADC_Init(ADC1,&ADC_InitStruct);
    
    //����ת��ͨ��
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_3Cycles); 
    
    DMA_DeInit(DMA2_Stream0);
    DMA_InitStruct.DMA_Channel               = DMA_Channel_0;
    DMA_InitStruct.DMA_BufferSize            = ADC1_LENGTH;
    DMA_InitStruct.DMA_DIR                   = DMA_DIR_PeripheralToMemory;
    DMA_InitStruct.DMA_FIFOMode              = DMA_FIFOMode_Disable; //��ֹFIFO
    DMA_InitStruct.DMA_Memory0BaseAddr       = (uint32_t)adc1Buffer;
    DMA_InitStruct.DMA_PeripheralBaseAddr    = ADC1_ADDRESS;
    DMA_InitStruct.DMA_MemoryBurst           = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_PeripheralBurst       = DMA_PeripheralBurst_Single;
    DMA_InitStruct.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_Mode                  = DMA_Mode_Circular; //ѭ��
    DMA_InitStruct.DMA_Priority              = DMA_Priority_Medium;
    DMA_InitStruct.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;  //��ַ������
    DMA_InitStruct.DMA_MemoryInc             = DMA_MemoryInc_Enable; //��ַ����
    DMA_Init(DMA2_Stream0,&DMA_InitStruct);
    
    
    DMA_ClearFlag(DMA2_Stream0,DMA_FLAG_TCIF0);
    //����������� �ж� 
    DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);
    
    NVIC_InitStruct.NVIC_IRQChannel    = DMA2_Stream0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =  3;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
    
    DMA_Cmd(DMA2_Stream0,ENABLE);
    
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
    
    ADC_Cmd(ADC1,ENABLE);
    
    ADC_DMACmd(ADC1,ENABLE);
}




/**
 * @brief  ADC2����ΪDMA �����������
 * @param  none
 * @retval ��
 */
void adc2Init(void)
{
    
    GPIO_InitTypeDef GPIO_InitStruct;
    ADC_InitTypeDef  ADC_InitStruct;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    DMA_InitTypeDef  DMA_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
        //����������ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB ,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
    
    
    //channel 2
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_2;   
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA,&GPIO_InitStruct); 
    
    //channel 3
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_3;
    GPIO_Init(GPIOA,&GPIO_InitStruct); 
    
    //channel 9
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_1;
    GPIO_Init(GPIOB,&GPIO_InitStruct); 
    
    
    
    ADC_CommonInitStructure.ADC_DMAAccessMode      =  ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_Mode               =  ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler          =  ADC_Prescaler_Div2;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay   =  ADC_TwoSamplingDelay_5Cycles; //�������
    ADC_CommonInit(&ADC_CommonInitStructure);
    
    
    
    ADC_InitStruct.ADC_Resolution            =  ADC_Resolution_12b;
     // ɨ��ģʽ	
    ADC_InitStruct.ADC_ScanConvMode          =  ENABLE;
    //����ת��
    ADC_InitStruct.ADC_ContinuousConvMode    =  ENABLE;

    ADC_InitStruct.ADC_ExternalTrigConvEdge  =  ADC_ExternalTrigConvEdge_None;
    //ADC_InitStruct.ADC_ExternalTrigConv      =  ADC_ExternalTrigConv_T2_TRGO;

    ADC_InitStruct.ADC_DataAlign             =  ADC_DataAlign_Right;
    //ת��ͨ���� 
    ADC_InitStruct.ADC_NbrOfConversion       =  3;

    ADC_Init(ADC2,&ADC_InitStruct);
    
     //����ת��ͨ��
    ADC_RegularChannelConfig(ADC2, ADC_Channel_2, 1, ADC_SampleTime_3Cycles); 
    ADC_RegularChannelConfig(ADC2, ADC_Channel_3, 2, ADC_SampleTime_3Cycles); 
    ADC_RegularChannelConfig(ADC2, ADC_Channel_9, 3, ADC_SampleTime_3Cycles); 
    
    
    DMA_DeInit(DMA2_Stream2);
    DMA_InitStruct.DMA_Channel               = DMA_Channel_1;
    DMA_InitStruct.DMA_BufferSize            = ADC2_LENGTH;
    DMA_InitStruct.DMA_DIR                   = DMA_DIR_PeripheralToMemory;
    DMA_InitStruct.DMA_FIFOMode              = DMA_FIFOMode_Disable; //��ֹFIFO
    DMA_InitStruct.DMA_Memory0BaseAddr       = (uint32_t)adc2Buffer;
    DMA_InitStruct.DMA_PeripheralBaseAddr    = ADC2_ADDRESS;
    DMA_InitStruct.DMA_MemoryBurst           = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_PeripheralBurst       = DMA_PeripheralBurst_Single;
    DMA_InitStruct.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_Mode                  = DMA_Mode_Circular; //ѭ��
    DMA_InitStruct.DMA_Priority              = DMA_Priority_Medium;
    DMA_InitStruct.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;  //��ַ������
    DMA_InitStruct.DMA_MemoryInc             = DMA_MemoryInc_Enable; //��ַ����
    DMA_Init(DMA2_Stream2,&DMA_InitStruct);
    
    DMA_ClearFlag(DMA2_Stream2,DMA_FLAG_TCIF2);
    //����������� �ж� 
    DMA_ITConfig(DMA2_Stream2,DMA_IT_TC,ENABLE);
    
    NVIC_InitStruct.NVIC_IRQChannel    = DMA2_Stream2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =  3;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
    
    DMA_Cmd(DMA2_Stream2,ENABLE);
    
    ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);
    
    ADC_Cmd(ADC2,ENABLE);
    
    ADC_DMACmd(ADC2,ENABLE);
    
    ADC_SoftwareStartConv(ADC2);
}


/**
 * @brief  ADC3��ʼ�� 
 * @param  none
 * @retval ��
 */
void adc3Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    ADC_InitTypeDef  ADC_InitStruct;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
 
    
        //����������ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA,&GPIO_InitStruct); 
    
    ADC_CommonInitStructure.ADC_DMAAccessMode      =  ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_Mode               =  ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler          =  ADC_Prescaler_Div4;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay   =  ADC_TwoSamplingDelay_5Cycles; //�������
    ADC_CommonInit(&ADC_CommonInitStructure);
    
     
    ADC_InitStruct.ADC_Resolution            =  ADC_Resolution_12b;
     // ɨ��ģʽ	
    ADC_InitStruct.ADC_ScanConvMode          =  DISABLE;
    //����ת��
    ADC_InitStruct.ADC_ContinuousConvMode    =  DISABLE;

    ADC_InitStruct.ADC_ExternalTrigConvEdge  =  ADC_ExternalTrigConvEdge_None;
    //ADC_InitStruct.ADC_ExternalTrigConv      =  ADC_ExternalTrigConv_T2_TRGO;

    ADC_InitStruct.ADC_DataAlign             =  ADC_DataAlign_Right;
    //ת��ͨ���� 
    ADC_InitStruct.ADC_NbrOfConversion       =  1;

    ADC_Init(ADC3,&ADC_InitStruct);

    ADC_Cmd(ADC3,ENABLE);
}



/**
 * @brief  ��ȡadc��ת����� 
 * @param  ch ADC_Channel_0 ~ ADC_Channel_16
 * @retval ��
 */
uint16_t getAdcValue(u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC3);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC3);	//�������һ��ADC1�������ת�����
}


/**********************(C)  COPYRIGHT 2017 ��Զ�� *********************************************************/


