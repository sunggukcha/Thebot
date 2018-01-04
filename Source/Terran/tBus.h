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
	BuildBus(){ UT = UnitTypes::None; UpT = UpgradeTypes::None; TT = TechTypes::None; }
};

struct WorkerBus{
	unsigned busno;
	UnitType UT;
};

struct SquadBus{
	unsigned busno;

};

struct Bus{
	unsigned busno;
	BuildBus bb;
	WorkerBus wb;
	SquadBus sb;
};

struct Resource{
	unsigned mineral;
	unsigned gas;
	Resource(){}
	Resource(int m, int g) :mineral(m), gas(g){}
};