#pragma once
#include <iostream>
#include <vector>
#include "pnl/pnl_vector.h"
#include "Position.h"
#include "MonteCarlo.h"

class Portfolio
{
	MonteCarlo* mc_;
	vector<Position> positions_;

	Portfolio(MonteCarlo* mc, vector<Position> positions);

};

