/***************************************************************************************************
  * ��    ����touch.c   
  * ��    �ߣ��µܱ�
  * ��д���ڣ�2015/7/12
  * ��    ����V1.4
  * �� �� �壺stm32f407ZG--2015�꾺����
  * �޸�ʱ�䣺
  * �� �� ��: keil.4
  *-------------------------------------------------------------------------------------------------
  * ��    �飺     
  *************************************************************************************************/

/*����--------------------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stdint.h"
#include "delay.h"
#include "touch.h"

/* ˽�к�-----------------------------------------------------------------------------------------*/
/* ˽�б��� --------------------------------------------------------------------------------------*/
Touch_TypeDef K={0,0xff,0};
Pen_Holder Pen_Point;//�����ʵ��
/* ˽�к���ģ�� ----------------------------------------------------------------------------------*/
static void Delay(__IO uint32_t nCount);
static void	GPIO_Configuration(void);
static void EXTILine13_Config(void);
static uint16_t ADS_Read_AD(uint8_t CMD);
static uint16_t ADS_Read_XY(uint8_t xy);
uint8_t Read_ADS(uint16_t *x,uint16_t *y);
static void ADS_Write_Byte(uint8_t data);
uint8_t Read_ADS2(u16 *x,u16 *y);
/* ˽�ж���ṹ�� --------------------------------------------------------------------------------*/
#define Touch_Delay() Delay(1)

/***************************************************************************************************
  * ����   : ����Ӳ����ʼ����ʼ��
  * ����   : �� 
  * ����   : ��
  * ע��   :
  *************************************************************************************************/
 void Touch_Init(void)
{
	uint16_t Pen_Point_X=0,Pen_Point_Y=0;
	Pen_Point.xfac=-0.42997;
	Pen_Point.yfac=-0.25159;
	Pen_Point.xoff=836;
	Pen_Point.yoff=500;
	GPIO_Configuration();
	Read_ADS(&Pen_Point_X,&Pen_Point_Y);
	delay_us(1000);
	EXTILine13_Config();
}
/***************************************************************************************************
  * ����   :  ���ݴ�������У׼����������ת����Ľ��,������X0,Y0��
  * ����   :  ��
  * ����   :  ��
  * ע��   :
  *************************************************************************************************/
void Convert_Pos(void)
{		 	  
	if(Read_ADS(&Pen_Point.X,&Pen_Point.Y))
	{
		Pen_Point.X0=Pen_Point.xfac*Pen_Point.X+Pen_Point.xoff;
		if(Pen_Point.X0 > 400)
			Pen_Point.X0 = 400-(Pen_Point.X0-400);
		else if(Pen_Point.X0 < 400)
			Pen_Point.X0 = 400+(400-Pen_Point.X0);
		Pen_Point.Y0=Pen_Point.yfac*Pen_Point.Y+Pen_Point.yoff;  
	}
}
/***************************************************************************************************
  * ����   : ���˲��������ȡ���˲�����Сֵ��������100
  * ����   : ���������ָ��
  * ����   : ������
  * ע��   :
  *************************************************************************************************/
uint8_t Read_ADS(uint16_t *x,uint16_t *y)
{
	uint16_t xtemp,ytemp;			 	 		  
	xtemp=ADS_Read_XY(CMD_RDX);
	ytemp=ADS_Read_XY(CMD_RDY);	  												   
	if(xtemp<100||ytemp<100)return 0;//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}
/***************************************************************************************************
  * ����   : �����жϿ���                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
  * ����   : State: 0 ���� 1 �ر�
  * ����   : ��
  * ˵��   :  
  *************************************************************************************************/
void Pen_Int_Set(uint8_t State)
{
	if(State)EXTI->IMR|=1<<13;   
	else EXTI->IMR&=~(1<<13); 
}	  
/***************************************************************************************************
  * ����   : ��ȡһ������ֵ
  * ����   : xy:��������
  * ����   : ��ȡ��ֵ
  * ˵��   : ������ȡREAD_TIMES������,����Щ������������,
	*          Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ�� 
  *************************************************************************************************/
static uint16_t ADS_Read_XY(uint8_t xy)
{
	uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint16_t sum=0;
	uint16_t temp;
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=ADS_Read_AD(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)//����
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
/***************************************************************************************************
  * ����   : ���������������� 
  * ����   : CMD:������ ��ȡ������
  * ����   : ��
  * ˵��   : ��7846/7843/XPT2046/UH7843/UH7846��ȡadcֵ
  *************************************************************************************************/
static uint16_t ADS_Read_AD(uint8_t CMD)	  
{ 	 
	uint8_t count=0; 	  
	u16 Num=0; 
	T_SCK_0;//������ʱ�� 	 
	T_CS_0; //ѡ��ADS7843	
	Touch_Delay();		
	ADS_Write_Byte(CMD);//����������
	delay_us(6);//ADS7846��ת��ʱ���Ϊ6us
	T_SCK_1;//��1��ʱ�ӣ����BUSY   
	Touch_Delay();	
	T_SCK_0; 	
	Touch_Delay();	
	for(count=0;count<16;count++)  
	{ 				  
		Num<<=1; 	 
		T_SCK_0;//�½�����Ч  
		Touch_Delay();		
		T_SCK_1;
		Touch_Delay();
		if(T_MISO)Num++; 	
		Touch_Delay();		
	}  	
	Num>>=4;   //ֻ�и�12λ��Ч.
	T_CS_1;//�ͷ�ADS7843	 
	Touch_Delay();
	return(Num);   
} 
/***************************************************************************************************
  * ����   : ����������д����
  * ����   : data:Ҫд�������
  * ����   : ��
  * ˵��   :
  *************************************************************************************************/
static void ADS_Write_Byte(uint8_t data)    
{  
	uint8_t count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(data&0x80)T_MOSI_1;  
		else T_MOSI_0;   
		data<<=1;  
		Touch_Delay();			
		T_SCK_0;//��������Ч	   
		Touch_Delay();		
		T_SCK_1; 
		Touch_Delay();			
	} 			    
}

void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line13) != RESET)
  {
		Convert_Pos(); 
		K.Key_Flag=1;
		EXTI_ClearITPendingBit(EXTI_Line13);
  }
}

/***************************************************************************************************
  * ����   : �����ж������ú�ʹ��
  * ����   : ��
  * ����   : ��
  * ˵��   :
  *************************************************************************************************/
static void EXTILine13_Config(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	/*ʹ�� SYSCFG ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* ����EXTI Line13 ��PD.13����*///˵�����ߺź�I/O�ں��Ƕ�Ӧ��
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource13);
	/*����EXTI Line8 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_ClearITPendingBit(EXTI_Line13);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	Pen_Int_Set(0);
} 
/***************************************************************************************************
  * ����   : touch��I/O����
  * ����   : ��
  * ����   : ��
  * ˵��   :
  *************************************************************************************************/
static void	GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_13;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* ����PA.6λ�������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* ����PD.13λ�������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	T_MOSI_1;
	T_SCK_1;
	T_CS_1;
}
/***************************************************************************************************
  * ����   : ��ʱ����
  * ����   : ��
  * ����   : ��
  * ˵��   :
  *************************************************************************************************/
static void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}
