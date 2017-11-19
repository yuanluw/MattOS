#include "stm32f4xx.h"
#include "string.h"
#include "math.h"
#include "stdio.h"

#include "bsp_ui.h"
#include "TFT_LCD.h"
#include "Character.h"
#include "touch.h"
#include "fonts.h"
#include "bsp_spi_flash.h"

#define ABS(X)  ((X) > 0 ? (X) : -(X)) 

/*  ����bsp_spi_flash.c*/
extern _font_info ftinfo;


/**
 * @brief  ָ����������
 * @param  ��
 * @retval ��
 */
void lcdClear(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color)
{
    LCD_Appoint_Clear(xs,ys,xe,ye,color);
}    



/**
 * @brief  ��ʾ�ַ���
 * @param  none 
 * @retval none
 */
void showStr(uint16_t x,uint16_t y,uint8_t*str,uint16_t color)
{
    uint16_t textColor;
    uint16_t backColor;
    LCD_GetColors(&textColor,&backColor);
    LCD_SetTextColor(color);
    
    LCD_DisplayStringLine(y,x,str);
    
    LCD_SetTextColor(textColor);
    
}

/**
 * @brief  ��һ�����ص�
 * @param  none 
 * @retval none
 */
void drawPoint(uint16_t x,uint16_t y,uint16_t color)
{
    uint16_t textColor;
    uint16_t backColor;
    LCD_GetColors(&textColor,&backColor);
    LCD_SetTextColor(color);
    PutPixel(x,y);
    LCD_SetTextColor(textColor);
}

/**
 * @brief  ��ʾ����
 * @param  unit ��ʾ��С��λ��С��λ���಻����ʹ�ô˷��� ���о��ȵ���� 
 * @retval none
 */
void showNum(uint16_t x,uint16_t y,float num,uint16_t color,uint8_t unit)
{
    uint8_t string[12];
    uint8_t  *pstr = string; 
	uint32_t integer = 0 , decimals = 0 ;
	
	uint16_t i = 0;
    float    product = 1.0f;
    uint8_t buf[100];
	
	while(unit--)  //�����С��λ
	{
		product *= 10.0f;
	}
	
	
	if(num<0)
	{
		*pstr++ = '-';
		num = -num;
	}

	integer = (uint32_t)num;
	

    decimals = (uint32_t)((num - (float)integer) * product);
    

	
	do{
	buf[i++] = integer % 10 +'0';
	integer /= 10;
	}while(integer > 0);
		
		
	while(i--)
	{
		*pstr++= buf[i] ;
	}
   
	if(decimals!=0)
	{
		*pstr++ = '.';
	
		for( i=0; i<sizeof(buf) ; i++)
		{
			buf[i] = 0;
		}
	
	
		i=0;
	
	    do{
			buf[i++] = decimals % 10  + '0';
			decimals /= 10;
			}while(decimals > 0);
		
		while(i--)
		{
			*pstr++ = buf[i] ;
		}
    }
	 
	*pstr = '\0';
    
    showStr(x,y,string,color);
    
}


/**
 * @brief  ����ֱ��
 * @param  none 
 * @retval none
 */
