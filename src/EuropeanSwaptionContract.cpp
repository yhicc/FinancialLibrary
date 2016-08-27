
#include "EuropeanSwaptionContract.h"
#include "SwaptionVolatility.h"
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
	double strike_swap_rate)
{
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
	const std::vector<SwaptionVolatility> &volatility_set, 
	int num_of_vol_strike_rate, 
	int num_of_vol_underlying_term_grid, 
	int num_of_vol_option_term_grid)
{
	
	//check if contract info is set or not
	if(m_contract_info_set_flag == 0){
		throw FinLibException("Contract information to calc PV has not been set.");
	}
	
//step.1 calculate forward swap rate
	int num_of_CF = (int)(m_underlying_swap_contract_term / m_underlying_swap_payment_period);
	DateUtil dateutil;
	
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
		discount_zero_rate = InterpolateRange(num_of_days_to_interest_end_date, discount_curve_term, discount_curve_value);
		discount_factor = 1 / (1 + discount_zero_rate);
		//for projection
		floating_rate_to_interest_end_date = InterpolateRange(num_of_days_to_interest_end_date, floating_rate_term, floating_rate_value);
		
		//Fixed Leg CF Calculation
		fixed_leg_CF = m_underlying_swap_notional_amount * m_strike_swap_rate;
		discounted_fixed_leg_CF = fixed_leg_CF * discount_factor;
		fixed_leg_value = fixed_leg_value + discounted_fixed_leg_CF;
		
		//Floating Leg CF Calculation
		//calc Forward Rate
		forwardRate = CalcForwardRate(floating_rate_to_interest_start_date, num_of_days_to_interest_start_date, 
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
//	std::string option_maturity_date = dateutil.addYear(m_effective_date, m_option_term, "MF");
	int option_term_days = dateutil.calcDateDiff(valuation_date, m_option_maturity_date);
	
	//volatility interpolation
	double volatility = 0;
	//interpolate strike rate
	std::vector<double> strike_interpolated_volatility;
	if(m_strike_swap_rate <= volatility_set[0].strike_rate){
		for(int i = 0; i < num_of_vol_underlying_term_grid * num_of_vol_option_term_grid; i++){
			strike_interpolated_volatility.push_back(volatility_set[i].volatility_val);
		}
	}else if(m_strike_swap_rate >= volatility_set[volatility_set.size()-1].strike_rate){
		for(int i = volatility_set.size() - (num_of_vol_underlying_term_grid * num_of_vol_option_term_grid + 1); i < volatility_set.size(); i++){
			strike_interpolated_volatility.push_back(volatility_set[i].volatility_val);
		}
	}else{
		int i;
		//search interval
		for(i = 1; i < volatility_set.size(); i++){
			if(volatility_set[0].strike_rate >= m_strike_swap_rate) break;
		}
		for(int j = 0; j < num_of_vol_underlying_term_grid * num_of_vol_option_term_grid; j++){
			strike_interpolated_volatility.push_back(Interpolate(m_strike_swap_rate, 
													volatility_set[i-(num_of_vol_underlying_term_grid * num_of_vol_option_term_grid)].strike_rate, 
													volatility_set[i].strike_rate, 
													volatility_set[i-(num_of_vol_underlying_term_grid * num_of_vol_option_term_grid)].volatility_val, 
													volatility_set[i].volatility_val));
			i = i + 1;
		}
	}
	//interpolate underlying term
	std::vector<double> underlying_interpolated_volatility;
	if(underlying_swap_term <= volatility_set[0].underlying_term){
		for(int i = 0; i < num_of_vol_option_term_grid; i++){
			underlying_interpolated_volatility.push_back(strike_interpolated_volatility[i]);
		}
	}else if(underlying_swap_term >= volatility_set[volatility_set.size()-1].underlying_term){
		for(int i = strike_interpolated_volatility.size() - (num_of_vol_option_term_grid + 1); i < strike_interpolated_volatility.size(); i++){
			underlying_interpolated_volatility.push_back(strike_interpolated_volatility[i]);
		}
	}else{
		int i;
		//search interval
		for(i = 1; i < num_of_vol_underlying_term_grid * num_of_vol_option_term_grid; i++){
			if(volatility_set[0].underlying_term >= underlying_swap_term) break;
		}
		for(int j = 0; j < num_of_vol_option_term_grid; j++){
			underlying_interpolated_volatility.push_back(Interpolate(underlying_swap_term, 
													volatility_set[i - num_of_vol_option_term_grid].underlying_term, 
													volatility_set[i].underlying_term, 
													strike_interpolated_volatility[i - num_of_vol_option_term_grid], 
													strike_interpolated_volatility[i]));
			i = i + 1;
		}
	}
	//interpolate option term
	if(option_term_days <= volatility_set[0].option_term){
		volatility = underlying_interpolated_volatility[0];
	}else if(option_term_days >= volatility_set[volatility_set.size()-1].option_term){
		volatility = underlying_interpolated_volatility[underlying_interpolated_volatility.size()-1];
	}else{
		int i;
		//search interval
		for(i = 1; i < num_of_vol_option_term_grid; i++){
			if(volatility_set[0].option_term >= option_term_days) break;
		}
		volatility = (Interpolate(option_term_days, 
								volatility_set[i - 1].option_term, 
								volatility_set[i].option_term, 
								underlying_interpolated_volatility[i - 1], 
								underlying_interpolated_volatility[i]));
	}
	
//step.3 calculate option value
	//calc d1 and d2
	double d1 = (std::log(forward_swap_rate / m_strike_swap_rate) + volatility * volatility * ((double)option_term_days / 365)) 
					/ (volatility * std::sqrt(option_term_days / 365));
	double d2 = d1 - (volatility * std::sqrt(option_term_days / 365));
	
	//calc swaption pv
	double pv = 0;
	if(m_receiver_or_payer == 0){
		pv = float_leg_value * CalcNormDistProbability(d1, 0, 1)
				- fixed_leg_value * CalcNormDistProbability(d2, 0, 1);
	}else{
		pv = fixed_leg_value * CalcNormDistProbability(-1 * d2, 0, 1)
				- float_leg_value * CalcNormDistProbability(-1 * d1, 0, 1);
	}
	
	return pv;
}



//Interpolate Func
double EuropeanSwaptionContract::Interpolate(double target_term, double pre_term, double post_term, double pre_value, double post_value){
	double target_value = 0;
	target_value = pre_value + (post_value - pre_value) / (post_term - pre_term) * (target_term - pre_term);
	return target_value;
}
double EuropeanSwaptionContract::Interpolate(int target_term, int pre_term, int post_term, double pre_value, double post_value){
	double target_value = 0;
	target_value = pre_value + (post_value - pre_value) / (post_term - pre_term) * (target_term - pre_term);
	return target_value;
}

//InterpolateRange Func
/*
double EuropeanSwaptionContract::interpolateRange(int targetGrid, int *gridArray, double *valueArray, int numOfArray){
	int i;
	//if targetgrid is out of argument range, return extrapolation value
	if(targetGrid <= gridArray[0]){
		return valueArray[0];
	}else if(targetGrid >= gridArray[numOfArray-1]){
		return valueArray[numOfArray-1];
	}
	
	for(i = 1; i < numOfArray; i++){
		if(gridArray[i] >= targetGrid) break;
	}
	return Interpolate(gridArray[i-1], gridArray[i], valueArray[i-1], valueArray[i], targetGrid);
}
*/
double EuropeanSwaptionContract::InterpolateRange(int target_term, const std::vector<int> &term, const std::vector<double> &value){
	int i;
	//if targetgrid is out of argument range, return extrapolated value
	if(target_term <= term[0]){
		return value[0];
	}else if(target_term >= term[term.size()-1]){
		return value[value.size()-1];
	//if target grid is in argument range, return linear interpolated value
	}else{
		//search interval
		for(i = 1; i < term.size(); i++){
			if(term[i] >= target_term) break;
		}
		return Interpolate(target_term, term[i-1], term[i], value[i-1], value[i]);
	}
}

//CalcForwardRate Func
double EuropeanSwaptionContract::CalcForwardRate(double startTermZR, int startTerm, double endTermZR, int endTerm){
	double forwardRate = ((endTermZR * endTerm) - (startTermZR * startTerm)) / (endTerm - startTerm);
	return forwardRate;
}

//Calc probability density function
double EuropeanSwaptionContract::CalcNormDistProbability(double x, double average, double variance){
	x = (x - average) / std::sqrt(2 * variance);
	double probability = std::erf(std::fabs(x)) / 2.0;
	if(x < 0){
		probability = 0.5 - probability;
	}else{
		probability = 0.5 + probability;
	}
 	return probability;
}


