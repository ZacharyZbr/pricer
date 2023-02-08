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

void MonteCarlo::priceAndDelta(PnlMat* Past, double t, double T, double& prix, double& std_dev,
                               PnlVect* delta, PnlVect* std_deltas){
    double sum = 0;
    double sum_d = 0;
    PnlMat* shiftedPathPlus = pnl_mat_create(this->opt_->nbTimeSteps_ + 1, this->mod_->nbCurrencies_ + this->mod_->assets_.size());
    PnlMat* shiftedPathMinus = pnl_mat_create(this->opt_->nbTimeSteps_ + 1, this->mod_->nbCurrencies_ + this->mod_->assets_.size());
    for (int iteration = 0; iteration < nbSamples_; iteration++) {
        mod_->sample(path_,Past, step_, rng_, t);
        for (int d = 0; d < path_->n; d++) {
            
            mod_->shiftSample(path_, shiftedPathPlus, shiftedPathMinus, fdStep_, t, step_, d);
           
            sum_d = (opt_->payoff(shiftedPathPlus) - opt_->payoff(shiftedPathMinus)) / pnl_mat_get(Past, (int) t/step_, d);
          
            pnl_vect_set(delta, d, sum_d + pnl_vect_get(delta, d));
        }
        sum+=opt_->payoff(path_);
    }
    pnl_mat_free(&shiftedPathPlus);
    pnl_mat_free(&shiftedPathMinus);
    pnl_vect_mult_scalar(delta, (exp(-mod_->r_ * (T - t)) / (nbSamples_ * 2 * fdStep_)));
    prix = exp(-mod_->r_ * (T - t)) * sum / nbSamples_;
}