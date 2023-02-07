#include "BlackScholesModel.h"

BlackScholesModel::BlackScholesModel(MultiCurrencyMarket* Market, PnlVect* riskFreeRates,
	PnlVect* spot, PnlMat* correlationMatrix) {
	this->Market_ = Market;
	this->riskFreeRates_ = riskFreeRates;
	this->spot_ = spot;
	this->correlationMatrix_ = correlationMatrix;
	this->cholMatrix_ = correlationMatrix;
	pnl_mat_chol(cholMatrix_);
}

BlackScholesModel::~BlackScholesModel() {
	pnl_mat_free(&correlationMatrix_);
	pnl_mat_free(&cholMatrix_);
	pnl_vect_free(&spot_);
	pnl_vect_free(&riskFreeRates_);
	delete Market_;
}


