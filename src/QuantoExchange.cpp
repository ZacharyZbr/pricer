#include "Option.h"
#include "QuantoExchange.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <cmath>

QuantoExchange::QuantoExchange(double T, int nbTimeSteps, vector<int> assetCurrencyMapping, int strike) {
	this->T_ = T;
	this->nbTimeSteps_ = nbTimeSteps;
    this->assetCurrencyMapping_ = assetCurrencyMapping;
	this->strike_ = strike;
}

QuantoExchange::~QuantoExchange(){}

double QuantoExchange::payoff(const PnlMat* path){
    
	double payoff = pnl_mat_get(path, this->nbTimeSteps_, 0) - pnl_mat_get(path, this->nbTimeSteps_, this->assetCurrencyMapping_.at(1))  - this->strike_;
	if (payoff > 0){
		return payoff;
	}
	return 0;
};

bool QuantoExchange::add(int date, int nbOfDays){
	if (date == (int)(T_ * nbOfDays)) {
		return true;
	}
	return false;
}