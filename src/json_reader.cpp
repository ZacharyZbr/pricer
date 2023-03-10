#include <iostream>
#include <fstream>
#include <string>
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
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "ForeignPerfBasket.h"


void from_json(const nlohmann::json &j, PnlVect *&vect) {
    std::vector<double> stl_v = j.get<std::vector<double>>();
    vect = pnl_vect_create_from_ptr(stl_v.size(), stl_v.data());
}

void from_json(const nlohmann::json &j, PnlMat *&mat) {
    std::vector<std::vector<double>> stl_m = j.get<std::vector<std::vector<double>>>();
    int nLines = stl_m.size();
    if (nLines == 0) {
        mat = pnl_mat_create(0, 0);
        return;
    }
    int nColumns = stl_m[0].size();
    for (auto it : stl_m) {
        if (nColumns != it.size()) {
            std::cerr << "Matrix is not regular" << std::endl;
            mat = NULL;
            return;
        }
    }
    mat = pnl_mat_create(nLines, nColumns);
    int m = 0;
    for (auto row : stl_m) {
        pnl_mat_set_row_from_ptr(mat, row.data(), m);
        m++;
    }
}

void parsefile(const nlohmann::json &jsonParams, GlobalModel* model, MonteCarlo* mc){
    PnlMat *correlation;
    jsonParams.at("Correlations").get_to(correlation);

    // Matrice de cholesky

    std::string domesticCurrencyId;
    jsonParams.at("DomesticCurrencyId").get_to(domesticCurrencyId);

    int assetNb = jsonParams.at("Assets").size();

    auto jsonCurrencies = jsonParams.at("Currencies");
    int currencyNb = jsonCurrencies.size();
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

    vector<Asset> AssetVector; 
    auto jsonAssets = jsonParams.at("Assets");
    double indiceAsset = 0;
    vector<int> nbOfAsset;
    for(int i=0; i<currencyNb; i++){
        nbOfAsset.push_back(0);
    }
    //nbOfAsset.at(0)++;

    string currentCurrency = domesticCurrencyId;
    int currentindiceOfAsset = 0;
    for (auto jsonAsset : jsonAssets) {
        indiceAsset ++;
        PnlVect* volatilityVector = pnl_vect_create(correlation->n);
        pnl_mat_get_row(volatilityVector, correlation, indiceAsset -1 );
        std::string currencyId(jsonAsset.at("CurrencyId").get<std::string>());
        double realVolatility = jsonAsset.at("Volatility").get<double>();
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

    int numberOfDaysPerYear = jsonParams.at("NumberOfDaysInOneYear").get<int>();
    double maturity = jsonParams.at("Option").at("MaturityInDays").get<int>() / double (numberOfDaysPerYear);
    std::string label = jsonParams.at("Option").at("Type").get<std::string>();

    std::cout << "Beginning the hedging of the option " << label << std::endl;
    std::cout << "Please Wait ... " << std::endl;


    Option* myOption;
    double step;
    double fdStep = jsonParams.at("RelativeFiniteDifferenceStep").get<double>();
    int NSample = jsonParams.at("SampleNb").get<int>();

    if(label == "foreign_asian"){
        double period = jsonParams.at("Option").at("FixingDatesInDays").at("Period").get<double>();
        step = period/numberOfDaysPerYear;
        double nbTimeStep = jsonParams.at("Option").at("MaturityInDays").get<double>()/period;
        myOption = new ForeignAsian(maturity,nbTimeStep,nbOfAsset, step);
    }
    else if(label == "call_currency"){
        double strike = jsonParams.at("Option").at("Strike").get<double>();
        Currency* foreign = &CurrencyVector.at(0);
        myOption = new CallCurrency(maturity,1,nbOfAsset,strike,foreign->foreignInterestRate_ );
        step = maturity;
    }
    else if(label == "quanto_exchange"){
        double strike = jsonParams.at("Option").at("Strike").get<double>();
        myOption = new QuantoExchange(maturity,1,nbOfAsset,strike);
        step = maturity;
    }
    else if(label =="call_quanto"){
        double strike = jsonParams.at("Option").at("Strike").get<double>();
        myOption = new CallQuanto(maturity,1,nbOfAsset,strike);
        step = maturity;
    }
    else if(label =="call_vanille"){
        double strike = jsonParams.at("Option").at("Strike").get<double>();
        myOption = new Call(maturity,1,nbOfAsset,strike);
        step = maturity;
    }
    else if (label == "foreign_perf_basket") {
        double strike = jsonParams.at("Option").at("Strike").get<double>();
        PnlVect* dates;
        jsonParams.at("Option").at("FixingDatesInDays").at("DatesInDays").get_to(dates);
        step = (pnl_vect_get(dates, 1) - pnl_vect_get(dates, 0)) / numberOfDaysPerYear;
        double nbTimeStep = dates->size;
        myOption = new ForeignPerfBasket(maturity, nbTimeStep-1, nbOfAsset, strike, step);
    }
    else {
        std::cout << "On ne traite pas ce genre d'option" << std::endl;
        abort();
    }

    model->set(currencyNb-1, nbOfAsset, AssetVector, CurrencyVector,domesticRate, maturity);

    PnlRng* pnl_rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(pnl_rng, time(NULL));

    mc->set(model,myOption, pnl_rng, fdStep, NSample, step );
}
