#include "Option.h"
#include "CallQuanto.h"

CallQuanto::CallQuanto(double T, int nbTimeSteps, int size, vector<int> assetCurrencyMapping) {
	this->T_ = T;
	this->size_ = size;
	this->nbTimeSteps_ = nbTimeSteps;
    this->assetCurrencyMapping_ = assetCurrencyMapping_;
}

CallQuanto::~CallQuanto(){}

double CallQuanto::Payoff(const PnlMat* path){
    
};

