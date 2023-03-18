#ifndef     __LIST_H_
#define     __LIST_H_

#include <stdbool.h>

typedef int DataType ;

// 链表的结点设计
typedef struct list
{
    /* 数据域 */
    DataType Data ;

    /* 指针域 */
    struct list *Next , *Prev ;
}Node , *P_Node ;

/**
 * @brief 
 * 
 * @param NewData 
 * @return P_Node 
 */
P_Node NewNode( DataType * NewData );


/**
 * @brief 
 * 
 * @param head 
 * @param Data 
 * @return P_Node 
 */
P_Node FindNode(P_Node head , DataType Data );

void Add2ListOrder( P_Node head , P_Node New );
void Add2ListHead1( P_Node New , P_Node head );
void DisplayList( P_Node head , bool Reverse );
void Add2List( P_Node Prev , P_Node New , P_Node Next );
P_Node DelNode ( P_Node Prev , P_Node Del  , P_Node Next );




#endif