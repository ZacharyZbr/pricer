#include <iostream>
#include "MonteCarlo.h"
#include "GlobalModel.h"
#include "Option.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"

using namespace std;

MonteCarlo::MonteCarlo(GlobalModel* mod, Option* opt, PnlRng* rng, double fdStep, long nbSamples){
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

void MonteCarlo::priceAndDelta(PnlMat* Past, double t, double& prix, double& std_dev, PnlVect* delta, PnlVect* std_deltas){

}