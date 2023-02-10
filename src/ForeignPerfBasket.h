#pragma once
#include <iostream>
#include "Option.h"
#include <vector>

using namespace std;

class ForeignPerfBasket : public Option {
public:
    double strike_;
	double step_;
	/**
	* Constructor of a CallCurrencyOption
	* @param[in] T ///  maturité 
	* @param[in] nbTimeSteps /// nombre de pas de temps de discrétisatio
    * @param[in] assetCurrencyMapping ///Vecteur de nombre d'asset par devises
	*/
    ForeignPerfBasket(double T, int nbTimeSteps, vector<int> assetCurrencyMapping, double strike, double step);
	~ForeignPerfBasket();
    double payoff(const PnlMat* path) override;
	bool add(int date, int nbOfDays) override; 
};