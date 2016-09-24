
#include "CapFloorContract.h"
#include "VolatilityCube.h"
#include "CalcUtil.h"
#include "DateUtil.h"
#include "FinLibException.h"
#include <string>
#include <cmath>

namespace FinLib{

//constructor
CapFloorContract::CapFloorContract(){
	m_contract_info_set_flag = 0;
}

//Destructor
CapFloorContract::~CapFloorContract(){
}

void CapFloorContract::SetContractInfo(
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
	m_effective_date = effective_date;
	m_currency = currency;
	m_cap_or_floor = cap_or_floor;
	m_notional_amount = notional_amount;
	m_contract_term = contract_term;
	m_strike_rate = strike_rate;
	m_frequency = frequency;
	m_day_count = day_count;
	m_next_index_rate = next_index_rate;
	m_contract_info_set_flag = 1;
}


//calcPV Func
double CapFloorContract::CalcPV(
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
	
	//check if contract info is set or not
	if(m_contract_info_set_flag == 0){
		throw FinLibException("Contract information to calc PV has not been set.");
	}
	
	int num_of_caplet = (int)(m_contract_term * 12 / m_frequency);
	int num_of_ended_caplet = 0;
	DateUtil dateutil;
	CalcUtil calc_util;
	std::string tmp_date = dateutil.addMonth(m_effective_date, (12 * m_frequency), "MF");
	for(int i = 0; i <= num_of_ended_caplet; i++){
		if(dateutil.stringToJulian(valuation_date) > dateutil.stringToJulian(tmp_date)){
			num_of_ended_caplet++;
			tmp_date = dateutil.addMonth(tmp_date, (12 * m_frequency), "MF");
		}
	}
	int num_of_remaining_caplet = num_of_caplet - num_of_ended_caplet;
	std::string next_rate_fixing_date = tmp_date;	
	
	//calculate each caplet value
	//declear needed valiables
	int num_of_days_to_caplet_end_date = dateutil.calcDateDiff(valuation_date, next_rate_fixing_date);;
	int num_of_days_to_caplet_start_date = 0;		//for calc forward rate
	double discount_factor = 0;
	double discount_zero_rate = 0;
	double index_rate_to_caplet_end_date = 0;
	double index_rate_to_caplet_start_date = 0;	//for calc forward rate
	double forward_rate = 0;		
	std::string cashflow_date = next_rate_fixing_date;	//cashflow date is interest end date
	int forward_rate_term = 0;
	double volatility = 0;
	double cap_value = 0;
	
	//calc each cashflow and add those valuee to fixed_leg_value and float_leg_value
	for(int i = 0; i < num_of_remaining_caplet; i++){
		//calc ZR and DF to cashflow date
		//for discount
		discount_zero_rate = calc_util.InterpolateRange(num_of_days_to_caplet_end_date, discount_curve_term, discount_curve_value);
		discount_factor = calc_util.CalcDiscountFactor(discount_zero_rate, num_of_days_to_caplet_end_date, discount_rate_day_count_basis, discount_rate_compound_period);
		//for projection
		index_rate_to_caplet_end_date = calc_util.InterpolateRange(num_of_days_to_caplet_end_date, index_rate_term, index_rate_value);
		//preFixedFloatRate
		if(i == 0){
			cap_value = m_notional_amount * m_next_index_rate * (num_of_days_to_caplet_end_date / (double)m_day_count) * discount_factor;
		}else{
			forward_rate = 
				calc_util.CalcForwardRate(
					index_rate_to_caplet_start_date, 
					num_of_days_to_caplet_start_date, 
					index_rate_to_caplet_end_date, 
					num_of_days_to_caplet_end_date, 
					index_rate_day_count_basis, 
					index_rate_compound_period
				)
				 * 
				((num_of_days_to_caplet_end_date - num_of_days_to_caplet_start_date) / (double)m_day_count)
				 * 
				(m_day_count / (double)index_rate_day_count_basis);
		}
		forward_rate_term = num_of_days_to_caplet_end_date - num_of_days_to_caplet_start_date;
		volatility = calc_util.InterpolateVolatility(
			m_strike_rate, 
			forward_rate_term, 
			num_of_days_to_caplet_start_date, 
			volatility_set, 
			num_of_vol_underlying_term_grid, 
			num_of_vol_option_term_grid
		);
		
		double d1 = (std::log(forward_rate / m_strike_rate) + volatility * volatility * ((double)num_of_days_to_caplet_start_date / (double)m_day_count)) 
						/ (volatility * std::sqrt(num_of_days_to_caplet_start_date / (double)365));
		double d2 = d1 - (volatility * std::sqrt(num_of_days_to_caplet_start_date / (double)365));
		
		//calc caplet value
		if(m_cap_or_floor == 0){
			cap_value = cap_value + 
						discount_factor
						 * 
						(forward_rate * calc_util.CalcNormDistProbability(d1, 0, 1)
						 - m_strike_rate * calc_util.CalcNormDistProbability(d2, 0, 1));
		}else{
			cap_value =  cap_value + 
						discount_factor
						 * 
						(m_strike_rate * calc_util.CalcNormDistProbability(-1 * d2, 0, 1)
						 - forward_rate * calc_util.CalcNormDistProbability(-1 * d1, 0, 1));
		}
		
		//set next CF term
		num_of_days_to_caplet_start_date = num_of_days_to_caplet_end_date;
		cashflow_date = dateutil.addMonth(cashflow_date, (12 * m_frequency), "MF");
		num_of_days_to_caplet_end_date = dateutil.calcDateDiff(valuation_date, cashflow_date);
		//set floating rate to interest start date for next cashflow
		index_rate_to_caplet_start_date = index_rate_to_caplet_end_date;
	}
	
	return cap_value;
}

}


