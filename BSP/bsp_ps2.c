#include "string.h"
#include "stm32f4xx.h"
#include "bsp_ps2.h" 
#include "bsp_ui.h"
#include "delay.h"
#include "TFT_LCD.h"
/* 私有宏 ------------------------------------------------------------------------------------*/
#define PS2_SCL   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)
#define PS2_SDA   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)
#define OPEN_CODE         0xf0   //断码
#define LONG_KEY_COUNT    3      //长按键计数
#define PS2_Count   10000
/* 私有变量 ----------------------------------------------------------------------------------*/
__IO uint32_t Count = PS2_Count;


/* 私有结构体 --------------------------------------------------------------------------------*/
__KEY_StateMachine  Key_FSM_PS2;

/* 私有函数模型 ------------------------------------------------------------------------------*/


/**********************************************************************************************
 *名    称：void PS2_GPIO_Init(void)
 *
 *参    数：无
 *
 *返 回 值：无
 *
 *描    述：PS2 IO口初始化
 *********************************************************************************************/
static void PS2_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/**********************************************************************************************
 *名    称：static void PS2_SCL_Set(uint8_t Mode)
 *
 *参    数：uint8_t Mode    1：输出   0：输入
 *
 *返 回 值：无
 *
 *描    述：SCL IO端口状态设置
 *********************************************************************************************/
static void PS2_SCL_Set(uint8_t Mode)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	if(Mode)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	}
	else
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	}
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	if(Mode)
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
}

/**********************************************************************************************
 *名    称：static uint8_t PS2_SCL_Wait(uint8_t Mode)
 *
 *参    数：uint8_t Mode  1等待上升沿   0等待下降沿
 *
 *返 回 值：1等待成功  0等待超时
 *
 *描    述：PS2 SCL等待跳变
 *********************************************************************************************/
static uint8_t PS2_SCL_Wait(uint8_t Mode)
{
	if(Mode)
	{
		while((!PS2_SCL) && Count)
		Count--;
		if(!Count)
		{
			Count = PS2_Count;
			PS2_SCL_Set(1);
			return 0;
		}
		else
			return 1;
	}
	else
	{
		while(PS2_SCL && Count)
		Count--;
		if(!Count)
		{
			Count = PS2_Count;
			PS2_SCL_Set(1);
			return 0;
		}
		else
			return 1;
	}
}


/**********************************************************************************************
 *名    称：void PS2_ReadKeyCodon(volatile uint8_t* Key_Num)
 *
 *参    数：volatile uint8_t* Key_Num  键码接收缓冲区指针
 *
 *返 回 值：void
 *
 *描    述：PS2 键码读取函数
 *********************************************************************************************/
void PS2_ReadKeyCodon(volatile uint8_t* Key_Num)
{
	uint8_t i;
	PS2_SCL_Set(0);
	if(!PS2_SCL_Wait(0))
	{
		*Key_Num = 0;
		return;
	}
	if(!PS2_SCL_Wait(1))
	{
		*Key_Num = 0;
		return;
	}
	if(!PS2_SCL_Wait(0))
	{
		*Key_Num = 0;
		return;
	}
	for(i = 0;i < 8;i++)
	{
		*Key_Num >>= 1;
		if(PS2_SDA)
			*Key_Num |= 0x80;
		if(!PS2_SCL_Wait(1))
		{
			*Key_Num = 0;
			return;
		}
		if(!PS2_SCL_Wait(0))
		{
			*Key_Num = 0;
			return;
		}
	}
	if(!PS2_SCL_Wait(1))
	{
		*Key_Num = 0;
		return;
	}
	if(!PS2_SCL_Wait(0))
	{
		*Key_Num = 0;
		return;
	}
	if(!PS2_SCL_Wait(1))
	{
		*Key_Num = 0;
		return;
	}
	PS2_SCL_Set(1);
}


