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
        pthread_cond_init(&q->cond,NULL);
		LOGD("queue new OK");
	}else
		LOGD("queue new FAIL");
	return q;
}
void queue_uninit(sc_queue_t *q){
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
void queue_destroy(sc_queue_t *q){
	if(q){
		queue_uninit(q);
		sc_lock_destroy(&q->mutex);
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
	//slogi("pusback begin");

	//assert(q!=NULL && pkt!=NULL);
	if(q==NULL || pkt==NULL){
		LOGE("QUEUE PUSHBACK FAIL FOR EXCEPTION!");
		sc_unlock(&q->mutex);
		return 	QNULL_FAIL;/*要exit的*/
	}
	if(queue_full(q))
	{
		sloge("queue is full [%d],cannot pushback",q->level);
		sc_unlock(&q->mutex);
		return QFULL_PUSHFAIL;//-1;
	}

    sc_entry_t *entry=(sc_entry_t *)malloc(sizeof(sc_entry_t));
	memset(entry,0,sizeof(sc_entry_t));
	/*内存如果是在外面分配的话，入队之后，要保证内存不被销毁*/
	entry->pkt=pkt;
	if(q->head==NULL){
		LOGD("first time pushback");
		/*head和tail一直没分配过内存啊，那么tai的几个域都是没内存的，也就无法赋值和使用*/
		q->head=entry; /*否则tail是NULL，后面会报错*/
        goto OK; /*注意解锁啊*/
	}
	slogi("push one");
	q->tail->next=entry; /*第一次插入时，tail还没有任何指向(本身也未被分配过内存)，此时的next是不存在的。*/
	//entry->prev=q->tail;
OK:
	entry->next=NULL;
	q->tail=entry;
	q->level+=1;
	sc_unlock(&q->mutex);
	//LOGD("push one ok");

	return 0;
}

/*pop之后delete内存*/
int queue_popfront(sc_queue_t * q, sc_pkt **pkt){
	sc_lock(&q->mutex);
	if(q!=NULL){
		if(queue_empty(q)){
			slogi("empty popFAIL");
			sc_unlock(&q->mutex);
			return  QEMPTY_POPFAIL;//-1;
		}
		slogi("q level [%d]",q->level);
		(*pkt)=q->head->pkt; //这里总是有问题啊�
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
		slogi("Q IS NULL ,pop  fail");
		sc_unlock(&q->mutex);
		return QNULL_FAIL;
	}
}

