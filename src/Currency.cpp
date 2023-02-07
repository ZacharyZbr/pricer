#include "RiskyAsset.h"
#include "Currency.h"
#include <cmath>

Currency::Currency(double drift, PnlVect* volatilityVector,
	double domesticInterestRate, double foreignInterestRate) {
	this->drift_ = drift;
	this->volatilityVector_ = volatilityVector;
	this->domesticInterestRate_ = domesticInterestRate;
	this->foreignInterestRate_ = foreignInterestRate;

}

void Currency::simulate(PnlVect* path, double spot, double T, int nbTimeStep, PnlMat*G)
{
	int nbDates = path->size;
	double step = T / nbTimeStep;
	pnl_vect_set(path, 0, spot);
	for (int i = 1; i < nbDates; i++) {
		spot = spot * exp((domesticInterestRate_ - 0.5 * pnl_vect_scalar_prod(volatilityVector_, volatilityVector_)) * step
			+ sqrt(step) * pnl_vect_scalar_prod(volatilityVector_, G));
		pnl_vect_set(path, i, spot);
	}
	
}
