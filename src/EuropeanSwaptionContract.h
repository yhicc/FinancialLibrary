
#ifndef ESWAPTIONCONTRACT_H_INCLUDED_
#define ESWAPTIONCONTRACT_H_INCLUDED_

#include "BaseContract.h"
#include "SwaptionVolatility.h"
#include <string>
#include <vector>


class EuropeanSwaptionContract : public BaseContract{

private:
	std::string m_effective_date;
	std::string m_currency;
	int m_receiver_or_payer;	//0 == receiver, 1 == payer
	std::string m_option_maturity_date;
	std::string m_underlying_swap_start_date;
	double m_underlying_swap_notional_amount;
	double m_underlying_swap_contract_term;		//1year = 1
	double m_underlying_swap_payment_period;	//6month = 0.5
	double m_strike_swap_rate;
	int m_contract_info_set_flag;
	
	double Interpolate(double target_term, double pre_term, double post_term, double pre_value, double post_value);
	double Interpolate(int target_term, int pre_term, int post_term, double pre_value, double post_value);
	double interpolateRange(int targetGrid, int *gridArray, 
											double *valueArray, int numOfArray);
	double InterpolateRange(int target_term, const std::vector<int> &term, const std::vector<double> &value);
	double CalcForwardRate(double startTermZR, int startTerm, double endTermZR, int endTerm);
	double CalcNormDistProbability(double x, double average, double variance);
	
public:
	
	EuropeanSwaptionContract();
	
	virtual ~EuropeanSwaptionContract();
	
	void SetContractInfo(
		const std::string &effective_date, 
		const std::string &currency, 
		int receiver_or_payer, 
		const std::string &option_maturity_date, 
		const std::string &underlying_swap_start_date, 
		double underlying_swap_notional_amount, 
		double underlying_swap_contract_term, 
		double underlying_swap_payment_period, 
		double strike_swap_rate
	);
	
	double CalcPV(
		const std::string &valuation_date, 
		const std::vector<int> &floating_rate_term, 
		const std::vector<double> &floating_rate_value, 
		const std::vector<int> &discount_curve_term, 
		const std::vector<double> &discount_curve_value, 
		const std::vector<SwaptionVolatility> &volatility_set, 
		int num_of_vol_strike_rate, 
		int num_of_vol_underlying_term_grid, 
		int num_of_vol_option_term_grid
	);

};


#endif

