
#include "IRSwapContract.h"
#include "MyDate.h"
#include <iostream>
#include <string>
using std::string;


//constructor
IRSwapContract::IRSwapContract(){
	floatRate = NULL;
	floatRateTerm = NULL;
}

//Destructor
IRSwapContract::~IRSwapContract(){
	if(floatRate != NULL){
		delete[] floatRate;
	}
	if(floatRateTerm != NULL){
		delete[] floatRateTerm;
	}
}


//setter
void IRSwapContract::setValuationDate(string date){
	valuationDate = date;
}
void IRSwapContract::setEffectiveDate(string date){
	effectiveDate = date;
}
void IRSwapContract::setCurrency(string cur){
	currency = cur;
}
void IRSwapContract::setFixedOrFloatPayer(int f){
	fixedOrFloatPayer = f;
}
void IRSwapContract::setNotionalAmount(double amount){
	notionalAmount = amount;
}
void IRSwapContract::setContractTerm(double term){
	contractTerm = term;
}
void IRSwapContract::setPaymentPeriod(double period){
	paymentPeriod = period;
}
void IRSwapContract::setFixedRate(double rate){
	fixedRate = rate;
}
void IRSwapContract::setNextFloatRate(double rate){
	nextFloatRate = rate;
}
void IRSwapContract::setFloatRate(const double *rate, const int *term, int numOfGrid){
	numOfFloatRateGrid = numOfGrid;
	if(floatRate != NULL){
		delete[] floatRate;
	}
	floatRate = new double[numOfFloatRateGrid];
	for(int i = 0; i < numOfFloatRateGrid; i++){
		floatRate[i] = rate[i];
	}
	if(floatRateTerm != NULL){
		delete[] floatRateTerm;
	}
	floatRateTerm = new int[numOfFloatRateGrid];
	for(int i = 0; i < numOfFloatRateGrid; i++){
		floatRateTerm[i] = term[i];
	}
}


//getter




//calcPV
double IRSwapContract::calcPV(){
	int numOfFixing = (int)(contractTerm / paymentPeriod);
	int numOfFixed = 0;
	string tmpDate = effectiveDate;
	MyDate mydate;
	for(int i = 0; i <= numOfFixed; i++){
		if(mydate.stringToJulian(valuationDate) > mydate.stringToJulian(tmpDate)){
			numOfFixed++;
			tmpDate = mydate.addMonth(tmpDate, (12 * paymentPeriod));
		}
	}
	int numOfRestLegCF = numOfFixing - numOfFixed;
	nextRateFixingDate = tmpDate;
	std::cout << nextRateFixingDate << std::endl;
	double *fixedLegCF;
	fixedLegCF = new double[numOfRestLegCF];
	double *floatLegCF;
	floatLegCF = new double[numOfRestLegCF];
	
	double *discountedFixedLegCF;
	double *discountedFloatLegCF;
	discountedFixedLegCF = new double[numOfRestLegCF];
	discountedFloatLegCF = new double[numOfRestLegCF];
	double fixedLegVal = 0;
	double floatLegVal = 0;
	int cfTerm = mydate.calcDateDiff(valuationDate, nextRateFixingDate);
	double discountFactor = 0;
	double zeroRate = 0;
	double preZeroRate = 0;
	double forwardRate = 0;
	string cfDate = nextRateFixingDate;
	for(int i = 0; i < numOfRestLegCF; i++){
		//calc ZR and DF
		zeroRate = interpolateRange(cfTerm, floatRateTerm, floatRate, numOfFloatRateGrid);
		for(int i = 0; i < numOfFloatRateGrid; i++){
			discountFactor = 1 / (1 + zeroRate);
		}
		
		//Fixed Leg CF Calculation
		fixedLegCF[i] = notionalAmount * fixedRate;
		discountedFixedLegCF[i] = fixedLegCF[i] * discountFactor;
		fixedLegVal = fixedLegVal + discountedFixedLegCF[i];
		
		//Floating Leg CF Calculation
		//preFixedFloatRate
		if(i == 0){
			floatLegCF[i] = notionalAmount * nextFloatRate;
			discountedFloatLegCF[i] = floatLegCF[i] * discountFactor;
		//calc Forward Rate
		}else{
			forwardRate = calcForwardRate(preZeroRate, zeroRate);
			floatLegCF[i] = notionalAmount * forwardRate;
			discountedFloatLegCF[i] = floatLegCF[i] * discountFactor;
		}
		floatLegVal = floatLegVal + discountedFloatLegCF[i];
		
		//Next CF Term set
		cfDate = mydate.addMonth(cfDate, (12 * paymentPeriod));
		cfTerm = mydate.calcDateDiff(valuationDate, cfDate);
		//preZeroRate set
		preZeroRate = zeroRate;
	}
	
	if(fixedLegCF != NULL){
		delete[] fixedLegCF;
	}
	if(floatLegCF != NULL){
		delete[] floatLegCF;
	}
	if(discountedFixedLegCF != NULL){
		delete[] discountedFixedLegCF;
	}
	if(discountedFloatLegCF != NULL){
		delete[] discountedFloatLegCF;
	}
	
	PV = fixedLegVal + floatLegVal;
	return PV;
	
}

//Interpolate Method
double IRSwapContract::interpolate(double preGrid, double postGrid, double preValue, double postValue, double targetGrid){
	double targetValue = 0;
	targetValue = preValue + (postValue - preValue) / (postGrid - preGrid) * (targetGrid - preGrid);
	return targetValue;
}

//InterpolateRange Method
double IRSwapContract::interpolateRange(int targetGrid, int *gridArray, double *valueArray, int numOfArray){
	int i;
	//targetGridの値が範囲外の場合は最大最小値の値を返す
	if(targetGrid <= gridArray[0]){
		return valueArray[0];
	}else if(targetGrid >= gridArray[numOfArray-1]){
		return valueArray[numOfArray-1];
	}
	
	for(i = 1; i < numOfArray; i++){
		if(gridArray[i] >= targetGrid) break;
	}
	return interpolate(gridArray[i-1], gridArray[i], valueArray[i-1], valueArray[i], targetGrid);
}

//CalcForwardRate Method
double IRSwapContract::calcForwardRate(double startTermZR, double endTermZR){
	double forwardRate = (1 + endTermZR) / (1 + startTermZR) - 1;
	return forwardRate;
}


