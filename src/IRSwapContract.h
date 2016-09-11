
#ifndef IRSWAPCONTRACT_H_INCLUDED_
#define IRSWAPCONTRACT_H_INCLUDED_

#include "BaseContract.h"
#include <string>
#include <vector>


class IRSwapContract : public BaseContract{

private:
	
	std::string m_effective_date;
	std::string m_next_rate_fixing_date;
	std::string m_currency;
	int m_fixed_or_float_payer;		//1 == fixed, 2 == floating
	double m_notional_amount;
	double m_contract_term;		//1year = 1
	double m_payment_period;	//6month = 0.5
	double m_fixed_rate;
	double m_next_float_rate;
	int m_contract_info_set_flag;
	
public:
	
	IRSwapContract();
	virtual ~IRSwapContract();
	void SetContractInfo(const std::string &effective_date, const std::string &currency,
							int fixed_or_float, double notional_amount, double contract_term,
							double payment_period, double fixed_rate, double next_float_rate);
	
	double CalcPV(const std::string &valuation_date, const std::vector<int> &floating_rate_term, const std::vector<double> &floating_rate_value, 
					const std::vector<int> &discount_curve_term, const std::vector<double> &discount_curve_value);

};


#endif

