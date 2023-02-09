#include "Portfolio.h"

Portfolio::Portfolio(MonteCarlo* mc) {
	this->mc_ = mc;
	vector<Position> positions;
	this->positions_ = positions;

}
