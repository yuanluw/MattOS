/***************************************************************************************************
  * ��    ����TFT_LCD.h     
  * ��    �ߣ�longwei
  * ��д���ڣ�2012/9/19
  * ��    ����V1.0
  * �� �� �壺stm32f407ZG-ʵ���ҿ�����
  * �޸�ʱ�䣺�� 
  * �� �� ��: keil.4.23
  *------------------------------------------------------------------------------------------------
  * ��    �飺   
  *************************************************************************************************/
/* ��ֹ�ݹ�����Ķ���-----------------------------------------------------------------------------*/
#ifndef __TFT_LCD_H
#define __TFT_LCD_H

#include "stm32f4xx.h"
#include "stdint.h"
 
/* ���� ------------------------------------------------------------------------------------------*/
/* �������� --------------------------------------------------------------------------------------*/
typedef struct 
{
  int16_t X;
  int16_t Y;
}Point, * pPoint;

/** @defgroup FONTS_Exported_Types
  * @{
  */ 
typedef struct _tFont
{  
  const unsigned char (*table)[64];
  uint8_t Height;
  uint8_t Width;
  uint8_t Mode;//�����ַ��Ƿ���ӣ�Ĭ��Ϊ�ǵ���
}sFONT;

/**********************************���ܵ�����******************************************************/
//�Ƿ�ʹ�û�ͼ�����Ŀ���
#define	_USE_GRAPH		1	/* 0:Disable or 1:Enable */
//�Ƿ�ʹ����ʾ����
#define _USE_DisplayOn	1  /* 0:Disable or 1:Enable */
//�Ƿ�ʹ����ʾ�����ַ�
#define _USE_HZK16 2	 /* 0:Disable or 1:Enable */
//�Ƿ�ʹ����ʾ16����������
#define _USE_0X  1	 /* 0:Disable or 1:Enable */
//�Ƿ�ʹ����ʾ����������
#define _USE_Float  1	 /* 0:Disable or 1:Enable */
#define __CONFIG_FONTS 0
//TFT����
#define __TFT_ID   0x1936
/*���ں�------------------------------------------------------------------------------------------*/
/*----------------TFT������-----------------------------------------------------------------------*/
#define LCD_REG_0             0x00
#define LCD_REG_1             0x01
#define LCD_REG_2             0x02
#define LCD_REG_3             0x03
#define LCD_REG_4             0x04
#define LCD_REG_5             0x05
#define LCD_REG_6             0x06
#define LCD_REG_7             0x07
#define LCD_REG_8             0x08
#define LCD_REG_9             0x09
#define LCD_REG_10            0x0A
#define LCD_REG_12            0x0C
#define LCD_REG_13            0x0D
#define LCD_REG_14            0x0E
#define LCD_REG_15            0x0F
#define LCD_REG_16            0x10
#define LCD_REG_17            0x11
#define LCD_REG_18            0x12
#define LCD_REG_19            0x13
#define LCD_REG_20            0x14
#define LCD_REG_21            0x15
#define LCD_REG_22            0x16
#define LCD_REG_23            0x17
#define LCD_REG_24            0x18
#define LCD_REG_25            0x19
#define LCD_REG_26            0x1A
#define LCD_REG_27            0x1B
#define LCD_REG_28            0x1C
#define LCD_REG_29            0x1D
#define LCD_REG_30            0x1E
#define LCD_REG_31            0x1F
#define LCD_REG_32            0x20
#define LCD_REG_33            0x21
#define LCD_REG_34            0x22
#define LCD_REG_36            0x24
#define LCD_REG_37            0x25
#define LCD_REG_40            0x28
#define LCD_REG_41            0x29
#define LCD_REG_43            0x2B
#define LCD_REG_45            0x2D
#define LCD_REG_48            0x30
#define LCD_REG_49            0x31
#define LCD_REG_50            0x32
#define LCD_REG_51            0x33
#define LCD_REG_52            0x34
#define LCD_REG_53            0x35
#define LCD_REG_54            0x36
#define LCD_REG_55            0x37
#define LCD_REG_56            0x38
#define LCD_REG_57            0x39
#define LCD_REG_58            0x3A
#define LCD_REG_59            0x3B
#define LCD_REG_60            0x3C
#define LCD_REG_61            0x3D
#define LCD_REG_62            0x3E
#define LCD_REG_63            0x3F
#define LCD_REG_64            0x40
#define LCD_REG_65            0x41
#define LCD_REG_66            0x42
#define LCD_REG_67            0x43
#define LCD_REG_68            0x44
#define LCD_REG_69            0x45
#define LCD_REG_70            0x46
#define LCD_REG_71            0x47
#define LCD_REG_72            0x48
#define LCD_REG_73            0x49
#define LCD_REG_74            0x4A
#define LCD_REG_75            0x4B
#define LCD_REG_76            0x4C
#define LCD_REG_77            0x4D
#define LCD_REG_78            0x4E
#define LCD_REG_79            0x4F
#define LCD_REG_80            0x50
#define LCD_REG_81            0x51
#define LCD_REG_82            0x52
#define LCD_REG_83            0x53
#define LCD_REG_96            0x60
#define LCD_REG_97            0x61
#define LCD_REG_106           0x6A
#define LCD_REG_118           0x76
#define LCD_REG_128           0x80
#define LCD_REG_129           0x81
#define LCD_REG_130           0x82
#define LCD_REG_131           0x83
#define LCD_REG_132           0x84
#define LCD_REG_133           0x85
#define LCD_REG_134           0x86
#define LCD_REG_135           0x87
#define LCD_REG_136           0x88
#define LCD_REG_137           0x89
#define LCD_REG_139           0x8B
#define LCD_REG_140           0x8C
#define LCD_REG_141           0x8D
#define LCD_REG_143           0x8F
#define LCD_REG_144           0x90
#define LCD_REG_145           0x91
#define LCD_REG_146           0x92
#define LCD_REG_147           0x93
#define LCD_REG_148           0x94
#define LCD_REG_149           0x95
#define LCD_REG_150           0x96
#define LCD_REG_151           0x97
#define LCD_REG_152           0x98
#define LCD_REG_153           0x99
#define LCD_REG_154           0x9A
#define LCD_REG_157           0x9D
#define LCD_REG_192           0xC0
#define LCD_REG_193           0xC1
#define LCD_REG_229           0xE5
#if __TFT_ID==0x1936
//SSD1963
#define  SSD1963_HDP 799 
#define  SSD1963_HT  928 
#define  SSD1963_HPS 46 
#define  SSD1963_LPS 15 
#define  SSD1963_HPW 48 
#define  SSD1963_VDP 479 
#define  SSD1963_VT  525 
#define  SSD1963_VPS 16 
#define  SSD1963_FPS 8 
#define  SSD1963_VPW 16 
#endif
/*----------------TFT��ɫ�궨��-------------------------------------------------------------------*/
/* LCD color */
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

