#ifndef BSP_UI_H
#define BSP_UI_H

#include "stdint.h"

#define White          0xFFFF //��ɫ
#define Black          0x0000 //��ɫ
#define Grey           0xF7DE //��ɫ
#define Blue           0x001F //��ɫ
#define Blue2          0x051F //����ɫ
#define Red            0xF800 //��ɫ
#define Magenta        0xF81F //Ʒ��
#define Green          0x07E0 //��ɫ
#define Cyan           0x7FFF //����ɫ
#define Yellow         0xFFE0 //��ɫ
#define BROWN 			   0XBC40 //��ɫ
#define BRRED 			   0XFC70 //�غ�ɫ
#define GRAY  			   0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			     0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


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




/**********************(C)  COPYRIGHT 2017 ��Զ�� *********************************************************/



