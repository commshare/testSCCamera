#ifndef _SC_LOCK_H_
#define _SC_LOCK_H_

////#include <pthread.h>
#include"pthread_common_internal.h"

#define sc_lock_t pthread_mutex_t
#define sc_lock_init(x,v) pthread_mutex_init(x,v)
#define sc_lock(x) pthread_mutex_lock(x)
#define sc_unlock(x) pthread_mutex_unlock(x)

#endif
