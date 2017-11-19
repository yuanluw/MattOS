#include "bspMalloc.h"



static uint8_t heap[HEAP_SIZE];
static BLOCK_T heapStart,*heapEnd = NULL;
//ʣ���ڴ��С
static uint32_t freeByteRemaining = 0U;
//��¼��ǰ��С���ڴ��
static uint32_t minFreeByteRemaining = 0U;




/**
 * @brief  �����ڴ��
 * @param  none
 * @retval none
 */
static void insertBLock(BLOCK_T *newBlock)
{
    BLOCK_T *iterator;
    uint8_t *p;
    //�ҵ������ڴ�����һ���ڴ�  ����ַ����
    for(iterator = &heapStart; iterator->nextFreeBlock < newBlock; iterator = iterator->nextFreeBlock)
    {
         
    }
    
    p = (uint8_t*)iterator;
    
    //���Ժϲ�
    if((p + iterator->blockSize) == (uint8_t*)newBlock)
    {
        iterator->blockSize += newBlock->blockSize;
        newBlock = iterator;
    }
    
    p = (uint8_t*)newBlock;
    //���Ժͺ���ϲ�
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
 * @brief  �ڴ�ѳ�ʼ��
 * @param  none
 * @retval none
 */
uint8_t heapInit(void)
{
    BLOCK_T *firstFreeBlock;
    
    uint8_t *alignHeap;
    uint32_t address;
    uint32_t totalHeapSize = HEAP_SIZE;
    
    //��һ��8�ֽڶ���Ĵ���
     address = (uint32_t)heap;
    if((address & (uint32_t)0x00000007) != 0)
    {
        address += 7;
        address &= ~(uint32_t)0x00000007;
        
        totalHeapSize -= address - (uint32_t)heap;
    }
    
    //�õ������ַ
    alignHeap = (uint8_t *) address;
    
    //��ʼָ���¼
    heapStart.nextFreeBlock = (void*)alignHeap;
    heapStart.blockSize = (uint32_t)0;
    
    //ƫ������β
    address = (uint32_t)alignHeap + totalHeapSize;
    address -= (sizeof(BLOCK_T));

    heapEnd = (void*)address;
    heapEnd->nextFreeBlock = NULL;
    heapEnd->blockSize = 0;
    
    //��ʼָ�븳ֵ ��ǰ����Ψһ�����ڴ��
    firstFreeBlock = (void*)alignHeap;
    firstFreeBlock->blockSize = address - (uint32_t)firstFreeBlock;
    firstFreeBlock->nextFreeBlock = heapEnd;
    
    
    //��¼ʣ���ڴ�������С���п�
    freeByteRemaining = firstFreeBlock->blockSize;
    minFreeByteRemaining = firstFreeBlock->blockSize;
    
    return True;
    
}



/**
 * @brief  �����ڴ�
 * @param  size �ֽڴ�С
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
    
    //δ��ʼ��
    if(heapEnd == NULL)
    {
       heapInit(); 
    }
    
    //�����ֽڲ��ܴ���0x7FFFFFFF - sizeof(BLOCK_T)
    if((size & BLOCK_USE_FLAG) == 0)
    {
        //�������ֽڵĻ���������һ����¼�ṹ��
        size += sizeof(BLOCK_T);
        
    }
    
    //�ֽڶ��봦��
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
            //��ַƫ��
            returnHeap = (void*)( (uint8_t *)(previousBlock->nextFreeBlock)+ sizeof(BLOCK_T)) ;
            
            previousBlock->nextFreeBlock = currentBlock->nextFreeBlock;
            
            //������ڴ�黹��ʣ�� �����µ��ڴ��
            if((currentBlock->blockSize - size) > MIN_BLOCK_SIZE )
            {
                
                newBlock = (void*) (((uint8_t*)currentBlock) + size);
                
                newBlock->blockSize = currentBlock->blockSize - size;
                currentBlock->blockSize = size;
                
                //�����ڴ���������
                insertBLock(newBlock);
                
            }
            
            //����ʣ��Ѵ�С����С���п�
            freeByteRemaining -= currentBlock->blockSize;
            
            if(freeByteRemaining < minFreeByteRemaining)
            {
                minFreeByteRemaining = freeByteRemaining;
            }
            
            //��Ǵ˿��ڴ��ѱ�ʹ��
            currentBlock->blockSize |= BLOCK_USE_FLAG;
            currentBlock->nextFreeBlock = NULL;
            
        }
    }
    
    
    return returnHeap;
}



/**
 * @brief  �ͷ��ڴ�
 * @param  size �ֽڴ�С
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
                 //��Ǵ��ڴ��δʹ��
                 blockLink->blockSize &= ~BLOCK_USE_FLAG;
                 //����ʣ���ڴ�
                 freeByteRemaining += blockLink->blockSize;
                 
                 insertBLock(blockLink);
             }
        }
    }
    
}




/**
 * @brief  �õ���ʣ��ռ�
 * @param  none
 * @retval ��ʣ��ռ�
 */
uint32_t getFreeByteRemaining(void)
{
    return freeByteRemaining;
}







/**********************(C)  COPYRIGHT 2017 ��Զ�� *********************************************************/


