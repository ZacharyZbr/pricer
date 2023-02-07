#include "RiskyAsset.h"
#include "Asset.h"

Asset::Asset(double drift, PnlVect* volatilityVector, double domesticInterestRate) {
	this->drift_ = drift;
	this->volatilityVector_ = volatilityVector;
	this->domesticInterestRate = domesticInterestRate;
}

