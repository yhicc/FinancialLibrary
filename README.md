
FinancialLibrary
==================

FinancialLibrary is C++ library to provide financial calculations such as pricing derivatives and building yield curve.  
  
  

##Description
FinancialLibrary provides following functions.  

1.Pricing Contract  
	-Pricing Interest Rate Swap (DCF method)  
	-Pricing European Swaption  (Black's model)  
	-Pricing Cap/Floor  (Black's model)  
	
2.Building Yield Curve  
	-Calculating zero rate from cash rate and swap rate  
  

##Usage
This is the example and overall flow to use the library. The actual methods are described in doc/api_spec.txt.  
  
####1.Pricing Contract  
Users have to include Contract.h and FinLibException.h, but don't have to include each product class header file such as EuropeanSwaptionContract.h.  
Depending on product, additional header files have to be included such as VolatilityCube.h.    
  
This is in the case of European Swaption. (Using flow is same, but arguments differ with each product.)  
1st step is creating Contract object. At this time, users have to designate product name by passing argument to Contract constructor.  
```
std::unique_ptr<Contract> eswpt;
eswpt.reset(new Contract("EuropeanSwaption"));  
```
2nd step is setting contract information to Contract object.  
Arguments differ with products.  
```
eswpt->SetContractInfo(
	"20160713", 	// Effective date
	"JPY", 			//	Currency
	1, 				// Receiver or Payer (Receiver = 0, Payer = 1)
	"20190822", 	// Swaption maturity date
	0.03, 			// Strike swap rate
	360, 			// Swaption day count convention (ACT/360 = 360, ACT/365 = 365)
	"20190829", 	// Underlying swap start date
	1000000, 		// Underlying swap notional amount
	3, 				// Underlying swap contract term (Year unit)
	0.5, 			// Underlying swap payment period (Year unit. Every 6 month = 0.5)
	360, 			// Underlying swap day count convention (ACT/360 = 360, ACT/365 = 365)
	0.001, 			//  Underlying swap spread on index rate
);
```
3rd step is to call CalcPV function passing market rate information.  
Arguments differ with products.  
```
pv = eswpt->CalcPV(
	"20160719", 					// Valuation date
	index_rate_term, 				// Vector of grids of index rate (The number of days)
	index_rate_value, 				// Vector of index rate corresponding index_rate_term (This is zero rate)
	360, 							// Day count convention of index rate (ACT/360 = 360, ACT/365 = 365)
	2, 								// Compounding period of index rate (PA = 1, SA = 2, Continuous = 0)
	discount_rate_term, 			// Vector of grids of discount rate (The number of days)
	discount_rate_value,	 		// Vector of index rate corresponding discount_rate_term (This is zero rate)
	360, 							// Day count convention of discount rate (ACT/360 = 360, ACT/365 = 365)
	2, 								// Compounding period of indiscountdex rate (PA = 1, SA = 2, Continuous = 0)
	volatility_set, 				// Vector of VolatilityCube(Structure of Strike rate, underlying term, option term and corresponding volatility)
	num_of_vol_strike_rate, 		// The number of strike rate included in volatility_set
	num_of_vol_underlying_term_grid, 		// The number of underlying term included in volatility_set
	num_of_vol_option_term_grid				// The number of option included in volatility_set
);
```
  
####2.Building Yield Curve  
Users have to include YieldCurve.h and FinLibException.h.  
1st step is creating Yield Curve object.  
```
std::unique_ptr<YieldCurve> ycv;
ycv.reset(new YieldCurve());  
```
2nd step is setting base date, cash rate and swap rate to Yield Curve object.  
```
ycv->SetBaseDate("20160617");
ycv->SetCashRate(libor);
ycv->SetSwapRate(swap_grid, swap_rate);  
```
3rd step is call CalcZeroRate function. At this time, user pass array to set calculated zero rate to the function.  
```
ycv->CalcZeroRate(zeroRate);  
```
  

##Requirement
C++11  


##Structure
The internal structure of the library is described in doc/library_structure.txt  

  
