#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <vector>

using namespace std;
    

/// \brief Classe Option abstraite
class Option
{
  public:
    double T_;        /// maturité
    int nbTimeSteps_; /// nombre de pas de temps de discrétisation
    vector<int> assetCurrencyMapping_; ///Vecteur de nombre d'asset par devises. 
    double strike_;

    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (N+1) x d
     * contenant une trajectoire du modèle telle que créée
     * par la fonction asset.
     * @return phi(trajectoire)
     */
    virtual double payoff(const PnlMat* path) = 0;
    virtual bool add(int date, int nbOfDays) = 0;
};
