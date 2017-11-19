/***************************************************************************************************
  * �� �� ����Character.c
  * ��    �ߣ��µܱ�
  *	��    �ڣ�2015.6.29
  * ��    ����Stm32F407ZG
  * �� �� �壺stm32f407ZG--2015������
  * �޸�ʱ�䣺�� 
  * �� �� ��: keil.5
  * �� �� ����V1.4
  *************************************************************************************************/  
/*--------����ͷ�ļ�------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stdint.h"
#include "TFT_LCD.h"
#include "Character.h"
#include "bsp_spi_flash.h"
#include "bsp_ui.h"


/*  ����bsp_spi_flash.c*/
extern _font_info ftinfo;


void OS_HzMat_Get(char *code, uint8_t *mat, u8 size)
{
    uint8_t qh, ql;
    uint8_t i;
    uint32_t foffset;

    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size); //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    qh = *code;
    ql = *(++code);
    if(qh < 0x81 || ql < 0x40 || ql == 0xff || qh == 0xff) //�ǳ��ú���
    {
        for(i = 0; i < csize; i++)*mat++ = 0x00; //�������
        return; //��������
    }
    if(ql < 0x7f)ql -= 0x40; //ע��!
    else ql -= 0x41;
    qh -= 0x81;
    foffset = ((uint32_t)190 * qh + ql) * csize;	//�õ��ֿ��е��ֽ�ƫ����
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
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size); //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    if(size != 32 && size != 16 && size != 24) return;	//��֧�ֵ�size
     LCD_GetColors(&textColor,&backColor);
    OS_HzMat_Get(font, dzk, size);	//�õ���Ӧ��С�ĵ�������
    for(t = 0; t < csize; t++)
    {
        temp = dzk[t];			//�õ���������
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


