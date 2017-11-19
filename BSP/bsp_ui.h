#ifndef BSP_UI_H
#define BSP_UI_H

#include "stdint.h"

#define White          0xFFFF //白色
#define Black          0x0000 //黑色
#define Grey           0xF7DE //灰色
#define Blue           0x001F //蓝色
#define Blue2          0x051F //深蓝色
#define Red            0xF800 //红色
#define Magenta        0xF81F //品红
#define Green          0x07E0 //绿色
#define Cyan           0x7FFF //蓝绿色
#define Yellow         0xFFE0 //黄色
#define BROWN 			   0XBC40 //棕色
#define BRRED 			   0XFC70 //棕红色
#define GRAY  			   0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


typedef struct
{
    uint16_t labelStr[20];
    uint16_t xs;
    uint16_t xe;
    uint16_t ys;
    uint16_t ye;
    uint8_t str[20];
    uint16_t length;
    uint16_t textColor;
    uint16_t backColor;
    uint32_t value;
}textboxTypeDef;





void UIInit(void);
void drawPoint(uint16_t x,uint16_t y,uint16_t color);
void showGB(uint16_t x,uint16_t y,uint8_t*str,uint8_t size);
void lcdClear(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color);
void showStr(uint16_t x,uint16_t y,uint8_t*str,uint16_t color);
void showNum(uint16_t x,uint16_t y,float num,uint16_t color,uint8_t unit);
void drawLine(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color);
void drawRect(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color,uint8_t mode);
void drawCircle(uint16_t x,uint16_t y,uint16_t r,uint16_t color,uint8_t mode);
void showSTR(uint16_t x,uint16_t y,uint8_t*str,uint8_t size,uint8_t mode);
void textInit(textboxTypeDef t);


#endif




/**********************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/