void drawLine(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color)
{
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
	curpixel = 0;

    uint16_t textColor;
    uint16_t backColor;
    LCD_GetColors(&textColor,&backColor);
    LCD_SetTextColor(color);
    
	deltax = ABS(xe - xs);        /* The difference between the x's */
	deltay = ABS(ye - ys);        /* The difference between the y's */
	x = xs;                       /* Start x off at the first pixel */
	y = ys;                       /* Start y off at the first pixel */
  
	if (xe >= xs)                 /* The x-values are increasing */
	{
		xinc1 = 1;
		xinc2 = 1;
	}
	else                          /* The x-values are decreasing */
	{
		xinc1 = -1;
		xinc2 = -1;
	}

	if (ye >= ys)                 /* The y-values are increasing */
	{
		yinc1 = 1;
		yinc2 = 1;
	}
	else                          /* The y-values are decreasing */
	{
		yinc1 = -1;
		yinc2 = -1;
	}
  
	if (deltax >= deltay)         /* There is at least one x-value for every y-value */
	{
		xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
		yinc2 = 0;                  /* Don't change the y for every iteration */
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;         /* There are more x-values than y-values */
	}
	else                          /* There is at least one y-value for every x-value */
	{
		xinc2 = 0;                  /* Don't change the x for every iteration */
		yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;         /* There are more y-values than x-values */
	}
  
	for (curpixel = 0; curpixel <= numpixels; curpixel++)
	{
		PutPixel(x, y);             /* Draw the current pixel */
		num += numadd;              /* Increase the numerator by the top of the fraction */
		if (num >= den)             /* Check if numerator >= denominator */
		{
			num -= den;               /* Calculate the new numerator value */
			x += xinc1;               /* Change the x as appropriate */
			y += yinc1;               /* Change the y as appropriate */
		}
		x += xinc2;                 /* Change the x as appropriate */
		y += yinc2;                 /* Change the y as appropriate */
	}
    
    LCD_SetTextColor(textColor);
    
}

/**
 * @brief  ������
 * @param  mode 1 ���  0 ����� 
 * @retval none
 */
void drawRect(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint16_t color,uint8_t mode)
{
    uint16_t i;
    
    if(mode == 0)
    {
        drawLine(xs,ys,xe,ys,color);
        drawLine(xs,ye,xe,ye,color);
        drawLine(xs,ys,xs,ye,color);
        drawLine(xe,ys,xe,ye,color);
    }
    else 
    {
        for(i=ys;i<=ye;i++)
        {
           drawLine(xs,i,xe,i,color); 
        }
    }
    
    
}

/**
 * @brief  ��Բ��
 * @param  mode 1 ���  0 ����� 
 * @retval none
 */
void drawCircle(uint16_t x,uint16_t y,uint16_t r,uint16_t color,uint8_t mode)
{
    
    uint16_t textColor;
    uint16_t backColor;
    LCD_GetColors(&textColor,&backColor);

    if(mode == 0)
    {
        LCD_SetTextColor(color);
        LCD_DrawCircle(x,y,r);
        LCD_SetTextColor(textColor);
    }
    else
    {
        LCD_DrawCircleS(x,y,r,color);
    }
}



/**
 * @brief  д�ַ�
 * @param  size 16 24 32
 * @retval none
 */
void showGB(uint16_t x,uint16_t y,uint8_t*str,uint8_t size)
{
    uint16_t x0 = x;
    uint8_t bHz = 0;   //�ַ���������
     while(*str != 0)		//����δ����
    {

        if(!bHz)
        {
            if((signed char)*str < 0)bHz = 1; //����
            else              //�ַ�
            {
                if(*str == '\n') //���з���
                {
                    y += size;
                    x = x0;
                    str++;
                }
                LCD_ShowChar(x, y, *str);
                str++;
                x += size / 2; //�ַ�,Ϊȫ�ֵ�һ��
            }
        }
        else //����
        {
            bHz = 0; //�к��ֿ�
            OS_Font_Show(x, y, (char*)str, size, 0);
            str += 2;
            x += size; //��һ������ƫ��
        }
    }

}

void textInit(textboxTypeDef t)
{
    uint8_t offset = 0;
    showGB(t.xs,t.ys+5,(uint8_t*)t.labelStr,32);
    offset = strlen((char*)t.labelStr) * 16;
    
    drawRect(t.xs+offset,t.ys,t.xe,t.ye,t.textColor,0);
 
}


/**
 * @brief  Һ������ʼ��
 * @param  none 
 * @retval none
 */
void UIInit(void)
{
    TFT_LCD_Init();
    font_init();
}


/**********************(C)  COPYRIGHT 2017 ��Զ�� *********************************************************/








