
#include "IRSwapContract.h"
#include "DateUtil.h"
#include "CalcUtil.h"
#include "FinLibException.h"
#include <string>

namespace FinLib{

//constructor
IRSwapContract::IRSwapContract(){
	m_contract_info_set_flag = 0;
}

//Destructor
IRSwapContract::~IRSwapContract(){
}

void IRSwapContract::SetContractInfo(
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
	m_effective_date = effective_date;
	m_currency = currency;
	m_fixed_or_float_payer = fixed_or_float;
	m_notional_amount = notional_amount;
	m_contract_term = contract_term;
	m_payment_period = payment_period;
	m_fixed_rate = fixed_rate;
	m_spread_on_index_rate = spread_on_index_rate;
	m_day_count = day_count;
	m_next_float_rate = next_float_rate;
	m_contract_info_set_flag = 1;
}

//calcPV Func
double IRSwapContract::CalcPV(
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
	//check if contract info is set or not
	if(m_contract_info_set_flag == 0){
		throw FinLibException("Contract information to calc PV has not been set.");
	}
	
	int num_of_CF = (int)(m_contract_term / m_payment_period);
	int num_of_paid_CF = 0;
	DateUtil dateutil;
	CalcUtil calc_util;
	std::string tmp_date = dateutil.addMonth(m_effective_date, (12 * m_payment_period), "MF");
	for(int i = 0; i <= num_of_paid_CF; i++){
		if(dateutil.stringToJulian(valuation_date) > dateutil.stringToJulian(tmp_date)){
			num_of_paid_CF++;
			tmp_date = dateutil.addMonth(tmp_date, (12 * m_payment_period), "MF");
		}
	}
	int num_of_unpaid_CF = num_of_CF - num_of_paid_CF;
	std::string next_rate_fixing_date = tmp_date;	
	
	//calculate each leg value
	//declear needed valiables
	int num_of_days_to_interest_end_date = dateutil.calcDateDiff(valuation_date, next_rate_fixing_date);;
	int num_of_days_to_interest_start_date = 0;		//for calc forward rate
	double discount_factor = 0;
	double discount_zero_rate = 0;
	double index_rate_to_interest_end_date = 0;
	double index_rate_to_interest_start_date = 0;	//for calc forward rate
	double forward_rate = 0;
	std::string cashflow_date = next_rate_fixing_date;	//cashflow date is interest end date
	double fixed_leg_CF;
	double float_leg_CF;
	double discounted_fixed_leg_CF;
	double discounted_float_leg_CF;
	double fixed_leg_value = 0;
	double float_leg_value = 0;
	double day_count_basis = 0;
	
	//calc each cashflow and add those valuee to fixed_leg_value and float_leg_value
	for(int i = 0; i < num_of_unpaid_CF; i++){
		//calc ZR and DF to cashflow date
		//for discount
		discount_zero_rate = calc_util.InterpolateRange(num_of_days_to_interest_end_date, discount_curve_term, discount_curve_value);
		discount_factor = calc_util.CalcDiscountFactor(discount_zero_rate, num_of_days_to_interest_end_date, discount_rate_day_count_basis, discount_rate_compound_period);
		//for projection
		index_rate_to_interest_end_date = calc_util.InterpolateRange(num_of_days_to_interest_end_date, index_rate_term, index_rate_value);
		
		//Fixed Leg CF Calculation
		fixed_leg_CF = m_notional_amount * m_fixed_rate * (num_of_days_to_interest_end_date / (double)m_day_count);
		discounted_fixed_leg_CF = fixed_leg_CF * discount_factor;
		fixed_leg_value = fixed_leg_value + discounted_fixed_leg_CF;
		
		//Floating Leg CF Calculation
		//preFixedFloatRate
		if(i == 0){
			float_leg_CF = m_notional_amount * m_next_float_rate * (num_of_days_to_interest_end_date / (double)m_day_count);
			discounted_float_leg_CF = float_leg_CF * discount_factor;
		//calc Forward Rate
		}else{
			forward_rate = calc_util.CalcForwardRate(
				index_rate_to_interest_start_date, 
				num_of_days_to_interest_start_date, 
				index_rate_to_interest_end_date, 
				num_of_days_to_interest_end_date, 
				index_rate_day_count_basis, 
				index_rate_compound_period
			);
			float_leg_CF = 
				m_notional_amount
				 * 
				(forward_rate + m_spread_on_index_rate)
				 * 
				((num_of_days_to_interest_end_date - num_of_days_to_interest_start_date) / (double)m_day_count)
				 * 
				(m_day_count / (double)index_rate_day_count_basis);
			discounted_float_leg_CF = float_leg_CF * discount_factor;
		}
		float_leg_value = float_leg_value + discounted_float_leg_CF;
		
		//set next CF term
		num_of_days_to_interest_start_date = num_of_days_to_interest_end_date;
		cashflow_date = dateutil.addMonth(cashflow_date, (12 * m_payment_period), "MF");
		num_of_days_to_interest_end_date = dateutil.calcDateDiff(valuation_date, cashflow_date);
		//set floating rate to interest start date for next cashflow
		index_rate_to_interest_start_date = index_rate_to_interest_end_date;
	}
	
	double pv = 0;
	if(m_fixed_or_float_payer == 0){
		pv = float_leg_value - fixed_leg_value;
	}else{
		pv = fixed_leg_value - float_leg_value;
	}
	return pv;
}

}

