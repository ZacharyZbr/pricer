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
    int numberOfDaysPerYear1 = jsonParams.at("NumberOfDaysInOneYear").get<int>();
    double maturity1 = jsonParams.at("Option").at("MaturityInDays").get<int>() / double (numberOfDaysPerYear1);

    GlobalModel* model1 = new GlobalModel();
    MonteCarlo* mc1 = new MonteCarlo();
    PnlMat* past1 = parsefile(jsonParams, model1, mc1);
    pnl_mat_print(past1);
    double price1;
    double std_dev1;
    PnlVect* deltas1 = pnl_vect_create(past1->n);
    PnlVect* stdDeltas1 = pnl_vect_create(past1->n);

    double t1 = 0.5;
    mc1->priceAndDelta(past1, t1, maturity1, price1, std_dev1, deltas1, stdDeltas1);

    std::cout << "price1 : " << price1 << std::endl;
    //PnlMat *market = pnl_mat_create_from_file(argv[2]);

    std::exit(0);
}