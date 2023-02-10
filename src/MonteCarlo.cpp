#include <iostream>
#include "MonteCarlo.h"
#include "GlobalModel.h"
#include "Option.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"
#include <cmath>

using namespace std;

MonteCarlo::MonteCarlo(GlobalModel* mod, Option* opt, PnlRng* rng, double fdStep, int nbSamples, double step){
      this->mod_ = mod; 
      this->opt_ = opt; 
      this->rng_ = rng; 
      this->fdStep_ = fdStep; 
      this->nbSamples_ = nbSamples; 
      this->step_ = step;
      this->path_ = pnl_mat_create(this->opt_->nbTimeSteps_ +1, this->mod_->nbCurrencies_ + this->mod_->assets_.size());

    }

MonteCarlo::~MonteCarlo(){
    pnl_mat_free(&this->path_);
  }

MonteCarlo::MonteCarlo(){
}

void MonteCarlo::set(GlobalModel* mod, Option* opt, PnlRng* rng, double fdStep, int nbSamples, double step){
      this->mod_ = mod; 
      this->opt_ = opt; 
      this->rng_ = rng; 
      this->fdStep_ = fdStep; 
      this->nbSamples_ = nbSamples; 
      this->step_ = step;

      this->path_ = pnl_mat_create(this->opt_->nbTimeSteps_ +1, this->mod_->nbCurrencies_ + this->mod_->assets_.size());
  }

void MonteCarlo::priceAndDelta(PnlMat* Past, double t, double T, double& prix, double& std_dev,
                               PnlVect* delta, PnlVect* std_deltas){
    double sum = 0;
    double sum_2 = 0;
    double sum_d = 0;
    double sum_d_2 = 0;
    pnl_vect_set_zero(delta);
    pnl_vect_set_zero(std_deltas);
    PnlMat* shiftedPathPlus = pnl_mat_create(this->opt_->nbTimeSteps_ + 1, this->mod_->nbCurrencies_ + this->mod_->assets_.size());
    PnlMat* shiftedPathMinus = pnl_mat_create(this->opt_->nbTimeSteps_ + 1, this->mod_->nbCurrencies_ + this->mod_->assets_.size());

    for (int iteration = 0; iteration < nbSamples_; iteration++) {

        mod_->sample(path_,Past, step_, rng_, t);

        for (int d = 0; d < path_->n; d++) {
            
            mod_->shiftSample(path_, shiftedPathPlus, shiftedPathMinus, fdStep_, t, step_, d);
            sum_d = (opt_->payoff(shiftedPathPlus) - opt_->payoff(shiftedPathMinus)) / pnl_mat_get(Past, Past->m-1, d);
            sum_d_2 = sum_d * sum_d;
            pnl_vect_set(delta, d, sum_d + pnl_vect_get(delta, d));
            pnl_vect_set(std_deltas, d, sum_d_2 + pnl_vect_get(std_deltas, d));
        }
        double payoff = opt_->payoff(path_);
        sum += payoff;
        sum_2 += payoff * payoff;
    }
    pnl_mat_free(&shiftedPathPlus);
    pnl_mat_free(&shiftedPathMinus);
    pnl_vect_mult_scalar(delta, (exp(-mod_->r_ * (T - t)) / (nbSamples_ * 2 * fdStep_)));
    
    // Computation of the price and its 5% confidence interval
    prix = exp(-mod_->r_ * (T - t)) * sum / nbSamples_;
    double priceVar = exp(-2 * mod_->r_ * (T - t)) * (sum_2 / nbSamples_ - (sum / nbSamples_) * (sum / nbSamples_));
    std_dev = sqrt(priceVar / nbSamples_);

    // Computation of the 5% confidence interval of the deltas
    pnl_vect_mult_scalar(std_deltas, exp(-2*mod_->r_ * (T - t)) / ((nbSamples_ * (2 * fdStep_)*(2 * fdStep_))));
    PnlVect* deltasSquared = pnl_vect_copy(delta);
    for (int d = 0; d < path_->n; d++) {
        double delta_d = pnl_vect_get(deltasSquared, d);
        pnl_vect_set(deltasSquared, d, delta_d * delta_d);
    }
    pnl_vect_minus_vect(std_deltas, deltasSquared);
    pnl_vect_map_inplace(std_deltas, sqrt);
    pnl_vect_mult_scalar(std_deltas, 1 / sqrt(nbSamples_));
    pnl_vect_free(&deltasSquared);
    
}