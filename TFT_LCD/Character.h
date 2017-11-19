/***************************************************************************************************
  * 文 件 名：Character.h
  * 作    者：陈弟斌
  *	日    期：2015.6.29
  * 版    本：Stm32F407ZG
  * 开 发 板：stm32f407ZG--2015竞赛板
  * 修改时间：无 
  * 编 译 器: keil.5
  * 库 版 本：V1.4
  *************************************************************************************************/  

/* -------防止递归包含的定义-----------------------------------------------------------------------*/
#ifndef __CHARACTER_H
#define __CHARACTER_H
/*--------包含头文件------------------------------------------------------------------------------*/
#include "stdint.h"
/*---------------------出口函数模型---------------------------------------------------------------*/  
void OS_HzMat_Get(char *code, uint8_t *mat, u8 size);
void OS_Font_Show(u16 x, u16 y, char *font, u8 size, u8 mode);
#endif
