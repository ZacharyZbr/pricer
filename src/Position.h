#pragma once
#include <iostream>
#include "pnl/pnl_vector.h"

class Position {
public:
	int date_;
	double portfolioValue_;
	double price_;
	double priceStdDev_;
	PnlVect* deltas_;
	PnlVect* deltasStdDev_;

	Position(int date, double pfValue, double price, double priceStdDev, PnlVect* deltas, PnlVect* deltasStdDev);
};