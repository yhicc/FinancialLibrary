
#include "DateUtil.h"
#include <sstream>

using std::string;
using std::stringstream;

namespace FinLib{

//constructor
DateUtil::DateUtil(){
	julianDay = 0;
}

DateUtil::DateUtil(const string &date){
	strDate = date;
	julianDay = 0;
}


//setter
void DateUtil::setDate(const string &date){
	strDate = date;
}
void DateUtil::setJulianDay(int julDay){
	julianDay = julDay;
}


//getter
string DateUtil::getDate(){
	return strDate;
}
int DateUtil::getJulianDay(){
	return julianDay;
}


//utilFunc
int DateUtil::stringToJulian(const string &date){
	string strY = date.substr(0, 4);
	string strM = date.substr(4, 2);
	string strD = date.substr(6, 2);

	int year;
	stringstream ss(strY);
	ss >> year;
	ss.str("");
	ss.clear(stringstream::goodbit);

	int month;
	ss.str(strM);
	ss >> month;
	ss.str("");
	ss.clear(stringstream::goodbit);

	int day;
	ss.str(strD);
	ss >> day;
	ss.str("");
	ss.clear(stringstream::goodbit);

	if(month <= 2){
		year--;
		month += 12;
	}

	int a = year / 100;
	int b = 2 - a + (int)(a / 4);

	int c = 365.25 * (year + 4716);
	int d = 30.6001 * (month +1);
	int e = day + b - 1524.5;

	return (c + d + e);
}


string DateUtil::julianToString(int julDay){
	double doublejulDay = julDay + 0.5;
	int z = doublejulDay;
	double f = doublejulDay - z;

	int a = 0;
	if(z < 2299161){
		a = z;
	}else{
		int x = (z - 1867216.25) / 36524.25;
		a = z + 1 + x - (int)(x / 4);
	}

	int b = a + 1524;
	int c = (b - 122.1) / 365.25;
	int d = 365.25 * c;
	int e = (b - d) / 30.6001;

	int day = b - d - (int)(30.6001 * e) + f;
	int month = 0;
	if( e < 14){
		month = e - 1;
	}else{
		month = e - 13;
	}
	int year = 0;
	if(month > 2){
		year = c - 4716;
	}else{
		year = c - 4715;
	}

	string strY, strM, strD;
	stringstream ss;
	ss << year;
	strY = ss.str();
	int length = strY.length();
	int i = 0;
	while(i < (4 - length)){
		strY = "0" + strY;
		i++;
	}
	ss.str("");
	ss << month;
	strM = ss.str();
	if(strM.length() == 1){
		strM = "0" + strM;
	}
	ss.str("");
	ss << day;
	strD = ss.str();
	ss.str("");
	ss.clear(stringstream::goodbit);
	if(strD.length() == 1){
		strD = "0" + strD;
	}

	return strY + strM + strD;

}

int DateUtil::calcDateDiff(const string &date1, const string &date2){
	int julDate1 = stringToJulian(date1);
	int julDate2 = stringToJulian(date2);
	return (julDate2 - julDate1);
}

int DateUtil::calcDateDiff(DateUtil date1, DateUtil date2){
	int julDate1 = stringToJulian(date1.getDate());
	int julDate2 = stringToJulian(date2.getDate());
	return (julDate2 - julDate1);
}

string DateUtil::addDate(const string &date, int days){
	int julDate = stringToJulian(date);
	int addedJulDate = julDate + days;
	return julianToString(addedJulDate);
}

string DateUtil::addDate(DateUtil date, int days){
	int julDate = stringToJulian(date.getDate());
	int addedJulDate = julDate + days;
	return julianToString(addedJulDate);
}

//Return following date if the date is holiday
string DateUtil::addDateHC(const string &date, int days){
	int julDate = stringToJulian(date);
	int addedJulDate = julDate + days;
	string addedStrDate = julianToString(addedJulDate);
	
	//holiday consideration
	//string -> int
	string year = addedStrDate.substr(0, 4);
	string month = addedStrDate.substr(4, 2);
	string day = addedStrDate.substr(6, 2);
	int intYear;
	stringstream ss(year);
	ss >> intYear;
	ss.str("");
	ss.clear(stringstream::goodbit);
	int intMonth;
	ss.str(month);
	ss >> intMonth;
	ss.str("");
	ss.clear(stringstream::goodbit);
	int intDay;
	ss.str(day);
	ss >> intDay;
	ss.str("");
	ss.clear(stringstream::goodbit);
	//get day of week
	int day_of_week = GetDayOfWeek(intYear, intMonth, intDay);
	//if Saturday
	if(day_of_week == 6){
		addedJulDate = addedJulDate + 2;
		string addedStrDate = julianToString(addedJulDate);
	//if Sunday
	}else if(day_of_week == 0){
		addedJulDate = addedJulDate + 1;
		string addedStrDate = julianToString(addedJulDate);
	}
	
	return addedStrDate;
}

string DateUtil::addMonth(const string &date, int months){
	//string -> int
	string strY = date.substr(0, 4);
	string strM = date.substr(4, 2);
	string strD = date.substr(6, 2);
	int year;
	stringstream ss(strY);
	ss >> year;
	ss.str("");
	ss.clear(stringstream::goodbit);
	int month;
	ss.str(strM);
	ss >> month;
	ss.str("");
	ss.clear(stringstream::goodbit);
	int day;
	ss.str(strD);
	ss >> day;
	ss.str("");
	ss.clear(stringstream::goodbit);
	
	//add month
	if(months > 12){
		year = year + (months / 12);
		months = months % 12;
	}
	int addedMonth = month + months;
	if(addedMonth > 12){
		year++;
		addedMonth = addedMonth - 12;
	}
	//end of month adjustment
	if(addedMonth == 2){
		if(day > 28){
			day = day - 28;
			addedMonth++;
		}
	}
	if(day > 30){
		if(addedMonth == 4 || addedMonth == 6 || addedMonth == 9 || addedMonth == 11){
			addedMonth++;
			day = day - 30;
		}
	}
	
	//int -> string
	ss << year;
	strY = ss.str();
	int length = strY.length();
	int i = 0;
	while(i < (4 - length)){
		strY = "0" + strY;
		i++;
	}
	ss.str("");
	ss << addedMonth;
	strM = ss.str();
	if(strM.length() == 1){
		strM = "0" + strM;
	}
	ss.str("");
	ss << day;
	strD = ss.str();
	ss.str("");
	ss.clear(stringstream::goodbit);
	if(strD.length() == 1){
		strD = "0" + strD;
	}

	return strY + strM + strD;

}



string DateUtil::addMonth(const string &date, int months, const string &holiday_adjustment){
	//string -> int conversion
	string strY = date.substr(0, 4);
	string strM = date.substr(4, 2);
	string strD = date.substr(6, 2);
	int year;
	stringstream ss(strY);
	ss >> year;
	ss.str("");
	ss.clear(stringstream::goodbit);
	int month;
	ss.str(strM);
	ss >> month;
	ss.str("");
	ss.clear(stringstream::goodbit);
	int day;
	ss.str(strD);
	ss >> day;
	ss.str("");
	ss.clear(stringstream::goodbit);
	
	//add month
	if(months > 12){
		year = year + (months / 12);
		months = months % 12;
	}
	int addedMonth = month + months;
	if(addedMonth > 12){
		year++;
		addedMonth = addedMonth - 12;
	}
	
	//end of month adjustment
	if(addedMonth == 2){
		if(day > 28){
			day = 28;
		}
	}else if(addedMonth == 4 || addedMonth == 6 || addedMonth == 9 || addedMonth == 11){
		if(day > 30){
			day = 30;
		}
	}
	
	//Holiday adjustment
	std::vector<int> vector_date;
	vector_date.push_back(year);
	vector_date.push_back(addedMonth);
	vector_date.push_back(day);
	AdjustHolidays(vector_date, holiday_adjustment);
	year = vector_date[0];
	addedMonth = vector_date[1];
	day = vector_date[2];
	
	//int -> string conversion
	ss << year;
	strY = ss.str();
	int length = strY.length();
	int i = 0;
	while(i < (4 - length)){
		strY = "0" + strY;
		i++;
	}
	ss.str("");
	ss << addedMonth;
	strM = ss.str();
	if(strM.length() == 1){
		strM = "0" + strM;
	}
	ss.str("");
	ss << day;
	strD = ss.str();
	ss.str("");
	ss.clear(stringstream::goodbit);
	if(strD.length() == 1){
		strD = "0" + strD;
	}

	return strY + strM + strD;

}


string DateUtil::addYear(const string &date, int years, const string &holiday_adjustment){
	//string -> int conversion
	string strY = date.substr(0, 4);
	string strM = date.substr(4, 2);
	string strD = date.substr(6, 2);
	int year;
	stringstream ss(strY);
	ss >> year;
	ss.str("");
	ss.clear(stringstream::goodbit);
	int month;
	ss.str(strM);
	ss >> month;
	ss.str("");
	ss.clear(stringstream::goodbit);
	int day;
	ss.str(strD);
	ss >> day;
	ss.str("");
	ss.clear(stringstream::goodbit);
	
	//add year
	int addedYear = year + years;
	
	//うるう年の調整
	
	
	//Holiday adjustment
	std::vector<int> vector_date;
	vector_date.push_back(addedYear);
	vector_date.push_back(month);
	vector_date.push_back(day);
	AdjustHolidays(vector_date, holiday_adjustment);
	addedYear = vector_date[0];
	month = vector_date[1];
	day = vector_date[2];
	
	//int -> string conversion
	ss << addedYear;
	strY = ss.str();
	int length = strY.length();
	int i = 0;
	while(i < (4 - length)){
		strY = "0" + strY;
		i++;
	}
	ss.str("");
	ss << month;
	strM = ss.str();
	if(strM.length() == 1){
		strM = "0" + strM;
	}
	ss.str("");
	ss << day;
	strD = ss.str();
	ss.str("");
	ss.clear(stringstream::goodbit);
	if(strD.length() == 1){
		strD = "0" + strD;
	}

	return strY + strM + strD;

}


//holiday adjustment
//Only "Modified Following" is available now
void DateUtil::AdjustHolidays(std::vector<int> &date, string holiday_adjustment){
	int year = date[0];
	int month = date[1];
	int day = date[2];
	
	int day_of_week = GetDayOfWeek(year, month, day);
	
	if(holiday_adjustment == "MF"){
		//if Saturday
		if(day_of_week == 6){
			day = day + 2;
		//if Sunday
		}else if(day_of_week == 0){
			day = day + 1;
		}
		if(month == 2){
			if(day > 28){
				if(day_of_week == 6){
					day = day - 3;			//
				}else if(day_of_week == 0){
					day = day - 3;			//
				}
			}
		}else if(month == 4 || month == 6 || month == 9 || month == 11){
			if(day > 30){
				if(day_of_week == 6){
					day = day - 3;
				}else if(day_of_week == 0){
					day = day - 3;
				}
			}
		}else{
			if(day > 31){
				if(day_of_week == 6){
					day = day - 3;
				}else if(day_of_week == 0){
					day = day - 3;
				}
			}
		}
	}
	
	date[0] = year;
	date[1] = month;
	date[2] = day;
}


// 0 = Sun, 1 = Mon, 2 = Tue, 3 = Wed, 4 = Thu, 5 = Fri, 6 = Sat
int DateUtil::GetDayOfWeek(int year, int month, int day){
	// １月 と ２月 を前年の 13月 と 14月 として扱う
	if ( month <= 2 ){
		year  -= 1;
		month += 12;
	}

	// return day of week number
	return (day + ((8 + (13 * month)) / 5) + (year + (year / 4) - (year / 100) + (year / 400))) % 7;
}

}




