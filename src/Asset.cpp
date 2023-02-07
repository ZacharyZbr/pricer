#include "RiskyAsset.h"
#include "Asset.h"

Asset::Asset(double drift, PnlVect* volatilityVector, double domesticInterestRate) {
	this->drift_ = drift;
	this->volatilityVector_ = volatilityVector;
	this->domesticInterestRate_ = domesticInterestRate;
}

void Asset::simulate(PnlVect* path, PnlVect* currencyVolatilityVector, 
					 double spot,  double T, int nbTimeStep, PnlMat* G)
{
	int nbDates = path->size;
	double step = T / nbTimeStep;
	PnlVect* volVector = pnl_vect_create(volatilityVector_->size);
	pnl_vect_plus_vect(currencyVolatilityVector, volatilityVector_);
	pnl_vect_set(path, 0, spot);
	for (int i = 1; i < nbDates; i++) {
		//spot = spot * exp((domesticInterestRate_ - 0.5 * pnl_vect_scalar_prod(volVector, volVector)) * step
		//	+ sqrt(step) * pnl_vect_scalar_prod(volVector, G));
		//pnl_vect_set(path, i, spot);
	}
	pnl_vect_free(&volVector);
}

