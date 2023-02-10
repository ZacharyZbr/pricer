#pragma once
#include <iostream>
#include "pnl/pnl_vector.h"
#include "nlohmann/json.hpp"

/*
* \brief Class that describes a position at a given date
*/
class Position {
public:
	int date_; /// date of the position
	double portfolioValue_; /// value of the hedging portfolio at the date of the position
	double price_; /// theorical price of the option computed with MonteCarlo->priceAndDelta()
	double riskFreeQuantity; /// investment in the domestic free rate to guarantee an autofinancing portfolio
	double priceStdDev_;
	PnlVect* deltas_; /// composition of the portfolio determined by the deltas computed with MonteCarlo method
	PnlVect* deltasStdDev_;

	/*
	* Constructor of the position at current date
	*/
	Position(int date, double pfValue, double price, double riskFreeQuantity,
			 double priceStdDev, PnlVect* deltas, PnlVect* deltasStdDev);

	friend void to_json(nlohmann::json& j, const Position& positions);
	void print() const;


};