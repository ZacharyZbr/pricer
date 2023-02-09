#include <iostream>
#include <fstream>
#include "json_reader.hpp"
#include "MonteCarlo.h"
#include "GlobalModel.h"
#include "Position.h"
#include "Portfolio.h"
#include <vector>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Wrong number of arguments. Exactly one argument is required" << std::endl;
        std::exit(0);
    }
    std::ifstream ifs(argv[1]);

    nlohmann::json jsonParams = nlohmann::json::parse(ifs);
    int numberOfDaysPerYear = jsonParams.at("NumberOfDaysInOneYear").get<int>();
    double maturityInDays = jsonParams.at("Option").at("MaturityInDays").get<int>();
    double maturity = maturityInDays / double(numberOfDaysPerYear);

    GlobalModel* model = new GlobalModel();
    MonteCarlo* mc = new MonteCarlo();
    parsefile(jsonParams, model, mc);
    // pnl_mat_print(past);
 

    
    Portfolio* hedgingPortfolio = new Portfolio(mc);
    cout << argv[2] << endl;

    PnlMat* marketData = pnl_mat_create_from_file(argv[2]);

    double price;
    double std_dev;
    double pfValue;
    PnlVect* deltas = pnl_vect_create(marketData->n);
    PnlVect* stdDeltas = pnl_vect_create(marketData->n);
    int rebalancingPeriod = jsonParams.at("PortfolioRebalancingOracleDescription").at("Period").get<int>();

    double t=0.;
    PnlMat* past = pnl_mat_create(1, marketData->n);
    pnl_mat_extract_subblock(past, marketData, 0, 1, 0, marketData->n);
    hedgingPortfolio->mc_->priceAndDelta(past, t, maturity, price, std_dev, deltas, stdDeltas);
    PnlVect* assetValues = pnl_vect_create(marketData->n);
    pnl_mat_get_row(assetValues, past, 0);
    double riskFreeQuantity = price - pnl_vect_scalar_prod(deltas, assetValues);
    Position* position = new Position(0, price, price, riskFreeQuantity, std_dev, deltas, stdDeltas);
    hedgingPortfolio->positions_.push_back(*position);
    

    for (int date = rebalancingPeriod; date < maturityInDays; date += rebalancingPeriod) {
        
        t = date / numberOfDaysPerYear;
        PnlMat* past = pnl_mat_create(date+1, marketData->n);
        pnl_mat_extract_subblock(past, marketData, 0, date+1, 0, marketData->n); //
        pnl_mat_get_row(assetValues, past, date);
        double pfValueBeforeRebalancing = pnl_vect_scalar_prod(deltas, assetValues) +
            hedgingPortfolio->positions_.at((int)date / rebalancingPeriod).riskFreeQuantity * exp(model->r_ * rebalancingPeriod / numberOfDaysPerYear);

        hedgingPortfolio->mc_->priceAndDelta(past, t, maturity, price, std_dev, deltas, stdDeltas);
        // nv quantité à mettre au taux sans risque
        double newRiskFree = pfValueBeforeRebalancing - pnl_vect_scalar_prod(deltas, assetValues);
        hedgingPortfolio->positions_.at((int)date / rebalancingPeriod).riskFreeQuantity = newRiskFree;


        Position* currentPosition = new Position(date, pfValueBeforeRebalancing, price,
                                                 newRiskFree, std_dev, deltas, stdDeltas);

        hedgingPortfolio->positions_.push_back(*currentPosition);

    }


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