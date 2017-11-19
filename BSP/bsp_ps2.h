#ifndef __PS2_H
#define __PS2_H

#include "stdint.h"




/* 私有宏 ------------------------------------------------------------------------------------*/
#define Key_Num_0  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_0) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_1  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_1) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_2  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_2) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_3  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_3) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_4  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_4) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_5  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_5) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_6  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_6) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_7  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_7) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_8  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_8) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Num_9  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_9) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Enter  			((Key_FSM_PS2.Key_Code_Bef == Key_Code_Enter) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Add    			((Key_FSM_PS2.Key_Code_Bef == Key_Code_Add) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Minus       ((Key_FSM_PS2.Key_Code_Bef == Key_Code_Minus) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Ride       	((Key_FSM_PS2.Key_Code_Bef == Key_Code_Ride) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Div    			((Key_FSM_PS2.Key_Code_Bef == Key_Code_Div) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_NumLock			((Key_FSM_PS2.Key_Code_Bef == Key_Code_NumLock) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Point       ((Key_FSM_PS2.Key_Code_Bef == Key_Code_Point) && (Key_FSM_PS2.Key_Mode == 1))
#define Key_Back        ((Key_FSM_PS2.Key_Code_Bef == Key_Code_Back) && (Key_FSM_PS2.Key_Mode == 1))


#define Key_Code_0  				0x70
#define Key_Code_1  				0x69
#define Key_Code_2  				0x72
#define Key_Code_3  				0x7a
#define Key_Code_4  				0x6b
#define Key_Code_5  				0x73
#define Key_Code_6  				0x74
#define Key_Code_7  				0x6c
#define Key_Code_8  				0x75
#define Key_Code_9  				0x7d
#define Key_Code_Enter  		0xE0
#define Key_Code_Add        0x79
#define Key_Code_Minus      0x7b
#define Key_Code_Ride       0x7c
#define Key_Code_Div        0x4a
#define Key_Code_NumLock    0x77
#define Key_Code_Point      0x71
#define Key_Code_Back       0x66



#ifndef True
#define True 1
#endif

#ifndef False
#define False 0
#endif


typedef struct 
{
	volatile uint16_t State_Flag;       //状态机状态标志
	volatile uint8_t  Key_Code_Pre;     //当前键码
	volatile uint8_t  Key_Code_Bef;     //前一次键码
	volatile uint16_t Key_Mode;         //按键模式
	volatile uint16_t Key_Count;        //长按键计数
	volatile uint16_t Key_Filt;         //PS2数据滤波
}__KEY_StateMachine;





typedef struct
{
    uint8_t str[20];     //输入字符串
    uint32_t num;       //输入值
    uint8_t inputFlag;  //输入框是否启用
    uint8_t length;     
    
}ps2InputData;




/* 函数声明 ----------------------------------------------------------------------------------*/

void PS2_Keyboard_Init(void);
void ps2Scan(void);



#endif






