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
		q->size=size;/*¶ÓÁÐÓÐ´óÐ¡ÉÏÏÞµÄ*/
		q->level=0;
		q->head=q->tail=NULL;/*¼Ó²»¼Ó¶¼Ò»ÑùµÄ*/
		sc_lock_init(&q->mutex,NULL);
		LOGD("queue new OK");
	}else
		LOGD("queue new FAIL");
	return q;
}
void queue_delete(sc_queue_t *q){
	if(q){
		sc_lock(&q->mutex);
		//memset(q,0,sizeof(sc_queue_t));
		//free(q);
		//q=NULL;
		q->head=q->tail=NULL;
		q->size=q->level=0;
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
	/*ÄÚ´æÈç¹ûÊÇÔÚÍâÃæ·ÖÅäµÄ»°£¬Èë¶ÓÖ®ºó£¬Òª±£Ö¤ÄÚ´æ²»±»Ïú»Ù*/
	entry->pkt=pkt;
	if(q->head==NULL){
		LOGD("first time pushback");
		/*headºÍtailÒ»Ö±Ã»·ÖÅä¹ýÄÚ´æ°¡£¬ÄÇÃ´taiµÄ¼¸¸öÓò¶¼ÊÇÃ»ÄÚ´æµÄ£¬Ò²¾ÍÎÞ·¨¸³ÖµºÍÊ¹ÓÃ*/
		q->head=entry; /*·ñÔòtailÊÇNULL£¬ºóÃæ»á±¨´í*/
		/*ÕâÊÇÔÚ¸Ä±äentryµÄprevºÍnext*/
		//q->head->next=NULL;
		//q->head->prev=NULL;
	}
	q->tail->next=entry;
	//entry->prev=q->tail;
	entry->next=NULL;
	q->tail=entry;
	q->level+=1;
	sc_unlock(&q->mutex);
	return 0;
}

/*popÖ®ºódeleteÄÚ´æ*/
int queue_popfront(sc_queue_t * q, sc_pkt **pkt){
	sc_lock(&q->mutex);
	if(q!=NULL){
		if(queue_empty(q)){
			LOGE("queue is empty ,popfront FAIL");
			sc_unlock(&q->mutex);
			return -1;
		}
		LOGD("q level [%d]",q->level);
		(*pkt)=q->head->pkt; //ÕâÀï×ÜÊÇÓÐÎÊÌâ°¡å
		q->level-=1;
		if(q->level ==0 ){
			q->head=q->tail=NULL;
			goto DONE;
		}
		q->head=q->head->next;

DONE:
	sc_unlock(&q->mutex);
	  return 0;
	}else{
		LOGE("Q IS NULL ,pop  fail");
		sc_unlock(&q->mutex);
		return -1;
	}
}

