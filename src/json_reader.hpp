#pragma once

#include <nlohmann/json.hpp>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <vector>
#include "json_reader.hpp"
#include "MonteCarlo.h"
#include "GlobalModel.h"
#include "Asset.h"
#include "Currency.h"
#include "CallCurrency.h"
#include "CallQuanto.h"
#include "ForeignAsian.h"
#include "Call.h"
#include "QuantoExchange.h"
#include <vector>
#include "Asset.h"


void from_json(const nlohmann::json &j, PnlVect *&vect);
void from_json(const nlohmann::json &j, PnlMat *&mat);
PnlMat* parsefile(const nlohmann::json &j, GlobalModel* model, MonteCarlo* mc);
