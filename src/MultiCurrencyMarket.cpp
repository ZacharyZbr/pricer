#include "MultiCurrencyMarket.h"


MultiCurrencyMarket::MultiCurrencyMarket(int nbDomesticRiskyAsset, int nbForeignCurrencies,
	PnlVect* nbForeignRiskyAssets) {
	this->nbDomesticRiskyAsset_ = nbDomesticRiskyAsset;
	this->nbForeignCurrencies_ = nbForeignCurrencies;
	this->nbForeignRiskyAssets_ = nbForeignRiskyAssets;
}

MultiCurrencyMarket::~MultiCurrencyMarket() {
	pnl_vect_free(&nbForeignRiskyAssets_);
}

void MultiCurrencyMarket::toDomesticMarket(PnlVect* spot, PnlVect* exchangeRate)
{
	int currentIndex = nbDomesticRiskyAsset_;
	for (int n = 0; n < nbForeignCurrencies_; n++) {
		for (int i = currentIndex; i < currentIndex + pnl_vect_get(nbForeignRiskyAssets_, n); i++) {
			double priceInDomesticMarket = pnl_vect_get(spot, i) * pnl_vect_get(exchangeRate, n);
			pnl_vect_set(spot, i, priceInDomesticMarket);
		}
		currentIndex += pnl_vect_get(nbForeignRiskyAssets_, n);
	}
}
