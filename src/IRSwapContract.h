
#ifndef IRSWAPCONTRACT_H_INCLUDED_
#define IRSWAPCONTRACT_H_INCLUDED_

#include <string>
using std::string;


class IRSwapContract{

private:

	string valuationDate;
	string effectiveDate;
	string nextRateFixingDate;
	string currency;
	int fixedOrFloatPayer;		//1 == fixed, 2 == float
	double notionalAmount;
	double contractTerm;
	double paymentPeriod;
	double fixedRate;
	double nextFloatRate;
//	double *floatRate;
//	int *floatRateTerm;
//	int numOfFloatRateGrid;
	double PV;
	
	double interpolate(double preGrid, double postGrid, 
						double preValue, double postValue, double targetGrid);
	double interpolateRange(int targetGrid, int *gridArray, 
											double *valueArray, int numOfArray);
	double calcForwardRate(double startTermZR, int startTerm, double endTermZR, int endTerm);
	
public:
	
	IRSwapContract();
	virtual ~IRSwapContract();
	void setValuationDate(string date);
	void setEffectiveDate(string date);
	void setCurrency(string cur);
	void setFixedOrFloatPayer(int f);
	void setNotionalAmount(double amount);
	void setContractTerm(double term);
	void setPaymentPeriod(double period);
	void setFixedRate(double rate);
	void setNextFloatRate(double rate);
//	void setFloatRate(const double *rate, const int *term, int numOfGrid);
	
	double calcPV(const double *rate, const int *term, int numOfGrid);

};


#endif

