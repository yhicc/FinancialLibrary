
#ifndef CONTRACT_H_INCLUDED_
#define CONTRACT_H_INCLUDED_

#include <string>
#include <memory>

class Contract{
	
public:
	
	Contract(const std::string &product);
	virtual ~Contract();
	
	void SetContractInfo(const std::string &effective_date, const std::string &currency,
							int fixed_or_float, double notional_amount, double contract_term,
							double payment_period, double fixed_rate, double next_float_rate);
	
	double CalcPV(const std::string &valuation_date, const std::vector<int> &floating_rate_term, const std::vector<double> &floating_rate_value, 
					const std::vector<int> &discount_curve_term, const std::vector<double> &discount_curve_value);
	
private:
	
	class ContractImpl;
	std::unique_ptr<ContractImpl> m_contract_impl;
	
};


#endif

