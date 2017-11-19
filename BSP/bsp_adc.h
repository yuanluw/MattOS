#ifndef BSP_ADC_H
#define BSP_ADC_H

#include "stdint.h"

#define ADC1_LENGTH  1024
#define ADC1_ADDRESS ((uint32_t)0x4001204C) 


#define ADC2_LENGTH   1024
#define ADC2_ADDRESS  ((uint32_t)0x4001214C) 


#define ADC3_LENGTH   1024
#define ADC3_ADDRESS  ((uint32_t)0x4002214C) 

void adc1Init(void);
void adc2Init(void);

void adc3Init(void);
uint16_t getAdcValue(u8 ch);
#endif









/**********************(C)  COPYRIGHT 2017 Œ‚‘∂„Ú *********************************************************/
