#include "Option.h"
#include "CallQuanto.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

CallQuanto::CallQuanto(double T, int nbTimeSteps, int size, vector<int> assetCurrencyMapping, int strike) {
	this->T_ = T;
	this->size_ = size;
	this->nbTimeSteps_ = nbTimeSteps;
    this->assetCurrencyMapping_ = assetCurrencyMapping;
	this->strike_ = strike;
}

CallQuanto::~CallQuanto(){}

double CallQuanto::payoff(const PnlMat* path){
    double payoff = pnl_mat_get(path, this->nbTimeSteps_, this->assetCurrencyMapping_.at(1)) - this->strike_;
	if (payoff > 0){
		return payoff;
	}
	return 0;
};

