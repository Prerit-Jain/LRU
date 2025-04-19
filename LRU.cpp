#include <stdio.h>
#include <stdlib.h>
#include <cstring>

typedef struct _Node {
   int key;
   struct _Node* prev;
   struct _Node* next;
} Node;

Node *pLnkHead;
Node *pLnkTail;

int maxIndexUsedCnt;
typedef struct {
  
    int key[10001]; 
    int value[3001];
    Node node[3001];
    int lnkLstCnt;
    int capacity;
    int freeIndex;
} LRUCache;
 void lRUCacheFree(LRUCache* obj);
void insertInBeg(Node *p,int key)
{
     printf("Start insertInBeg pLinkTail =%p \n",pLnkTail);
    if(pLnkHead == NULL)
    {
        p->prev = NULL;
        pLnkHead = p;
        pLnkTail = p;
        p->next = NULL;
    }
    else
    {
        p->prev = NULL;
        p->next = pLnkHead;
        pLnkHead->prev = p;
        pLnkHead=p;
    }
    p->key =key;

  //  printf("InsertInBeg- key =%d, tail key=%d\n",pLnkHead->key, pLnkTail->key);
}

void moveNodeToBeg(Node *p)
{
   
    if(p== pLnkHead)
    {
        return;
    }

    if(p->prev != NULL)
    {
        p->prev->next = p->next;
        if(p == pLnkTail)
        {
            pLnkTail = p->prev;
        }
        else
        {
            if(p->next != NULL)
                p->next->prev = p->prev;
        }
    }
    p->next = pLnkHead;
    pLnkHead->prev = p;
    pLnkHead = p;
    p->prev = NULL;
    printf("Move Key =%d to beginning\n", p->key);
 printf("End pLinkTail =%p \n",pLnkTail);
   // printf(" Key at Tail =%d , Key at Head=%d \n", pLnkTail->key, pLnkHead->key);
}


LRUCache* lRUCacheCreate(int capacity) {
    LRUCache *obj =  (LRUCache *)malloc(sizeof(LRUCache));
    obj->capacity = capacity;
    obj->freeIndex = 0;
    obj->lnkLstCnt =0;


    memset(obj->key,-1,sizeof(int)*(10001));
 
    /*initialize the link list */
    pLnkHead = NULL;
    pLnkTail = NULL;
    return obj;
}

int lRUCacheGet(LRUCache* obj, int key) {
    if(obj->key[key] == -1)
        return -1;
    int index = obj->key[key];
    /*Move the entry to the beginning of the link list */
    moveNodeToBeg(&obj->node[index]);
    return obj->value[index];
}

void lRUCachePut(LRUCache* obj, int key, int value) {
    /*Get Free entry in LRU Cache */
   int index= obj->key[key];
  
   
    if(index != -1)
    {
        obj->value[index] = value;
        moveNodeToBeg(&obj->node[index]);
        return;
    }
    
    if (obj->lnkLstCnt == obj->capacity) 
    {
        /*Remove the least recently used entry */
        lRUCacheFree(obj); 
        /* Add the new entry in its place */   
        if(obj->freeIndex<0)
        {
       
            return;
        }
        index = obj->freeIndex;
        obj->key[key] = index;
        obj->value[index] = value;
    }
    else
    {
        index = obj->freeIndex;
        obj->key[key] = index;
        obj->value[index] = value;
        obj->lnkLstCnt++;
        obj->freeIndex++;
    }
  
    /*Insert in the Beginning of the link list */
    insertInBeg(&obj->node[index],key);

    
}

void lRUCacheFree(LRUCache* obj) {
    Node *pNodeToBeFreed = pLnkTail;
   
    if(pLnkTail !=NULL)
    {
        pLnkTail = pLnkTail->prev;
        if(pLnkTail)
            pLnkTail->next = NULL;
        else
            pLnkHead = pLnkTail;
        obj->freeIndex = obj->key[pNodeToBeFreed->key];
        obj->key[pNodeToBeFreed->key]=-1;
    }    
}

int main()
{
   LRUCache* obj;
   obj = lRUCacheCreate(3);
   lRUCachePut(obj, 1,1);
   lRUCacheGet(obj, 1);
   lRUCachePut(obj, 2,1);
   return 0;
}
