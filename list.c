#include "list.h"
#include <stdlib.h>
#include "common.h"





int addListNode(List *lst,ListNode *lt){
    if(!lst || !lt){
        return 1;
    }

    if (!lst->head){
        lst->head = lt;
    }

    if(!lst->tail){
        lst->tail = lt;
    }

    else{
        lst->tail->next = lt;
        lt->prev = lst->tail;
        lst->tail = lt;
    }
    return 0;
}

ListNode* popFront(List *lst){
    ListNode *ret;
    if (!lst || !lst->head){
        return NULL;
    }
    ret = lst->head;
    lst->head = ret->next;
    ret->next = NULL;
    if(lst->head){
        lst->head->prev = NULL;
    }

    return ret;

}

void freeListNode(ListNode *ln){
    free(ln->page->url);
    free(ln->page->html);
    free(ln->page);
    free(ln);
}

