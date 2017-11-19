#include "bsp_spi_flash.h"
#include "stm32f4xx.h"
#include "bsp_key.h"
#include "delay.h"
/**
 * @brief  ͨ��SPI1��д����  
 * @param  ��
 * @retval ��
 */
u8 spiReadWriteByte(u8 data)
{
    
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  

    SPI_I2S_SendData(SPI1, data); //ͨ������SPIx����һ��byte  ����

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  

    return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����	
    
}

/**
 * @brief  дʹ��  
 * @param  ��
 * @retval ��
 */
void writeEnable(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
    
    spiReadWriteByte(WRITE_ENABLE);
    
    GPIO_SetBits(GPIOB,GPIO_Pin_9); 
}

/**
 * @brief  дʧ��  
 * @param  ��
 * @retval ��
 */
void writeDisable(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
    
    spiReadWriteByte(WRITE_DISABLE);
    
    GPIO_SetBits(GPIOB,GPIO_Pin_9); 
}


/**
 * @brief  д״̬�Ĵ���  
 * @param  ��
 * @retval ��
 */
void writeReg(u8 command)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
    
    spiReadWriteByte(WRITE_STATUS_REG);//����д״̬�Ĵ���ָ��
    
    spiReadWriteByte(command);//дָ��
    
    GPIO_SetBits(GPIOB,GPIO_Pin_9);  
    
}

/**
 * @brief  ��״̬�Ĵ���  
 * @param  ��
 * @retval ��
 */
u8 readReg()
{
    
    u8 byte;
    
    GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
    
    spiReadWriteByte(READ_STATUS_REG1);//���Ͷ�״̬�Ĵ���ָ��
    
    byte = spiReadWriteByte(0xff);//��ָ��
    
    GPIO_SetBits(GPIOB,GPIO_Pin_9);   
    
    return byte;
}

/**
 * @brief  ��ȡ����ID 
 * @param  ��
 * @retval ��
 */
u16 readW25Q64ID(void)
{
    u16 temp;
    GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
    spiReadWriteByte(0x90);
    spiReadWriteByte(0x00);
    spiReadWriteByte(0x00);
    spiReadWriteByte(0x00);
    
    temp |= spiReadWriteByte(0xff) << 8;
    temp |= spiReadWriteByte(0xff);
    
    GPIO_SetBits(GPIOB,GPIO_Pin_9); 
    
    return temp;
    
}

/**
 * @brief  �ȴ�������æ
 * @param  ��
 * @retval ��
 */
void W25QXX_Wait_Busy(void)   
{   
	while((readReg()&0x01)==0x01);   // �ȴ�BUSYλ���
}  

/**
 * @brief  ����һ������
 * @param  ��
 * @retval ��
 */
void clearEraseSector(u32 sector)
{
    sector *= 4096  ;//ƫ�Ƶ���N��������ͷ
    writeEnable();
    W25QXX_Wait_Busy();
    GPIO_ResetBits(GPIOB,GPIO_Pin_9); 

    spiReadWriteByte(SECTOR_ERASE);
    spiReadWriteByte((u8)(sector>>16));
    spiReadWriteByte((u8)(sector>>8));
    spiReadWriteByte((u8)sector);
    
    GPIO_SetBits(GPIOB,GPIO_Pin_9);
    W25QXX_Wait_Busy();
}


/**
 * @brief  ������
 * @param  *pbuffer :�������
           address : ���ĳ�ʼ��ַ
           length : ��ȡ �ĳ���
 * @retval ��
 */
void readData(u8 *pbuffer,u32 address,u16 length)
{
    u16 i;
    
    
    GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
    
    spiReadWriteByte(READ_DATA); //���Ͷ�����ָ��

    spiReadWriteByte((u8)(address>>16));
    spiReadWriteByte((u8)(address>>8));
    spiReadWriteByte((u8)address);
    
    for(i=0 ; i<length ; i++)
    {
        pbuffer[i] = spiReadWriteByte(0xff);   //������
    }
    
    GPIO_SetBits(GPIOB,GPIO_Pin_9); 
    
}




/**
 * @brief  дһҳ����
 * @param  *pbuffer :��д������
           address : д�ĳ�ʼ��ַ
           length : д�ĳ���
 * @retval ��
 */
 void writePage(u8 *pbuffer,u32 address,u16 length)
{
    u16 i;
    writeEnable(); //дʹ�� 
 
    GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
    
    spiReadWriteByte(PAGE_PROGRAM);
    
    spiReadWriteByte((u8)(address>>16)); //���͵�ַ
    spiReadWriteByte((u8)(address>>8));
    spiReadWriteByte((u8)address);
    
    for(i=0 ; i<length ; i++)
    {
        spiReadWriteByte(pbuffer[i]);
    }
 
    GPIO_SetBits(GPIOB,GPIO_Pin_9); 
    
    W25QXX_Wait_Busy();	
}

/**
 * @brief  дһ����������
 * @param  *pbuffer :��д������
           address : д�ĳ�ʼ��ַ
           length : д�ĳ���
 * @retval ��
 */
