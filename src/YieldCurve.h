
#ifndef YIELDCURVE_H_INCLUDED_
#define YIELDCURVE_H_INCLUDED_

#include <string>
#include <vector>
#include <memory>

namespace FinLib{

class YieldCurve{

public:
	
	//Constructor
	YieldCurve();
	
	//Destructor
	virtual ~YieldCurve();
	
	//setter
	void SetBaseDate(const std::string &base_date);
	void SetCurrency(const std::string &cur);
	
	//cash rate for ON, TN, 1W, 2W, 1M, 2M, 3M, 6M and 12M
	void SetCashRate(
		const std::vector<double> &cash_rate_value, 
		int cash_day_count
	);
	
	//tem is the number of year
	void SetSwapRate(
		const std::vector<double> &swap_term, 
		const std::vector<double> &swap_rate, 
		int swap_day_count
	);
	
	//getter
	std::string GetBaseDate();
	std::string GetCurrency();
	void GetCashRate(std::vector<double> &cashrate);
	void GetSwapRate(
		std::vector<double> &swap_term, 
		std::vector<double> &swap_rate
	);
	
	//Build Yield Curve function
	void CalcDiscountFactor(std::vector<double> &df);
	void CalcZeroRate(
		std::vector<double> &zero_rate, 
		int day_count, 
		int compound_period
	);
	
	
private:
	
	class YieldCurveImpl;
	std::unique_ptr<YieldCurveImpl> m_yieldcurve_impl;
	
};

}

#endif

