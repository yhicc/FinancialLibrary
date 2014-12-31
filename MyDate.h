
#ifndef MYDATE_H_INCLUDED_
#define MYDATE_H_INCLUDED_

#include <string>
using namespace std;

class MyDate{

private:

	string strDate;	//yyyymmdd format
	int julianDay;



public:

	//constructor
	MyDate();
	MyDate(string date);

	//destructor
	~MyDate();

	//setter
	void setDate(string date);
	void setJulianDay(int julDay);

	//getter
	string getDate();
	int getJulianDay();

	//utilFunc
	int stringToJulian(string date);
	string julianToString(int julDay);
	int calcDateDiff(string date1, string date2);
	int calcDateDiff(MyDate date1, MyDate date2);

};

#endif

