#pragma once
#include <iostream>
#include "RiskyAsset.h"
#include "pnl/pnl_random.h"

class Currency :public RiskyAsset {
public:
	double domesticInterestRate_;
	double foreignInterestRate_;

	/**
	* Constructor of currency object
	*/
	Currency(double drift, PnlVect* volatilityVector,
		double domesticInterestRate, double foreignInterestRate);

	/*
	* Simulates the asset on a given temporal subdivision grid
	* @param[out] *path the simulation on the temporal grid
	* @param[in] spot spot price of the asset
	* @param[in] T maturity
	* @param[in] nbTimeStep
	* @param[in] *G Normal vector that corresponds to the Brownian Motion
	*/
	void simulate(PnlVect* path, double spot, double T, int nbTimeStep, PnlMat *G);

};