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

void Currency::simulate(PnlVect* path, double step, PnlMat*G)
{
	
	PnlVect* brownian = pnl_vect_create(G->n);
	double spot = pnl_vect_get(path, 0);
	for (int i = 1; i < path->size; i++) {
		pnl_mat_get_row(brownian, G, i);
		spot = spot * exp((domesticInterestRate_ - 0.5 * pnl_vect_scalar_prod(volatilityVector_, volatilityVector_)) * step
			+ sqrt(step) * pnl_vect_scalar_prod(volatilityVector_, brownian));
		pnl_vect_set(path, i, spot);
	}
	pnl_vect_free(&brownian);
	
}
