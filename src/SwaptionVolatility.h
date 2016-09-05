
#ifndef SWAPTION_VOLATILITY_H_INCLUDED_
#define SWAPTION_VOLATILITY_H_INCLUDED_

struct SwaptionVolatility{
	double strike_rate;
	int underlying_term;
	int option_term;
	double volatility_val;
};

#endif

