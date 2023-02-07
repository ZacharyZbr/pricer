#pragma once
#include <iostream>
#include "Option.h"

class CallCurrency : public Option {
public:
	
	/**
	* Constructor of a CallCurrencyOption
	* @param[in] T ///  maturité 
	* @param[in] nbTimeSteps /// nombre de pas de temps de discrétisatio
	* @param[in] size ///nombre d'actif risqué (Asset + Currency)
    * @param[in] assetCurrencyMapping ///Vecteur de nombre d'asset par devises
	*/
    CallCurrency(double T, int nbTimeSteps, int size, vector<int> assetCurrencyMapping);
	~CallCurrency();
    double payoff(const PnlMat* path) override;

};