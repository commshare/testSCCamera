#include "CTimer.h"
#include<iostream>

//#include <time.h>   /*http://blog.csdn.net/coder_xia/article/details/6566708*/
#include <ctime>

using namespace std;




#ifdef WIN32

#include <Windows.h>

static LARGE_INTEGER frequency; // ticks per second

void init_timer( void ){
	QueryPerformanceFrequency(&frequency);
}

double get_current_time() {  /*当前时间，单位应该是ms*/
	static LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return 1000.0 * t.QuadPart /frequency.QuadPart;
}

#else /*linux*/


#include <sys/time.h>

void init_timer( void ){ }

double get_current_time() {
	static timeval t;
    gettimeofday(&t, NULL);
	return 1.0 * t.tv_sec * 1000.0  // sec to ms
        + 1.0 * t.tv_usec / 1000.0; // us to ms
}

#endif

  int CTimer::count = 0; /*静态成员变量的初始化*/
CTimer::CTimer() :m_Elapse(0), m_hThread(NULL)
{
    init_timer();
    /*
        http://www.debugease.com/vc/2149406.html
    */
    m_hThread = 0;

}
CTimer::~CTimer()
{
    if(m_hThread){
        printf("timer thread running ,call EndTimer \n");
        /*should use std::thread
            https://github.com/forhappy/Cplusplus-Concurrency-In-Practice/blob/master/zh/chapter3-Thread/Introduction-to-Thread.md
        */
        EndTimer();
    }
}
void CTimer::addcount(){
    count++;
}
void CTimer::StartTimer(unsigned int nElapse)
{
	m_Elapse = nElapse;
	m_hThread = CreateThread(NULL, 0, ThreadFunc, (LPVOID)(&m_Elapse), 0, NULL);
}
void CTimer::EndTimer()
{
    /* http://stackoverflow.com/questions/18465681/when-to-call-closehandle
        The thread object remains in the system until the thread has terminated and all handles to it have been closed through a call to CloseHandle.
    */
	CloseHandle(m_hThread);
}
DWORD WINAPI CTimer::ThreadFunc(LPVOID pParam)
{
	time_t t1, t2;
	double  Diff = 0;
	/*要定时的间隔时间*/
	int elapse = *((int *)pParam);
    #if 0
	/*获取系统当前时间*/
	t1 = time(NULL);
	while (1)
	{
		/*以秒为单位获取系统当前时间*/
		t2 = time(NULL);
		/*比较第二次获取的时间与第一次的时间是不是间隔了两秒*/
		Diff = difftime(t2, t1);
		/*间隔两秒打印Diff和i*/
		if ((int)Diff == elapse)
		{
			cout << "Time out!" << endl;
			t1 = t2;
		}
	}
    #else
    double begin_time=get_current_time();
    while(1){
        double now=get_current_time();
        Diff=now - begin_time;
        if((int)Diff >=elapse){
            cout << "##"<<count <<"##"<< endl;
            begin_time=now;
			count = 0;
        }
    }
    #endif
	return 0;
}
