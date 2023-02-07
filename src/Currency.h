#include <iostream>
#include "RiskyAsset.h"

class Currency :public RiskyAsset {
public:
	double domesticInterestRate_;
	double foreignInterestRate_;

	Currency(double drift, PnlVect* volatilityVector,
		double domesticInterestRate, double foreignInterestRate);

};