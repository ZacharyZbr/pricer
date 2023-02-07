#include "GlobalModel.h"

GlobalModel::GlobalModel(int nbCurrencies, vector<int> nbOfAssets,
						 vector<Asset> assets, vector<Currency> currencies) {
	this->assets_ = assets;
	this->currencies_ = currencies;
	this->nbCurrencies_ = nbCurrencies;
	this->nbOfAssets_ = nbOfAssets;
}