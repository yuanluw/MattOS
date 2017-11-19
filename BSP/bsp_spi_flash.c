#include "bsp_spi_flash.h"
#include "stm32f4xx.h"
#include "bsp_key.h"
#include "delay.h"
/**
 * @brief  通过SPI1读写数据  
 * @param  无
 * @retval 无
 */
u8 spiReadWriteByte(u8 data)
{
    
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  

    SPI_I2S_SendData(SPI1, data); //通过外设SPIx发送一个byte  数据

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  

    return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
    
}

/**
 * @brief  写使能  
 * @param  无
 * @retval 无
 */
void writeEnable(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
    
    spiReadWriteByte(WRITE_ENABLE);
    
    GPIO_SetBits(GPIOB,GPIO_Pin_9); 
}

/**
 * @brief  写失能  
 * @param  无
 * @retval 无
 */
void writeDisable(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
    
    spiReadWriteByte(WRITE_DISABLE);
    
    GPIO_SetBits(GPIOB,GPIO_Pin_9); 
}


/**
 * @brief  写状态寄存器  
 * @param  无
 * @retval 无
 */
void writeReg(u8 command)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
    
    spiReadWriteByte(WRITE_STATUS_REG);//发送写状态寄存器指令
    
    spiReadWriteByte(command);//写指令
    
    GPIO_SetBits(GPIOB,GPIO_Pin_9);  
    
}

/**
 * @brief  读状态寄存器  
 * @param  无
 * @retval 无
 */
u8 readReg()
{
    
    u8 byte;
    
    GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
    
    spiReadWriteByte(READ_STATUS_REG1);//发送读状态寄存器指令
    
    byte = spiReadWriteByte(0xff);//读指令
    
    GPIO_SetBits(GPIOB,GPIO_Pin_9);   
    
    return byte;
}

/**
 * @brief  读取器件ID 
 * @param  无
 * @retval 无
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
 * @brief  等待器件不忙
 * @param  无
 * @retval 无
 */
void W25QXX_Wait_Busy(void)   
{   
	while((readReg()&0x01)==0x01);   // 等待BUSY位清空
}  

/**
 * @brief  擦除一个扇区
 * @param  无
 * @retval 无
 */
void clearEraseSector(u32 sector)
{
    sector *= 4096  ;//偏移到第N个扇区开头
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
 * @brief  读数据
 * @param  *pbuffer :填充数组
           address : 读的初始地址
           length : 读取 的长度
 * @retval 无
 */
void readData(u8 *pbuffer,u32 address,u16 length)
{
    u16 i;
    
    
    GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
    
    spiReadWriteByte(READ_DATA); //发送读数据指令

    spiReadWriteByte((u8)(address>>16));
    spiReadWriteByte((u8)(address>>8));
    spiReadWriteByte((u8)address);
    
    for(i=0 ; i<length ; i++)
    {
        pbuffer[i] = spiReadWriteByte(0xff);   //读数据
    }
    
    GPIO_SetBits(GPIOB,GPIO_Pin_9); 
    
}




/**
 * @brief  写一页数据
 * @param  *pbuffer :代写入数组
           address : 写的初始地址
           length : 写的长度
 * @retval 无
 */
 void writePage(u8 *pbuffer,u32 address,u16 length)
{
    u16 i;
    writeEnable(); //写使能 
 
    GPIO_ResetBits(GPIOB,GPIO_Pin_9); 
    
    spiReadWriteByte(PAGE_PROGRAM);
    
    spiReadWriteByte((u8)(address>>16)); //发送地址
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
 * @brief  写一个扇区数据
 * @param  *pbuffer :代写入数组
           address : 写的初始地址
           length : 写的长度
 * @retval 无
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
 * @brief  写数据 
 * @param  *pbuffer :代写入数组
           address : 写的初始地址
           length : 写的长度
 * @retval 无
 */
u8 readTempBuff[4096];//作为读数据的缓存区
void writeData(u8 *pbuffer,u32 address,u16 length)
{
    u32 firstAdd;
    u16 offset;
    u16 leftover;
    u16 i;
    
    firstAdd = address/4096; //计算是在第几个扇区
    offset = address%4096;  //计算偏移地址
    leftover = 4096 - offset; 
    
    if(length < leftover)
    {
        leftover = length;
    }
    
    while(1)
    {
        readData(readTempBuff,firstAdd*4096,4096); // 读出整个扇区的内容
        
        for(i=0 ; i<leftover ; i++)
        {
            if(readTempBuff[offset + i] != 0xFF) //代表这个扇区存在原来有的数据 需要擦除
            {
                break;
            }
        }
        
        if(i < leftover)
        {
            clearEraseSector(firstAdd); //擦除整个扇区
            
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
            break; //写完数据
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
 * @brief  SPI1初始化  
 * @param  无
 * @retval 无
 */
u8 spi1Init(void)
{
    
    u16 temp;
    
    GPIO_InitTypeDef GPIO_InitStruct;
    SPI_InitTypeDef  SPI_InitStruct;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

    //IO口复用    
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
    
    //拉高片选信号
    GPIO_SetBits(GPIOB,GPIO_Pin_9); 
    
    SPI_DeInit(SPI1);
    
    //配置为8位主机 双向通信
    SPI_InitStruct.SPI_BaudRatePrescaler= SPI_BaudRatePrescaler_4; //84/4 设置为21M 高速模式
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
    
    if(temp == W25Q64ID) //检验驱动是否编写成功
    {
        return True;
    }
    return False;
}





//字库区域占用的总扇区数大小(3个字库+unigbk表+字库信息=3238700字节,约占791个W25QXX扇区)
#define FONTSECSIZE	 	791
//字库存放起始地址 
#define FONTINFOADDR 	1024*1024*12 					//STM32F4是从12M地址以后开始存放字库
														//前面12M被fatfs占用了.
														//12M以后紧跟3个字库+UNIGBK.BIN,总大小3.09M,被字库占用了,不能动!
														//15.10M以后,用户可以自由使用.建议用最后的100K字节比较好.
														
//用来保存字库基本信息，地址，大小等
_font_info ftinfo;


//初始化字体
//返回值:0,字库完好.
//		 其他,字库丢失
u8 font_init(void)
{		
	u16 t=0;
	spi1Init();  
	while(t<10)//连续读取10次,都是错误,说明确实是有问题,得更新字库了
	{
		t++;
		readData((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//读出ftinfo结构体数据
		if(ftinfo.fontok==0XAA)break; 
		delay_ms(20); 
	}
	if(ftinfo.fontok!=0XAA)return 1;
	return 0;		    
}





/**********************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/






