#include "Portfolio.h"
#include <list>

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
    list<double> deltas;
    list<double> StdDeltas;
    for (int i = 0; i < position.deltas_->size; i++) {
        deltas.push_back(pnl_vect_get(position.deltas_, i));
        StdDeltas.push_back(pnl_vect_get(position.deltasStdDev_, i));
    }
    j["deltas"] = deltas;
    j["deltasStdDev"] = StdDeltas;
}

