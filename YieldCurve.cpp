
#include "YieldCurve.h"
#include <cmath>
#include <string>
using namespace std;


//YieldCurve Class


//InterpolateMethod
double YieldCurve::interpolate(double preGlid, double postGlid, double preValue, double postValue, double targetGlid){
	double targetValue = 0;
	targetValue = preValue + (postValue - preValue) / (postGlid - preGlid) * (targetGlid - preGlid);
	return targetValue;
}


//constructor
YieldCurve::YieldCurve(string currency, const double *liborArr, const double *swapArr){
	//Currency set
	cur = currency;
	//Libor set
	for(int i = 0; i < 7; ++i){
		libor[i] = liborArr[i];
	}
	//Swap set
	for(int i = 0; i < 15; i++){
		swapRate[i] = swapArr[i];
	}	
}

//setter
void YieldCurve::setcur(string currency){
	cur = currency;
}

void YieldCurve::setlibor(const double *liborArr){
	for(int i = 0; i < 7; ++i){
		libor[i] = liborArr[i];
	}
}

void YieldCurve::setswapRate(const double *swapArr){
	for(int i = 0; i < 15; ++i){
		swapRate[i] = swapArr[i];
	}
}


//getter
string YieldCurve::getcur(){
	return cur;
}

double* YieldCurve::getinterpolatedSwapRate(){
	int size = sizeof interpolatedSwapRate / sizeof interpolatedSwapRate[0];
	double *itpltdSwprt = new double[size];
	for(int i = 0; i < size; i++){
		itpltdSwprt[i] = interpolatedSwapRate[i];
	}
	return itpltdSwprt;
}

double* YieldCurve::getdiscountFactor(){
	int size = sizeof discountFactor / sizeof discountFactor[0];
	double *dscntFactr = new double[size];
	for(int i = 0; i < size; i++){
		dscntFactr[i] = discountFactor[i];
	}
	return dscntFactr;
}

double* YieldCurve::getZeroRate(){
	int size = sizeof zeroRate / sizeof zeroRate[0];
	double *zrRt = new double[size];
	for(int i = 0; i < size; i++){
		zrRt[i] = zeroRate[i];
	}
	return zrRt;
}


//bootstrapMethod
void YieldCurve::bootstrap(){
	
	//LiborをDFへ代入
	double liborGlid[] = {0.0027, 0.0192, 0.0833, 0.1667, 0.2500, 0.5000, 1};
	for(int i = 0; i < 7; i++){
		discountFactor[i] = 1 / (1 + libor[i] * liborGlid[i]);
	}
	
	//SwapRateを0.5年刻みで補間
	double swapGlid[] = {1,2,3,4,5,6,7,8,9,10,12,15,20,25,30};
	double interpolatedSwapGlid[59];
	for(int i = 0; i < 59; i++){
		interpolatedSwapGlid[i] = 1 + i *0.5;
	}
	interpolatedSwapRate[0] = swapRate[0];
	int j = 1;
	for(int i = 1; i < 59; i++){
		if(interpolatedSwapGlid[i] == swapGlid[j]){
			interpolatedSwapRate[i] = swapRate[j];
			j++;
		}else{
			interpolatedSwapRate[i] = interpolate(swapGlid[j-1], swapGlid[j], swapRate[j-1], swapRate[j], interpolatedSwapGlid[i]);
		}
	}
	
	//BootStrap(create DF)
	double sumDF = discountFactor[5];
	int k = 1;
	for(int i = 7; i < 65; i++){
		sumDF = sumDF + discountFactor[i-1];
		discountFactor[i] = (1 - sumDF * interpolatedSwapRate[k] / 2)
							 / (1 + interpolatedSwapRate[k] / 2);
		k++;
	}
	
	//from DF to ZeroRate
	double zeroRateGlid[65];
	for(int i = 0; i < 7; i++){
		zeroRateGlid[i] = liborGlid[i];
	}
	for(int i = 7; i < 65; i++){
		zeroRateGlid[i] = interpolatedSwapGlid[i-7];
	}
	for(int i = 0; i < 65; i++){
		zeroRate[i] = (log(discountFactor[i])) / (zeroRateGlid[i] * -1);
	}
}

