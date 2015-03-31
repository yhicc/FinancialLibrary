
#ifndef YIELDCURVE_H_INCLUDED_
#define YIELDCURVE_H_INCLUDED_

#include <string>

#define NUM_OF_LIBORGRD 7
#define NUM_OF_INTPLTDSWPGRD 59
#define NUM_OF_DEFAULT_ZRRTGRD 65

class YieldCurve{

private:

	std::string currency;
	double defaultGridLibor[NUM_OF_LIBORGRD];
	double defaultLiborGrid[NUM_OF_LIBORGRD];
	double *libor;
	int numOfLiborGrid;
	double *liborGrid;
	double *swapRate;
	int numOfSwapGrid;
	double *swapGrid;
	double interpolatedSwapRate[NUM_OF_INTPLTDSWPGRD];
	double interpolatedSwapGrid[NUM_OF_INTPLTDSWPGRD];
	double defaultDiscountFactor[NUM_OF_DEFAULT_ZRRTGRD];
	double defaultZeroRate[NUM_OF_DEFAULT_ZRRTGRD];
	double defaultZeroRateGrid[NUM_OF_DEFAULT_ZRRTGRD];
	double *designatedDiscountFactor;
	double *designatedZeroRate;
	int NumOfDesignatedGrid;
	double *designatedGrid;
	int numOfZeroRateGrid;
	int buildZeroRateFlag;
	
	//interpolateMethod
	double interpolate(
		double preGrid, 
		double postGrid, 
		double preValue, 
		double postValue, 
		double targetGrid);
	
	//interpolateRangeMethod
	double interpolateRange(
		double targetGrid, 
		double *gridArray, 
		double *valueArray, 
		int numOfArray);
	
	//bootstrapMethod
	void bootstrap();
	
	void liborInterpolation();
	
	
public:
	
	//Constructor
	YieldCurve();
	YieldCurve(
		std::string currency, 
		const double *liborValue, 
		int liborGridNum, 
		const double *liborgrid, 
		const double *swapValue, 
		int swapGridNum, 
		const double *swapgrid
	);
	
	//Destructor
	~YieldCurve();

	//setter
	void setcur(std::string cur);
	void setlibor(const double *liborValue, const double *liborgrid, int liborGridNum);
	void setswapRate(const double *swapValue, int swapGridNum, const double *swapgrid);
	
	//getter
	std::string getcur();
	void getLibor(double *liborVal);
	void getSwapRate(double *swapRateVal);
	
	void getDefaultGrid(double *defaultZRGrid);
	
	void getDiscountFactor(double *DF);
	void getDiscountFactor(double *DF, int gridNum, const double *grid);
	void getZeroRate(double *rtnZeroRate);
	void getZeroRate(double *rtnZeroRate, int gridNum, const double *grid);
	
	void getinterpolatedSwapRate(double *interpolatedSwapRateVal);
	
	void buildYieldCurve();
	
};


#endif

