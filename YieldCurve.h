
#ifndef YIELDCURVE_H_INCLUDED_
#define YIELDCURVE_H_INCLUDED_

#include <string>
using namespace std;

class YieldCurve{

private:
	string cur;							//currency
	double libor[7];					//libor
	double swapRate[15];				//swapRate
	double swapGlid[15];				//glid of swap
	double interpolatedSwapRate[59];	//interpolatedSwapRate(6 Month)
	double discountFactor[65];			//discount factor
	double zeroRate[65];				//zeroRate
	
	//interpolateMethod
	double interpolate(double preGlid, double postGlid, double preValue, double postValue, double targetGlid);
	
public:
	
	//Constructor
	YieldCurve(string currency, const double *liborArr, const double *swapArr);
	
	//setter
	void setcur(string currency);
	void setlibor(const double *liborArr);
	void setswapRate(const double *swapArr);
	
	//getter
	string getcur();
	double* getlibor();
	double* getswapRate();
	double* getinterpolatedSwapRate();
	double* getdiscountFactor();
	double* getZeroRate();
	
	//bootstrapMethod
	void bootstrap();
	
};


#endif

