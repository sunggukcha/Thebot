#pragma once
#include <iostream>
#include <BWAPI.h>

using namespace std;
using namespace BWAPI;

struct BuildBus{
	unsigned busno;
	UnitType UT;
	UpgradeType UpT;
	TechType TT;
};