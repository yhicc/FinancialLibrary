
#include "YieldCurve.h"
#include "YieldCurveImpl.h"
#include "DateUtil.h"
#include "CalcUtil.h"
#include "FinLibException.h"
#include <cmath>

namespace FinLib{

#define DEFAULT_LIBOR_GRID 1, 7, 30, 60, 90, 180, 360
#define DEFAULT_LIBOR_GRID_NUM 9
#define NUM_OF_INTPLTDSWPGRD 60
#define NUM_OF_DEFAULT_ZRRTGRD 67
#define NUM_OF_CASH_RATE_TERM 9
#define CASH_DAY_COUNT 360
#define SWAP_DAY_COUNT 365


void YieldCurve::YieldCurveImpl::BuildYieldCurve(){
	
	//check if required data is set
	if(m_cash_rate_value.size() == 0){
		throw FinLibException("Lack of required data for Building Yield Curve : Cash rates haven't benn set.");
	}
	if(m_swap_rate_value.size() == 0){
		throw FinLibException("Lack of required data for Building Yield Curve : Swap rates haven't benn set.");
	}
	
	m_zero_rate_term.clear();
	m_zero_rate_value.clear();
	m_discount_factor_value.clear();
	
	BuildShortEndOfCurve();
	BuildLongEndOfCurve();
	built_zero_rate_flag = 1;
}


void YieldCurve::YieldCurveImpl::BuildShortEndOfCurve(){
	//calc start and end date for each term
	std::vector<std::string> start_date(NUM_OF_CASH_RATE_TERM);
	std::vector<std::string> end_date(NUM_OF_CASH_RATE_TERM);
	std::string spot_date;
	DateUtil date_util;
	spot_date = date_util.addDateHC(m_base_date, 2);
	
	//calc start date of each terms
	start_date[0] = m_base_date;
	start_date[1] = date_util.addDateHC(m_base_date, 1);
	for(int i = 2; i < NUM_OF_CASH_RATE_TERM; i++){
		start_date[i] = spot_date;
	}
	
	//calc end date of each terms
	end_date[0] = date_util.addDateHC(m_base_date, 1);
	end_date[1] = date_util.addDateHC(start_date[1], 1);
	end_date[2] = date_util.addDateHC(spot_date, 7);
	end_date[3] = date_util.addDateHC(spot_date, 14);
	end_date[4] = date_util.addMonth(spot_date, 1, "MF");
	end_date[5] = date_util.addMonth(spot_date, 2, "MF");
	end_date[6] = date_util.addMonth(spot_date, 3, "MF");
	end_date[7] = date_util.addMonth(spot_date, 6, "MF");
	end_date[8] = date_util.addMonth(spot_date, 12, "MF");
	
	//calc number of days between start date and end date for each terms
	std::vector<int> num_of_days_to_maturity(NUM_OF_CASH_RATE_TERM);
	for(int i = 0; i < NUM_OF_CASH_RATE_TERM; i++){
		num_of_days_to_maturity[i] = date_util.calcDateDiff(start_date[i], end_date[i]);
		m_zero_rate_term.push_back(num_of_days_to_maturity[i]);
	}
	
	//calc DiscountFactor from Cash rate
	//ON
	m_discount_factor_value.push_back(1 / (1 + m_cash_rate_value[0] * num_of_days_to_maturity[0] / (double)m_cash_rate_day_count));
	//TN
	m_discount_factor_value.push_back((1 / (1 + m_cash_rate_value[1] * num_of_days_to_maturity[1] / (double)m_cash_rate_day_count))
										* m_discount_factor_value[0]);
	//DiscountFactor to Spot date
	double df_spot = m_discount_factor_value[0] * m_discount_factor_value[1];
	
	//DiscountFactor to today for other terms
	for(int i = 2; i < NUM_OF_CASH_RATE_TERM; i++){
		m_discount_factor_value.push_back(1 / ((1 + m_cash_rate_value[i] * num_of_days_to_maturity[i] / (double)m_cash_rate_day_count))
										* df_spot);
	}
}


//bootstrap Func
void YieldCurve::YieldCurveImpl::BuildLongEndOfCurve(){
	//calc end date and the number of days for interpolated swap rate terms
	std::string start_date;		//this date is spot date (common in all swap rate)
	DateUtil date_util;
	start_date = date_util.addDateHC(m_base_date, 2);
	
	//calc the number of days between start date and end date for each swap rate
	std::vector<std::string> end_date_of_swap(m_swap_rate_term.size());
	std::vector<int> num_of_days_swap(m_swap_rate_term.size());
	for(int i = 0; i < m_swap_rate_term.size(); i++){
		end_date_of_swap[i] = date_util.addYear(start_date, m_swap_rate_term[i], "MF");
		num_of_days_swap[i] = date_util.calcDateDiff(start_date, end_date_of_swap[i]);
	}
	
	//calc the number of days between start date and end date for each interpolated swap rate
	//calc interpolated SwapRate
	std::vector<std::string> end_date_of_interpolated_swap(NUM_OF_INTPLTDSWPGRD);
	std::vector<int> num_of_days_interpolated_swap(NUM_OF_INTPLTDSWPGRD);
	std::vector<double> interpolated_swap_rate_value(NUM_OF_INTPLTDSWPGRD);
	CalcUtil calc_util;
	for(int i = 0; i < NUM_OF_INTPLTDSWPGRD; i++){
		end_date_of_interpolated_swap[i] = date_util.addMonth(start_date, 6 * (i + 1), "MF");
		num_of_days_interpolated_swap[i] = date_util.calcDateDiff(start_date, end_date_of_interpolated_swap[i]);
		interpolated_swap_rate_value[i] = calc_util.InterpolateRange(num_of_days_interpolated_swap[i], num_of_days_swap, m_swap_rate_value);
	}
	
	//Arrays to store long term discount factor
	//This arrays have additional term in comparison with interpolated swap rate (0.5, 1, 1.5, 2,,,)
	std::vector<double> long_term_discount_factor_value(NUM_OF_INTPLTDSWPGRD);
	//use short term zero curve value for 0.5 year and 1 year
	long_term_discount_factor_value[0] = m_discount_factor_value[DEFAULT_LIBOR_GRID_NUM -2];
	long_term_discount_factor_value[1] = m_discount_factor_value[DEFAULT_LIBOR_GRID_NUM -1];
	
	//BootStrap(Swap rate to DF)
	double sum_interest = 0;
	for(int i = 2; i < NUM_OF_INTPLTDSWPGRD; i++){
		sum_interest = 0;
		for(int j = 0; j < i; j++){
			sum_interest = sum_interest + (interpolated_swap_rate_value[i] * ((num_of_days_interpolated_swap[i] - num_of_days_interpolated_swap[i-1]) / (double)m_swap_rate_day_count)
							 * long_term_discount_factor_value[j]);
		}
		long_term_discount_factor_value[i] = (100 - sum_interest) / (double)(100 + interpolated_swap_rate_value[i-1]
												 * ((num_of_days_interpolated_swap[i] - num_of_days_interpolated_swap[i-1]) / (double)m_swap_rate_day_count));
		m_zero_rate_term.push_back(num_of_days_interpolated_swap[i]);
	}
	
	//take long term zero rate and DF into member variable zero rate and DF
	for(int i = 2; i < NUM_OF_INTPLTDSWPGRD; i++){
		m_discount_factor_value.push_back(long_term_discount_factor_value[i]);
	}
}

void YieldCurve::YieldCurveImpl::ConvertZeroRate(int day_count, int compound_period){
	if(compound_period == 1 || compound_period == 2){
		for(int i = 0; i < NUM_OF_DEFAULT_ZRRTGRD; i++){
			m_zero_rate_value.push_back(
				compound_period
				 * 
				(std::pow(1 / m_discount_factor_value[i], day_count / (double)(compound_period * m_zero_rate_term[i]))
				 - 1)
			);
		}
	}else{
		for(int i = 0; i < NUM_OF_DEFAULT_ZRRTGRD; i++){
			m_zero_rate_value.push_back(-1 * std::log(m_discount_factor_value[i] * m_zero_rate_term[i] / (double)day_count));
		}
	}
}

}


