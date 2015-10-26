#include <assert.h> //for assert
#include<stdlib.h> //for malloc free
  #include <string.h> //for memset

#include "sc_queue.h"
#include"sc_log.h"

//#define CLEAR(x) memset(&(x), 0, sizeof(x))
static
int queue_init(sc_queue_t **q){

	(*q)=(sc_queue_t *)malloc(sizeof(sc_queue_t));
	if(*q){
		memset((*q),0,sizeof(sc_queue_t));
		return 0;
	}else{
	  (*q)=NULL;
	  return -1;
	}
}
sc_queue_t *queue_new(int size){
    sc_queue_t * q=NULL;
	if(queue_init(&q)==0){
		q->size=size;/*队列有大小上限的*/
		q->level=0;
		q->head=q->tail=NULL;/*加不加都一样的*/
		sc_lock_init(&q->mutex,NULL);
		LOGD("queue new OK");
	}else
		LOGD("queue new FAIL");
	return q;
}
void queue_delete(sc_queue_t *q){
	if(q){
		sc_lock(&q->mutex);
		memset(q,0,sizeof(sc_queue_t));
		free(q);
		q=NULL;
		sc_unlock(&q->mutex);

	}
}

int queue_empty(sc_queue_t *q){
	assert(q!=NULL);
	if(q->level==0)
		return 1;
	else
		return 0;
}
int queue_full(sc_queue_t *q){
	assert(q!=NULL);
	if(q->level==q->size)
		return 1;
	else
		return 0;
}


int queue_pushback(sc_queue_t * q, sc_pkt * pkt){
		sc_lock(&q->mutex);

	//assert(q!=NULL && pkt!=NULL);
	if(q==NULL || pkt==NULL){
		LOGE("QUEUE PUSHBACK FAIL FOR EXCEPTION!");
		sc_unlock(&q->mutex);
		return -1;
	}
	if(queue_full(q))
	{
		LOGD("queue is full,cannot pushback");
		sc_unlock(&q->mutex);
		return -1;
	}

    sc_entry_t *entry=NULL;
    entry=(sc_entry_t *)malloc(sizeof(sc_entry_t));
	memset(entry,0,sizeof(sc_entry_t));
	entry->pkt=pkt;
	if(q->head==NULL){
		LOGD("first time pushback");
		/*head和tail一直没分配过内存啊，那么tai的几个域都是没内存的，也就无法赋值和使用*/
		q->head=q->tail=entry; /*否则tail是NULL，后面会报错*/
		/*这是在改变entry的prev和next*/
		//q->head->next=NULL;
		//q->head->prev=NULL;
	}
	q->tail->next=entry;
	q->tail->next->prev=q->tail;
	q->tail=entry;
	q->tail->next=NULL;
	q->level+=1;
	sc_unlock(&q->mutex);
	return 0;
}

int queue_popfront(sc_queue_t * q, sc_pkt **pkt){
	sc_lock(&q->mutex);
	if(q!=NULL){
		if(queue_empty(q)){
			LOGE("queue is empty ,popfront FAIL");
			sc_unlock(&q->mutex);
			return -1;
		}
		LOGD("q level [%d]",q->level);
		(*pkt)=q->head->pkt;
		LOGD("$$$$$$$$$$$$$");
		#if 0
		if((*pkt)==NULL){
			LOGE("##WRONG###head pkt is NULL !!!!!");
			return -1;
		}
		#endif
		LOGD("-----1---------");
		sc_entry_t *next=q->head->next;
		if(next)/*取完之后，next可能是空的*/
		{
			next->prev=NULL;
			q->head=next;
			q->level-=1;
		}else
		{
			LOGD("after pop ,next is NULL");
			/*检查pop后是不是空了*/
			if(q->level==0){
				LOGD("AT THIS TIME ,level SHOULD BE  0");
			}
			q->head=q->tail=NULL;
			/*俩空指针怎么会有prev和next呢，而且他们都没分配空间的，所以不能给各个域名赋值的*/
			//q->head->prev=NULL;
			//q->head->next=NULL;

	    }
	sc_unlock(&q->mutex);
	  return 0;
	}else{
		LOGE("Q IS NULL ,pop front queue fail");
		sc_unlock(&q->mutex);
		return -1;
	}
}

