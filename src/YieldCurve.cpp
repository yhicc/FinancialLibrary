
#include "YieldCurve.h"
#include "DateUtil.h"
#include <cmath>

#define DEFAULT_LIBOR_GRID 1, 7, 30, 60, 90, 180, 360
#define DEFAULT_LIBOR_GRID_NUM 9
#define NUM_OF_INTPLTDSWPGRD 60
#define NUM_OF_DEFAULT_ZRRTGRD 67
#define NUM_OF_CASH_RATE_TERM 9
#define CASH_DAY_COUNT 360
#define SWAP_DAY_COUNT 365


class YieldCurve::YieldCurveImpl{
public:
	
	std::string m_base_date;
	std::string m_currency;
	
	std::vector<int> m_cash_rate_term;
	std::vector<double> m_cash_rate_value;
	std::vector<int> m_default_grid_cash_rate_term;
	std::vector<double> m_default_grid_cash_rate_value;
	
	std::vector<double> m_swap_rate_term;
	std::vector<double> m_swap_rate_value;
	
	std::vector<int> m_zero_rate_term;
	std::vector<double> m_zero_rate_value;
	std::vector<double> m_discount_factor_value;
	
	int built_zero_rate_flag;
	
	
	//interpolateMethod
	double Interpolate(double target_term, double pre_term, double post_term, double pre_value, double post_value);
	double InterpolateInt(int target_term, int pre_term, int post_term, double pre_value, double post_value);
	//interpolateRangeMethod
	double InterpolateRange(double target_term, const std::vector<double> &term, const std::vector<double> &value);
	double InterpolateRange(int target_term, const std::vector<int> &term, const std::vector<double> &value);
	
	//bootstrapMethod
	void BuildLongEndOfCurve();
	void BuildShortEndOfCurve();
	void BuildZeroCurve();
};

//constructor
YieldCurve::YieldCurve()
	:m_yieldcurve_impl(new YieldCurveImpl())
{}



//Destructor
YieldCurve::~YieldCurve(){

}


//setter
void YieldCurve::SetBaseDate(const std::string &base_date){
	m_yieldcurve_impl->m_base_date = base_date;
	m_yieldcurve_impl->built_zero_rate_flag = 0;
}

void YieldCurve::SetCurrency(const std::string &cur){
	m_yieldcurve_impl->m_currency = cur;
	m_yieldcurve_impl->built_zero_rate_flag = 0;
}

void YieldCurve::SetCashRate(const std::vector<double> &cash_rate_value){
	//check arguments
//	if(libor_term.size() != libor_value.size()){
//		// throw exception
//	}
	
/*	m_cash_rate_term.clear();
	for(int i = 0; i < libor_term.size(); ++i){
		m_cash_rate_term.push_back(cash_rate_term[i]);
	}
*/	
	m_yieldcurve_impl->m_cash_rate_value.clear();
	for(int i = 0; i < NUM_OF_CASH_RATE_TERM; ++i){
		m_yieldcurve_impl->m_cash_rate_value.push_back(cash_rate_value[i]);
	}
	
	m_yieldcurve_impl->built_zero_rate_flag = 0;
}

void YieldCurve::SetSwapRate(const std::vector<double> &swap_term, const std::vector<double> &swap_rate){
	//check arguments を後で作る
//	if(swap_term.size() != swap_value.size()){
//		// throw exception
//	}
	
	m_yieldcurve_impl->m_swap_rate_term.clear();
	for(int i = 0; i < swap_term.size(); ++i){
		m_yieldcurve_impl->m_swap_rate_term.push_back(swap_term[i]);
	}
	
	m_yieldcurve_impl->m_swap_rate_value.clear();
	for(int i = 0; i < swap_rate.size(); ++i){
		m_yieldcurve_impl->m_swap_rate_value.push_back(swap_rate[i]);
	}
	
	m_yieldcurve_impl->built_zero_rate_flag = 0;
}


//getter
std::string YieldCurve::GetBaseDate(){
	return m_yieldcurve_impl->m_base_date;
}

std::string YieldCurve::GetCurrency(){
	return m_yieldcurve_impl->m_currency;
}

void YieldCurve::GetCashRate(std::vector<double> &cashrate){
	for(int i = 0; i < NUM_OF_CASH_RATE_TERM; i++){
		cashrate[i] = m_yieldcurve_impl->m_cash_rate_value[i];
	}
}

void YieldCurve::GetSwapRate(std::vector<double> &swap_term, std::vector<double> &swap_rate){
	for(int i = 0; i < m_yieldcurve_impl->m_swap_rate_term.size(); i++){
		swap_term[i] = m_yieldcurve_impl->m_swap_rate_term[i];
		swap_rate[i] = m_yieldcurve_impl->m_swap_rate_value[i];
	}
}


//Build Yield Curve function
void YieldCurve::CalcZeroRate(std::vector<double> &zero_rate){
	if(m_yieldcurve_impl->built_zero_rate_flag != 1){
		m_yieldcurve_impl->BuildZeroCurve();
	}
	for(int i = 0; i < NUM_OF_DEFAULT_ZRRTGRD; i++){
		zero_rate[i] = m_yieldcurve_impl->m_zero_rate_value[i];
	}
	return;
}

