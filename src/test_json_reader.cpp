#include <iostream>
#include <fstream>
#include "json_reader.hpp"
#include "MonteCarlo.h"
#include "GlobalModel.h"
#include "Asset.h"
#include "Currency.h"
#include "CallCurrency.h"
#include "CallQuanto.h"
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

    PnlMat *correlation;
    jsonParams.at("Correlations").get_to(correlation);
    std::cout << "Matrice de Correlation : " << std::endl;
    pnl_mat_print(correlation);

    std::cout << "Matrice de cholesky : "  << std::endl;
    pnl_mat_chol(correlation);
    pnl_mat_print(correlation);

    std::string domesticCurrencyId;
    jsonParams.at("DomesticCurrencyId").get_to(domesticCurrencyId);
    std::cout << "Domestic ID : " << domesticCurrencyId << std::endl;
    int assetNb = jsonParams.at("Assets").size();
    std::cout << "Number of assets " << assetNb << std::endl;

    std::cout << "-- currencies" << std::endl;
    auto jsonCurrencies = jsonParams.at("Currencies");
    int currencyNb = jsonCurrencies.size();
    std::cout << "Number of currency " << currencyNb << std::endl;
    double domesticRate = 0;
    double indiceCurrency = 0;
    vector<Currency> CurrencyVector; 
    for (auto jsonCurrency : jsonCurrencies) {
        std::string CurrencyId;
        jsonCurrency.at("id").get_to(CurrencyId);
        double rf = jsonCurrency.at("InterestRate").get<double>();
        if (CurrencyId == domesticCurrencyId){
            domesticRate =  rf;
        }
        else{
            indiceCurrency ++;
            PnlVect* volatilityVector = pnl_vect_create(correlation->n);
            pnl_mat_get_row(volatilityVector, correlation, correlation->m - currencyNb + indiceCurrency);
            std::string currencyId(jsonCurrency.at("id").get<std::string>());
            double realVolatility = jsonCurrency.at("Volatility").get<double>();
            std::cout << "interest rate " << rf << std::endl;
            std::cout << "real volatility " << realVolatility << std::endl;
            pnl_vect_mult_scalar(volatilityVector,realVolatility);
            Currency* myCurrency = new Currency(domesticRate, volatilityVector, domesticRate, rf);
            CurrencyVector.push_back(*myCurrency);
        }
        
        // regarder volatilyty vector pour Domestic currency
    }
    for (int i=0; i<CurrencyVector.size(); i++){
        CurrencyVector.at(i).drift_ = domesticRate;
        CurrencyVector.at(i).domesticInterestRate_ = domesticRate;
    }

    std::cout << "domestic interest rate " << domesticRate << std::endl;

    std::cout << "-- assets" << std::endl;

    vector<Asset> AssetVector; 
    auto jsonAssets = jsonParams.at("Assets");
    double indiceAsset = 0;
    vector<int> nbOfAsset;
    for(int i=0; i<currencyNb; i++){
        nbOfAsset.push_back(0);
    }
    //nbOfAsset.at(0)++;
    //std::cout << " test " <<  nbOfAsset.at(0) << std::endl;
    string currentCurrency = domesticCurrencyId;
    int currentindiceOfAsset = 0;
    for (auto jsonAsset : jsonAssets) {
        indiceAsset ++;
        PnlVect* volatilityVector = pnl_vect_create(correlation->n);
        pnl_mat_get_row(volatilityVector, correlation, indiceAsset -1 );
        std::string currencyId(jsonAsset.at("CurrencyId").get<std::string>());
        double realVolatility = jsonAsset.at("Volatility").get<double>();
        std::cout << "currency " << currencyId << std::endl;
        std::cout << "real volatility " << realVolatility << std::endl;
        pnl_vect_mult_scalar(volatilityVector,realVolatility);
        Asset* myAsset= new Asset(domesticRate, volatilityVector, domesticRate);
        AssetVector.push_back(*myAsset);
        if(currencyId==currentCurrency){
            nbOfAsset.at(currentindiceOfAsset)++;
        }
        else{
            currentindiceOfAsset++;
            nbOfAsset.at(currentindiceOfAsset)++;
            currentCurrency = currencyId;
        }
    }

    for(int i=0; i<currencyNb; i++){
        std::cout << " test " <<  nbOfAsset.at(i) << std::endl;
    }

    int numberOfDaysPerYear = jsonParams.at("NumberOfDaysInOneYear").get<int>();
    double maturity = jsonParams.at("Option").at("MaturityInDays").get<int>() / double (numberOfDaysPerYear);
    std::string label = jsonParams.at("Option").at("Type").get<std::string>();
    std::cout << "Type option : " << label << std::endl;
    std::cout << "Maturité : " << maturity << std::endl;
    std::cout << "numberOfDaysPerYear : " << numberOfDaysPerYear << std::endl;

    PnlMat* path;
    Option* myOption;
    double step;
    double fdStep = jsonParams.at("RelativeFiniteDifferenceStep").get<double>();
    int NSample = jsonParams.at("SampleNb").get<int>();

    if(label == "foreign_asian"){

    }
    else if(label == "call_currency"){
        double strike = jsonParams.at("Option").at("Strike").get<double>();
        Currency* foreign = &CurrencyVector.at(0);
        myOption = new CallCurrency(maturity,1,0,nbOfAsset,strike,foreign->foreignInterestRate_ );
        path =  pnl_mat_create(2, assetNb+currencyNb-1);
        pnl_mat_set(path,0,0,10);
        step = maturity;
    }
    else if(label == "quanto_exchange"){

    }
    else if(label =="call_quanto"){

    }
    GlobalModel* model = new GlobalModel(currencyNb-1, nbOfAsset, AssetVector, CurrencyVector, domesticRate);
    PnlRng* pnl_rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(pnl_rng, time(NULL));

    MonteCarlo* mc = new MonteCarlo(model, myOption, pnl_rng, fdStep, NSample, step);
    double price;
    double std_dev;
    PnlVect* deltas = pnl_vect_create(assetNb);
    PnlVect* stdDeltas = pnl_vect_create(assetNb);

    mc->priceAndDelta(path, 0., maturity, price, std_dev, deltas, stdDeltas);

    std::cout << "price : " << price << std::endl;

    pnl_mat_free(&correlation);
    pnl_vect_free(&deltas);
    pnl_vect_free(&stdDeltas);
    delete model;
    delete mc;
    std::exit(0);
}