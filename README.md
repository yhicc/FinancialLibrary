
FinancialLibrary
==================

FinancialLibrary is C++ library to provide financial calculations such as building yield curve and pricing derivatives.  
  
  

##Description
FinancialLibrary provides following functions.

1.Building Yield Curve  
	-Calculating zero rate from cash rate and swap rate 

2.Pricing Contract  
	-Pricing Interest Rate Swap (DCF method)  
	-Pricing European Swaption  (Black's model)  
	-Pricing Cap/Floor  (Black's model)

##Usage
This is the overall flow to use the library. The actual methods are described in doc/api_spec.txt.  
  
####1.Building Yield Curve  
Users have to include YieldCurve.h and FinLibException.h.  
1st step is creating Yield Curve object.  
```
std::unique_ptr<YieldCurve> ycv;
ycv.reset(new YieldCurve());  
```
2nd step is setting base date, cash rate and swap rate to Yield Curve instance.  
```
ycv->SetBaseDate("20160617");
ycv->SetCashRate(libor);
ycv->SetSwapRate(swap_grid, swap_rate);  
```
3rd step is call CalcZeroRate function. At this time, user pass array to set calculated zero rate to the function.  
```
ycv->CalcZeroRate(zeroRate);  
```
  
  
####2.Pricing Contract  
Users have to include Contract.h and FinLibException.h. Depending on product, additional header files have to be included such as VolatilityCube.h.    
1st step is creating Contract object. At this time, user has to designate product name by passing argument to constructor.  
```
std::unique_ptr<Contract> irs;
irs.reset(new Contract("IRSwap"));  
```
2nd step is setting contract information to Contract instance. Arguments differ with products.  
```
irs->SetContractInfo("20160713", "JPY", 1, 1000000, 3, 0.5, 0.03, 0.01);
```
3rd step is call CalcPV function.  
```
pv = irs->CalcPV("20160719", float_rate_term, float_rate_value, zero_rate_term, zero_rate_value);  
```


##Structure
The internal structure of the library is described in doc/library_structure.txt

  
