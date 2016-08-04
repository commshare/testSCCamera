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

double get_current_time() {  /*��ǰʱ�䣬��λӦ����ms*/
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

  int CTimer::count = 0; /*��̬��Ա�����ĳ�ʼ��*/
CTimer::CTimer() :m_Elapse(0), m_hThread(NULL)
{
    init_timer();
    
}
CTimer::~CTimer()
{
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
	CloseHandle(m_hThread);
}
DWORD WINAPI CTimer::ThreadFunc(LPVOID pParam)
{
	time_t t1, t2;
	double  Diff = 0;
	/*Ҫ��ʱ�ļ��ʱ��*/
	int elapse = *((int *)pParam);
    #if 0
	/*��ȡϵͳ��ǰʱ��*/
	t1 = time(NULL);
	while (1)
	{
		/*����Ϊ��λ��ȡϵͳ��ǰʱ��*/
		t2 = time(NULL);
		/*�Ƚϵڶ��λ�ȡ��ʱ�����һ�ε�ʱ���ǲ��Ǽ��������*/
		Diff = difftime(t2, t1);
		/*��������ӡDiff��i*/
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