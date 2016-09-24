
#include "Contract.h"
#include "BaseContract.h"
#include "IRSwapContract.h"
#include "EuropeanSwaptionContract.h"
#include "CapFloorContract.h"
#include "VolatilityCube.h"
#include "FinLibException.h"
#include <string>

namespace FinLib{

//constructor
Contract::Contract(const std::string &product){
	if("IRSwap" == product){
		m_contract.reset(new IRSwapContract);
	}else if("EuropeanSwaption" == product){
		m_contract.reset(new EuropeanSwaptionContract);
	}else if("CapFloor" == product){
		m_contract.reset(new CapFloorContract);
	}else{
		throw FinLibException("Bad argument for Contract constructor : The product name is not covered.");
	}
}

//Destructor
Contract::~Contract(){
}

//set IR Swap contract info
void Contract::SetContractInfo(
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
	m_contract->SetContractInfo(
		effective_date, 
		currency, 
		fixed_or_float, 
		notional_amount, 
		contract_term, 
		payment_period, 
		fixed_rate, 
		spread_on_index_rate, 
		day_count, 
		next_float_rate
	);
}

//set European Swaption contract info
void Contract::SetContractInfo(
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
	m_contract->SetContractInfo(
		effective_date, 
		currency, 
		receiver_or_payer, 
		option_maturity_date, 
		strike_swap_rate, 
		option_day_count, 
		underlying_swap_start_date, 
		underlying_swap_notional_amount, 
		underlying_swap_contract_term, 
		underlying_swap_payment_period, 
		underlying_swap_day_count, 
		undelying_swap_spread_on_index_rate
	);
}

//set Cap/Floor contract info
void Contract::SetContractInfo(
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
	m_contract->SetContractInfo(
		effective_date, 
		currency, 
		cap_or_floor, 
		notional_amount, 
		contract_term, 
		strike_rate, 
		frequency, 
		day_count, 
		next_index_rate
	);
}


//calc PV of IR Swap Func
double Contract::CalcPV(
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
	return m_contract->CalcPV(
		valuation_date, 
		index_rate_term,  
		index_rate_value, 
		index_rate_day_count_basis, 	// ACT/360 == 360, ACT/365 == 365
		index_rate_compound_period, 	// PA == 1, SA == 2, Continuous == 0
		discount_curve_term, 
		discount_curve_value, 
		discount_rate_day_count_basis, 	// ACT/360 == 360, ACT/365 == 365
		discount_rate_compound_period	// PA == 1, SA == 2, Continuous == 0
	);
}

//calc PV of European Swaption and Cap/Floor Func
double Contract::CalcPV(
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
	return m_contract->CalcPV(
		valuation_date, 
		index_rate_term, 
		index_rate_value, 
		index_rate_day_count_basis, 
		index_rate_compound_period, 
		discount_curve_term, 
		discount_curve_value, 
		discount_rate_day_count_basis, 
		discount_rate_compound_period, 
		volatility_set, 
		num_of_vol_strike_rate, 
		num_of_vol_underlying_term_grid, 
		num_of_vol_option_term_grid
	);
}

}

