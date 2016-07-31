
#include "IRSwapContract.h"
#include "DateUtil.h"
#include <iostream>
#include <string>
#include <vector>
using std::vector;

int main(void){
	
	vector<int> float_rate_term{1, 7, 30, 180, 360, 720, 1080};
	vector<double> float_rate_value{0.001, 0.003, 0.007, 0.01, 0.02, 0.025, 0.03};
	vector<int> zero_rate_term{1, 7, 30, 180, 360, 720, 1080};
	vector<double> zero_rate_value{0.001, 0.003, 0.007, 0.01, 0.02, 0.025, 0.03};
	
	IRSwapContract *irs;
	irs = new IRSwapContract();
	
	irs->SetContractInfo("20160713", "JPY", 1, 1000000, 3, 0.5, 0.03, 0.01);
	double pv = irs->CalcPV("20160719", float_rate_term, float_rate_value,
							zero_rate_term, zero_rate_value);
	
	std::cout << "pv = " << pv << std::endl;
	
	return 0;

}

