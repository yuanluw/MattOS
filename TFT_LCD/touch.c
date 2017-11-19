/***************************************************************************************************
  * 文    件：touch.c   
  * 作    者：陈弟斌
  * 编写日期：2015/7/12
  * 版    本：V1.4
  * 开 发 板：stm32f407ZG--2015年竞赛板
  * 修改时间：
  * 编 译 器: keil.4
  *-------------------------------------------------------------------------------------------------
  * 简    介：     
  *************************************************************************************************/

/*包含--------------------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stdint.h"
#include "delay.h"
#include "touch.h"

/* 私有宏-----------------------------------------------------------------------------------------*/
/* 私有变量 --------------------------------------------------------------------------------------*/
Touch_TypeDef K={0,0xff,0};
Pen_Holder Pen_Point;//定义笔实体
/* 私有函数模型 ----------------------------------------------------------------------------------*/
static void Delay(__IO uint32_t nCount);
static void	GPIO_Configuration(void);
static void EXTILine13_Config(void);
static uint16_t ADS_Read_AD(uint8_t CMD);
static uint16_t ADS_Read_XY(uint8_t xy);
uint8_t Read_ADS(uint16_t *x,uint16_t *y);
static void ADS_Write_Byte(uint8_t data);
uint8_t Read_ADS2(u16 *x,u16 *y);
/* 私有定义结构体 --------------------------------------------------------------------------------*/
#define Touch_Delay() Delay(1)

/***************************************************************************************************
  * 描述   : 触屏硬件初始化初始化
  * 参数   : 无 
  * 返回   : 无
  * 注意   :
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
  * 描述   :  根据触摸屏的校准参数来决定转换后的结果,保存在X0,Y0中
  * 参数   :  无
  * 返回   :  无
  * 注意   :
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
  * 描述   : 带滤波的坐标读取，滤波的最小值不能少于100
  * 参数   : 坐标的数据指针
  * 返回   : 错误码
  * 注意   :
  *************************************************************************************************/
uint8_t Read_ADS(uint16_t *x,uint16_t *y)
{
	uint16_t xtemp,ytemp;			 	 		  
	xtemp=ADS_Read_XY(CMD_RDX);
	ytemp=ADS_Read_XY(CMD_RDY);	  												   
	if(xtemp<100||ytemp<100)return 0;//读数失败
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}
/***************************************************************************************************
  * 描述   : 触屏中断开关                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
  * 参数   : State: 0 开启 1 关闭
  * 返回   : 无
  * 说明   :  
  *************************************************************************************************/
void Pen_Int_Set(uint8_t State)
{
	if(State)EXTI->IMR|=1<<13;   
	else EXTI->IMR&=~(1<<13); 
}	  
/***************************************************************************************************
  * 描述   : 读取一个坐标值
  * 参数   : xy:单轴坐标
  * 返回   : 读取的值
  * 说明   : 连续读取READ_TIMES次数据,对这些数据升序排列,
	*          然后去掉最低和最高LOST_VAL个数,取平均 
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
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
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
  * 描述   : 向触屏控制器读数据 
  * 参数   : CMD:命令字 读取的数据
  * 返回   : 无
  * 说明   : 从7846/7843/XPT2046/UH7843/UH7846读取adc值
  *************************************************************************************************/
static uint16_t ADS_Read_AD(uint8_t CMD)	  
{ 	 
	uint8_t count=0; 	  
	u16 Num=0; 
	T_SCK_0;//先拉低时钟 	 
	T_CS_0; //选中ADS7843	
	Touch_Delay();		
	ADS_Write_Byte(CMD);//发送命令字
	delay_us(6);//ADS7846的转换时间最长为6us
	T_SCK_1;//给1个时钟，清除BUSY   
	Touch_Delay();	
	T_SCK_0; 	
	Touch_Delay();	
	for(count=0;count<16;count++)  
	{ 				  
		Num<<=1; 	 
		T_SCK_0;//下降沿有效  
		Touch_Delay();		
		T_SCK_1;
		Touch_Delay();
		if(T_MISO)Num++; 	
		Touch_Delay();		
	}  	
	Num>>=4;   //只有高12位有效.
	T_CS_1;//释放ADS7843	 
	Touch_Delay();
	return(Num);   
} 
/***************************************************************************************************
  * 描述   : 向触屏控制器写数据
  * 参数   : data:要写入的数据
  * 返回   : 无
  * 说明   :
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
		T_SCK_0;//上升沿有效	   
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
  * 描述   : 触屏中断线配置和使能
  * 参数   : 无
  * 返回   : 无
  * 说明   :
  *************************************************************************************************/
static void EXTILine13_Config(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	/*使能 SYSCFG 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* 连接EXTI Line13 到PD.13引脚*///说明：线号和I/O口号是对应的
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource13);
	/*配置EXTI Line8 */
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
  * 描述   : touch的I/O配置
  * 参数   : 无
  * 返回   : 无
  * 说明   :
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

  /* 配置PA.6位浮空输入 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* 配置PD.13位浮空输入 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	T_MOSI_1;
	T_SCK_1;
	T_CS_1;
}
/***************************************************************************************************
  * 描述   : 延时函数
  * 参数   : 无
  * 返回   : 无
  * 说明   :
  *************************************************************************************************/
static void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}
