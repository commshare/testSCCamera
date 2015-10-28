#ifndef __SC_QUEUE_H_
#define __SC_QUEUE_H_


//#include"sc_sdf.h"
#include"sc_frame.h"

#include"sc_lock.h"

#define sc_pkt sc_frame_t

typedef struct sc_entry_s{
//	struct sc_entry_s *prev;
	struct sc_entry_s *next;
	sc_pkt *pkt;
}sc_entry_t;

typedef struct sc_queue_s{
	sc_entry_t *head;
	sc_entry_t *tail;
	int size;
	int level;
	sc_lock_t mutex;
}sc_queue_t;


int queue_pushback(sc_queue_t * q, sc_pkt * pkt);
int queue_full(sc_queue_t *q);

int queue_empty(sc_queue_t *q);
sc_queue_t *queue_new(int size);
void queue_delete(sc_queue_t *q);

int queue_popfront(sc_queue_t * q, sc_pkt **pkt);
#endif
