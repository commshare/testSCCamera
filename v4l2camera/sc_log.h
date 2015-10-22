

#ifndef _SC_LOG_HXX_
#define _SC_LOG_HXX_



#define  SCLOG_TAG    "SC_ME"
#define FF_TMP "FF_"

#define USE_ANDROID 0
#define ME_PRINTF 0
#define CAI_PRINT 0

#if USE_ANDROID
#include <android/log.h>
//#define  LOGI(...)
#define LOGI(message,...)  __android_log_print(ANDROID_LOG_INFO, SCLOG_TAG, "[%s_%d_%s]"message,__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
#define LOGD(message,...)  __android_log_print(ANDROID_LOG_DEBUG, SCLOG_TAG, "[%s_%d_%s]"message,__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
#define LOGE(message,...)  __android_log_print(ANDROID_LOG_ERROR, SCLOG_TAG,"[%s_%d_%s]"message,__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
#define LOGW(message,...)  __android_log_print(ANDROID_LOG_ERROR, SCLOG_TAG,"[%s_%d_%s]"message,__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)


#define slogi(message,...) __android_log_print(ANDROID_LOG_INFO, SCLOG_TAG,"##"message,##__VA_ARGS__)
#define slogd(message,...) __android_log_print(ANDROID_LOG_DEBUG, SCLOG_TAG,"##"message,##__VA_ARGS__)
#define sloge(message,...) __android_log_print(ANDROID_LOG_ERROR, SCLOG_TAG,"##"message,##__VA_ARGS__)
#define slogw(message,...) __android_log_print(ANDROID_LOG_WARN, SCLOG_TAG,"##"message,##__VA_ARGS__)


#define  MMLOGI(TAG, ...)   __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define  MMLOGD(TAG, ...)   __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define  MMLOGE(TAG, ...)   __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define  MMLOGW(TAG, ...)   __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define  MMLOGV(TAG, ...)   __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)


#define sc_die(format,...) \
	__android_log_print(ANDROID_LOG_ERROR, SCLOG_TAG, "[%s_%d_%s]"format,__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__);\
	LOGE("sc_die shutdown[%s]",format);\
	shairport_shutdown(1)

//20141216 change it from 0 to 10 ,make it large enough to enable debug
//int debuglev = 10;
//change from int to #define for multiple definations
#define debuglev 10

#define sc_debug(level,format,...) \
if(level > debuglev ) \
	return; \
__android_log_print(ANDROID_LOG_DEBUG, SCLOG_TAG, "[%s_%d_%s]"format,__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)

#define sc_warn(format,...)  __android_log_print(ANDROID_LOG_ERROR, SCLOG_TAG,"[%s_%d_%s]"format,__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)

//add by me 20141106
//#define dprintf(fmt,arg...) __android_log_print(ANDROID_LOG_INFO, SCLOG_TAG, "[%s_%d_%s]"fmt,arg,__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)

//#define vdprintf(fmt,arg...) __android_log_print(ANDROID_LOG_INFO, SCLOG_TAG, "[%s_%d_%s]"fmt,arg,__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)

#else

#include<stdio.h>

#if ME_PRINTF
#define LOGI(format,...)  printf("[%s_%d_%s]"format"\n",__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
#define LOGD(format,...)  printf("[%s_%d_%s]"format"\n",__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
#define LOGW(format,...)  printf("[%s_%d_%s]"format"\n",__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
#define LOGE(format,...)  printf("[%s_%d_%s]"format"\n",__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
#else
#define LOGI(format,...)  fprintf(stderr,"[%s_%d_%s]"format"\n",__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
#define LOGD(format,...)  fprintf(stderr,"[%s_%d_%s]"format"\n",__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
#define LOGW(format,...)  fprintf(stderr,"[%s_%d_%s]"format"\n",__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
#define LOGE(format,...)  fprintf(stderr,"[%s_%d_%s]"format"\n",__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
#endif
/*
#define HAL_INFO(fmt...) \
do{\
	printf(fmt);\
}while(0)


*/
//printf("##"message"\n",#__VA_ARGS__);


#if ME_PRINTF

#define slogi(message,...)   do{\
fprintf(stderr, "##"message"\n",##__VA_ARGS__);\
}while(0)

#define slogd(message,...) 	do{\
fprintf(stderr, "##"message"\n",##__VA_ARGS__);\
	}while(0)

#define sloge(message,...) 	do{\
	fprintf(stderr, "##"message"\n",##__VA_ARGS__);\
	}while(0)

#define slogw(message,...) 	do{\
	fprintf(stderr, "##"message"\n",##__VA_ARGS__);\
	}while(0)

#elif CAI_PRINT
#define slogi(message,...) \
	do{\
		printf(message);\
	}while(0)
#define slogd(message,...) \
	do{\
		printf(message);\
	}while(0)
#define slogw(message,...) \
	do{\
		printf(message);\
	}while(0)
#define sloge(message,...) \
	do{\
		printf(message);\
	}while(0)
#else
#if USE_DETAIL
#define slogi(m,...) fprintf(stderr, "%s_%d" m " \n", __FILE__, __LINE__, ##__VA_ARGS__)
#define slogd(m,...) fprintf(stderr, "%s_%d" m " \n", __FILE__, __LINE__, ##__VA_ARGS__)
#define slogw(m,...) fprintf(stderr, "%s_%d" m " \n", __FILE__, __LINE__, ##__VA_ARGS__)
#define sloge(m,...) fprintf(stderr, "%s_%d" m " \n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define slogi(m,...) fprintf(stderr,  m " \n",##__VA_ARGS__)
#define slogd(m,...) fprintf(stderr,  m " \n",##__VA_ARGS__)
#define slogw(m,...) fprintf(stderr,  m " \n",##__VA_ARGS__)
#define sloge(m,...) fprintf(stderr,  m " \n",##__VA_ARGS__)
#endif


#endif


//add by me 20141106
#define dprintf(fmt,arg...) printf(fmt, ##arg)
#define vdprintf(fmt,arg...) printf(fmt, ##arg)

#endif

#endif //file end



