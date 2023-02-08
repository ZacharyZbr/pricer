#include "Option.h"
#include "Call.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <cmath>

Call::Call(double T, int nbTimeSteps, vector<int> assetCurrencyMapping, int strike) {
	this->T_ = T;
	this->nbTimeSteps_ = nbTimeSteps;
    this->assetCurrencyMapping_ = assetCurrencyMapping;
	this->strike_ = strike;
	this->r = r;
}

CallCurrency::~Call(){}

double CallCurrency::payoff(const PnlMat* path){
    
	double payoff = pnl_mat_get(path, this->nbTimeSteps_, 0) - this->strike_;
	if (payoff > 0){
		return payoff;
	}
	return 0;

}