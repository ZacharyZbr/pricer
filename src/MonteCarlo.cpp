#include <iostream>

using namespace std;

MonteCarlo::MonteCarlo(GlobalMarket* mod, Option* opt, PnlRng* rng, double fdStep, long nbSamples){
      this->mod_ = mod; 
      this->opt_ = opt; 
      this->rng_ = rng; 
      this->fdStep_ = fdStep; 
      this->nbSamples_ = nbSamples; 

      // this->path_ = pnl_mat_create(this->mod_->size_, this->opt_->nbTimeSteps_ + 1);
      // this->path_ = pnl_mat_create(this->opt_->nbTimeSteps_ + 1, this->mod_->size_);
    }

MonteCarlo::~MonteCarlo(){
    pnl_mat_free(&this->path_);
  }

MonteCarlo::PriceAndDelta(PnlMat* Past; double t; double& prix, double& std_dev, PnlVect* delta, PnlVect* std_deltas){

}