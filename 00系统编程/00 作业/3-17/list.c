#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"




P_Node NewNode( DataType * NewData )
{

    P_Node New = calloc(1 , sizeof(Node));

    if ( NewData != NULL )
    {
        memcpy( &New->Data , NewData , sizeof(DataType) );
    }
    
    New->Next = New->Prev = New ;

    return New ;
}

void Add2ListHead1( P_Node New , P_Node head )
{

    // 1 让新结点的后继指针指向头结点的后继结点
    New->Next = head->Next ;

    // 2. 让新节点的前驱指针指向头结点
    New->Prev = head ;

    // 3. 让第一个有效数据（头集结点的后继结点） 它的前驱指针指向新结点
    head->Next->Prev = New ;

    // 4. 让头结点的后继指针 指向 新结点
    head->Next = New ;

    return ;
}

void Add2List( P_Node Prev , P_Node New , P_Node Next )
{

    Prev->Next = New ;
    New->Next = Next ;

    Next->Prev = New ;
    New->Prev = Prev ;

    return ;
}

void Add2ListOrder( P_Node head , P_Node New )
{

    // 通过遍历链表寻找一个合适的位置来进行里插入操作
    P_Node tmp = NULL  ;
    for ( tmp = head->Next ; tmp != head ; tmp = tmp->Next )
    {
        if (tmp->Data > New->Data)
        {
            break ;
        }
    }

    Add2List( tmp->Prev  , New ,  tmp );

    return ;
}



void DisplayList( P_Node head , bool Reverse )
{

    if (Reverse)
    {
        for (P_Node tmp = head->Prev ; tmp != head ; tmp = tmp->Prev )
        {
            printf("Data:%d\n" , tmp->Data );
        }
    }
    else 
    {
        for (P_Node tmp = head->Next ; tmp != head ; tmp = tmp->Next )
        {
            printf("Data:%d\n" , tmp->Data );
        }
    }
    
    
}

P_Node FindNode(P_Node head , DataType Data )
{

    for ( P_Node tmp = head->Next ; tmp != head  ; tmp = tmp->Next)
    {
        if ( tmp->Data == Data )
        {
            return tmp ;
        }
        
    }
    
    return NULL ;
}

P_Node DelNode ( P_Node Prev , P_Node Del  , P_Node Next ) 
{

    Prev->Next = Next ;
    Next->Prev = Prev;

    Del->Next = Del->Prev = Del ;
    
    return Del ;
}
