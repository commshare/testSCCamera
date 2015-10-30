#include"sc_log.h"
#include"sc_timer.h"
#include <time.h>


struct tm *GetCurrTimeAtom()
{
    time_t lt;
    time(&lt);
    return localtime(&lt);
}

void showCaculator(){
    time_t rawtime;
    struct tm * timeinfo; //两次用struct tm会报错啊。
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    /*
    asctime （）-- 转为标准ASCII时间格式：星期 月 日 时：分：秒 年
    */
    LOGE("###The current date/time is: %s", asctime (timeinfo) );
}

//我觉得这个精确度可能不够吧，录制文件的话，精确度应该可以，到秒，但是比较时间还是要到微秒啊。
void threadFunc(sc_timer_t *t) {
  //  prctl(PR_SET_NAME, (unsigned long)"StagefrightRecorder_timer", 0, 0, 0);

    //线程的优先级高
    //androidSetThreadPriority(0, ANDROID_PRIORITY_HIGHEST);
    //互斥访问喔
   // Mutex::Autolock autoLock(mLock);
   sc_lock(&t->mLock);
    while (!t->mStopTimerFlag) {

	    time_t now;
    	time(&now);
	    //struct tm* tm_t;
    	//tm_t = localtime(&timer);
       //ALOGE( "#@@##timer[%d]- mStartRecordTime[%d]=[%d]", timer, mStartRecordTime,timer - mStartRecordTime);
	    if((now - t->mStartRecordTime) >= t->interval/*一分钟*/){
			#if 0
		   // ALOGE("!!! 222 StagefrightRecorder timer thread is ryning now time hour=%d, min=%d, second=%d ,now timer=%d, mStartRecordTime=%d !!!", tm_t->tm_hour, tm_t->tm_min, tm_t->tm_sec, timer, mStartRecordTime);
    		//tm_t->tm_year+1900, tm_t->tm_mon+1, tm_t->tm_mday, tm_t->tm_hour, tm_t->tm_min, tm_t->tm_sec);
	    	//定时器需要完成的功能 60秒钟的定时器
	    	ALOGE("####DO STH#####");
           #if 1
            recordByOneMinutes();
           #else
            showCaculator();
           #endif
           #endif

		   #if 0
			showCaculator();
		   #else
		   //	slogi("do do do ");
		   if(t->callback){
		   //	slogi("do callback");
		     t->callback(t->arg);
			 //		   	slogi("after do callback");

		   }
		   #endif
		    //重新计时
		    t->mStartRecordTime = now;
        }

        //延时20秒
        sleep(t->interval/5); //second
    	//usleep(30*1000);// milliseconds毫秒啊
    }
	sc_unlock(&t->mLock);
}

#if 0
void recordByOneMinutes(){
    struct tm *newtime;
    char tmpbuf[128];

    newtime=GetCurrTimeAtom();
    //to get a filename named by current time
    strftime(tmpbuf, 128, "MEZB_%Y_%m_%d__%H_%M_%S.mp4", newtime);
    //    printf("%s\n",tmpbuf);
    char *basestr="/mnt/sdcard/";
    char mNewFilename[256];
    strcpy(mNewFilename,basestr);
    strcat(mNewFilename,tmpbuf);
    ALOGE("###the new filename[%s]",mNewFilename);
    //call this
    ALOGE("#####1 stop current worker thread  begin");
    mMEWriter->stopCurWriter();
    ALOGE("#####1 stop current worker thread  end");

    ALOGE("#####2 reinit current worker thread");
    mMEWriter->reInit(mNewFilename);
    ALOGE("#####3 restart current worker thread");
    //a@zb begin : add this restart metadata  20140819
     //如果重录制一次，这个metadata也应该重新来一次 step2
    int64_t startTimeUs = systemTime() / 1000;
    setupMPEG4MetaData(startTimeUs, METotalBitRate, &MEMeta);
    ALOGE("##before restart: startTimeUs[%lld] METotalBitRate[%d]",startTimeUs,METotalBitRate);
    //a@end
    mMEWriter->restart(MEMeta.get());
    ALOGE("#####restart over");

}
#endif
//线程实体：
void * ThreadWrapper(void *timer) {
  //  StagefrightRecorder *recorder = static_cast<StagefrightRecorder *>(me);
 //   recorder->threadFunc();
    sc_timer_t *t=(sc_timer_t *)timer;
  /*
  【格式】：	time_t time(time_t *timer);
【說明】：	型態time_t在01/19/2038之3:14:07就不能正常運作，
需特別注意，它是一長整數long int，用於儲存時間之總秒數

头文件：#include <time.h>

定义函数：time_t time(time_t *t);

函数说明：此函数会返回从公元 1970 年1 月1 日的UTC 时间从0 时0 分0 秒算起到现在所经过的秒数。如果t 并非空指针的话，此函数也会将返回值存到t 指针所指的内存。

返回值：成功则返回秒数，失败则返回((time_t)-1)值，错误原因存于errno 中。
  */
    time(&t->mStartRecordTime);
	//struct tm* tm_t;
	time_t ttt;
   // tm_t = localtime(&ttt);
   LOGD("mStartRecordTime[%d]",t->mStartRecordTime);
  //  LOGD( "#@@##timer[%d]- mStartRecordTime[%d]=[%d]", ttt, t->mStartRecordTime,ttt - t->mStartRecordTime);
    t->state=1;
	threadFunc(t);

    return NULL;
}
void createTimerThread(sc_timer_t *t){
	GetCurrTimeAtom();
    //add to support timer 20140813
    {
        LOGE("create timer thread ....");
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_create(&t->mTimerThread, &attr, ThreadWrapper, (void *)t);
        pthread_attr_destroy(&attr);
    }
}
int timerNew(sc_timer_t **t,sc_timer_callback callback,void *arg){
	(*t)=malloc(sizeof(sc_timer_t));
	if(*t){
		memset(*t,0,sizeof(sc_timer_t));
		(*t)->callback=callback;
  		  (*t)->interval=30;
		  (*t)->arg=arg;
		createTimerThread(*t);
		return 0;
	}
	return -1;

}

int timerDelete(sc_timer_t *t){
	if(t)
	{
		if(t->state){
			pthread_join(t->mTimerThread,NULL);
			t->state=0;
		}
		free(t);
		t=NULL;
	}
	return 0;

}
