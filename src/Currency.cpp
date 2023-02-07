#include "RiskyAsset.h"
#include "Currency.h"

Currency::Currency(double drift, PnlVect* volatilityVector,
	double domesticInterestRate, double foreignInterestRate) {
	this->drift_ = drift;
	this->volatilityVector_ = volatilityVector;
	this->domesticInterestRate_ = domesticInterestRate;
	this->foreignInterestRate_ = foreignInterestRate;

}