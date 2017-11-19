#include "bspMalloc.h"



static uint8_t heap[HEAP_SIZE];
static BLOCK_T heapStart,*heapEnd = NULL;
//剩余内存大小
static uint32_t freeByteRemaining = 0U;
//记录当前最小的内存块
static uint32_t minFreeByteRemaining = 0U;




/**
 * @brief  插入内存块
 * @param  none
 * @retval none
 */
static void insertBLock(BLOCK_T *newBlock)
{
    BLOCK_T *iterator;
    uint8_t *p;
    //找到插入内存块的下一块内存  按地址排序
    for(iterator = &heapStart; iterator->nextFreeBlock < newBlock; iterator = iterator->nextFreeBlock)
    {
         
    }
    
    p = (uint8_t*)iterator;
    
    //可以合并
    if((p + iterator->blockSize) == (uint8_t*)newBlock)
    {
        iterator->blockSize += newBlock->blockSize;
        newBlock = iterator;
    }
    
    p = (uint8_t*)newBlock;
    //可以和后面合并
    if((p + newBlock->blockSize) == (uint8_t*) iterator->nextFreeBlock)
    {
        if(iterator->nextFreeBlock != heapEnd)
        {
            newBlock->blockSize += iterator->nextFreeBlock->blockSize;
            newBlock->nextFreeBlock = iterator->nextFreeBlock->nextFreeBlock;
        }
        else
        {
            newBlock->nextFreeBlock = heapEnd;
        }
    }
    else
    {
        newBlock->nextFreeBlock = iterator->nextFreeBlock;
    }
    
    
    if(iterator != newBlock)
    {
        iterator->nextFreeBlock = newBlock;
    }
}


/**
 * @brief  内存堆初始化
 * @param  none
 * @retval none
 */
uint8_t heapInit(void)
{
    BLOCK_T *firstFreeBlock;
    
    uint8_t *alignHeap;
    uint32_t address;
    uint32_t totalHeapSize = HEAP_SIZE;
    
    //做一个8字节对齐的处理
     address = (uint32_t)heap;
    if((address & (uint32_t)0x00000007) != 0)
    {
        address += 7;
        address &= ~(uint32_t)0x00000007;
        
        totalHeapSize -= address - (uint32_t)heap;
    }
    
    //得到对齐地址
    alignHeap = (uint8_t *) address;
    
    //起始指针记录
    heapStart.nextFreeBlock = (void*)alignHeap;
    heapStart.blockSize = (uint32_t)0;
    
    //偏移至堆尾
    address = (uint32_t)alignHeap + totalHeapSize;
    address -= (sizeof(BLOCK_T));

    heapEnd = (void*)address;
    heapEnd->nextFreeBlock = NULL;
    heapEnd->blockSize = 0;
    
    //起始指针赋值 当前堆中唯一空闲内存块
    firstFreeBlock = (void*)alignHeap;
    firstFreeBlock->blockSize = address - (uint32_t)firstFreeBlock;
    firstFreeBlock->nextFreeBlock = heapEnd;
    
    
    //记录剩余内存数和最小空闲块
    freeByteRemaining = firstFreeBlock->blockSize;
    minFreeByteRemaining = firstFreeBlock->blockSize;
    
    return True;
    
}



/**
 * @brief  申请内存
 * @param  size 字节大小
 * @retval none
 */
void *myMalloc(uint32_t size)
{
    BLOCK_T *currentBlock,*previousBlock,*newBlock;
    
    void *returnHeap = NULL;
    
    if(size == 0)
    {
        return returnHeap;
    }
    
    //未初始化
    if(heapEnd == NULL)
    {
       heapInit(); 
    }
    
    //申请字节不能大于0x7FFFFFFF - sizeof(BLOCK_T)
    if((size & BLOCK_USE_FLAG) == 0)
    {
        //在申请字节的基础上增加一个记录结构体
        size += sizeof(BLOCK_T);
        
    }
    
    //字节对齐处理
     if((size & (uint32_t)0x00000007) != 0)
    {
        size += 7;
        size &= ~(uint32_t)0x00000007;
    }
    
    if(size <= freeByteRemaining)
    {
        previousBlock = &heapStart;
        currentBlock = heapStart.nextFreeBlock;
        
        while(( currentBlock->blockSize < size ) && currentBlock->nextFreeBlock != NULL)
        {
            previousBlock = currentBlock;
            currentBlock = currentBlock->nextFreeBlock;
        }
        
        if(currentBlock != heapEnd)
        {
            //地址偏移
            returnHeap = (void*)( (uint8_t *)(previousBlock->nextFreeBlock)+ sizeof(BLOCK_T)) ;
            
            previousBlock->nextFreeBlock = currentBlock->nextFreeBlock;
            
            //分配后内存块还有剩余 创建新的内存块
            if((currentBlock->blockSize - size) > MIN_BLOCK_SIZE )
            {
                
                newBlock = (void*) (((uint8_t*)currentBlock) + size);
                
                newBlock->blockSize = currentBlock->blockSize - size;
                currentBlock->blockSize = size;
                
                //将新内存块加入链表
                insertBLock(newBlock);
                
            }
            
            //更新剩余堆大小和最小空闲块
            freeByteRemaining -= currentBlock->blockSize;
            
            if(freeByteRemaining < minFreeByteRemaining)
            {
                minFreeByteRemaining = freeByteRemaining;
            }
            
            //标记此块内存已被使用
            currentBlock->blockSize |= BLOCK_USE_FLAG;
            currentBlock->nextFreeBlock = NULL;
            
        }
    }
    
    
    return returnHeap;
}



/**
 * @brief  释放内存
 * @param  size 字节大小
 * @retval none
 */
void myFree(void *freeBlock)
{
    uint8_t *p = (uint8_t*)freeBlock;
    
    BLOCK_T *blockLink;
    
    if(p != NULL)
    {
        p -=  sizeof(BLOCK_T);
        
        blockLink = (void*) p;
        
        
        if((blockLink->blockSize & BLOCK_USE_FLAG)!=0)
        {
             if(blockLink->nextFreeBlock == NULL)
             {
                 //标记此内存块未使用
                 blockLink->blockSize &= ~BLOCK_USE_FLAG;
                 //更新剩余内存
                 freeByteRemaining += blockLink->blockSize;
                 
                 insertBLock(blockLink);
             }
        }
    }
    
}




/**
 * @brief  得到堆剩余空间
 * @param  none
 * @retval 堆剩余空间
 */
uint32_t getFreeByteRemaining(void)
{
    return freeByteRemaining;
}







/**********************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/


