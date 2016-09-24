
#include "YieldCurve.h"
#include "YieldCurveImpl.h"
#include "DateUtil.h"
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

void YieldCurve::SetCashRate(const std::vector<double> &cash_rate_value, int cash_day_count){
	//check arguments
	if(cash_rate_value.size() != NUM_OF_CASH_RATE_TERM){
		throw FinLibException("Bad argument for SetCashRate method : Terms of cash rates don't match with defined ones.");
	}
	
	m_yieldcurve_impl->m_cash_rate_value.clear();
	for(int i = 0; i < NUM_OF_CASH_RATE_TERM; ++i){
		m_yieldcurve_impl->m_cash_rate_value.push_back(cash_rate_value[i]);
	}
	
	m_yieldcurve_impl->m_cash_rate_day_count = cash_day_count;
	
	m_yieldcurve_impl->built_zero_rate_flag = 0;
}

void YieldCurve::SetSwapRate(const std::vector<double> &swap_term, const std::vector<double> &swap_rate, int swap_day_count){
	//check arguments を後で作る
	if(swap_term.size() != swap_rate.size()){
		throw FinLibException("Bad argument for SetSwapRate method : The number of items of swap terms and swap rates don't match.");
	}
	
	m_yieldcurve_impl->m_swap_rate_term.clear();
	for(int i = 0; i < swap_term.size(); ++i){
		m_yieldcurve_impl->m_swap_rate_term.push_back(swap_term[i]);
	}
	
	m_yieldcurve_impl->m_swap_rate_value.clear();
	for(int i = 0; i < swap_rate.size(); ++i){
		m_yieldcurve_impl->m_swap_rate_value.push_back(swap_rate[i]);
	}
	
	m_yieldcurve_impl->m_swap_rate_day_count = swap_day_count;
	
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
void YieldCurve::CalcZeroRate(
	std::vector<double> &zero_rate, 
	int day_count, 
	int compound_period
){
	if(m_yieldcurve_impl->built_zero_rate_flag != 1){
		m_yieldcurve_impl->BuildYieldCurve();
	}
	m_yieldcurve_impl->ConvertZeroRate(day_count, compound_period);
	for(int i = 0; i < NUM_OF_DEFAULT_ZRRTGRD; i++){
		zero_rate[i] = m_yieldcurve_impl->m_zero_rate_value[i];
	}
	return;
}

void YieldCurve::CalcDiscountFactor(std::vector<double> &df){
	if(m_yieldcurve_impl->built_zero_rate_flag != 1){
		m_yieldcurve_impl->BuildYieldCurve();
	}
	for(int i = 0; i < NUM_OF_DEFAULT_ZRRTGRD; i++){
		df[i] = m_yieldcurve_impl->m_discount_factor_value[i];
	}
	return;
}

}