// LCD��ȱʡ���� 
#define LCD_DEFAULT_FONT         Font32x16
#define LCD_DEFAULT_FONT1        Font16x08
//LCD Size (Width and Height)  
#define LCD_Pixel_Width          0x0320
#define LCD_Pixel_Height         0x01E0
#define LCD_Pixel_Max            0x5DC00
#define MAX_Line  (LCD_Pixel_Height/(((sFONT *)LCD_GetFont())->Height))
#define MAX_Column (LCD_Pixel_Width/(((sFONT *)LCD_GetFont())->Width))
#define ASSEMBLE_RGB(R, G, B)    ((((R)& 0xF8) << 8) | (((G) & 0xFC) << 3) | (((B) & 0xF8) >> 3))

/*---------------------���ں꺯��-----------------------------------------------------------------*/  
#if __CONFIG_FONTS==1  
#define Line(x)  ((x)<<4)
#define Column(x)((x)<<3)
#else  
#define Line(x)  ((x) * (((sFONT *)LCD_GetFont())->Height))
#define Column(x)((x) * (((sFONT *)LCD_GetFont())->Width))
#endif
/*---------------------���ں���ģ��---------------------------------------------------------------*/  
void TFT_LCD_Init(void);

void LCD_SetColors(__IO uint16_t _TextColor, __IO uint16_t _BackColor); 
void LCD_GetColors(__IO uint16_t *_TextColor, __IO uint16_t *_BackColor);
void LCD_SetTextColor(__IO uint16_t Color);
void LCD_SetBackColor(__IO uint16_t Color);
void LCD_SetFont(sFONT *fonts,uint8_t Mode);//��������
sFONT *LCD_GetFont(void);	//��ȡ��ǰ����			

