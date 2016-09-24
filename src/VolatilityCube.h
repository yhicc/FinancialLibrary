
#ifndef VOLATILITY_CUBE_H_INCLUDED_
#define VOLATILITY_CUBE_H_INCLUDED_

namespace FinLib{

struct VolatilityCube{
	double strike_rate;		//both absolute strike and relative from ATM are fine
	int underlying_term;	//number of days of underlying contract
	int option_term;		//number of days to option maturity
	double volatility_val;
};

}

#endif

