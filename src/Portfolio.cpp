#include "Portfolio.h"

Portfolio::Portfolio(MonteCarlo* mc) {
	this->mc_ = mc;
	vector<Position> positions;
	this->positions_ = positions;

}

void to_json(nlohmann::json& j, const Position& position) {
    j["date"] = position.date_;
    j["value"] = position.portfolioValue_;
    j["price"] = position.price_;
    j["priceStdDev"] = position.priceStdDev_;
    
    j["deltas"] = pnl_vect_get(position.deltas_, 0);
    j["deltasStdDev"] = pnl_vect_get(position.deltasStdDev_, 0);
}

