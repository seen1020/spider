#include "CConvert.h"
#include "CValid.h"
int CConvert::ToInt(const string &str){
	int res;
	if (CValid::IsValidInteger(str))
	{
		istrstream ss(str.c_str());
		ss>>res;
	}else {
		res=-1;
// 		throw "不是整形";
	}
	return res;
}
double CConvert::ToDouble(const string &str){
	if (str.size()==0)
	{
		return false;
	}
	istrstream ss(str.c_str());
	double res;
	ss>>res;
	if (!ss.eof()) {
		throw "不是整形";
	}
	return res;
}
string CConvert::ToString(int data){
	char s[20];
	sprintf(s,"%d",data);
	return s;
}
string CConvert::ToString(double data){
	char s[100];
	sprintf(s,"%lf",data);
	return s;
}