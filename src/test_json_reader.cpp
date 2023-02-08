#include <iostream>
#include <fstream>
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
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Wrong number of arguments. Exactly one argument is required" << std::endl;
        std::exit(0);
    }
    std::ifstream ifs(argv[1]);

    nlohmann::json jsonParams = nlohmann::json::parse(ifs);
    int numberOfDaysPerYear = jsonParams.at("NumberOfDaysInOneYear").get<int>();
    double maturity = jsonParams.at("Option").at("MaturityInDays").get<int>() / double (numberOfDaysPerYear);

    GlobalModel* model = new GlobalModel();
    MonteCarlo* mc = new MonteCarlo();
    PnlMat* past = parsefile(jsonParams, model, mc);
    pnl_mat_print(past);
    double price;
    double std_dev;
    PnlVect* deltas = pnl_vect_create(past->n);
    PnlVect* stdDeltas = pnl_vect_create(past->n);

    double t = 0.5;

    std::cout << "nsample : " << mc->nbSamples_ << std::endl;

    mc->priceAndDelta(past, t, maturity, price, std_dev, deltas, stdDeltas);

    std::cout << "price : " << price << std::endl;
    //PnlMat *market = pnl_mat_create_from_file(argv[2]);
    std::cout << "deltas : " << std::endl;
   
    pnl_vect_print(deltas);
    pnl_vect_free(&deltas);
    pnl_vect_free(&stdDeltas);
    delete model;
    delete mc;

    std::exit(0);
}