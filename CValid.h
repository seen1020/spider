#ifndef CVALID_H
#define CVALID_H

#include <string>
#include <strstream>
#include <string>
using namespace std;
class CValid
{
public:
	static bool IsValidURL(const string &value);
	static bool IsValidInteger(const string &value);
	static bool IsValidIP(const string &value);
protected:
private:
};

#endif
