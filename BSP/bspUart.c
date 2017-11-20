#include "stm32f4xx.h"
#include "stdio.h"


#include "bspUart.h"

uint8_t sendDataBuff[1024];


/**
 * @brief  DMA����
 * @param  none 
 * @retval none
 */
static void uartDmaConfig(void)
{
    
    DMA_InitTypeDef  DMA_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
    
    DMA_DeInit(DMA2_Stream7);
    
    while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE)
    {
        
    }//�ȴ�DMA������ 

     /* ���� DMA Stream */
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //ͨ��ѡ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;//DMA�����ַ
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&sendDataBuff;//DMA �洢��0��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�洢��������ģʽ
    DMA_InitStructure.DMA_BufferSize = UART_SEND_LENGTH;//���ݴ����� 
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
    DMA_Init(DMA2_Stream7, &DMA_InitStructure);//��ʼ��DMA Stream
    
    
}


/**
 * @brief  ����1��ʼ��
 * @param  bound ������ 
 * @retval none
 */
void uartInit(uint32_t bound)
{
      //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

    //����1��Ӧ���Ÿ���ӳ��
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 

    //USART1�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure); 

    //USART1 ��ʼ������
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

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
    NVIC_Init(&NVIC_InitStructure);	

    
    printf("%s","test\r\n");
}




/**
 * @brief  �ض���printf
 * @param  none
 * @retval none
 */
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}



/**
 * @brief  ����һ�����ݷ���
 * @param  none
 * @retval none
 */
void startSendData(void)
{
    
    USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����     
    

    DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//���DMA2_Steam7������ɱ�־

    DMA_Cmd(DMA2_Stream7, DISABLE);    

    while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE){}	//ȷ��DMA���Ա�����  
		
	DMA_SetCurrDataCounter(DMA2_Stream7,UART_SEND_LENGTH);          //���ݴ�����  
 
	DMA_Cmd(DMA2_Stream7, ENABLE);                      //����DMA���� 
}



/**
 * @brief  DMA����
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









/**********************(C)  COPYRIGHT 2017 ��Զ�� *********************************************************/
