/**
  ******************************************************************************
  * @file    fonts.h
  * @author  MCD Application Team/longwei
  * @version V4.5.1
  * @date    19-9-2012
  * @brief   Header for fonts.c
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FONTS_H
#define __FONTS_H
/* define ------------------------------------------------------------------*/
#define __FONTS 2

#include "stdint.h"

/** @defgroup FONTS_Exported_Types
  * @{
  */ 
typedef struct 
{  
  const unsigned char (*table)[64];
  uint8_t Height;
  uint8_t Width;
  uint8_t Mode;//设置字符是否叠加，默认为非叠加
}sFONTTypeDef;


/** @defgroup FONTS_Exported_Constants
  * @{
  */ 

extern const uint8_t Font_1608[95][64];
extern const uint8_t Font_3216[95][64];
extern const uint8_t Font_2412[195][64];
#endif /* __FONTS_H */

/******************* (C) COPYRIGHT 2011 longwei *****END OF FILE***************/
