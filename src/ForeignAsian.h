#pragma once
#include <iostream>
#include "Option.h"
#include <vector>

using namespace std;

class ForeignAsian : public Option {
public:
	double step_;
	/**
	* Constructor of a CallCurrencyOption
	* @param[in] T ///  maturité 
	* @param[in] nbTimeSteps /// nombre de pas de temps de discrétisatio
    * @param[in] assetCurrencyMapping ///Vecteur de nombre d'asset par devises
	*/
    ForeignAsian(double T, int nbTimeSteps, vector<int> assetCurrencyMapping, double step);
	~ForeignAsian();
    double payoff(const PnlMat* path) override;
	bool add(int date, int nbOfDays) override;

};