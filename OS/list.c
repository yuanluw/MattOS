#include "list.h"
#include "bspMalloc.h"


//�����б�
LIST_TypeDef  taskReadyLists[MAX_PRIORITIES]; 



/**
 * @brief  ����ʼ��
 * @param  none
 * @retval none
 */
void nodeInit(tNode *node)
{
    node->nextNode = node;
    node->preNode = node;
    node->owner = NULL;
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
    node->nextNode = &(list->headNode);
    node->preNode = list->headNode.preNode;
    
    list->headNode.preNode = node;
    list->headNode.preNode->nextNode = node;
    
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
uint8_t addTaskToReadyList(tTaskHandler_t *task)
{
   
    tNode *newNode = myMalloc(sizeof(tNode));
    
    if(newNode == NULL)
    {
        return False;
    }
    
    nodeInit(newNode);
    
    newNode->owner = task;
    
    if(taskReadyLists[task->prio].nodeCount == 0)
    {
        listInit(&taskReadyLists[task->prio]);
    }
    
    listAddLast(&taskReadyLists[task->prio],newNode);

}



/**
 * @brief  �Ӿ����б����Ƴ�����
 * @param  none
 * @retval none
 */
uint8_t removeTaskToReadyList(tTaskHandler_t *task)
{
    uint8_t i;
    uint8_t position = task->prio;
    tNode *node;
    if(taskReadyLists[position].nodeCount == 0)
    {
        return False;
    }
    
    node = taskReadyLists[position].headNode.nextNode;
    for(i=taskReadyLists[position].nodeCount; i!=0 ; i--)
    {
        if(node->owner == task)
        {
            //���б����Ƴ�
            listRemove(&taskReadyLists[position],node);
            //�����ڴ�
            myFree(node);
            
            break;
        }
        
        node = node->nextNode;
        
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
tTaskHandler_t* getHighReadyTask(void)
{
    uint8_t i=0 ;
    uint8_t j;
    tNode *node;
    for(i=MAX_PRIORITIES; i!=0 ; i--)
    {
        if(taskReadyLists[i-1].nodeCount == 0)
        {
            continue;
        }
        
        if(taskReadyLists[i-1].nodeCount == 1)
        {
            return taskReadyLists[i-1].headNode.nextNode->owner;
        }
        
        if(taskReadyLists[i-1].nodeCount>=2)
        {
            
            node = taskReadyLists[i-1].headNode.nextNode;
            for(j=0; j<taskReadyLists[i-1].index; j++)
            {
                node = node->nextNode;
            }  
            
            taskReadyLists[i-1].index++;
            taskReadyLists[i-1].index%=taskReadyLists[i-1].nodeCount;
            
            return node->owner;
        }
        
    }    
}



/**********************(C)  COPYRIGHT 2017 ��Զ�� *********************************************************/
