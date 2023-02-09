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
    if (argc != 4) {
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

   
  
    Portfolio* hedgingPortfolio = new Portfolio(mc);

    PnlMat* marketData = pnl_mat_create_from_file(argv[2]);

    double price;
    double std_dev;
    double pfValue;
    PnlVect* deltas = pnl_vect_create_from_zero(marketData->n);
    
    PnlVect* stdDeltas = pnl_vect_create_from_zero(marketData->n);

    int rebalancingPeriod = jsonParams.at("PortfolioRebalancingOracleDescription").at("Period").get<int>();

    double t=0.;
    PnlMat* past = pnl_mat_create(1, marketData->n);
    int nbPastRow = 1;
    pnl_mat_extract_subblock(past, marketData, 0, 1, 0, marketData->n);
    pnl_mat_print(past);
    int compteur = 0;
    //std::cout << "vector.end() : " << model->nbOfAssets_.end() << std::endl;
    double place = std::accumulate(model->nbOfAssets_.begin(), model->nbOfAssets_.end(), 0);
    for (int i = 1; i < model->nbCurrencies_ + 1; i++) {
        compteur += model->nbOfAssets_.at(i - 1);
        //place += model->nbOfAssets_.at(i-1);
        std::cout << " compteur : " << compteur <<std::endl;
        for (int j = 0; j < model->nbOfAssets_.at(i); j++) {
            double s = pnl_mat_get(past, 0, compteur + j);
            //std::cout << " s : " << s <<std::endl;
            //std::cout << " place : " << place <<std::endl;
            double x = pnl_mat_get(past, 0, (int)place + i-1 );
            //std::cout << " x : " << x <<std::endl;
            pnl_mat_set(past, 0, compteur + j, s * x);
        }
    }
    //pnl_mat_print(past);

    hedgingPortfolio->mc_->priceAndDelta(past, t, maturity, price, std_dev, deltas, stdDeltas);
    
    PnlVect* assetValues = pnl_vect_create(marketData->n);
    pnl_mat_get_row(assetValues, past, 0);
    double riskFreeQuantity = price - pnl_vect_scalar_prod(deltas, assetValues);
   
    Position* position = new Position(0, price, price, riskFreeQuantity, std_dev, deltas, stdDeltas);

    hedgingPortfolio->positions_.push_back(*position);
    PnlVect*  currentMarketDataRow = pnl_vect_create(marketData->n);
    for (int date = rebalancingPeriod; date <= maturityInDays; date += rebalancingPeriod) {
        if (hedgingPortfolio->mc_->opt_->add(date-1,numberOfDaysPerYear) || (date == rebalancingPeriod) ){
            PnlMat* newpast = pnl_mat_create(nbPastRow + 1, marketData->n);
            pnl_mat_set_subblock(newpast, past, 0, 0);
            pnl_mat_get_row(currentMarketDataRow, marketData, date);
            pnl_mat_set_row(newpast, currentMarketDataRow, nbPastRow);
            pnl_mat_clone(past, newpast);
            nbPastRow++; 
            pnl_mat_free(&newpast);
        }
        else{
            pnl_mat_get_row(currentMarketDataRow, marketData, date);
            pnl_mat_set_row(past, currentMarketDataRow, nbPastRow-1);
        }
        t = (double)date / numberOfDaysPerYear;
        pnl_mat_get_row(assetValues, marketData, date);
        Position currentPosition = hedgingPortfolio->positions_.at(((int)date / rebalancingPeriod) - 1);

        double pfValueBeforeRebalancing = pnl_vect_scalar_prod(currentPosition.deltas_, assetValues) +
                          currentPosition.riskFreeQuantity * exp(model->r_ * rebalancingPeriod / numberOfDaysPerYear);
        
        hedgingPortfolio->mc_->priceAndDelta(past, t, maturity, price, std_dev, deltas, stdDeltas);


        // nv quantit� � mettre au taux sans risque
        double newRiskFree = pfValueBeforeRebalancing - pnl_vect_scalar_prod(deltas, assetValues);

        Position* newPosition = new Position(date, pfValueBeforeRebalancing, price,
                                                 newRiskFree, std_dev, deltas, stdDeltas);

        hedgingPortfolio->positions_.push_back(*newPosition);
        
    }

    nlohmann::json jsonPortfolio = hedgingPortfolio->positions_;
    std::ofstream ifout(argv[3], std::ios_base::out);
    if (!ifout.is_open()) {
        std::cout << "Unable to open file " << argv[3] << std::endl;
        std::exit(1);
    }
    ifout << jsonPortfolio.dump(4);

    pnl_vect_free(&currentMarketDataRow);
    pnl_vect_free(&deltas);
    pnl_vect_free(&stdDeltas);
    delete model;
    delete mc;

    std::exit(0);
}