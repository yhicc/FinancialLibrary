
#include "IRSwapContract.h"
#include "DateUtil.h"
#include "CalcUtil.h"
#include "FinLibException.h"
#include <string>


//constructor
IRSwapContract::IRSwapContract(){
	m_contract_info_set_flag = 0;
}

//Destructor
IRSwapContract::~IRSwapContract(){
}

void IRSwapContract::SetContractInfo(const std::string &effective_date, const std::string &currency,
						int fixed_or_float, double notional_amount, double contract_term,
						double payment_period, double fixed_rate, double next_float_rate){
	m_effective_date = effective_date;
	m_currency = currency;
	m_fixed_or_float_payer = fixed_or_float;
	m_notional_amount = notional_amount;
	m_contract_term = contract_term;
	m_payment_period = payment_period;
	m_fixed_rate = fixed_rate;
	m_next_float_rate = next_float_rate;
	m_contract_info_set_flag = 1;
}

//calcPV Func
double IRSwapContract::CalcPV(const std::string &valuation_date, const std::vector<int> &floating_rate_term, const std::vector<double> &floating_rate_value, 
								const std::vector<int> &discount_curve_term, const std::vector<double> &discount_curve_value){
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
	double floating_rate_to_interest_end_date = 0;
	double floating_rate_to_interest_start_date = 0;	//for calc forward rate
	double forwardRate = 0;		
	std::string cashflow_date = next_rate_fixing_date;	//cashflow date is interest end date
	double fixed_leg_CF;
	double float_leg_CF;
	double discounted_fixed_leg_CF;
	double discounted_float_leg_CF;
	double fixed_leg_value = 0;
	double float_leg_value = 0;
	//calc each cashflow and add those valuee to fixed_leg_value and float_leg_value
	for(int i = 0; i < num_of_unpaid_CF; i++){
		//calc ZR and DF to cashflow date
		//for discount
		discount_zero_rate = calc_util.InterpolateRange(num_of_days_to_interest_end_date, discount_curve_term, discount_curve_value);
		discount_factor = 1 / (1 + discount_zero_rate);
		//for projection
		floating_rate_to_interest_end_date = calc_util.InterpolateRange(num_of_days_to_interest_end_date, floating_rate_term, floating_rate_value);
		
		//Fixed Leg CF Calculation
		fixed_leg_CF = m_notional_amount * m_fixed_rate;
		discounted_fixed_leg_CF = fixed_leg_CF * discount_factor;
		fixed_leg_value = fixed_leg_value + discounted_fixed_leg_CF;
		
		//Floating Leg CF Calculation
		//preFixedFloatRate
		if(i == 0){
			float_leg_CF = m_notional_amount * m_next_float_rate;
			discounted_float_leg_CF = float_leg_CF * discount_factor;
		//calc Forward Rate
		}else{
			forwardRate = calc_util.CalcForwardRate(floating_rate_to_interest_start_date, num_of_days_to_interest_start_date, 
											floating_rate_to_interest_end_date, num_of_days_to_interest_end_date);
			float_leg_CF = m_notional_amount * forwardRate;
			discounted_float_leg_CF = float_leg_CF * discount_factor;
		}
		float_leg_value = float_leg_value + discounted_float_leg_CF;
		
		//set next CF term
		num_of_days_to_interest_start_date = num_of_days_to_interest_end_date;
		cashflow_date = dateutil.addMonth(cashflow_date, (12 * m_payment_period), "MF");
		num_of_days_to_interest_end_date = dateutil.calcDateDiff(valuation_date, cashflow_date);
		//set floating rate to interest start date for next cashflow
		floating_rate_to_interest_start_date = floating_rate_to_interest_end_date;
	}
	
	double pv;
	if(m_fixed_or_float_payer == 1){
		pv = float_leg_value - fixed_leg_value;
	}else{
		pv = fixed_leg_value - float_leg_value;
	}
	return pv;
}


