#include "MattOS.h"
#include "bspMalloc.h"


//就绪列表
 LIST_TypeDef  taskReadyLists[MAX_PRIORITIES]; 
 LIST_TypeDef  taskDelayLists;


/**
 * @brief  结点初始化
 * @param  none
 * @retval none
 */
void nodeInit(tNode *node)
{
    node->nextNode = node;
    node->preNode = node;
}


/**
 * @brief  列表初始化
 * @param  none
 * @retval none
 */
void listInit(LIST_TypeDef *list)
{
    
    list->headNode.nextNode = &(list->headNode);
    list->headNode.preNode = &(list->headNode);
    list->nodeCount = 0;
    list->index = 0;
    
}


/**
 * @brief  获取链表的节点数
 * @param  none
 * @retval none
 */
uint32_t getListCount(LIST_TypeDef *list)
{
    return list->nodeCount;
}


/**
 * @brief  在链表头部插入一个节点
 * @param  none
 * @retval none
 */
void listAddFirst(LIST_TypeDef* list,tNode *node)
{
    
    node->preNode = list->headNode.nextNode->preNode;
    node->nextNode = list->headNode.nextNode;
    
    list->headNode.nextNode->preNode = node;
    list->headNode.nextNode = node;
    
    list->nodeCount++;
    
}

/**
 * @brief  在链表尾部插入一个节点
 * @param  none
 * @retval none
 */
void listAddLast(LIST_TypeDef* list,tNode *node)
{
    node->preNode = list->headNode.preNode;
    node->nextNode = &(list->headNode);
    
    
    list->headNode.preNode->nextNode = node;
    list->headNode.preNode = node;
    
    
    list->nodeCount++;
}


/**
 * @brief  移除列表中指定的节点
 * @param  none
 * @retval none
 */
void listRemove(LIST_TypeDef* list,tNode *node)
{
    node->preNode->nextNode = node->nextNode;
    node->nextNode->preNode = node->preNode;
    
    list->nodeCount--;
}


/**
 * @brief  移除列表中所有节点
 * @param  none
 * @retval none
 */
void listRemoveAll(LIST_TypeDef *list)
{
    
    uint32_t count;
    
    tNode *nextNode = list->headNode.nextNode;
    tNode *currentNode = nextNode;
    for(count=list->nodeCount ; count!=0 ; count--)
    {
        nextNode = nextNode->nextNode;
        
        //各个节点均指向自己本身
        currentNode->nextNode = currentNode;
        currentNode->preNode = currentNode;
        
    }
    
    list->headNode.nextNode = &(list->headNode);
    list->headNode.preNode = &(list->headNode);
    list->nodeCount = 0;
    
    
}





/**
 * @brief  添加任务至就绪列表
 * @param  none
 * @retval none
 */
uint8_t addTaskToReadyList(tNode *node)
{
   
    taskHandler_t *task = (taskHandler_t*)((uint32_t)node - (uint32_t)(sizeof(taskHandler_t)-sizeof(tNode)));
    
    if(taskReadyLists[task->prio].nodeCount == 0)
    {
        listInit(&taskReadyLists[task->prio]);
    }
    
    listAddLast(&(taskReadyLists[task->prio]),node);
    
    return True;
}



/**
 * @brief  从就绪列表中移除任务
 * @param  none
 * @retval none
 */
uint8_t removeTaskToReadyList(tNode *node)
{
    uint8_t i;
    taskHandler_t *task = (taskHandler_t*)((uint32_t)node - (uint32_t)(sizeof(taskHandler_t)-sizeof(tNode)));
    uint8_t position = task->prio;
    tNode *iterator;
    if(taskReadyLists[position].nodeCount == 0)
    {
        return False;
    }
    
    iterator = taskReadyLists[position].headNode.nextNode;
    for(i=taskReadyLists[position].nodeCount; i!=0 ; i--)
    {
        if(iterator == node)
        {
            //从列表中移除
            listRemove(&taskReadyLists[position],node);
            
            break;
        }
        
        iterator = iterator->nextNode;
        
    }
    
    if(i != 0)
    {
        return True;
    }
    
    return False;
    
}



/**
 * @brief  从就绪列表中获取最高优先级任务
 * @param  none
 * @retval none
 */
taskHandler_t* getHighReadyTask(void)
{
    uint8_t i=0 ;
    uint8_t j;
    taskHandler_t *task;// = (taskHandler_t*)(node - (sizeof(taskHandler_t)-sizeof(tNode)));
    tNode *node;
    for(i=0; i<MAX_PRIORITIES ; i++)
    {
        if(taskReadyLists[i].nodeCount == 0)
        {
            continue;
        }
        
        if(taskReadyLists[i].nodeCount == 1)
        {
            node = taskReadyLists[i].headNode.nextNode;
            task = (taskHandler_t*)((uint32_t)node - (uint32_t)(sizeof(taskHandler_t)-sizeof(tNode)));
            return task;
        }
        
        if(taskReadyLists[i].nodeCount>=2)
        {
            
            node = taskReadyLists[i].headNode.nextNode;
            for(j=0; j<taskReadyLists[i].index; j++)
            {
                node = node->nextNode;
            }  
            
            taskReadyLists[i].index++;
            taskReadyLists[i].index%=taskReadyLists[i].nodeCount;
            
            task = (taskHandler_t*)((uint32_t)node - (uint32_t)(sizeof(taskHandler_t)-sizeof(tNode)));
            return task;
        }
        
    }  

    return NULL;
}


/**
 * @brief  添加任务延时列表
 * @param  none
 * @retval none
 */
uint8_t addTaskToDelayList(tNode *node)
{
    
    
    if(taskDelayLists.nodeCount == 0)
    {
        listInit(&taskDelayLists);
    }
    
    listAddLast(&taskDelayLists,node);
   
    
    return True;
}



/**
 * @brief  从延时列表中移除任务
 * @param  none
 * @retval none
 */
uint8_t removeTaskToDelayList(tNode *node)
{
    
    uint8_t i;
    tNode *iterator;
    if(taskDelayLists.nodeCount == 0)
    {
        return False;
    }
    
    iterator = taskDelayLists.headNode.nextNode;
    for(i=taskDelayLists.nodeCount; i!=0 ; i--)
    {
        if(iterator == node)
        {
            //从列表中移除
            listRemove(&taskDelayLists,node);
            break;
        }
        
        iterator = iterator->nextNode;
        
    }
    
    if(i != 0)
    {
        return True;
    }
    
    return False;
    
}


/**********************(C)  COPYRIGHT 2017 吴远泸 *********************************************************/
