FinancialLibrary
==================

FinancialLibrary is C++ library to provide financial calculations such as building yield curve and pricing derivatives.


#Example of using FinancialLibrary

\#include "YieldCurve.h"  
\#include "Contract.h"  
\#include "FinLibException.h"  
\#include \<iostream\>  
\#include \<string\>  
\#include \<vector\>  
\#include \<memory\>  

int main(void){
	
	//////////////////////
	// Using YieldCurve
	//////////////////////
	//ON, TN, 1W, 2W, 1M, 2M, 3M, 6M and 12M
	std::vector<double> libor{0.00095, 0.001017, 0.0010357, 0.0011014, 0.0012014, 0.0013857, 0.0015429, 0.00242145, 0.0043643};
	std::vector<double> swap_rate{0.0024, 0.0026, 0.0030, 0.0036, 0.0045, 0.0055, 0.0067, 0.0078, 0.0088, 0.0099, 0.0118, 0.0145, 0.0175, 0.0189, 0.0196};
	std::vector<double> swap_grid{1,2,3,4,5,6,7,8,9,10,12,15,20,25,30};
	std::string cur = "JPY";
	std::vector<double> zeroRate(67);
	
	std::unique_ptr<YieldCurve> ycv;
	try{
		ycv.reset(new YieldCurve());
		ycv->SetBaseDate("20160617");
		ycv->SetCurrency(cur);
		ycv->SetCashRate(libor);
		ycv->SetSwapRate(swap_grid, swap_rate);
		//get ZeroRate
		ycv->CalcZeroRate(zeroRate);
	}catch(FinLibException &e){
		std::cout << "Error!" << std::endl;
		std::cout << e.What() << std::endl;
		return 0;
	}
	
	//standard output result
	std::cout << "Zero Rate" << std::endl;
	std::vector<double> termArray{1, 2, 7, 14, 30, 60, 90, 180, 360};
	for(int i = 0; i < 58; i++){
		termArray.push_back(360 + (1 + i) * 180);
	}	
	for(int i = 0; i < zeroRate.size(); i++){
		std::cout << termArray[i] << " : " <<zeroRate[i] << std::endl;
	}
	
	
	//////////////////////
	// Using Contract
	//////////////////////
	std::vector<int> float_rate_term{1, 7, 30, 180, 360, 720, 1080};
	std::vector<double> float_rate_value{0.001, 0.003, 0.007, 0.01, 0.02, 0.025, 0.03};
	std::vector<int> zero_rate_term{1, 7, 30, 180, 360, 720, 1080};
	std::vector<double> zero_rate_value{0.001, 0.003, 0.007, 0.01, 0.02, 0.025, 0.03};
	double pv = 0;
	
	std::unique_ptr<Contract> irs;
	try{
		irs.reset(new Contract("IRSwap"));
		irs->SetContractInfo("20160713", "JPY", 1, 1000000, 3, 0.5, 0.03, 0.01);
		pv = irs->CalcPV("20160719", float_rate_term, float_rate_value,
								zero_rate_term, zero_rate_value);
	}catch(FinLibException &e){
		std::cout << "Error!" << std::endl;
		std::cout << e.What() << std::endl;
		return 0;
	}
	
	//standard output result
	std::cout << "pv = " << pv << std::endl;
	
	return 0;

}
