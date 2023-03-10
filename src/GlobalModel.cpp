#include "GlobalModel.h"

GlobalModel::GlobalModel(int nbCurrencies, vector<int> nbOfAssets,
						 vector<Asset> assets, vector<Currency> currencies, double r, double T) {
	this->assets_ = assets;
	this->currencies_ = currencies;
	this->nbCurrencies_ = nbCurrencies;
	this->nbOfAssets_ = nbOfAssets;
	this->r_ = r;
	this->T_ = T;
}

void GlobalModel::set(int nbCurrencies, vector<int> nbOfAssets,
						 vector<Asset> assets, vector<Currency> currencies, double r, double T){
	this->assets_ = assets;
	this->currencies_ = currencies;
	this->nbCurrencies_ = nbCurrencies;
	this->nbOfAssets_ = nbOfAssets;
	this->r_ = r;
	this->T_ = T;
}

GlobalModel::GlobalModel(){
}

void GlobalModel::sample(PnlMat* path, PnlMat* past, double step, PnlRng* rng, double t)
{
	// Cr�ation d'une matrice G de numberOfRiskyAssets lignes et 
	// nbTimeSteps colonnes qui est la simulation de notre al�a
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
	PnlVect* pastSimulOfAnAsset = pnl_vect_create(past->m);
	int j = 0;
	for (int i = 0; i < assets_.size(); i++) {
		
		if (j == nbOfAssets_.at(idMarche)) {
			idMarche++;
			j = 0;
		}
		j++;
		pnl_mat_get_col(pastSimulOfAnAsset, past, i);
		
		//pnl_vect_set(pastSimulOfAnAsset, 0, pnl_mat_get(path, 0, i)); // set pathSimulOfAnAsset[0] = spot of the asset
		if (idMarche == 0) {
			PnlVect* volVector = pnl_vect_create_from_zero(assets_.size() + currencies_.size());
			assets_.at(i).simulateT(pathSimulOfAnAsset, volVector, step, G, t, pastSimulOfAnAsset, T_);
			pnl_mat_set_col(path, pathSimulOfAnAsset, i);
			pnl_vect_free(&volVector);

		}
		else {
			assets_.at(i).simulateT(pathSimulOfAnAsset, currencies_.at(idMarche-1).volatilityVector_, step, G, t, pastSimulOfAnAsset, T_);
			pnl_mat_set_col(path, pathSimulOfAnAsset, i);
		}
	}
	
	for (int i = 0; i < currencies_.size(); i++) {
		pnl_mat_get_col(pastSimulOfAnAsset, past, i + assets_.size());
		//pnl_vect_set(pathSimulOfAnAsset, 0, pnl_mat_get(path, 0, i + assets_.size()));
		currencies_.at(i).simulateT(pathSimulOfAnAsset, step, G, t, pastSimulOfAnAsset, T_);
		pnl_mat_set_col(path, pathSimulOfAnAsset, i + assets_.size());
	}

	pnl_vect_free(&pastSimulOfAnAsset);
	pnl_vect_free(&pathSimulOfAnAsset);
	pnl_mat_free(&G);

	}

	void GlobalModel::shiftSample(PnlMat* path, PnlMat* shiftedPathPlus, PnlMat* shiftedPathMinus, double fdStep, double t, double step, int d) {
		pnl_mat_clone(shiftedPathPlus, path);
		pnl_mat_clone(shiftedPathMinus, path);
		double nextDate = 0.;
		int quotient = t / step;
		if (t != 0) {
			nextDate = quotient * step + step;
		}
		int nextDateIndexInMatrix = nextDate / step;

		for (int index = nextDateIndexInMatrix; index < path->m; index++) {
			double shiftedPlusValue = pnl_mat_get(path, index, d) * (1 + fdStep);
			double shiftedMinusValue = pnl_mat_get(path, index, d) * (1 - fdStep);
			pnl_mat_set(shiftedPathPlus, index, d, shiftedPlusValue);
			pnl_mat_set(shiftedPathMinus, index, d, shiftedMinusValue);
		}
}
