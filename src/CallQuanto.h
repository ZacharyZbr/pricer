#pragma once
#include <iostream>
#include "Option.h"

class CallQuanto : public Option {
public:
	
	/**
	* Constructor of a CallCurrencyOption
	* @param[in] T ///  maturité 
	* @param[in] nbTimeSteps /// nombre de pas de temps de discrétisatio
	* @param[in] size ///nombre d'actif risqué (Asset + Currency)
    * @param[in] assetCurrencyMapping ///Vecteur de nombre d'asset par devises
	*/
    CallQuanto(double T, int nbTimeSteps, int size, vector<int> assetCurrencyMapping);
	~CallQuanto();
    double payoff(const PnlMat* path) override;

};