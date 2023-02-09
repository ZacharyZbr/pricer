#pragma once
#include <iostream>
#include <list>
#include <vector>
#include "pnl/pnl_vector.h"
#include "Position.h"
#include "MonteCarlo.h"
#include <nlohmann/json.hpp>

class Portfolio
{
public:
	MonteCarlo* mc_;
	vector<Position> positions_;

	Portfolio(MonteCarlo* mc);

	friend void to_json(nlohmann::json& j, const Position& positions);
};

