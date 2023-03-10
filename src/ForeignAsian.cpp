#include "Option.h"
#include "ForeignAsian.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <cmath>

ForeignAsian::ForeignAsian(double T, int nbTimeSteps, vector<int> assetCurrencyMapping, double step) {
	this->T_ = T;
	this->nbTimeSteps_ = nbTimeSteps;
    this->assetCurrencyMapping_ = assetCurrencyMapping;
    this->step_ = step;
}

ForeignAsian::~ForeignAsian(){}

double ForeignAsian::payoff(const PnlMat* path){
    double payoff = 0 ;
    double payoff_tot = 0 ;
    //std::cout << "nbTimeSteps_ : " << nbTimeSteps_<< std::endl;
    for (int i=1; i<=nbTimeSteps_; i++){
        payoff += pnl_mat_get(path, i, this->assetCurrencyMapping_.at(0));
    }
    //std::cout << "pnl_mat_get(path, nbTimeSteps_, 0) : " << pnl_mat_get(path, nbTimeSteps_, 0) << std::endl;
    payoff_tot = (1.0/nbTimeSteps_)*payoff - pnl_mat_get(path, nbTimeSteps_, 0);
    //std::cout << "payoff : " << payoff_tot << std::endl;
	if (payoff_tot > 0){
		return payoff_tot;
	}
	return 0;
};

bool ForeignAsian::add(int date, int nbOfDays){
    double s = ((double)date / nbOfDays) / step_;
    double diff = (int)s - s;
    if(abs(diff)<0.0001){
        return true;
    }
    return false;
};