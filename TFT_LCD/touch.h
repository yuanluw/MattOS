/***************************************************************************************************
  * 文    件：touch.h   
  * 作    者：陈弟斌
  * 编写日期：2015/7/12
  * 版    本：V1.4
  * 开 发 板：stm32f407ZG--2015年竞赛板
  * 修改时间：
  * 编 译 器: keil.4
  *-------------------------------------------------------------------------------------------------
  * 简    介：     
  *************************************************************************************************/
/* 防止递归包含的定义-----------------------------------------------------------------------------*/
#ifndef __TOUCH_H
#define __TOUCH_H 1

#ifdef __cplusplus
 extern "C" {
#endif 
/* 包含 ------------------------------------------------------------------------------------------*/
/*出口函数模型------------------------------------------------------------------------------------*/
//按键状态	 
#define Key_Down 0x01
#define Key_Up   0x00 
//笔杆结构体
typedef struct 
{
	u16 X0;//原始坐标
	u16 Y0;
	u16 X; //最终/暂存坐标
	u16 Y;						   	    
	u8  Key_Sta;//笔的状态			  
//触摸屏校准参数
	float xfac;
	float yfac;
	short xoff;
	short yoff;
//新增的参数,当触摸屏的左右上下完全颠倒时需要用到.
//touchtype=0的时候,适合左右为X坐标,上下为Y坐标的TP.
//touchtype=1的时候,适合左右为Y坐标,上下为X坐标的TP.
	u8 touchtype;
}Pen_Holder;	
typedef struct
{
  volatile uint8_t Key_Flag;
  volatile uint8_t Keynum;
  volatile uint8_t Keycnt;
} Touch_TypeDef;

extern Pen_Holder Pen_Point;
extern Touch_TypeDef K;
/*-----TOUCH硬件连接-----------------------------------------------------------------------------*/
//与触摸屏芯片连接引脚	   
//#define T_PEN  GPIOD_IN(6)   //PG0  INT
//#define T_MISO GPIOB_IN(14)   //PG5  MISO
//#define T_MOSI GPIOB_OUT(15)  //PG2  MOSI
//#define T_SCK  GPIOB_OUT(13)  //PG1  SCLK
//#define T_CS   GPIOC_OUT(13) //PG3 CS 

#define T_PEN GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6)
#define T_MISO GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)
#define T_MOSI_1 GPIO_SetBits(GPIOB, GPIO_Pin_15)
#define T_MOSI_0 GPIO_ResetBits(GPIOB, GPIO_Pin_15)
#define T_SCK_1 GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define T_SCK_0 GPIO_ResetBits(GPIOB, GPIO_Pin_13)
#define T_CS_1 GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define T_CS_0 GPIO_ResetBits(GPIOC, GPIO_Pin_13)

/*-------函数ADS_Read_XY参数设置-----------------------------------------------------------------*/
#define READ_TIMES 15 //采样次数
#define LOST_VAL 5	 //舍弃值
//默认为touchtype=0的数据.
#define CMD_RDX 0XD0
#define CMD_RDY 0X90   
/*出口函数模型---------------------------------------------------------------------------------- */
//High layer function
void Touch_Init(void);
void Pen_Int_Set(uint8_t State);
uint8_t Read_ADS(uint16_t *x,uint16_t *y);
void Convert_Pos(void);
#ifdef __cplusplus
}
#endif

#endif 




