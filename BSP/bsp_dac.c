#include "stm32f4xx.h"
#include "bsp_dac.h"

uint16_t dacBuffer[DAC_LENGTH] = {0};

/**
 * @brief  dac  DMA TIM6触发
 * @param  无
 * @retval 无
 */
void dacInit(void)
{
    
    GPIO_InitTypeDef GPIO_InitStruct;
    DMA_InitTypeDef  DMA_InitStruct;
    DAC_InitTypeDef  DAC_InitStruct;
    
    //使能相应时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_DMA1,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
    
    //配置IO口
    GPIO_InitStruct.GPIO_Mode   =  GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_PuPd   =  GPIO_PuPd_DOWN; 
    GPIO_InitStruct.GPIO_Pin    =  GPIO_Pin_4|GPIO_Pin_5;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
    
    //配置DAC1
    DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude   = DAC_LFSRUnmask_Bit0;
    DAC_InitStruct.DAC_OutputBuffer   = DAC_OutputBuffer_Disable;
    DAC_InitStruct.DAC_Trigger     =  DAC_Trigger_T6_TRGO;
    DAC_InitStruct.DAC_WaveGeneration    = DAC_WaveGeneration_None;
    DAC_Init(DAC_Channel_1,&DAC_InitStruct);
    
    
    
    DMA_DeInit(DMA1_Stream5);
    
    DMA_InitStruct.DMA_Mode                  = DMA_Mode_Circular;   //循环模式
    DMA_InitStruct.DMA_Channel               = DMA_Channel_7;
    DMA_InitStruct.DMA_BufferSize            = DAC_LENGTH;
    DMA_InitStruct.DMA_DIR                   = DMA_DIR_MemoryToPeripheral;
    DMA_InitStruct.DMA_FIFOMode              = DMA_FIFOMode_Disable;   //禁止FIFO
    DMA_InitStruct.DMA_FIFOThreshold         = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStruct.DMA_PeripheralBaseAddr    = (u32)&(DAC->DHR12R1);
    DMA_InitStruct.DMA_Memory0BaseAddr       = (u32)dacBuffer;
    DMA_InitStruct.DMA_PeripheralBurst       = DMA_PeripheralBurst_Single;
    DMA_InitStruct.DMA_MemoryBurst           = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc             = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_Priority              = DMA_Priority_High;
    DMA_Init(DMA1_Stream5,&DMA_InitStruct);
    
    DMA_Cmd(DMA1_Stream5,ENABLE);
    
    DAC_DMACmd(DAC_Channel_1,ENABLE);
    
    DAC_Cmd(DAC_Channel_1,ENABLE);
    
    DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
    

}







/**********************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/

