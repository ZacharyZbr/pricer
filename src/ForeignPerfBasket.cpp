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
    //std::cout << " path : " << std::endl;
    //pnl_mat_print(path);
    //std::cout << " size : " << this->assetCurrencyMapping_.size()<< std::endl;
    int place = 0;
    PnlVect *Pi = pnl_vect_create_from_zero(this->assetCurrencyMapping_.size()-1);
    //std::cout << " aaaa : " << this->assetCurrencyMapping_.size() - 1<< std::endl;
    for (int i = 0; i<this->assetCurrencyMapping_.size()-1; i++){ //on itere sur chaque marché
        double sumT2 =0.;
        double sumT1 =0.;
        place += assetCurrencyMapping_.at(i);
        //std::cout << " i : " << i << std::endl;
        for(int j =0; j<this->assetCurrencyMapping_.at(i+1); j++){ //pour chaque marché, o, somme le prix des asset
            //std::cout << " j : " << j << std::endl;
            //std::cout << " path->n : " << path->m << std::endl;
            //pnl_mat_print(path);
            //std::cout << " debut1  " << std::endl;
            sumT1 += pnl_mat_get(path, path->m-3, j + place);
            //std::cout << " debut2  " << std::endl;
            sumT2 += pnl_mat_get(path, path->m-2, j + place);
            //std::cout << " apres " << std::endl;
        }
        
       // std::cout << " i - 1 : " << i-1 << std::endl;
        //std::cout << " size pi : " << Pi->size<< std::endl;
        //pnl_vect_print(Pi);
        
        pnl_vect_set(Pi, i, sumT2/sumT1);
    }
    //std::cout << " fin 2 : " << std::endl;
    //std::cout << " Pi : "  << std::endl;
    //pnl_vect_print(Pi);
    double init = 0.;
    int init2 = 0;
    //std::cout << " avant : " << std::endl;
    double *Pmax = &init;
    int *PmaxIndex = &init2;
    //std::cout << " PmaxIndex avant : " << *PmaxIndex << std::endl;
    pnl_vect_max_index(Pmax, PmaxIndex, Pi); // on trouve Pmax et NiMax l'indice du marché max
    // double max = pnl_vect_get(Pi, 0);
    // int indice = 0;
    // for (int i =1; i< Pi->size; i++){
    //     double valeur = pnl_vect_get(Pi, i);
    //     if (valeur > max){
    //         max = valeur;
    //         indice = i;
    //     }
    // }

   //std::cout << " apres " << std::endl;
    //std::cout << " PmaxIndex apres : " << *PmaxIndex << std::endl;
    pnl_vect_free(&Pi);
    int PmaxIndexNoPoint = *PmaxIndex +1;
    int indexMarcheMax = 0;//*PmaxIndex+1;   // on cherche l'indice dans path du marché max
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
    //std::cout << " payoff " << payoff << std::endl;
	if (payoff > 0){
		return payoff;
	}
	return 0;
};

bool ForeignPerfBasket::add(int date, int nbOfDays){
    
    if((double)(date / nbOfDays) == step_){

        return true;
    }
    return false;
};