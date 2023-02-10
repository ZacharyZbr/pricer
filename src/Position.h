#pragma once
#include <iostream>
#include "pnl/pnl_vector.h"
#include "nlohmann/json.hpp"

class Position {
public:
	int date_;
	double portfolioValue_;
	double price_;
	double riskFreeQuantity;
	double priceStdDev_;
	PnlVect* deltas_;
	PnlVect* deltasStdDev_;

	Position(int date, double pfValue, double price, double riskFreeQuantity,
			 double priceStdDev, PnlVect* deltas, PnlVect* deltasStdDev);

	friend void to_json(nlohmann::json& j, const Position& positions);
	void print() const;


};