#include "RiskyAsset.h"
#include "Asset.h"
#include "pnl/pnl_matrix.h"
#include <cmath>

Asset::Asset(double drift, PnlVect* volatilityVector, double domesticInterestRate) {
	this->drift_ = drift;
	this->volatilityVector_ = volatilityVector;
	this->domesticInterestRate_ = domesticInterestRate;
}

void Asset::simulate(PnlVect* path, PnlVect* currencyVolatilityVector, 
					 double step, PnlMat* G)
{
	int nbDates = path->size;
	PnlVect* volVector = pnl_vect_create(volatilityVector_->size);
	pnl_vect_plus_vect(currencyVolatilityVector, volatilityVector_);
	double spot = pnl_vect_get(path, 0);
	PnlVect* brownian = pnl_vect_create(G->n);
	for (int i = 1; i < nbDates; i++) {
		pnl_mat_get_row(brownian, G, i);
		spot = spot * exp((domesticInterestRate_ - 0.5 * pnl_vect_scalar_prod(volVector, volVector)) * step
			+ sqrt(step) * pnl_vect_scalar_prod(volVector, brownian));
		pnl_vect_set(path, i, spot);
	}
	pnl_vect_free(&volVector);
	pnl_vect_free(&brownian);
}

