
#include "Contract.h"
#include "BaseContract.h"
#include "VolatilityCube.h"
#include <string>


class Contract::ContractImpl{
public:
	
	// for setting IR Swap contract info
	void SetContractInfo(
		const std::string &effective_date, 
		const std::string &currency, 
		int fixed_or_float, 			// 0 == fixed payer, 1 == float payer
		double notional_amount, 
		double contract_term, 
		double payment_period, 
		double fixed_rate, 
		double spread_on_index_rate, 
		int day_count, 
		double next_float_rate
	);
	
	// for setting European Swaption contract info
	void SetContractInfo(
		const std::string &effective_date, 
		const std::string &currency, 
		int receiver_or_payer, 
		const std::string &option_maturity_date, 
		const std::string &underlying_swap_start_date, 
		double underlying_swap_notional_amount, 
		double underlying_swap_contract_term, 
		double underlying_swap_payment_period, 
		int underlying_swap_day_count, 
		double undelying_swap_spread_on_index_rate, 
		double strike_swap_rate
	);
	
	// for calculating IR Swap PV
	double CalcPV(
		const std::string &valuation_date, 
		const std::vector<int> &index_rate_term,  
		const std::vector<double> &index_rate_value, 
		int index_rate_day_count_basis, 	// ACT/360 == 360, ACT/365 == 365
		int index_rate_compound_period, 	// PA == 1, SA == 2, Continuous == 0
		const std::vector<int> &discount_curve_term, 
		const std::vector<double> &discount_curve_value, 
		int discount_rate_day_count_basis, 	// ACT/360 == 360, ACT/365 == 365
		int discount_rate_compound_period	// PA == 1, SA == 2, Continuous == 0
	);
	
	// for calculating European Swaption PV
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
	
	std::unique_ptr<BaseContract> m_contract;
	void CreateContract(const std::string &product);
	
};



