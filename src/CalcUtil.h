
#include "VolatilityCube.h"
#include <string>
#include <vector>

namespace FinLib{

class CalcUtil{
	
public:
	
	//Interpolate Func
	double Interpolate(
		double target_term, 
		double pre_term, 
		double post_term, 
		double pre_value, 
		double post_value
	);
	
	double Interpolate(
		int target_term, 
		int pre_term, 
		int post_term, 
		double pre_value, 
		double post_value
	);
	
	//InterpolateRange Func
	double InterpolateRange(
		int target_term, 
		const std::vector<int> &term, 
		const std::vector<double> &value
	);
	
	//Interpolate volatility Func
	double InterpolateVolatility(
		double target_strike, 
		int target_underlying_term, 
		int target_option_term, 
		const std::vector<VolatilityCube> &volatility_set, 
		int num_of_vol_underlying_term_grid, 
		int num_of_vol_option_term_grid
	);
	
	//CalcForwardRate Func
	double CalcForwardRate(
		double startTermZR, 
		int startTerm, 
		double endTermZR, 
		int endTerm
	);
	
	double CalcForwardRate(
		double startTermZR, 
		int startTerm, 
		double endTermZR, 
		int endTerm, 
		int day_count_basis, 
		int compound_period		//PA == 1, SA == 2, Continuous == 0
	);
	
	//calc discount factor function
	double CalcDiscountFactor(
		double zero_rate, 
		int term, 
		int day_count_basis, 
		int compound_period		//PA == 1, SA == 2, Continuous == 0
	);
	
	//Calc probability density function
	double CalcNormDistProbability(
		double x, 
		double average, 
		double variance
	);
	
};

}

