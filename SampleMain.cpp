
#include <iostream>
#include <string>
#include "YieldCurve.h"
using namespace std;


//配列画面表示関数（double）
void printout(double *arr, int size){
	for(int i = 0; i < size; i++){
		cout << arr[i] << endl;
	}
}

int main(void){
	
	double liborArr[] = {0.00095,0.0010357,0.0012014,0.0013857,0.0015429,0.00242145,0.0043643};
	double swapArr[] = {0.0024,0.0026,0.0030,0.0036,0.0045,0.0055,0.0067,0.0078,0.0088,0.0099,0.0118,0.0145,0.0175,0.0189,0.0196};
	double liborGrid[] = {0.0027, 0.0192, 0.0833, 0.1667, 0.2500, 0.5000, 1};
	double swapGrid[] = {1,2,3,4,5,6,7,8,9,10,12,15,20,25,30};
	int liborGridNum = 7;
	int swapGridNum = 15;
	
	YieldCurve ycv("JPY", liborArr, liborGridNum, liborGrid, swapArr, swapGridNum, swapGrid);
	
	//グリッド指定なしでZeroRateを取得
	int defaultSize = 65;
	double *zeroRate = new double[defaultSize];
	ycv.getZeroRate(zeroRate);
	
	//グリッド指定なしでDFを取得
	double *DF = new double[defaultSize];
	ycv.getDiscountFactor(DF);
	
	cout << "-----ZeroRate-----" << endl;
	printout(zeroRate, defaultSize);
	cout << "-----DF-----" << endl;
	printout(DF, defaultSize);
	
	
	//グリッド指定ありでZeroRateを取得
	double grid[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	int gridNum = 20;
	double *desZeroRate = new double[gridNum];
	ycv.getZeroRate(desZeroRate, gridNum, grid);
	
	//グリッド指定ありでDFを取得
	double *desDF = new double[gridNum];
	ycv.getDiscountFactor(desDF, gridNum, grid);
	
	cout << "-----DesZeroRate-----" << endl;
	printout(desZeroRate, gridNum);
	cout << "-----DesDF-----" << endl;
	printout(desDF, gridNum);
	
	delete[] zeroRate;
	delete[] DF;
	delete[] desZeroRate;
	delete[] desDF;

	return 0;

}

