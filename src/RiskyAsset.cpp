#include <iostream>
#include "RiskyAsset.h"

RiskyAsset::RiskyAsset(double drift, PnlVect* volatilityVector) {
	this->drift_ = drift;
	this->volatilityVector_ = volatilityVector;
}