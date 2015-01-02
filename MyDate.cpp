
#include "MyDate.h"
using std::string;
using std::stringstream;


//constructor
MyDate::MyDate(){
	julianDay = 0;
}

MyDate::MyDate(string date){
	strDate = date;
	julianDay = 0;
}


//destructor
/*
MyDate::~MyDate(){

}
*/

//setter
void MyDate::setDate(string date){
	strDate = date;
}
void MyDate::setJulianDay(int julDay){
	julianDay = julDay;
}


//getter
string MyDate::getDate(){
	return strDate;
}
int MyDate::getJulianDay(){
	return julianDay;
}


//utilFunc
int MyDate::stringToJulian(string date){
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


string MyDate::julianToString(int julDay){
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

int MyDate::calcDateDiff(string date1, string date2){
	int julDate1 = stringToJulian(date1);
	int julDate2 = stringToJulian(date2);
	return (julDate2 - julDate1);
}

int MyDate::calcDateDiff(MyDate date1, MyDate date2){
	int julDate1 = stringToJulian(date1.getDate());
	int julDate2 = stringToJulian(date2.getDate());
	return (julDate2 - julDate1);
}

string MyDate::addDate(string date, int days){
	int julDate = stringToJulian(date);
	int addedJulDate = julDate + days;
	return julianToString(addedJulDate);
}

string MyDate::addDate(MyDate date, int days){
	int julDate = stringToJulian(date.getDate());
	int addedJulDate = julDate + days;
	return julianToString(addedJulDate);
}


