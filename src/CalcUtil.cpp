
#include "CalcUtil.h"
#include "VolatilityCube.h"
#include <string>
#include <cmath>


//Interpolate Func
double CalcUtil::Interpolate(double target_term, double pre_term, double post_term, double pre_value, double post_value){
	double target_value = 0;
	target_value = pre_value + (post_value - pre_value) / (post_term - pre_term) * (target_term - pre_term);
	return target_value;
}
double CalcUtil::Interpolate(int target_term, int pre_term, int post_term, double pre_value, double post_value){
	double target_value = 0;
	target_value = pre_value + (post_value - pre_value) / (post_term - pre_term) * (target_term - pre_term);
	return target_value;
}

//InterpolateRange Func
double CalcUtil::InterpolateRange(int target_term, const std::vector<int> &term, const std::vector<double> &value){
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
double CalcUtil::CalcForwardRate(double startTermZR, int startTerm, double endTermZR, int endTerm){
	double forwardRate = ((endTermZR * endTerm) - (startTermZR * startTerm)) / (endTerm - startTerm);
	return forwardRate;
}

double CalcUtil::CalcForwardRate(
	double startTermZR, 
	int startTerm, 
	double endTermZR, 
	int endTerm, 
	int day_count_basis, 
	int compound_period		//PA == 1, SA == 2, Continuous == 0
){
	if(compound_period == 1 || compound_period == 2){
		return (
					std::pow(
						(std::pow(1 + endTermZR / compound_period, compound_period * endTerm / (double)day_count_basis))
						/ 
						(std::pow(1 + startTermZR / compound_period, compound_period * startTerm / (double)day_count_basis))
					, 
						day_count_basis / (double)((endTerm - startTerm) * compound_period)
					)
					- 1
				)
				* compound_period;
	}else if(compound_period == 0){
		return ((endTermZR * endTerm) - (startTermZR * startTerm)) / (endTerm - startTerm);
	}
}

//calc discount factor function
double CalcUtil::CalcDiscountFactor(
		double zero_rate, 
		int term, 
		int day_count_basis, 
		int compound_period		//PA == 1, SA == 2, Continuous == 0
){
	if(compound_period == 1 || compound_period == 2){
		return std::pow( 1 / (1 + zero_rate / compound_period), compound_period * term / (double)day_count_basis );
	}else if(compound_period == 0){
		return std::exp(-1 * term / (double)day_count_basis);
	}
}

//Calc probability density function
double CalcUtil::CalcNormDistProbability(double x, double average, double variance){
	x = (x - average) / std::sqrt(2 * variance);
	double probability = std::erf(std::fabs(x)) / 2.0;
	if(x < 0){
		probability = 0.5 - probability;
	}else{
		probability = 0.5 + probability;
	}
 	return probability;
}


//calc option term and underlying swap term
double CalcUtil::InterpolateVolatility(
	double target_strike, 
	int target_underlying_term, 
	int target_option_term, 
	const std::vector<VolatilityCube> &volatility_set, 
	int num_of_vol_underlying_term_grid, 
	int num_of_vol_option_term_grid
	)
{
	
	double volatility = 0;	//return volatility
	
	//interpolate strike rate
	std::vector<double> strike_interpolated_volatility;
	if(target_strike <= volatility_set[0].strike_rate){
		for(int i = 0; i < num_of_vol_underlying_term_grid * num_of_vol_option_term_grid; i++){
			strike_interpolated_volatility.push_back(volatility_set[i].volatility_val);
		}
	}else if(target_strike >= volatility_set[volatility_set.size()-1].strike_rate){
		for(int i = volatility_set.size() - (num_of_vol_underlying_term_grid * num_of_vol_option_term_grid + 1); i < volatility_set.size(); i++){
			strike_interpolated_volatility.push_back(volatility_set[i].volatility_val);
		}
	}else{
		int i;
		//search interval
		for(i = 1; i < volatility_set.size(); i++){
			if(volatility_set[i].strike_rate >= target_strike) break;
		}
		for(int j = 0; j < num_of_vol_underlying_term_grid * num_of_vol_option_term_grid; j++){
			strike_interpolated_volatility.push_back(Interpolate(target_strike, 
													volatility_set[i-(num_of_vol_underlying_term_grid * num_of_vol_option_term_grid)].strike_rate, 
													volatility_set[i].strike_rate, 
													volatility_set[i-(num_of_vol_underlying_term_grid * num_of_vol_option_term_grid)].volatility_val, 
													volatility_set[i].volatility_val));
			i = i + 1;
		}
	}
	
	//interpolate underlying term
	std::vector<double> underlying_interpolated_volatility;
	if(target_underlying_term <= volatility_set[0].underlying_term){
		for(int i = 0; i < num_of_vol_option_term_grid; i++){
			underlying_interpolated_volatility.push_back(strike_interpolated_volatility[i]);
		}
	}else if(target_underlying_term >= volatility_set[volatility_set.size()-1].underlying_term){
		for(int i = strike_interpolated_volatility.size() - (num_of_vol_option_term_grid + 1); i < strike_interpolated_volatility.size(); i++){
			underlying_interpolated_volatility.push_back(strike_interpolated_volatility[i]);
		}
	}else{
		int i;
		//search interval
		for(i = 1; i < num_of_vol_underlying_term_grid * num_of_vol_option_term_grid; i++){
			if(volatility_set[i].underlying_term >= target_underlying_term) break;
		}
		for(int j = 0; j < num_of_vol_option_term_grid; j++){
			underlying_interpolated_volatility.push_back(Interpolate(target_underlying_term, 
													volatility_set[i - num_of_vol_option_term_grid].underlying_term, 
													volatility_set[i].underlying_term, 
													strike_interpolated_volatility[i - num_of_vol_option_term_grid], 
													strike_interpolated_volatility[i]));
			i = i + 1;
		}
	}
	
	//interpolate option term
	if(target_option_term <= volatility_set[0].option_term){
		volatility = underlying_interpolated_volatility[0];
	}else if(target_option_term >= volatility_set[volatility_set.size()-1].option_term){
		volatility = underlying_interpolated_volatility[underlying_interpolated_volatility.size()-1];
	}else{
		int i;
		//search interval
		for(i = 1; i < num_of_vol_option_term_grid; i++){
			if(volatility_set[i].option_term >= target_option_term) break;
		}
		volatility = (Interpolate(target_option_term, 
								volatility_set[i - 1].option_term, 
								volatility_set[i].option_term, 
								underlying_interpolated_volatility[i - 1], 
								underlying_interpolated_volatility[i]));
	}
	
	return volatility;
}



