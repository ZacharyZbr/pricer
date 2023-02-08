#include "Option.h"
#include "ForeignAsian.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

ForeignAsian::ForeignAsian(double T, int nbTimeSteps, int size, vector<int> assetCurrencyMapping) {
	this->T_ = T;
	this->size_ = size;
	this->nbTimeSteps_ = nbTimeSteps;
    this->assetCurrencyMapping_ = assetCurrencyMapping;
}

ForeignAsian::~ForeignAsian(){}

double ForeignAsian::payoff(const PnlMat* path){
    double payoff = 0 ;
    for (int i=1; i<=nbTimeSteps_; i++){
        payoff += pnl_mat_get(path, i, this->assetCurrencyMapping_.at(1));
    }
    payoff = (1/nbTimeSteps_)*payoff - pnl_mat_get(path, nbTimeSteps_, 0);
	if (payoff > 0){
		return payoff;
	}
	return 0;
};