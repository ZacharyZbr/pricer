#pragma once
#include <iostream>
#include "Option.h"

#include <vector>


using namespace std;


class CallCurrency : public Option {
public:
	double r;
	double strike;
	/**
	* Constructor of a CallCurrencyOption
	* @param[in] T ///  maturité 
	* @param[in] nbTimeSteps /// nombre de pas de temps de discrétisatio
	* @param[in] size ///nombre d'actif risqué (Asset + Currency)
    * @param[in] assetCurrencyMapping ///Vecteur de nombre d'asset par devises
	* @param[in] strike /// strike de l'option
	* @param[in] r /// strike de l'option
	*/
    CallCurrency(double T, int nbTimeSteps, int size, vector<int> assetCurrencyMapping, int strike, double r);
	~CallCurrency();
    double payoff(const PnlMat* path) override;

};