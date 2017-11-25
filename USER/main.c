#include "stm32f4xx.h"
#include "stdio.h"

#include "main.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_pwm.h"
#include "bsp_adc.h"
#include "bsp_ui.h"
#include "bspUart.h"
#include "delay.h"

#include "app.h"




/**
 * @brief  ÷˜≥Ã–Ú
 * @param  none 
 * @retval none
 */
int main()
{
   
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delayInit(168);
	ledInit();
	keyInit();
    uartInit(115200);
    
  
    appRun();
   
    return 0;
  
}





/******************************************(C)  COPYRIGHT 2017 Œ‚‘∂„Ú *********************************************************/



