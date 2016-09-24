
#ifndef MYDATE_H_INCLUDED_
#define MYDATE_H_INCLUDED_

#include <string>
#include <vector>

namespace FinLib{

class DateUtil{

private:

	std::string strDate;	//yyyymmdd format
	int julianDay;
	
	void AdjustHolidays(std::vector<int> &date, std::string holiday_adjustment);



public:

	//constructor
	DateUtil();
	DateUtil(const std::string &date);

	//setter
	void setDate(const std::string &date);
	void setJulianDay(int julDay);

	//getter
	std::string getDate();
	int getJulianDay();

	//utilFunc
	int stringToJulian(const std::string &date);
	std::string julianToString(int julDay);
	int calcDateDiff(const std::string &date1, const std::string &date2);
	int calcDateDiff(DateUtil date1, DateUtil date2);
	std::string addDate(const std::string &date, int days);
	std::string addDate(DateUtil date, int days);
	std::string addDateHC(const std::string &date, int days);
	std::string addMonth(const std::string &date, int months);
	std::string addMonth(const std::string &date, int months, const std::string &holiday_adjustment);
	std::string addYear(const std::string &date, int years, const std::string &holiday_adjustment);
	int GetDayOfWeek(int year, int month, int day);

};

}

#endif

