#include "Option.h"
#include "ForeignPerfBasket.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

ForeignPerfBasket::ForeignPerfBasket(double T, int nbTimeSteps, vector<int> assetCurrencyMapping, double strike) {
	this->T_ = T;
	this->nbTimeSteps_ = nbTimeSteps;
    this->assetCurrencyMapping_ = assetCurrencyMapping;
    this->strike_ = strike;
}

ForeignPerfBasket::~ForeignPerfBasket(){}

double ForeignPerfBasket::payoff(const PnlMat* path){
    // calcul du Pmax
    PnlVect *Pi = pnl_vect_create(this->assetCurrencyMapping_.size());
    for (int i = 0; i<this->assetCurrencyMapping_.size(); i++){ //on itere sur chaque marché
        double sumT2 =0;
        double sumT1 =0;
        for(int j =0; j<this->assetCurrencyMapping_.at(i); j++){ //pour chaque marché, o, somme le prix des asset
            sumT1 += pnl_mat_get(path, nbTimeSteps_-2, j);
            sumT2 += pnl_mat_get(path, nbTimeSteps_-1, j);
        }
        pnl_vect_set(Pi, sumT1/sumT2, i);
    }
    double *Pmax;
    int *PmaxIndex;
    pnl_vect_max_index(Pmax, PmaxIndex, Pi); // on trouve Pmax et NiMax l'indice du marché max

    int indexMarcheMax = 0;   // on cherche l'indice dans path du marché max
    if (PmaxIndex != 0){
       for (int i=0; i<*PmaxIndex; i++){
            indexMarcheMax+= this->assetCurrencyMapping_.at(i);
       }
    }
    
    double payoff =0;
    for (int i=indexMarcheMax; i<this->assetCurrencyMapping_.at(*PmaxIndex) + indexMarcheMax; i++){
        payoff += pnl_mat_get(path, nbTimeSteps_, i);
    }
    payoff = payoff/this->assetCurrencyMapping_.at(*PmaxIndex);

    double sum0 = 0;
    for (int i=0; i<this->assetCurrencyMapping_.at(0); i++){
        sum0 += pnl_mat_get(path, nbTimeSteps_, i);
    }
    sum0 = sum0/this->assetCurrencyMapping_.at(0);

    payoff = payoff - sum0 - strike_;

	if (payoff > 0){
		return payoff;
	}
	return 0;
};

bool ForeignPerfBasket::add(int date, int nbOfDays){
    return false;
};