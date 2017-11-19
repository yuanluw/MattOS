#include "bsp_led.h"
#include "stm32f4xx.h"
#include "delay.h"



#define LED1_PORT     GPIOB
#define LED1_PIN      GPIO_Pin_7
#define LED1_CLK      RCC_AHB1Periph_GPIOB

#define LED2_PORT     GPIOB
#define LED2_PIN      GPIO_Pin_8
#define LED2_CLK      RCC_AHB1Periph_GPIOB

#define LED3_PORT     GPIOC
#define LED3_PIN      GPIO_Pin_6
#define LED3_CLK      RCC_AHB1Periph_GPIOC

#define LED4_PORT     GPIOC
#define LED4_PIN      GPIO_Pin_7
#define LED4_CLK      RCC_AHB1Periph_GPIOC


/**
 * @brief  led控制 
 * @param  无
 * @retval 无
 */
static void LED_Control(u8 flag)
{
			switch(flag)
		{
			case 0: LED1 = 0; LED2 = 1; LED3 = 1; LED4 = 1; break;
			case 1: LED1 = 1; LED2 = 0; LED3 = 1; LED4 = 1; break;
			case 2: LED1 = 1; LED2 = 1; LED3 = 0; LED4 = 1; break;
			case 3: LED1 = 1; LED2 = 1; LED3 = 1; LED4 = 0; break;
		  default : LED1 = 1; LED2 = 1; LED3 = 1; LED4 = 1; break;
		}
	
}


/**
 * @brief  ledIo口配置 
 * @param  无
 * @retval 无
 */
void ledInit(void)
{
	
	
	GPIO_InitTypeDef   GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(LED1_CLK|LED2_CLK|LED3_CLK|LED4_CLK,ENABLE);
	
	
	GPIO_InitStruct.GPIO_Mode    = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType   = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin     = LED1_PIN;
	GPIO_InitStruct.GPIO_PuPd    = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed   = GPIO_Speed_50MHz;
	
	GPIO_Init(LED1_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin     = LED2_PIN;
	GPIO_Init(LED2_PORT,&GPIO_InitStruct);
	
	
	GPIO_InitStruct.GPIO_Pin     = LED3_PIN;
	GPIO_Init(LED3_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin     = LED4_PIN;
	GPIO_Init(LED4_PORT,&GPIO_InitStruct);
	
}




/**
 * @brief  led灯运行 
 * @param  无
 * @retval 无
 */
void ledControl(void)
{
   static uint8_t ledCnt =0 ;
	 
	 LED_Control(ledCnt++);
	 
	 if(ledCnt & 0x04)
	 {
		 ledCnt &= 0;
	 }
}



/**********************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/

