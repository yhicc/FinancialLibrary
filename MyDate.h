
#ifndef MYDATE_H_INCLUDED_
#define MYDATE_H_INCLUDED_

#include <string>

class MyDate{

private:

	std::string strDate;	//yyyymmdd format
	int julianDay;



public:

	//constructor
	MyDate();
	MyDate(std::string date);

	//destructor
//	~MyDate();

	//setter
	void setDate(std::string date);
	void setJulianDay(int julDay);

	//getter
	std::string getDate();
	int getJulianDay();

	//utilFunc
	int stringToJulian(std::string date);
	std::string julianToString(int julDay);
	int calcDateDiff(std::string date1, std::string date2);
	int calcDateDiff(MyDate date1, MyDate date2);
	std::string addDate(std::string date, int days);
	std::string addDate(MyDate date, int days);
	std::string addMonth(std::string date, int months);
//	std::string addMonth(MyDate date, int months);

};

#endif

