#pragma once

#include "Option.h"
#include "GlobalModel.h"
#include "pnl/pnl_random.h"

class MonteCarlo
{
  public:
    GlobalModel* mod_; /*! pointeur vers le modèle */
    Option* opt_;            /*! pointeur sur l'option */
    PnlRng* rng_;            /*! pointeur sur le générateur */
    PnlMat* path_;
    double fdStep_;          /*! pas de différence finie */
    long nbSamples_;         /*! nombre de tirages Monte Carlo */

    /**
     * Constructeur de la classe
     */
    MonteCarlo(GlobalModel*, Option*, PnlRng*, double, long);

    ~MonteCarlo();

    /**
     * Calcule le prix de l'option à la date t
     *
     * @param[in]  past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] prix contient le prix
     * @param[out] std_dev contient l'écart type de l'estimateur
     * @param[out] deltas contient les deltas
     * @param[out] std_deltas contient l'écart type des deltas
     */
    void priceAndDelta(PnlMat* Past; double t; double& prix, double& std_dev, PnlVect* deltas, PnlVect* std_deltas);

    


}