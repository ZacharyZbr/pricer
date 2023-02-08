#include "Portfolio.h"

Portfolio::Portfolio(MonteCarlo* mc, vector<Position> positions) {
	this->mc_ = mc;
	this->positions_ = positions;
}
