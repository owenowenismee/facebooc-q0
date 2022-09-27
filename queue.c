#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *newq=malloc(sizeof(queue_t));// allocate the queue_t's memory and save to pointer newq.
    if(newq==0) return NULL;//couldn't allocate space so return null.
    newq->head=NULL;//empty queue so there is no head element.
    newq->tail=NULL;//empty queue so there is no tail element.
    newq->size=0;//empty queue size=0.
    return newq;
} 

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if(q==NULL)return; //4 sure
    element_t *now ,*prev;
    now=q->head;//from head to free
    prev=now;
    while(prev!=NULL){
        prev=now;
        now=now->next;
        free(prev->value);//free value memory
        free(prev);//free element prev;
    }
    free(q);//free whole queue structure
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */

bool q_insert_head(queue_t *q, char *s)
{
    if(q==NULL)
        return false;
    element_t *ele=malloc(sizeof(element_t));

    if(ele==NULL) 
        return false;
    
    ele->value=malloc(strlen(s)+1);//allocate new memory for string s.
    
    if(ele->value==NULL){
        free(ele);
        return false;
        }

    strcpy(ele->value,s);

    ele->next=q->head;
    q->head=ele;

    if(ele->next==NULL)
    q->tail=ele;//when there is only one element in queue.

    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{   
    if(q==NULL)
        return false;

    element_t *ele=malloc(sizeof(element_t));

    if(ele==NULL) 
        return false;
    
    ele->value=malloc(strlen(s)+1);//allocate new memory for string s.
    
    if(ele->value==NULL){
        free(ele);
          return false;
        }

    strcpy(ele->value,s);

    ele->next=NULL;
    q->tail=ele;

    if(q->head==NULL)
    q->head=ele;//when there is only one element in queue.

    q->size++;
    return true;
    
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{   
    if(q==NULL)
    return false;

    if(q->head==NULL)
    return false;

    element_t *ele=q->head; 
    q->head=q->head->next;

    
    strncpy(sp,ele->value,bufsize-1);
    free(ele->value);
    free(ele);
    q->size--;
    
    
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
size_t q_size(queue_t *q)
{   
    if(q!=NULL)
        return q->size;
    return 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if(q==NULL)
    return;

    element_t *prev=q->head;
    element_t *now=prev->next;
    element_t *nexttt=now->next;
    prev->next=NULL;//head change to tail so pointer change to null. 

    while(nexttt!=NULL){
        nexttt=now->next;
        now->next=prev;
        prev=now;
        now=nexttt;
    }
    now->next=prev;

    element_t *tmp=q->head;//swap head and tail.
    q->head=q->tail;
    q->tail=tmp;

    
}

/*
 * The function's sorting algorithm should be merge sort.
 */

void q_spilt(element_t *src,element_t **front,element_t **back){//ref:https://ithelp.ithome.com.tw/articles/10242225
    
    element_t *fast,*slow;//implement with two pointer.
    slow=src;
    fast=src->next;

    while(fast!=NULL){

        fast=fast->next;

        if(fast!=NULL){

            slow=slow->next;
            fast=fast->next;

        }
    }

    front=src;
    back=slow->next;
    slow->next=NULL;

}

element_t *sortedmerge(element_t *a,element_t *b){

    element_t *result=NULL;
    if(a==NULL){
        return b;
    }

    if(b==NULL){
        return a;
    }

    if(a->value<=b->value){
        result=a;
        result->next=sortedmerge(a->next,b);
    }
    else{
        result=b;
        result->next=sortedmerge(a,b->next);
    }

    return result;
}

void merge_sort(element_t **head)
{
    if (!(*head) || !(*head)->next)
        return;
    
    element_t *a,*b;

    q_spilt(head,&a,&b);

    merge_sort(&a);
    merge_sort(&b);

    head=sortedmerge(a,b);
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head)
        return;

    merge_sort(&q->head);
    element_t *walk = q->head;
    while (walk->next)
        walk = walk->next;

    q->tail = walk;
}
