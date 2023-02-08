#pragma once
#include <iostream>
#include <vector>
#include "Asset.h"
#include "Currency.h"


using namespace std;

class GlobalModel {
public:
	
	int nbCurrencies_; // \hat(n) the number of different currencies in the global market
	vector<int> nbOfAssets_; // vector containing the number of assets in each currency
	vector<Asset> assets_; //assets of the model
	vector<Currency> currencies_; 
	double r_; //domestic risk free rate (redundant with asset->domesticRate but can't do it differently yet
	

	/*
	* Constructor of a Global Model
	* @param[in] nbCurrencies \hat(n) + 1
	* @param[in] nbOfAssets mapping between currency and nb of assets
	* @param[in] assets assets of the model
	* @param[in] currencies
	*/
	GlobalModel(int nbCurrencies, vector<int> nbOfAssets,
				vector<Asset> assets, vector<Currency> currencies, double r);

	GlobalModel(GlobalModel &g);

	void set(int nbCurrencies, vector<int> nbOfAssets,
						 vector<Asset> assets, vector<Currency> currencies, double r);

	GlobalModel();


	/**
	* Simulates the trajectories for all the underlying assets 
	* and exchange rates in the multi-currency market 
	* @param[out] *path matrix of size ( nbTimeSteps \times sum(nbOfAssets) + \hat(n)) in which we store the simulations
	* @param[in] step time difference between two dates in the temporal subdiv grid
	* @param[in] *rng
	* 
	*/
	void sample(PnlMat* path,PnlMat* past, double step, PnlRng*rng, double t);

	/*
	* Shift the path for the computation of the deltas
	* @param[in] *path path of the underlyig assets
	* @param[out] *shiftedPathPlus path \times (1+fdStep)
	* @param[out] *shiftedPathPlus path \times (1-fdStep)
	* @param[in] fdStep finite difference step 
	* @param[in] t time at which we compute the deltas
	* @param[in] step time gap in the temporal subdivision grid
	*/
	void shiftSample(PnlMat* path, PnlMat* shiftedPathPlus, PnlMat* shiftedPathMinus, double fdStep, double t, double step, int d);
 };