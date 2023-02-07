#pragma once
#include <iostream>
#include "pnl/pnl_vector.h"

/**
* \brief abstract class for a risky asset
*/
class RiskyAsset 
{
public:
	double drift_; 
	PnlVect* volatilityVector_;
	
};