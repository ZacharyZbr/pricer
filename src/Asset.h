#include <iostream>
#include "RiskyAsset.h"

class Asset : public RiskyAsset {
public:
	
	double domesticInterestRate;

	/**
	* Constructor of a (risky) asset
	* @param[in] drift the drift
	* @param[in] volatilityVector the line of the cholesky matrix that corresponds to the asset volatility
	* @param[in] domesticInterestRate the domestic risk free rate
	*/
	Asset(double drift, PnlVect* volatilityVector, double domesticInterestRate);

};