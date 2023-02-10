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
    //std::cout << " Path : " << std::endl;
    //pnl_mat_print(path);
    //std::cout << " size : " << this->assetCurrencyMapping_.size()<< std::endl;
    int place = 0;
    PnlVect *Pi = pnl_vect_create(this->assetCurrencyMapping_.size());
    for (int i = 1; i<this->assetCurrencyMapping_.size()-1; i++){ //on itere sur chaque marché
        double sumT2 =0;
        double sumT1 =0;
        place += assetCurrencyMapping_.at(i-1);
        for(int j =0; j<this->assetCurrencyMapping_.at(i); j++){ //pour chaque marché, o, somme le prix des asset
            sumT1 += pnl_mat_get(path, nbTimeSteps_-2, j + place);
            sumT2 += pnl_mat_get(path, nbTimeSteps_-1, j + place);
        }
        pnl_vect_set(Pi, sumT1/sumT2, i);
    }
    double init = 0.;
    int init2 = 0;
    double *Pmax = &init;
    //std::cout << " Pmax avant : " << *Pmax << std::endl;
    int *PmaxIndex = &init2;
    //std::cout << " PmaxIndex avant : " << *PmaxIndex << std::endl;
    pnl_vect_max_index(Pmax, PmaxIndex, Pi); // on trouve Pmax et NiMax l'indice du marché max
    //std::cout << " Pmax apres : " << *Pmax << std::endl;
    //std::cout << " PmaxIndex apres : " << *PmaxIndex << std::endl;
    int indexMarcheMax = 1;//*PmaxIndex+1;   // on cherche l'indice dans path du marché max
       for (int i=1; i<*PmaxIndex; i++){
           indexMarcheMax+= 1;//this->assetCurrencyMapping_.at(i);
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
    if(date % nbTimeSteps_ == 0){
        return true;
    }
    return false;
};