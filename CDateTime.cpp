#include "CDateTime.h"
int CDateTime::mn[12]={31,28,31,30,31,30,31,31,30,31,30,31};
bool CDateTime::IsValidDate(int year, int month, int day){
	if (year<1||!(month>0&&month<13))
	{
		return false;
	}
	int mday=mn[month-1];
	if (month==2)
	{
		if ((year%100&&year%4==0)||(year%400==0))
		{
			mday++;
		}
	}
	if (day<1||day>mday)
	{
		return false;
	}
	return true;
}
bool CDateTime::IsValidTime(int hour, int minu, int sec){
	if (hour<0||hour>23)
	{
		return false;
	}
	if (minu<0||minu>59)
	{
		return false;
	}
	if (sec<0||sec>59)
	{
		return false;
	}
	return true;
}
CDateTime::CDateTime(){
	t = time(NULL);
}
CDateTime::CDateTime(int year,int month,int day,int hour,int minu,int sec){
	if (IsValidDate(year,month,day)&&IsValidTime(hour,minu,sec))
	{
		tm* pTmp=new tm();
		pTmp->tm_year=year-1900;
		pTmp->tm_mon=month-1;
		pTmp->tm_mday=day;
		pTmp->tm_hour=hour;
		pTmp->tm_min=minu;
		pTmp->tm_sec=sec;
		t=mktime(pTmp);
		delete pTmp;
	}else {
		t=0;
	}
}
void CDateTime::getSysTime(){
	t = time(NULL);
}
time_t CDateTime::operator-(const CDateTime& b)const{
	time_t res=t-b.t;
	return res;
}
bool CDateTime::operator<(const CDateTime& b)const{
	time_t res=t-b.t;
	return res<0;
}
string CDateTime::getStr(){
	tm * pTmp;
	char s[100];
	pTmp=localtime(&t);
	sprintf(s,"%d-%d-%d %d:%d:%d", pTmp->tm_year + 1900, pTmp->tm_mon + 1, pTmp->tm_mday, pTmp->tm_hour, pTmp->tm_min, pTmp->tm_sec);
	return s;
}