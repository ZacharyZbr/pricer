#include "Position.h"

Position::Position(int date, double pfValue, double price, double priceStdDev, PnlVect* deltas, PnlVect* deltasStdDev)
{
	this->date_ = date;
	this->portfolioValue_ = pfValue;
	this->price_ = price;
	this->priceStdDev_ = priceStdDev;
	this->deltas_ = deltas;
	this->deltasStdDev_ = deltasStdDev;
}


