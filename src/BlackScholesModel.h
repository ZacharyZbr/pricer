#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "MultiCurrencyMarket.h"



class BlackScholesModel
{
	MultiCurrencyMarket *Market_;
	PnlVect* riskFreeRates_; /// vector containing the risk free rates of the domestic and the foreign markets
	PnlVect* spot_; /// spot prices in the DOMESTIC market
	PnlMat* correlationMatrix_; /// correlation matrix given in the JSON file
	PnlMat* cholMatrix_; /// square root of the correlation matrix


	/**
	* Constructor of a Black & Scholes Model
	* @param[in] *Market the multicurrency market
	* @param[in] *riskFreeRates the vector with the risk free rates for the different currencies
	* @param[in] *spot spot prices in the domestic market
	* @param[in] correlationMatrix correlation matrix computed with the market data
	*/
	BlackScholesModel(MultiCurrencyMarket* Market, PnlVect* riskFreeRates,
					  PnlVect* spot, PnlMat* correlationMatrix);

	~BlackScholesModel();

	/*
	* Computes one trajectory of the assets in the domestic market and stores them in the *path matrix
	* @param[in] T matrurity
	* @param[in] nbTimeSteps nb of dates to compute the price
	*/
	void asset(PnlMat* path, double T, int nbTimeSteps, PnlRng* rng);


};

