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

struct Resource{
	unsigned mineral;
	unsigned gas;
	Resource(){}
	Resource(int m, int g) :mineral(m), gas(g){}
};