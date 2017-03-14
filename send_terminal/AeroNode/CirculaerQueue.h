#ifndef _CIRCULARQUEUE_H__
#define _CIRCULARQUEUE_H__

typedef struct
{
    int iTime;
}ModelData,*pModelData;

typedef struct 
{
    pModelData QueueData;
    int iFront,iRear;
    int iMaxNumQueue;
}QueueList,*pQueueList;


extern BOOL InitQueue(pQueueList& pMyQL,int iMaxNum);

extern BOOL IsEmpty(pQueueList pMyQL);

extern BOOL IsFull(pQueueList pMyQL);

extern BOOL EnterQueue(pQueueList pMyQL,pModelData pMyData);

extern BOOL QuitQueue(pQueueList pMyQL,pModelData pMyData);

extern void ClearQueue(pQueueList pMyQL);


#endif    //END  _CIRCULARQUEUE_H__