#ifndef CCONVERT_H
#define CCONVERT_H
#include <strstream>
using namespace std;
class CConvert
{
public:
	static int ToInt(const string &str);
	static double ToDouble(const string &str);
	static string ToString(int data);
	static string ToString(double data);
protected:
private:
};
#endif