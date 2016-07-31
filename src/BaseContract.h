
#ifndef BASE_CONTRACT_H_INCLUDED_
#define BASE_CONTRACT_H_INCLUDED_

#include <string>
#include <memory>
#include <vector>

class BaseContract{
	
public:
	
	virtual void SetContractInfo(const std::string &effective_date, const std::string &currency,
							int fixed_or_float, double notional_amount, double contract_term,
							double payment_period, double fixed_rate, double next_float_rate) = 0;
	
	virtual double CalcPV(const std::string &valuation_date, const std::vector<int> &floating_rate_term, const std::vector<double> &floating_rate_value, 
					const std::vector<int> &discount_curve_term, const std::vector<double> &discount_curve_value) = 0;
	
};


#endif

