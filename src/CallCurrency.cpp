#include "Option.h"
#include "CallCurrency.h"

CallCurrency::CallCurrency(double T, int nbTimeSteps, int size, vector<int> assetCurrencyMapping) {
	this->T_ = T;
	this->size_ = size;
	this->nbTimeSteps_ = nbTimeSteps;
    this->assetCurrencyMapping_ = assetCurrencyMapping_;
}

CallCurrency::~CallCurrency(){}

double CallCurrency::Payoff(const PnlMat* path){
    
};

