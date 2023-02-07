#include "GlobalModel.h"

GlobalModel::GlobalModel(int nbCurrencies, vector<int> nbOfAssets,
						 vector<Asset> assets, vector<Currency> currencies) {
	this->assets_ = assets;
	this->currencies_ = currencies;
	this->nbCurrencies_ = nbCurrencies;
	this->nbOfAssets_ = nbOfAssets;
}

void GlobalModel::sample(PnlMat* path, double T, int nbTimeSteps, PnlRng* rng)
{

	int numberOfRiskyAssets = path->n;
	
	// Création d'une matrice G de numberOfRiskyAssets lignes et 
	// nbTimeSteps colonnes qui est la simulation de notre aléa
	PnlMat* G = pnl_mat_create(numberOfRiskyAssets, nbTimeSteps);
	PnlVect* gaussianVector = pnl_vect_create(nbTimeSteps);
	for (int i = 0; i < numberOfRiskyAssets; i++) {
		pnl_vect_rng_normal(gaussianVector, nbTimeSteps, rng);
		pnl_mat_set_row(G, gaussianVector, i);
	}
	pnl_vect_free(&gaussianVector);

	// On parcourt tous les assets
	for (int i = 0; i < assets_.size(); i++) {

		

	}
}
