#include "RiskyAsset.h"
#include "Asset.h"

Asset::Asset(double drift, PnlVect* volatilityVector, double domesticInterestRate) {
	RiskyAsset(drift, volatilityVector);
	this->domesticInterestRate = domesticInterestRate;
}

