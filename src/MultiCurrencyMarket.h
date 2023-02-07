#pragma once

#include "pnl/pnl_vector.h"

class MultiCurrencyMarket
{
public: 
	int nbDomesticRiskyAsset_; /// n0 the number of risky assets in the domestic market
	int nbForeignCurrencies_; /// the number of foreign currencies
	PnlVect* nbForeignRiskyAssets_; /// vector of size nbDomesticRiskyAsset
	

	/**
	* Constructor of a MultiCurrencyMarket
	* @param[in] nbDomesticRiskyAsset_ the number of risky assets in the domestic market
	* @param[in] nbForeignCurrencies_ the number of foreign currencies
	* @param[in] nbForeignRiskyAssets vector of size nbDomesticRiskyAsset
	*/
	MultiCurrencyMarket(int nbDomesticRiskyAsset_, int nbForeignCurrencies_,
						PnlVect* nbForeignRiskyAssets);

	~MultiCurrencyMarket();

	/*
	* Convert the spot prices of the multi currency market into the domestic market
	* @param[in] *spot spot prices in the multicurrency market
	* @param[in] *echange rates the exchange rates
	* @param[out] *spot the spot prices in the DOMESTIC market
	*/
	void toDomesticMarket(PnlVect* spot, PnlVect* exchangeRate);

};


// Ajoute greg