void LCD_ClearLine(uint16_t x,uint8_t size,uint16_t color);//����
void LCD_ClearLineChar(uint16_t x,uint16_t y,uint8_t Num,uint8_t size,uint16_t Color);//���ַ���
void LCD_Clear(uint16_t Color);	 //����									  
//ָ����Χ����
void LCD_Appoint_Clear(uint16_t Xs,uint16_t Ys,uint16_t Xe,uint16_t Ye,uint16_t Color);

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t Char);//д�ַ�
uint32_t LCD_DisplayStringLine(uint16_t Line,uint16_t Column, uint8_t *ptr);
void LCD_DisplayNum(uint16_t Line,uint16_t Column, uint32_t num,uint8_t size);
void LCD_ShowCharx(uint16_t x,uint16_t y,uint8_t Char);
uint32_t LCD_DisplayStringLinex(uint16_t Line,uint16_t Column, uint8_t *ptr);
void LCD_DisplayNumx(uint16_t Line,uint16_t Column, uint32_t num);

#if _USE_Float 
void LCD_Display_FloatNum(uint16_t Line,uint16_t Column, double num,uint8_t count);
#endif

#if _USE_0X
void LCD_Display0x(uint16_t Line,uint16_t Column, uint8_t *X,uint8_t N);
#endif

#if _USE_GRAPH
void PutPixel(int16_t x, int16_t y);
void LCD_WeBMP_SIZE(uint16_t *ptr,uint32_t SIZE);
void Show_Picture0(u16 x1,u16 y1,const u8 *pic);
void Show_Picture1(u16 x1,u16 y1,u16 x2,u16 y2,const u8 *pic);
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction);
void LCD_DrawuniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width);
void LCD_DrawRectS(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void LCD_DrawCircleS(uint16_t x,uint16_t y,uint16_t size,uint16_t color);
#endif

/*-------------�м������ģ��-------------------------------------------------------------------*/
void LCD_DrawPoint(uint16_t X,uint16_t Y,uint16_t Col);//������ʽ��ʾ���ص�
#if __TFT_ID==0x1936
void LCD_DrawPoint_4(uint16_t X,uint16_t Y,uint16_t Col);
void LCD_SetDisplayWindow(uint16_t SC,uint16_t SP,uint16_t EC,uint16_t EP);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos); //���ù��
#else
void LCD_SetCursor(uint16_t Xpos, uint8_t Ypos); //���ù��
#endif

void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
uint16_t LCD_ReadReg(uint16_t LCD_Reg);
/*------------�ײ㺯��ģ��------------------------------------------------------------------------*/
#if _USE_DisplayOn 
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
#endif
void LCD_CtrlLinesConfig(void);
void LCD_FSMCConfig(void);
void LCD_WriteRAM_Prepare(void);
#ifdef __cplusplus
}
#endif

#endif /* __TFT_LCD_H */

void LCD_WriteRAM(uint16_t RGB_Code);
void LCD_WriteRAM_Prepare(void);

/******************* (C) ��Ȩ 2012 longwei *****�ļ�����*******************************************/
