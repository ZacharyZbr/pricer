#include "RiskyAsset.h"
#include "Currency.h"

Currency::Currency(double drift, PnlVect* volatilityVector,
	double domesticInterestRate, double foreignInterestRate) {
	RiskyAsset(drift, volatilityVector);
	this->domesticInterestRate_ = domesticInterestRate;
	this->foreignInterestRate_ = foreignInterestRate;


}