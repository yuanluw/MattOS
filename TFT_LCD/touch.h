/***************************************************************************************************
  * ��    ����touch.h   
  * ��    �ߣ��µܱ�
  * ��д���ڣ�2015/7/12
  * ��    ����V1.4
  * �� �� �壺stm32f407ZG--2015�꾺����
  * �޸�ʱ�䣺
  * �� �� ��: keil.4
  *-------------------------------------------------------------------------------------------------
  * ��    �飺     
  *************************************************************************************************/
/* ��ֹ�ݹ�����Ķ���-----------------------------------------------------------------------------*/
#ifndef __TOUCH_H
#define __TOUCH_H 1

#ifdef __cplusplus
 extern "C" {
#endif 
/* ���� ------------------------------------------------------------------------------------------*/
/*���ں���ģ��------------------------------------------------------------------------------------*/
//����״̬	 
#define Key_Down 0x01
#define Key_Up   0x00 
//�ʸ˽ṹ��
typedef struct 
{
	u16 X0;//ԭʼ����
	u16 Y0;
	u16 X; //����/�ݴ�����
	u16 Y;						   	    
	u8  Key_Sta;//�ʵ�״̬			  
//������У׼����
	float xfac;
	float yfac;
	short xoff;
	short yoff;
//�����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
//touchtype=0��ʱ��,�ʺ�����ΪX����,����ΪY�����TP.
//touchtype=1��ʱ��,�ʺ�����ΪY����,����ΪX�����TP.
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
/*-----TOUCHӲ������-----------------------------------------------------------------------------*/
//�봥����оƬ��������	   
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

/*-------����ADS_Read_XY��������-----------------------------------------------------------------*/
#define READ_TIMES 15 //��������
#define LOST_VAL 5	 //����ֵ
//Ĭ��Ϊtouchtype=0������.
#define CMD_RDX 0XD0
#define CMD_RDY 0X90   
/*���ں���ģ��---------------------------------------------------------------------------------- */
//High layer function
void Touch_Init(void);
void Pen_Int_Set(uint8_t State);
uint8_t Read_ADS(uint16_t *x,uint16_t *y);
void Convert_Pos(void);
#ifdef __cplusplus
}
#endif

#endif 




