
#ifndef ESWAPTIONCONTRACT_H_INCLUDED_
#define ESWAPTIONCONTRACT_H_INCLUDED_

#include "BaseContract.h"
#include "VolatilityCube.h"
#include <string>
#include <vector>

namespace FinLib{

class EuropeanSwaptionContract : public BaseContract{

private:
	std::string m_effective_date;
	std::string m_currency;
	int m_receiver_or_payer;					//0 == receiver, 1 == payer
	std::string m_option_maturity_date;
	double m_strike_swap_rate;
	int m_option_day_count;
	std::string m_underlying_swap_start_date;
	double m_underlying_swap_notional_amount;
	double m_underlying_swap_contract_term;		//1year = 1
	double m_underlying_swap_payment_period;	//6month = 0.5
	int m_underlying_swap_day_count;
	double m_undelying_swap_spread_on_index_rate;
	int m_contract_info_set_flag;
		
public:
	
	EuropeanSwaptionContract();
	
	virtual ~EuropeanSwaptionContract();
	
	void SetContractInfo(
		const std::string &effective_date, 
		const std::string &currency, 
		int receiver_or_payer, 
		const std::string &option_maturity_date, 
		double strike_swap_rate, 
		int option_day_count, 
		const std::string &underlying_swap_start_date, 
		double underlying_swap_notional_amount, 
		double underlying_swap_contract_term, 
		double underlying_swap_payment_period, 
		int underlying_swap_day_count, 
		double undelying_swap_spread_on_index_rate
	);
	
	double CalcPV(
		const std::string &valuation_date, 
		const std::vector<int> &index_rate_term, 
		const std::vector<double> &index_rate_value, 
		int index_rate_day_count_basis, 	// ACT/360 == 360, ACT/365 == 365
		int index_rate_compound_period, 	// PA == 1, SA == 2, Continuous == 0
		const std::vector<int> &discount_curve_term, 
		const std::vector<double> &discount_curve_value, 
		int discount_rate_day_count_basis, 	// ACT/360 == 360, ACT/365 == 365
		int discount_rate_compound_period, 	// PA == 1, SA == 2, Continuous == 0
		const std::vector<VolatilityCube> &volatility_set, 
		int num_of_vol_strike_rate, 
		int num_of_vol_underlying_term_grid, 
		int num_of_vol_option_term_grid
	);

};

}

#endif

