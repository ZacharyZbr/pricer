#pragma once
#include <iostream>
#include "RiskyAsset.h"

class Asset : public RiskyAsset {
public:
	
	double domesticInterestRate_;

	/**
	* Constructor of a (risky) asset
	* @param[in] drift the drift
	* @param[in] volatilityVector the line of the cholesky matrix that corresponds to the asset volatility
	* @param[in] domesticInterestRate the domestic risk free rate
	*/
	Asset(double drift, PnlVect* volatilityVector, double domesticInterestRate);


	/**
	* Simulates the dynamic X\times S which represents the foreign asset
	* in the domestic market. 
	* 
	* The spot price is stored at index 0 of the vect *path
	* 
	* @param[in] *currencyVolatilityVector the vol vector of the currency X
	* @param[in] step time difference between two dates in the temporal subdiv grid
	*/
	void simulate(PnlVect* path, PnlVect* currencyVolatilityVector,
				  double step, PnlMat* G);

	void simulateT(PnlVect* path, PnlVect* currencyVolatilityVector, 
					 double step, PnlMat* G, double currentDate, PnlVect* past, double T);
};