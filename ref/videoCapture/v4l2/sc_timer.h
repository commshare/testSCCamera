#ifndef __SC_TIMER_H_
#define __SC_TIMER_H_

#include<stdio.h>
#include<stdlib.h>
//#include<pthread.h>

#include"sc_lock.h"
typedef  int (*sc_timer_callback)(void *);

typedef struct sc_timer_s{
	int interval;
	pthread_t mTimerThread;
	int mStopTimerFlag;
	sc_lock_t mLock;
    time_t mStartRecordTime;
	sc_timer_callback callback;
	int state;

}sc_timer_t;


#endif
