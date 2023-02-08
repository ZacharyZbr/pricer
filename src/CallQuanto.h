#pragma once
#include <iostream>
#include "Option.h"
#include <vector>

using namespace std;

class CallQuanto : public Option {
public:
	double strike;
	/**
	* Constructor of a CallCurrencyOption
	* @param[in] T ///  maturité 
	* @param[in] nbTimeSteps /// nombre de pas de temps de discrétisatio
    * @param[in] assetCurrencyMapping ///Vecteur de nombre d'asset par devises
	* @param[in] strike /// strike de l'option
	*/
    CallQuanto(double T, int nbTimeSteps, vector<int> assetCurrencyMapping, int strike);
	~CallQuanto();
    double payoff(const PnlMat* path) override;

};