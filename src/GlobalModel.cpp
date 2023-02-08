#include "GlobalModel.h"

GlobalModel::GlobalModel(int nbCurrencies, vector<int> nbOfAssets,
						 vector<Asset> assets, vector<Currency> currencies, double r) {
	this->assets_ = assets;
	this->currencies_ = currencies;
	this->nbCurrencies_ = nbCurrencies;
	this->nbOfAssets_ = nbOfAssets;
	this->r_ = r;
}

void GlobalModel::sample(PnlMat* path, double step, PnlRng* rng)
{

	// Création d'une matrice G de numberOfRiskyAssets lignes et 
	// nbTimeSteps colonnes qui est la simulation de notre aléa
	PnlMat* G = pnl_mat_create(path->m, path->n);
	PnlVect* gaussianVector = pnl_vect_create(path->n);
	for (int i = 0; i < path->m; i++) {
		pnl_vect_rng_normal(gaussianVector, path->n, rng);
		pnl_mat_set_row(G, gaussianVector, i);
	}
	pnl_vect_free(&gaussianVector);
	
	// On parcourt tous les assets et on set la simulation
	int idMarche = 0;
	PnlVect* pathSimulOfAnAsset = pnl_vect_create(path->m);
	int j = 0;
	for (int i = 0; i < assets_.size(); i++) {
		j++;
		if (j == nbOfAssets_.at(idMarche)) {
			idMarche++;
			j = 0;
		}
		pnl_vect_set(pathSimulOfAnAsset, 0, pnl_mat_get(path, 0, i)); // set pathSimulOfAnAsset[0] = spot of the asset
		assets_.at(i).simulate(pathSimulOfAnAsset, currencies_.at(idMarche).volatilityVector_, step, G);
		pnl_mat_set_col(path, pathSimulOfAnAsset, i);
	}
	for (int i = 0; i < currencies_.size(); i++) {
		currencies_.at(i).simulate(pathSimulOfAnAsset, step, G);
		pnl_mat_set_col(path, pathSimulOfAnAsset, i + assets_.size());
	}

}
