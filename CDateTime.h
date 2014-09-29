
#if !defined(_CDATETIME_H)
#define _CDATETIME_H
#include <ctime>
#include <string>
#include <iostream>
using namespace std;
class CDateTime{
private:
	static int mn[12];
	time_t t;
public:
	static bool IsValidDate(int year, int month, int day);
	static bool IsValidTime(int hour, int minu, int sec);
	CDateTime();
	CDateTime(int year,int month,int day,int hour,int minu,int sec);
	void getSysTime();
	time_t operator-(const CDateTime& b)const;
	bool operator<(const CDateTime& b)const;
	string getStr();
};
#endif