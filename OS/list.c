#include "MattOS.h"
#include "bspMalloc.h"


//�����б�
 LIST_TypeDef  taskReadyLists[MAX_PRIORITIES]; 
 LIST_TypeDef  taskDelayLists;


/**
 * @brief  ����ʼ��
 * @param  none
 * @retval none
 */
void nodeInit(tNode *node)
{
    node->nextNode = node;
    node->preNode = node;
}


/**
 * @brief  �б��ʼ��
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
 * @brief  ��ȡ����Ľڵ���
 * @param  none
 * @retval none
 */
uint32_t getListCount(LIST_TypeDef *list)
{
    return list->nodeCount;
}


/**
 * @brief  ������ͷ������һ���ڵ�
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
 * @brief  ������β������һ���ڵ�
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
 * @brief  �Ƴ��б���ָ���Ľڵ�
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
 * @brief  �Ƴ��б������нڵ�
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
        
        //�����ڵ��ָ���Լ�����
        currentNode->nextNode = currentNode;
        currentNode->preNode = currentNode;
        
    }
    
    list->headNode.nextNode = &(list->headNode);
    list->headNode.preNode = &(list->headNode);
    list->nodeCount = 0;
    
    
}





/**
 * @brief  ��������������б�
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
 * @brief  �Ӿ����б����Ƴ�����
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
            //���б����Ƴ�
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
 * @brief  �Ӿ����б��л�ȡ������ȼ�����
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
 * @brief  ���������ʱ�б�
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
 * @brief  ����ʱ�б����Ƴ�����
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
            //���б����Ƴ�
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


/**********************(C)  COPYRIGHT 2017 ��Զ�� *********************************************************/
