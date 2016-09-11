
#include "Contract.h"
#include "ContractImpl.h"
#include "BaseContract.h"
#include "IRSwapContract.h"
#include "EuropeanSwaptionContract.h"
#include "VolatilityCube.h"
#include "FinLibException.h"
#include <string>


//constructor
Contract::Contract(const std::string &product){
	if(product == "IRSwap" || product == "EuropeanSwaption"){
		m_contract_impl.reset(new ContractImpl());
		m_contract_impl->CreateContract(product);
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
	double next_float_rate)
{
	m_contract_impl->SetContractInfo(
		effective_date, 
		currency, 
		fixed_or_float, 
		notional_amount, 
		contract_term, 
		payment_period, 
		fixed_rate, 
		next_float_rate
	);
}

//set European Swaption contract info
void Contract::SetContractInfo(
	const std::string &effective_date, 
	const std::string &currency, 
	int receiver_or_payer, 
	const std::string &option_maturity_date, 
	const std::string &underlying_swap_start_date, 
	double underlying_swap_notional_amount, 
	double underlying_swap_contract_term, 
	double underlying_swap_payment_period, 
	double strike_swap_rate)
{
	m_contract_impl->SetContractInfo(
		effective_date, 
		currency, 
		receiver_or_payer, 
		option_maturity_date, 
		underlying_swap_start_date, 
		underlying_swap_notional_amount, 
		underlying_swap_contract_term, 
		underlying_swap_payment_period, 
		strike_swap_rate
	);
}

//calc PV of IR Swap Func
double Contract::CalcPV(
	const std::string &valuation_date, 
	const std::vector<int> &floating_rate_term, 
	const std::vector<double> &floating_rate_value, 
	const std::vector<int> &discount_curve_term, 
	const std::vector<double> &discount_curve_value)
{
	return m_contract_impl->CalcPV(
		valuation_date, 
		floating_rate_term, 
		floating_rate_value, 
		discount_curve_term, 
		discount_curve_value
	);
}

//calc PV of European Swaption Func
double Contract::CalcPV(
	const std::string &valuation_date, 
	const std::vector<int> &floating_rate_term, 
	const std::vector<double> &floating_rate_value, 
	const std::vector<int> &discount_curve_term, 
	const std::vector<double> &discount_curve_value, 
	const std::vector<VolatilityCube> &volatility_set, 
	int num_of_vol_strike_rate, 
	int num_of_vol_underlying_term_grid, 
	int num_of_vol_option_term_grid)
{
	return m_contract_impl->CalcPV(
		valuation_date, 
		floating_rate_term, 
		floating_rate_value, 
		discount_curve_term, 
		discount_curve_value, 
		volatility_set, 
		num_of_vol_strike_rate, 
		num_of_vol_underlying_term_grid, 
		num_of_vol_option_term_grid
	);
}


