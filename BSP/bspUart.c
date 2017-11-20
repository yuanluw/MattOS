#include "stm32f4xx.h"
#include "stdio.h"


#include "bspUart.h"

uint8_t sendDataBuff[1024];


/**
 * @brief  DMA配置
 * @param  none 
 * @retval none
 */
static void uartDmaConfig(void)
{
    
    DMA_InitTypeDef  DMA_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 
    
    DMA_DeInit(DMA2_Stream7);
    
    while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE)
    {
        
    }//等待DMA可配置 

     /* 配置 DMA Stream */
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;//DMA外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&sendDataBuff;//DMA 存储器0地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
    DMA_InitStructure.DMA_BufferSize = UART_SEND_LENGTH;//数据传输量 
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
    DMA_Init(DMA2_Stream7, &DMA_InitStructure);//初始化DMA Stream
    
    
}


/**
 * @brief  串口1初始化
 * @param  bound 波特率 
 * @retval none
 */
void uartInit(uint32_t bound)
{
      //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

    //串口1对应引脚复用映射
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 

    //USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure); 

    //USART1 初始化设置
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE); 

    USART_ClearFlag(USART1, USART_FLAG_TC);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
    NVIC_Init(&NVIC_InitStructure);	

    
    printf("%s","test\r\n");
}




/**
 * @brief  重定向printf
 * @param  none
 * @retval none
 */
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}



/**
 * @brief  进行一次数据发送
 * @param  none
 * @retval none
 */
void startSendData(void)
{
    
    USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //使能串口1的DMA发送     
    

    DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//清除DMA2_Steam7传输完成标志

    DMA_Cmd(DMA2_Stream7, DISABLE);    

    while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE){}	//确保DMA可以被设置  
		
	DMA_SetCurrDataCounter(DMA2_Stream7,UART_SEND_LENGTH);          //数据传输量  
 
	DMA_Cmd(DMA2_Stream7, ENABLE);                      //开启DMA传输 
}



/**
 * @brief  DMA测试
 * @param  none
 * @retval none
 */
void dmaTest(void)
{
    uint16_t i=0;
    
    for(i=0 ; i<UART_SEND_LENGTH; i++)
    {
        sendDataBuff[i] = (i%255);
    }
    startSendData();
    
}









/**********************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/
