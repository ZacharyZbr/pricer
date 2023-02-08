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
	PnlVect* volVector = pnl_vect_copy(currencyVolatilityVector);
	pnl_vect_plus_vect(volVector, volatilityVector_);
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

void Asset::simulateT(PnlVect* path, PnlVect* currencyVolatilityVector, 
					 double step, PnlMat* G, double currentDate, PnlVect* past)
{
	double nextDate = 0.;
	double spot;
	int quotient = currentDate / step;
	if (currentDate != 0){
		nextDate = quotient * step + step;
	}
	pnl_vect_set_subblock(path, past, 0);
	double timeGap = nextDate - currentDate;

	PnlVect* volVector = pnl_vect_copy(currencyVolatilityVector);
	pnl_vect_plus_vect(volVector, volatilityVector_);
	PnlVect* brownian = pnl_vect_create(G->n);
	if (timeGap != 0){
		int indexSt = nextDate / step ;
		pnl_mat_get_row(brownian, G, indexSt);
		spot = pnl_vect_get(past, past->size-1) * exp((domesticInterestRate_ - 0.5 * pnl_vect_scalar_prod(volVector, volVector)) * timeGap
		+ sqrt(timeGap) * pnl_vect_scalar_prod(volVector, brownian));
		pnl_vect_set(path, past->size-1, spot);
	}

	for (int i = past->size-1; i<path->size-1; i++){

		pnl_mat_get_row(brownian, G, i);
		spot = pnl_vect_get(path, i) * exp((domesticInterestRate_ - 0.5 * pnl_vect_scalar_prod(volVector, volVector)) * step
		+ sqrt(step) * pnl_vect_scalar_prod(volVector, brownian));
		pnl_vect_set(path, i+1, spot);

    }
		
	pnl_vect_free(&volVector);
	pnl_vect_free(&brownian);

    }
    



