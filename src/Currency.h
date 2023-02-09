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
	* 
	* The spot price is stored at index 0 of the vect *path
	* 
	* @param[out] *path the simulation on the temporal grid
	* @param[in] step time difference between two dates in the temporal subdiv grid
	* @param[in] *G Normal vector that corresponds to the Brownian Motion
	*/
	void simulate(PnlVect* path, double step, PnlMat *G);
	void simulateT(PnlVect* path, double step, PnlMat* G, double currentDate, PnlVect* past, double T);

};