static void writeSector(u8 *pbuffer,u32 address,u16 length)
{
    u16 pageLeftover;
    pageLeftover = 256 - address%256;
    
    if(length <= pageLeftover) pageLeftover = length;
    while(1)
    {
        writePage(pbuffer,address,pageLeftover);
        
        if(length == pageLeftover)
        {
            break;
        }
        else
        {
            pbuffer += pageLeftover;
            address += pageLeftover;
            
            length -= pageLeftover;
            
            if(length > 256)
            {
                pageLeftover = 256;
            }
            else
            {
                pageLeftover = length;
            }
        }
    }
}


/**
 * @brief  д���� 
 * @param  *pbuffer :��д������
           address : д�ĳ�ʼ��ַ
           length : д�ĳ���
 * @retval ��
 */
u8 readTempBuff[4096];//��Ϊ�����ݵĻ�����
void writeData(u8 *pbuffer,u32 address,u16 length)
{
    u32 firstAdd;
    u16 offset;
    u16 leftover;
    u16 i;
    
    firstAdd = address/4096; //�������ڵڼ�������
    offset = address%4096;  //����ƫ�Ƶ�ַ
    leftover = 4096 - offset; 
    
    if(length < leftover)
    {
        leftover = length;
    }
    
    while(1)
    {
        readData(readTempBuff,firstAdd*4096,4096); // ������������������
        
        for(i=0 ; i<leftover ; i++)
        {
            if(readTempBuff[offset + i] != 0xFF) //���������������ԭ���е����� ��Ҫ����
            {
                break;
            }
        }
        
        if(i < leftover)
        {
            clearEraseSector(firstAdd); //������������
            
            for(i=0 ; i<leftover ; i++)
            {
               readTempBuff[i + offset] =  pbuffer[i];
            }
            
            writeSector(readTempBuff,firstAdd*4096,4096); 
            
        }
        else
        {
           writeSector(pbuffer,address,leftover);
        }
        
        if(length == leftover)
        {
            break; //д������
        }
        else
        {
            firstAdd ++;
            offset = 0;
            
            pbuffer += leftover;
            address += leftover;
            length -= leftover;
            
            if(length > 4096)
            {
                leftover = 4096;
            }
            else
            {
                leftover = length;
            }
            
        }
        
    }
    
}


/**
 * @brief  SPI1��ʼ��  
 * @param  ��
 * @retval ��
 */
u8 spi1Init(void)
{
    
    u16 temp;
    
    GPIO_InitTypeDef GPIO_InitStruct;
    SPI_InitTypeDef  SPI_InitStruct;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

    //IO�ڸ���    
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
    
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;   //clk
    GPIO_Init(GPIOA,&GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;   //IN
    GPIO_Init(GPIOA,&GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;   //OUT
    GPIO_Init(GPIOA,&GPIO_InitStruct);
     
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;   //CS
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    //����Ƭѡ�ź�
    GPIO_SetBits(GPIOB,GPIO_Pin_9); 
    
    SPI_DeInit(SPI1);
    
    //����Ϊ8λ���� ˫��ͨ��
    SPI_InitStruct.SPI_BaudRatePrescaler= SPI_BaudRatePrescaler_4; //84/4 ����Ϊ21M ����ģʽ
    SPI_InitStruct.SPI_CPHA             = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_CPOL             = SPI_CPOL_High;
    SPI_InitStruct.SPI_CRCPolynomial    = 7;
    SPI_InitStruct.SPI_DataSize         = SPI_DataSize_8b;
    SPI_InitStruct.SPI_Direction        = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_FirstBit         = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_Mode             = SPI_Mode_Master;
    SPI_InitStruct.SPI_NSS              = SPI_NSS_Soft;
    SPI_Init(SPI1,&SPI_InitStruct);
    
    SPI_Cmd(SPI1,ENABLE);
    
    temp  = readW25Q64ID();
    
    if(temp == W25Q64ID) //���������Ƿ��д�ɹ�
    {
        return True;
    }
    return False;
}





//�ֿ�����ռ�õ�����������С(3���ֿ�+unigbk��+�ֿ���Ϣ=3238700�ֽ�,Լռ791��W25QXX����)
#define FONTSECSIZE	 	791
//�ֿ�����ʼ��ַ 
#define FONTINFOADDR 	1024*1024*12 					//STM32F4�Ǵ�12M��ַ�Ժ�ʼ����ֿ�
														//ǰ��12M��fatfsռ����.
														//12M�Ժ����3���ֿ�+UNIGBK.BIN,�ܴ�С3.09M,���ֿ�ռ����,���ܶ�!
														//15.10M�Ժ�,�û���������ʹ��.����������100K�ֽڱȽϺ�.
														
//���������ֿ������Ϣ����ַ����С��
_font_info ftinfo;


//��ʼ������
//����ֵ:0,�ֿ����.
//		 ����,�ֿⶪʧ
u8 font_init(void)
{		
	u16 t=0;
	spi1Init();  
	while(t<10)//������ȡ10��,���Ǵ���,˵��ȷʵ��������,�ø����ֿ���
	{
		t++;
		readData((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//����ftinfo�ṹ������
		if(ftinfo.fontok==0XAA)break; 
		delay_ms(20); 
	}
	if(ftinfo.fontok!=0XAA)return 1;
	return 0;		    
}





/**********************(C)  COPYRIGHT 2017 ��Զ�� *********************************************************/






