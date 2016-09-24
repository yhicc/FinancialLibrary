
#ifndef YIELDCURVEIMPL_H_INCLUDED_
#define YIELDCURVEIMPL_H_INCLUDED_

#include "YieldCurve.h"

namespace FinLib{

class YieldCurve::YieldCurveImpl{
public:
	
	std::string m_base_date;
	std::string m_currency;
	
	std::vector<int> m_cash_rate_term;
	std::vector<double> m_cash_rate_value;
	int m_cash_rate_day_count;
	std::vector<int> m_default_grid_cash_rate_term;
	std::vector<double> m_default_grid_cash_rate_value;
	
	std::vector<double> m_swap_rate_term;
	std::vector<double> m_swap_rate_value;
	int m_swap_rate_day_count;
	
	std::vector<int> m_zero_rate_term;
	std::vector<double> m_zero_rate_value;
	std::vector<double> m_discount_factor_value;
	
	int built_zero_rate_flag;
	
	
	//bootstrapMethod
	void BuildLongEndOfCurve();
	void BuildShortEndOfCurve();
	void BuildYieldCurve();
	
	//convert discount factor to zero rate
	void ConvertZeroRate(int day_count, int compound_period);
};

}

#endif

