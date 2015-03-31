
#include "YieldCurve.h"
#include "MyDate.h"
#include "IRSwapContract.h"
#include <iostream>
#include <string>
using std::cout;
using std::endl;


int main(void){
	
	double liborArr[] = {0.00095,0.0010357,0.0012014,0.0013857,0.0015429,0.00242145,0.0043643};
	double swapArr[] = {0.0024,0.0026,0.0030,0.0036,0.0045,0.0055,0.0067,0.0078,0.0088,0.0099,0.0118,0.0145,0.0175,0.0189,0.0196};
	double liborGrid[] = {0.0027, 0.0192, 0.0833, 0.1667, 0.2500, 0.5000, 1};
	double swapGrid[] = {1,2,3,4,5,6,7,8,9,10,12,15,20,25,30};
	int liborGridNum = 7;
	int swapGridNum = 15;
	
	YieldCurve *ycv;
	ycv = new YieldCurve("JPY", liborArr, liborGridNum, liborGrid, swapArr, swapGridNum, swapGrid);
	
	//get ZeroRate
	int defaultSize = 65;
	double *zeroRate = new double[defaultSize];
	ycv->getZeroRate(zeroRate);
		
	//get default(on YieldCurve Class) grid
	double *defaultGrid = new double[defaultSize];
	ycv->getDefaultGrid(defaultGrid);
	
	//valuate Swap Contract
	string valuationDate = "20141215";
	string effectiveDate = "20130601";
	string currency = "JPY";
	int fixedOrFloatPayer = 1;
	double notionalAmount = 100000000;
	double contractTerm = 5;
	double paymentPeriod = 0.5;
	double fixedRate = 0.002;
	double preFixedFloatRate = 0.0025;
	//transrform grid to number of days from years
	int *floatRateTerm = new int[defaultSize];
	for(int i = 0; i < defaultSize; i++){
		floatRateTerm[i] = defaultGrid[i] * 365;
	}
	
	IRSwapContract *swapcont;
	swapcont = new IRSwapContract();
	swapcont->setValuationDate(valuationDate);
	swapcont->setEffectiveDate(effectiveDate);
	swapcont->setCurrency(currency);
	swapcont->setFixedOrFloatPayer(fixedOrFloatPayer);
	swapcont->setNotionalAmount(notionalAmount);
	swapcont->setContractTerm(contractTerm);
	swapcont->setPaymentPeriod(paymentPeriod);
	swapcont->setFixedRate(fixedRate);
	swapcont->setNextFloatRate(preFixedFloatRate);
	swapcont->setFloatRate(zeroRate, floatRateTerm, defaultSize);

	double PV = swapcont->calcPV();
	cout << "PV = " << PV << endl;

	delete[] zeroRate;
	delete[] defaultGrid;
	delete[] floatRateTerm;
	delete ycv;
	delete swapcont;
	
	return 0;

}

