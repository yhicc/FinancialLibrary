
#ifndef CAPFLOORCONTRACT_H_INCLUDED_
#define CAPFLOORCONTRACT_H_INCLUDED_

#include "BaseContract.h"
#include "VolatilityCube.h"
#include <string>
#include <vector>

namespace FinLib{

class CapFloorContract : public BaseContract{

private:
	
	std::string m_effective_date;
	std::string m_currency;
	int m_cap_or_floor;			// 0 == cap, 1 == floor
	double m_notional_amount;
	double m_contract_term;		// year unit. 1 year = 1
	double m_strike_rate;
	int m_frequency;			// month unit. 6 month = 6
	int m_day_count;
	double m_next_index_rate;
	int m_contract_info_set_flag;
	
public:
	
	CapFloorContract();
	
	virtual ~CapFloorContract();
	
	void SetContractInfo(
		const std::string &effective_date, 
		const std::string &currency, 
		int cap_or_floor, 
		double notional_amount, 
		double contract_term, 
		double strike_rate, 
		int frequency, 
		int day_count, 
		double next_index_rate
	);
	
	double CalcPV(
		const std::string &valuation_date, 
		const std::vector<int> &index_rate_term, 
		const std::vector<double> &index_rate_value, 
		int index_rate_day_count_basis, 		// ACT/360 == 360, ACT/365 == 365
		int index_rate_compound_period, 		// PA == 1, SA == 2, Continuous == 0
		const std::vector<int> &discount_curve_term, 
		const std::vector<double> &discount_curve_value, 
		int discount_rate_day_count_basis, 		// ACT/360 == 360, ACT/365 == 365
		int discount_rate_compound_period, 		// PA == 1, SA == 2, Continuous == 0
		const std::vector<VolatilityCube> &volatility_set, 
		int num_of_vol_strike_rate, 
		int num_of_vol_underlying_term_grid, 
		int num_of_vol_option_term_grid
	);

};

}

#endif

