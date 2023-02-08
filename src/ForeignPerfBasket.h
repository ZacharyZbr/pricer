#pragma once
#include <iostream>
#include "Option.h"
#include <vector>

using namespace std;

class ForeignPerfBasket : public Option {
public:
    double strike_;
	/**
	* Constructor of a CallCurrencyOption
	* @param[in] T ///  maturité 
	* @param[in] nbTimeSteps /// nombre de pas de temps de discrétisatio
	* @param[in] size ///nombre d'actif risqué (Asset + Currency)
    * @param[in] assetCurrencyMapping ///Vecteur de nombre d'asset par devises
	*/
    ForeignPerfBasket(double T, int nbTimeSteps, int size, vector<int> assetCurrencyMapping, double strike);
	~ForeignPerfBasket();
    double payoff(const PnlMat* path) override;

};