/**********************************************************************************************
 *名    称：void Key_StateSweep(__KEY_StateMachine *Key_FSM)
 *
 *参    数：__KEY_StateMachine *Key_FSM  状态机结构体指针
 *
 *返 回 值：void
 *
 *描    述：状态机扫描函数
 *********************************************************************************************/
void Key_StateSweep(__KEY_StateMachine *Key_FSM)
{
	PS2_ReadKeyCodon(&Key_FSM ->Key_Code_Pre);
	if(Key_FSM ->Key_Code_Pre == 0)
	{
		Key_FSM ->Key_Filt++;
		if(Key_FSM ->Key_Filt > 50)
		{
			Key_FSM ->Key_Filt = 0;
			Key_FSM ->State_Flag = 0;
		}
		return;
	}
	else
		Key_FSM ->Key_Filt = 0;
	switch(Key_FSM ->State_Flag)
	{
		case 0:
			if((Key_FSM ->Key_Code_Pre != 0) && (Key_FSM ->Key_Code_Pre != OPEN_CODE))
			{
				Key_FSM ->Key_Code_Bef = Key_FSM ->Key_Code_Pre;
				Key_FSM ->State_Flag = 1;
				Key_FSM ->Key_Mode = 1;
			}
			else
			{
				Key_FSM ->Key_Code_Bef = 0;
				Key_FSM ->Key_Mode = 0;
				Key_FSM ->State_Flag = 0;
			}
			break;
			
		case 1:
			if(Key_FSM ->Key_Code_Bef == Key_FSM ->Key_Code_Pre)
			{
				Key_FSM ->State_Flag = 1;   //停留在第二级计数
				Key_FSM ->Key_Count++;
				if(Key_FSM ->Key_Count == LONG_KEY_COUNT)
				{
					Key_FSM ->State_Flag = 2;
					Key_FSM ->Key_Count = 0;
					Key_FSM ->Key_Mode = 2;  //独立长按键
				}
			}
			else if(Key_FSM ->Key_Code_Pre == OPEN_CODE)
			{
				Key_FSM ->Key_Code_Bef = Key_FSM ->Key_Code_Pre;
				Key_FSM ->State_Flag = 1;
			}
			else
			{
				Key_FSM ->Key_Count = 0;
				Key_FSM ->State_Flag = 0;
				Key_FSM ->Key_Code_Bef = 0;
			}
			break;
			
		case 2:
			if(Key_FSM ->Key_Code_Bef == Key_FSM ->Key_Code_Pre)
			{
					Key_FSM ->Key_Mode = 3;   //独立持续按键
			}
			else if(Key_FSM ->Key_Code_Pre == OPEN_CODE)
			{
				Key_FSM ->Key_Code_Bef = Key_FSM ->Key_Code_Pre;
				Key_FSM ->State_Flag = 2;
			}
			else
			{
				Key_FSM ->Key_Mode = 0;
				Key_FSM ->State_Flag = 0;
			}
			break;
	}
}


ps2InputData ps2Data;

/**
 * @brief  ps2 初始化
 * @param  无
 * @retval 无
 */
void PS2_Keyboard_Init(void)
{
	PS2_GPIO_Init(); 
    ps2Data.length = 0;
    ps2Data.num = 0 ;
    ps2Data.inputFlag = False;

}



/**
 * @brief  ps2按键扫描
 * @param  无
 * @retval 无
 */
