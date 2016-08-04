/********CTimer.h***********/
#ifndef CTIMER_H_
#define CTIMER_H_
#include <Windows.h>

/*
http://blog.csdn.net/rabbit729/article/details/2729065
*/
class CTimer
{
public:
	CTimer();
	~CTimer();
	void StartTimer(unsigned int nElapse);
	void EndTimer();
	static DWORD WINAPI ThreadFunc(LPVOID pParam);
	void addcount();
private:
	unsigned int m_Elapse;
	HANDLE m_hThread;
	static int count; /*必须搞成静态的，才能在线程函数里头直接使用*/
};
#endif