void YieldCurve::CalcDiscountFactor(std::vector<double> &df){
	if(m_yieldcurve_impl->built_zero_rate_flag != 1){
		m_yieldcurve_impl->BuildZeroCurve();
	}
	for(int i = 0; i < NUM_OF_DEFAULT_ZRRTGRD; i++){
		df[i] = m_yieldcurve_impl->m_discount_factor_value[i];
	}
	return;
}




//Private functions

//Interpolate Func
double YieldCurve::YieldCurveImpl::Interpolate(double target_term, double pre_term, double post_term, double pre_value, double post_value){
	double target_value = 0;
	target_value = pre_value + (post_value - pre_value) / (post_term - pre_term) * (target_term - pre_term);
	return target_value;
}
double YieldCurve::YieldCurveImpl::InterpolateInt(int target_term, int pre_term, int post_term, double pre_value, double post_value){
	double target_value = 0;
	target_value = pre_value + (post_value - pre_value) / (post_term - pre_term) * (target_term - pre_term);
	return target_value;
}

//InterpolateRange Func
double YieldCurve::YieldCurveImpl::InterpolateRange(double target_term, const std::vector<double> &term, const std::vector<double> &value){
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
double YieldCurve::YieldCurveImpl::InterpolateRange(int target_term, const std::vector<int> &term, const std::vector<double> &value){
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


void YieldCurve::YieldCurveImpl::BuildZeroCurve(){
	// vectorのclearを入れる！
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
	}
	
	//calc DiscountFactor from Cash rate
	//ON
	m_discount_factor_value.push_back(1 / (1 + m_cash_rate_value[0] * num_of_days_to_maturity[0] / (double)CASH_DAY_COUNT));
	//TN
	m_discount_factor_value.push_back((1 / (1 + m_cash_rate_value[1] * num_of_days_to_maturity[1] / (double)CASH_DAY_COUNT))
										* m_discount_factor_value[0]);
	//DiscountFactor to Spot date
	double df_spot = m_discount_factor_value[0] * m_discount_factor_value[1];
	//DiscountFactor to today for other terms
	for(int i = 2; i < NUM_OF_CASH_RATE_TERM; i++){
		m_discount_factor_value.push_back(1 / ((1 + m_cash_rate_value[i] * num_of_days_to_maturity[i] / (double)CASH_DAY_COUNT))
										* df_spot);
	}
	
	//Calc ZeroRates from DiscountFactors
	for(int i = 0; i < NUM_OF_CASH_RATE_TERM; i++){
		m_zero_rate_value.push_back(std::log(m_discount_factor_value[i]) / (double)(num_of_days_to_maturity[i] / (double)CASH_DAY_COUNT) * (-1));
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
	for(int i = 0; i < NUM_OF_INTPLTDSWPGRD; i++){
		end_date_of_interpolated_swap[i] = date_util.addMonth(start_date, 6 * (i + 1), "MF");
		num_of_days_interpolated_swap[i] = date_util.calcDateDiff(start_date, end_date_of_interpolated_swap[i]);
		interpolated_swap_rate_value[i] = InterpolateRange(num_of_days_interpolated_swap[i], num_of_days_swap, m_swap_rate_value);
	}
	
	
	//Arrays to store long term zero rate and discount factor
	//These arrays have additional term in comparison with interpolated swap rate (0.5, 1, 1.5, 2,,,)
	std::vector<double> long_term_zero_rate_term(NUM_OF_INTPLTDSWPGRD);
	std::vector<double> long_term_zero_rate_value(NUM_OF_INTPLTDSWPGRD);
	std::vector<double> long_term_discount_factor_value(NUM_OF_INTPLTDSWPGRD);
	for(int i = 0; i < NUM_OF_INTPLTDSWPGRD; i++){
		long_term_zero_rate_term[i] = 0.5 + i * 0.5;
	}
	
	//use short term zero curve value for 0.5 year and 1 year
	long_term_zero_rate_value[0] = m_zero_rate_value[DEFAULT_LIBOR_GRID_NUM - 2];
	long_term_zero_rate_value[1] = m_zero_rate_value[DEFAULT_LIBOR_GRID_NUM - 1];
	long_term_discount_factor_value[0] = m_discount_factor_value[DEFAULT_LIBOR_GRID_NUM -2];
	long_term_discount_factor_value[1] = m_discount_factor_value[DEFAULT_LIBOR_GRID_NUM -1];
	
	//BootStrap(Swap rate to DF)
	double sum_interest = 0;
	for(int i = 2; i < NUM_OF_INTPLTDSWPGRD; i++){
		sum_interest = 0;
		for(int j = 0; j < i; j++){
			sum_interest = sum_interest + (interpolated_swap_rate_value[i] * ((num_of_days_interpolated_swap[i] - num_of_days_interpolated_swap[i-1]) / (double)SWAP_DAY_COUNT)
							 * long_term_discount_factor_value[j]);
		}
		long_term_discount_factor_value[i] = (100 - sum_interest) / (double)(100 + interpolated_swap_rate_value[i-1]
												 * ((num_of_days_interpolated_swap[i] - num_of_days_interpolated_swap[i-1]) / (double)SWAP_DAY_COUNT));
		long_term_zero_rate_value[i] = -1 * std::log(long_term_discount_factor_value[i]);
	}
	
	//take long term zero rate and DF into member variable zero rate and DF
	for(int i = 2; i < NUM_OF_INTPLTDSWPGRD; i++){
		m_discount_factor_value.push_back(long_term_discount_factor_value[i]);
		m_zero_rate_value.push_back(long_term_zero_rate_value[i]);
	}
	
}





