#include <iostream>
#include <vector>
#include "Asset.h"
#include "Currency.h"

using namespace std;

class GlobalModel {
public:
	
	int nbCurrencies_; // \hat(n) + 1 the number of different currencies in the global market
	vector<int> nbOfAssets_; // vector containing the number of assets in each currency
	vector<Asset> assets_; //assets of the model
	vector<Currency> currencies_; 

	/*
	* Constructor of a Global Model
	* @param[in] nbCurrencies \hat(n) + 1
	* @param[in] nbOfAssets mapping between currency and nb of assets
	* @param[in] assets assets of the model
	* @param[in] currencies
	*/
	GlobalModel(int nbCurrencies, vector<int> nbOfAssets,
				vector<Asset> assets, vector<Currency> currencies);
 };