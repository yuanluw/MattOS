#ifndef BSP_MALLOC_H
#define BSP_MALLOC_H

#include "MattOS.h"
#include "stdint.h"



#define BLOCK_USE_FLAG 0x80000000

#define MIN_BLOCK_SIZE  (sizeof(BLOCK_T) << 1)

#ifndef True
#define True 1
#endif

#ifndef False
#define False 0
#endif

#ifndef NULL
#define NULL 0
#endif

typedef struct BLOCK_T
{
   struct BLOCK_T *nextFreeBlock;
   uint32_t blockSize;
}BLOCK_T;


uint8_t heapInit(void);
void *myMalloc(uint32_t size);
void myFree(void *freeBlock);
uint32_t getFreeByteRemaining(void);





#endif

/**********************(C)  COPYRIGHT 2017 ÎâÔ¶ãò *********************************************************/

