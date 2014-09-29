#ifndef CSUMMARY_H
#define CSUMMARY_H
#include <iostream>
#include <string>
#include "CCriticalSection.h"
#include <Windows.h>
using namespace std;
#include<windows.h>//记住把头文件那个了
class CSummary{
	int totalUp,totalDown,total2;
	int nSuccess;
	int nFail;
	int t;
	int speed;
	CCriticalSection me;
	HANDLE hThread;
	CSummary();
	~CSummary();
	static DWORD CALLBACK Thread(PVOID pvoid);
	static void WINAPI TimerProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime);
public:
	static CSummary o;
	static CSummary& getInstance();
	void init();
	int addTotalUp(int x);
	int addTotalDown(int x);
	int addNSuccess(int x);
	int addNFail(int x);
	int getTime();
	int getSpeed();
};
#endif