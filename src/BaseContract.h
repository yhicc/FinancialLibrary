
#ifndef BASE_CONTRACT_H_INCLUDED_
#define BASE_CONTRACT_H_INCLUDED_

#include "FinLibException.h"
#include "VolatilityCube.h"
#include <string>
#include <memory>
#include <vector>

namespace FinLib{

class BaseContract{
	
public:
	
	// for setting IR Swap contract info
	virtual void SetContractInfo(
		const std::string &effective_date, 
		const std::string &currency, 
		int fixed_or_float, 
		double notional_amount, 
		double contract_term, 
		double payment_period, 
		double fixed_rate, 
		double spread_on_index_rate, 
		int day_count, 
		double next_float_rate
	){
		throw FinLibException("Bad argument for SetContractInfo");
	}
	
	// for setting European Swaption contract info
	virtual void SetContractInfo(
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
	){
		throw FinLibException("Bad argument for SetContractInfo");
	}
	
	// for setting Cap/Floor contract info
	virtual void SetContractInfo(
		const std::string &effective_date, 
		const std::string &currency, 
		int cap_or_floor, 
		double notional_amount, 
		double contract_term, 
		double strike_rate, 
		int frequency, 
		int day_count, 
		double next_index_rate
	){
		throw FinLibException("Bad argument for SetContractInfo");
	}
	
	
	// for calculating IR Swap PV
	virtual double CalcPV(
		const std::string &valuation_date, 
		const std::vector<int> &index_rate_term,  
		const std::vector<double> &index_rate_value, 
		int index_rate_day_count_basis, 	// ACT/360 == 360, ACT/365 == 365
		int index_rate_compound_period, 	// PA == 1, SA == 2, Continuous == 0
		const std::vector<int> &discount_curve_term, 
		const std::vector<double> &discount_curve_value, 
		int discount_rate_day_count_basis, 	// ACT/360 == 360, ACT/365 == 365
		int discount_rate_compound_period	// PA == 1, SA == 2, Continuous == 0
	){
		throw FinLibException("Bad argument for CalcPV");
	}
	
	// for calculating European Swaption and Cap/Floor PV
	virtual double CalcPV(
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
	){
		throw FinLibException("Bad argument for CalcPV");
	}
	
};

}

#endif

