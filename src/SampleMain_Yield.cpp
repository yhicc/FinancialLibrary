
#include "YieldCurve.h"
#include "FinLibException.h"
#include <iostream>
#include <string>
#include <vector>
using std::vector;

int main(void){
	//ON, TN, 1W, 2W, 1M, 2M, 3M, 6M and 12M
	vector<double> liborArr{0.00095, 0.001017, 0.0010357, 0.0011014, 0.0012014, 0.0013857, 0.0015429, 0.00242145, 0.0043643};
	vector<double> swapArr{0.0024, 0.0026, 0.0030, 0.0036, 0.0045, 0.0055, 0.0067, 0.0078, 0.0088, 0.0099, 0.0118, 0.0145, 0.0175, 0.0189, 0.0196};
	
	vector<double> swapGrid{1,2,3,4,5,6,7,8,9,10,12,15,20,25,30};
	
	std::string cur = "JPY";
	
	YieldCurve *ycv;
	ycv = new YieldCurve();
	vector<double> zeroRate(67);
	
	try{
		ycv->SetBaseDate("20160617");
		ycv->SetCurrency(cur);
		ycv->SetCashRate(liborArr);
		ycv->SetSwapRate(swapGrid, swapArr);
		//get ZeroRate
		ycv->CalcZeroRate(zeroRate);
	}catch(FinLibException &e){
		std::cout << "Error!" << std::endl;
		std::cout << e.What() << std::endl;
		delete ycv;
		return 0;
	}
	
	//print check
	std::cout << "Base date : " << ycv->GetBaseDate() << std::endl;
	std::cout << "Currency : " << ycv->GetCurrency() << std::endl;

	vector<double> termArray{1, 2, 7, 14, 30, 60, 90, 180, 360};
	for(int i = 0; i < 58; i++){
		termArray.push_back(360 + (1 + i) * 180);
	}	
	for(int i = 0; i < zeroRate.size(); i++){
		std::cout << termArray[i] << " : " <<zeroRate[i] << std::endl;
	}
	
	delete ycv;
	
	return 0;

}

