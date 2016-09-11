
#include "EuropeanSwaptionContract.h"
#include "VolatilityCube.h"
#include "CalcUtil.h"
#include "DateUtil.h"
#include "FinLibException.h"
#include <string>
#include <cmath>


//constructor
EuropeanSwaptionContract::EuropeanSwaptionContract(){
	m_contract_info_set_flag = 0;
}

//Destructor
EuropeanSwaptionContract::~EuropeanSwaptionContract(){
}

void EuropeanSwaptionContract::SetContractInfo(
	const std::string &effective_date, 
	const std::string &currency, 
	int receiver_or_payer, 
	const std::string &option_maturity_date, 
	const std::string &underlying_swap_start_date, 
	double underlying_swap_notional_amount, 
	double underlying_swap_contract_term, 
	double underlying_swap_payment_period, 
	double strike_swap_rate
){
	m_effective_date = effective_date;
	m_currency = currency;
	m_receiver_or_payer = receiver_or_payer;
	m_option_maturity_date = option_maturity_date;
	m_underlying_swap_start_date = underlying_swap_start_date;
	m_underlying_swap_notional_amount = underlying_swap_notional_amount;
	m_underlying_swap_contract_term = underlying_swap_contract_term;
	m_underlying_swap_payment_period = underlying_swap_payment_period;
	m_strike_swap_rate = strike_swap_rate;
	m_contract_info_set_flag = 1;
}


//calcPV Func
double EuropeanSwaptionContract::CalcPV(
	const std::string &valuation_date, 
	const std::vector<int> &floating_rate_term, 
	const std::vector<double> &floating_rate_value, 
	const std::vector<int> &discount_curve_term, 
	const std::vector<double> &discount_curve_value, 
	const std::vector<VolatilityCube> &volatility_set, 
	int num_of_vol_strike_rate, 
	int num_of_vol_underlying_term_grid, 
	int num_of_vol_option_term_grid
){
	
	//check if contract info is set or not
	if(m_contract_info_set_flag == 0){
		throw FinLibException("Contract information to calc PV has not been set.");
	}
	
	//step.1 calculate forward swap rate
	int num_of_CF = (int)(m_underlying_swap_contract_term / m_underlying_swap_payment_period);
	DateUtil dateutil;
	CalcUtil calc_util;
	
	//calculate each leg value
	//declear needed valiables
	int num_of_days_to_interest_end_date = dateutil.calcDateDiff(valuation_date, m_underlying_swap_start_date);;
	int num_of_days_to_interest_start_date = 0;		//for calc forward rate
	double discount_factor = 0;
	double discount_zero_rate = 0;
	double floating_rate_to_interest_end_date = 0;
	double floating_rate_to_interest_start_date = 0;	//for calc forward rate
	double forwardRate = 0;		
	std::string cashflow_date = m_underlying_swap_start_date;	//cashflow date is interest end date
	double fixed_leg_CF;
	double float_leg_CF;
	double discounted_fixed_leg_CF;
	double discounted_float_leg_CF;
	double fixed_leg_value = 0;
	double float_leg_value = 0;
	//calc each cashflow and add those valuee to fixed_leg_value and float_leg_value
	for(int i = 0; i < num_of_CF; i++){
		//calc ZR and DF to cashflow date
		//for discount
		discount_zero_rate = calc_util.InterpolateRange(num_of_days_to_interest_end_date, discount_curve_term, discount_curve_value);
		discount_factor = 1 / (1 + discount_zero_rate);
		//for projection
		floating_rate_to_interest_end_date = calc_util.InterpolateRange(num_of_days_to_interest_end_date, floating_rate_term, floating_rate_value);
		
		//Fixed Leg CF Calculation
		fixed_leg_CF = m_underlying_swap_notional_amount * m_strike_swap_rate;
		discounted_fixed_leg_CF = fixed_leg_CF * discount_factor;
		fixed_leg_value = fixed_leg_value + discounted_fixed_leg_CF;
		
		//Floating Leg CF Calculation
		//calc Forward Rate
		forwardRate = calc_util.CalcForwardRate(floating_rate_to_interest_start_date, num_of_days_to_interest_start_date, 
										floating_rate_to_interest_end_date, num_of_days_to_interest_end_date);
		float_leg_CF = m_underlying_swap_notional_amount * forwardRate;
		discounted_float_leg_CF = float_leg_CF * discount_factor;
		float_leg_value = float_leg_value + discounted_float_leg_CF;
		
		//set next CF term
		num_of_days_to_interest_start_date = num_of_days_to_interest_end_date;
		cashflow_date = dateutil.addMonth(cashflow_date, (12 * m_underlying_swap_payment_period), "MF");
		num_of_days_to_interest_end_date = dateutil.calcDateDiff(valuation_date, cashflow_date);
		//set floating rate to interest start date for next cashflow
		floating_rate_to_interest_start_date = floating_rate_to_interest_end_date;
	}
	
	double forward_swap_rate = float_leg_value / fixed_leg_value * m_strike_swap_rate;
	
	//step.2 interpolate volatility
	//calc option term and underlying swap term
	int underlying_swap_term = 0;
	std::string underlying_swap_end_date = dateutil.addYear(m_underlying_swap_start_date, m_underlying_swap_contract_term, "MF");
	underlying_swap_term = dateutil.calcDateDiff(m_underlying_swap_start_date, underlying_swap_end_date);
	int option_term_days = dateutil.calcDateDiff(valuation_date, m_option_maturity_date);
	
	double volatility = 0;
	volatility = calc_util.InterpolateVolatility(
		m_strike_swap_rate, 
		underlying_swap_term, 
		option_term_days, 
		volatility_set, 
		num_of_vol_underlying_term_grid, 
		num_of_vol_option_term_grid
	);
	
	//step.3 calculate option value
	//calc d1 and d2
	double d1 = (std::log(forward_swap_rate / m_strike_swap_rate) + volatility * volatility * ((double)option_term_days / 365)) 
					/ (volatility * std::sqrt(option_term_days / 365));
	double d2 = d1 - (volatility * std::sqrt(option_term_days / 365));
	
	//calc swaption pv
	double pv = 0;
	if(m_receiver_or_payer == 0){
		pv = float_leg_value * calc_util.CalcNormDistProbability(d1, 0, 1)
				- fixed_leg_value * calc_util.CalcNormDistProbability(d2, 0, 1);
	}else{
		pv = fixed_leg_value * calc_util.CalcNormDistProbability(-1 * d2, 0, 1)
				- float_leg_value * calc_util.CalcNormDistProbability(-1 * d1, 0, 1);
	}
	
	return pv;
}


