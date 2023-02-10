#include "Option.h"
#include "ForeignPerfBasket.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

ForeignPerfBasket::ForeignPerfBasket(double T, int nbTimeSteps, vector<int> assetCurrencyMapping, double strike, double step) {
	this->T_ = T;
	this->nbTimeSteps_ = nbTimeSteps;
    this->assetCurrencyMapping_ = assetCurrencyMapping;
    this->strike_ = strike;
    this->step_ = step;

}

ForeignPerfBasket::~ForeignPerfBasket(){}

double ForeignPerfBasket::payoff(const PnlMat* path){
    // calcul du Pmax
    int place = 0;
    PnlVect *Pi = pnl_vect_create_from_zero(this->assetCurrencyMapping_.size()-1);
    int nbOfCurrencie =  this->assetCurrencyMapping_.size()-1;
    for (int i = 1; i<this->assetCurrencyMapping_.size(); i++){ //on itere sur chaque marché
        double sumT2 =0.;
        double sumT1 =0.;
        place += assetCurrencyMapping_.at(i-1);
        for(int j =0; j<this->assetCurrencyMapping_.at(i); j++){ //pour chaque marché, o, somme le prix des asset
            sumT1 += pnl_mat_get(path, path->m-3, j + place)/pnl_mat_get(path, path->m-3, path->n - nbOfCurrencie + i -1 );
            sumT2 += pnl_mat_get(path, path->m-2, j + place)/pnl_mat_get(path, path->m-2, path->n - nbOfCurrencie + i -1 );
        }
        
        pnl_vect_set(Pi,i-1,sumT2/sumT1);
    }
    double init = 0.;
    int init2 = 0;
    double *Pmax = &init;
    int *PmaxIndex = &init2;
    pnl_vect_max_index(Pmax, PmaxIndex, Pi); // on trouve Pmax et NiMax l'indice du marché max
    pnl_vect_free(&Pi);
    int PmaxIndexNoPoint = *PmaxIndex +1;
    int indexMarcheMax = 0;// on cherche l'indice dans path du marché max
       for (int i=0; i<PmaxIndexNoPoint; i++){
           indexMarcheMax+= this->assetCurrencyMapping_.at(i);
       }
    

    double payoff =0;
    for (int i=indexMarcheMax; i<this->assetCurrencyMapping_.at(PmaxIndexNoPoint) + indexMarcheMax; i++){
        payoff += pnl_mat_get(path, path->m-1, i);
    }
    payoff = payoff/(double)this->assetCurrencyMapping_.at(PmaxIndexNoPoint);

    double sum0 = 0;
    for (int i=0; i<this->assetCurrencyMapping_.at(0); i++){
        sum0 += pnl_mat_get(path, path->m -1, i);
    }
    sum0 = sum0/(double)this->assetCurrencyMapping_.at(0);

    payoff = payoff - sum0 - strike_;
	if (payoff > 0){
		return payoff;
	}
	return 0;
};

bool ForeignPerfBasket::add(int date, int nbOfDays){

    double s = ((double)date / nbOfDays) / step_;
    double diff = (int)s - s;
    if(abs(diff)<0.0001){
        return true;
    }
    return false;
};