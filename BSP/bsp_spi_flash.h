#ifndef BSP_SPI_FLASH_H
#define BSP_SPI_FLASH_H

#include "stdint.h"


#define  W25Q64ID    0xEF16


#define  WRITE_ENABLE       0x06
#define  WRITE_DISABLE      0x04
#define  READ_STATUS_REG1   0x05
#define  READ_STATUS_REG2   0x35
#define  WRITE_STATUS_REG   0x01
#define  PAGE_PROGRAM       0x02
#define  READ_DATA          0x03

#define  QUAD_PAGE_PROGRAM  0x32
#define  BLOCK_ERASE32KB    0xD8
#define  BLOCK_ERASE64KB    0x52
#define  SECTOR_ERASE       0x20
#define  CHIP_ERASE         0xC7
#define  ERASE_SUSPEND      0x75
#define  ERASE_RESUME       0x7A
#define  POWER_DOWN         0xB9



__packed typedef struct 
{
	u8 fontok;				//字库存在标志，0XAA，字库正常；其他，字库不存在
	u32 ugbkaddr; 			//unigbk的地址
	u32 ugbksize;			//unigbk的大小	 
	u32 f12addr;			//gbk12地址	
	u32 gbk12size;			//gbk12的大小	 
	u32 f16addr;			//gbk16地址
	u32 gbk16size;			//gbk16的大小		 
	u32 f24addr;			//gbk24地址
	u32 gkb24size;			//gbk24的大小 
}_font_info; 



uint8_t spi1Init(void);

void writeData(uint8_t *pbuffer,uint32_t address,uint16_t length);
void readData(uint8_t *pbuffer,uint32_t address,uint16_t length);
void clearEraseSector(uint32_t sector);
u8 font_init(void);


#endif



/**********************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/

