#include "Option.h"
#include "CallCurrency.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <cmath>

CallCurrency::CallCurrency(double T, int nbTimeSteps, vector<int> assetCurrencyMapping, int strike, double r) {
	this->T_ = T;
	this->nbTimeSteps_ = nbTimeSteps;
    this->assetCurrencyMapping_ = assetCurrencyMapping;
	this->strike_ = strike;
	this->r = r;
}

CallCurrency::~CallCurrency(){}

double CallCurrency::payoff(const PnlMat* path){
    
	double payoff = pnl_mat_get(path, this->nbTimeSteps_, 0)*exp(this->r * this->T_) - this->strike_;
	if (payoff > 0){
		return payoff;
	}
	return 0;
};

bool CallCurrency::add(int date, int nbOfDays){
    if (date==(int) T_*nbOfDays){
		return true;
	}
	return false;
};

