/***************************************************************************************************
  * 文 件 名：Character.c
  * 作    者：陈弟斌
  *	日    期：2015.6.29
  * 版    本：Stm32F407ZG
  * 开 发 板：stm32f407ZG--2015竞赛板
  * 修改时间：无 
  * 编 译 器: keil.5
  * 库 版 本：V1.4
  *************************************************************************************************/  
/*--------包含头文件------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stdint.h"
#include "TFT_LCD.h"
#include "Character.h"
#include "bsp_spi_flash.h"
#include "bsp_ui.h"


/*  来自bsp_spi_flash.c*/
extern _font_info ftinfo;


void OS_HzMat_Get(char *code, uint8_t *mat, u8 size)
{
    uint8_t qh, ql;
    uint8_t i;
    uint32_t foffset;

    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size); //得到字体一个字符对应点阵集所占的字节数
    qh = *code;
    ql = *(++code);
    if(qh < 0x81 || ql < 0x40 || ql == 0xff || qh == 0xff) //非常用汉字
    {
        for(i = 0; i < csize; i++)*mat++ = 0x00; //填充满格
        return; //结束访问
    }
    if(ql < 0x7f)ql -= 0x40; //注意!
    else ql -= 0x41;
    qh -= 0x81;
    foffset = ((uint32_t)190 * qh + ql) * csize;	//得到字库中的字节偏移量
    switch(size)
    {
    case 32:
        readData(mat, foffset + ftinfo.f12addr, csize);
        break;
    case 16:
        readData(mat, foffset + ftinfo.f16addr, csize);
        break;
    case 24:
        readData(mat, foffset + ftinfo.f24addr, csize);
        break;

    }
}

void OS_Font_Show(u16 x, u16 y, char *font, u8 size, u8 mode)
{
    uint16_t textColor;
    uint16_t backColor;
   
    u8 temp, t, t1;
    u16 y0 = y;
    u8 dzk[144];
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size); //得到字体一个字符对应点阵集所占的字节数
    if(size != 32 && size != 16 && size != 24) return;	//不支持的size
     LCD_GetColors(&textColor,&backColor);
    OS_HzMat_Get(font, dzk, size);	//得到相应大小的点阵数据
    for(t = 0; t < csize; t++)
    {
        temp = dzk[t];			//得到点阵数据
        for(t1 = 0; t1 < 8; t1++)
        {
            if(temp & 0x80) drawPoint(x, y, textColor);
            else if(mode == 1) drawPoint(x, y, backColor);
            temp <<= 1;
            y++;
            if((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}