textboxTypeDef ps2Text;
void ps2Scan(void)
{
    uint8_t data ;
    
    Key_StateSweep(&Key_FSM_PS2);
    
    if(Key_Num_0)
    { 
        data='0',Key_FSM_PS2.Key_Mode = 0;
    }
    else if(Key_Num_1)
    {
        data='1';Key_FSM_PS2.Key_Mode = 0;
    }
    else if(Key_Num_2)
    {
        data='2';Key_FSM_PS2.Key_Mode = 0;
    }
    else if(Key_Num_3)
    {
        data='3';Key_FSM_PS2.Key_Mode = 0;
    }
    else if(Key_Num_4)
    {
        data='4';Key_FSM_PS2.Key_Mode = 0;
    }
    else if(Key_Num_5)
    {
        data='5';Key_FSM_PS2.Key_Mode = 0;
    }
    else if(Key_Num_6)
    {
        data='6';Key_FSM_PS2.Key_Mode = 0;
    }
    else if(Key_Num_7)
    {
       data='7';Key_FSM_PS2.Key_Mode = 0;
    }
    else if(Key_Num_8)
    {
        data='8';Key_FSM_PS2.Key_Mode = 0;
    }
    else if(Key_Num_9)
    {
        data='9';Key_FSM_PS2.Key_Mode = 0;
    }
    else if(Key_Enter)
    {
        data='E';Key_FSM_PS2.Key_Mode = 0;
    }
    else if(Key_Add)
    {
        data='+';Key_FSM_PS2.Key_Mode = 0;
    }
    else if(Key_Minus)
    {
        data='-';Key_FSM_PS2.Key_Mode = 0;
    }
    else if(Key_Ride)
    {
        data = '*';Key_FSM_PS2.Key_Mode = 0;
    }
    else if(Key_Div)
    {
        data='/';Key_FSM_PS2.Key_Mode = 0;
    }
    else if(Key_NumLock)
    {
        data='N';Key_FSM_PS2.Key_Mode = 0;

    }
    else if(Key_Point)
    {
        data='.';Key_FSM_PS2.Key_Mode = 0;

    }
    else if(Key_Back)
    {
        data='B';Key_FSM_PS2.Key_Mode = 0;

    }
    else 
    {
        Key_FSM_PS2.Key_Mode = 0;
        return ;
    }
    
    if(data == 'N')
    {
        if(ps2Data.inputFlag == False)
        {

            ps2Data.inputFlag = True; 
            strcpy((char*)ps2Text.labelStr,"输入:");
            ps2Text.length = 0;
            ps2Text.textColor = Blue;
            ps2Text.backColor = Black;
            ps2Text.xs = 550;
            ps2Text.xe = 780;
            ps2Text.ys = 400;
            ps2Text.ye = 450;
            ps2Text.value = 0 ;
            textInit(ps2Text);
        }
        else
        {
            ps2Data.inputFlag = False; 
            lcdClear( ps2Text.xs-5, ps2Text.ys-5, ps2Text.xe + 5, ps2Text.ye+5,ps2Text.backColor); 
            ps2Text.value = 0 ;
 
            ps2Text.str[0] = '\0';

            ps2Text.length = 0;
        }

        
    }
    else if(ps2Data.inputFlag == True)
    {
        if(data == 'E')
        {
            
        }
        else if(data == 'B')
        {
            if(ps2Text.length > 0)
            {
                 ps2Text.str[--ps2Text.length] = '\0';
            }
            else
            {
                ps2Text.str[ps2Text.length] = '\0';
            }
            
            
             ps2Text.value -= (ps2Text.value%10);
             ps2Text.value/=10;
             lcdClear(ps2Text.xs + strlen((char*)ps2Text.labelStr)*16, ps2Text.ys-5, ps2Text.xe + 5, ps2Text.ye+5,ps2Text.backColor); 
             textInit(ps2Text);
        }
        else 
        {
            ps2Text.str[ps2Text.length++] = data;
            ps2Text.str[ps2Text.length] = '\0';
            
            if(data>='0' && data <='9')
            {
                ps2Text.value =  ps2Text.value * 10 + (data-'0');
            }
        
        }
        showStr(ps2Text.xs + strlen((char*)ps2Text.labelStr)*17,ps2Text.ys + 10,(uint8_t*)ps2Text.str,ps2Text.textColor);

    }   
 
}



/**********************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/



