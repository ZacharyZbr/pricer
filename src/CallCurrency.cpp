#include "Option.h"
#include "CallCurrency.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <cmath>

CallCurrency::CallCurrency(double T, int nbTimeSteps, int size, vector<int> assetCurrencyMapping, int strike, double r) {
	this->T_ = T;
	this->size_ = size;
	this->nbTimeSteps_ = nbTimeSteps;
    this->assetCurrencyMapping_ = assetCurrencyMapping;
	this->strike_ = strike;
	this->r = r;
}

CallCurrency::~CallCurrency(){}

double CallCurrency::payoff(const PnlMat* path){
    
	double payoff = pnl_mat_get(path, this->nbTimeSteps_, 1)*exp(this->r * this->T_) - this->strike;
	if (payoff > 0){
		return payoff;
	}
	return 0;
};

