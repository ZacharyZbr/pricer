#include "Position.h"

Position::Position(int date, double pfValue, double price, double riskFreeQuantity, 
				   double priceStdDev, PnlVect* deltas, PnlVect* deltasStdDev)
{
	this->date_ = date;
	this->portfolioValue_ = pfValue;
	this->price_ = price;
	this->riskFreeQuantity = riskFreeQuantity;
	this->priceStdDev_ = priceStdDev;
	this->deltas_ = pnl_vect_copy(deltas);
	this->deltasStdDev_ = pnl_vect_copy(deltasStdDev);


}

void Position::print() const {
	nlohmann::json j = *this;
	std::cout << j.dump(4);
}

