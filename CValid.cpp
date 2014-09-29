#include "CValid.h"
bool CValid::IsValidURL(const string &value){
	return false;
}
bool CValid::IsValidInteger(const string &value){
	if (value.size()==0)
	{
		return false;
	}
	istrstream ss(value.c_str());
	int res;
	ss>>res;
	if (ss.eof())
	{
		return true;
	}
	return false;

}
bool CValid::IsValidIP(const string &value){
	if (value.size()==0)
	{
		return false;
	}
	for (int i=0;i<value.size();i++)
	{
		char c=value.at(i);
		if (!isdigit(c)&&c!='.')
		{
			return false;
		}
	}
	istrstream ss(value.c_str());
	int res;
	char c;
	for (int i=0;i<3;i++)
	{
		if (ss>>res&&(res>=0&&res<256))
		{
			ss>>c;
		}else return false;
	}
	if (ss>>res&&(res>=0&&res<256))
	{
		return true;
	}else return false;

}