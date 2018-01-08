#pragma once
#include <iostream>
#include <BWAPI.h>
#include "tBuildLocation.h"

using namespace std;
using namespace BWAPI;

struct BuildBus{
	unsigned busno;
	UnitType UT;
	UpgradeType UpT;
	TechType TT;
	map<UnitType, vector<Unit>> table;
	BuildBus(){ UT = UnitTypes::None; UpT = UpgradeTypes::None; TT = TechTypes::None; }
};

struct CentreBus{
	unsigned busno;
	UnitType UT;
	CentreBus(){}
	CentreBus(unsigned bno, UnitType ut) : busno(bno), UT(ut) {}
};

struct WorkerBus : CentreBus{
	WorkerBus(){}
	WorkerBus(CentreBus cb) : CentreBus(cb.busno, cb.UT) {}
	tBuildLocation BL;
};

struct SquadBus{
	unsigned busno;

};

struct Bus{
	unsigned busno;
	BuildBus bb;
	CentreBus cb;
	SquadBus sb;
};

struct Resource{
	unsigned mineral;
	unsigned gas;
	Resource(){}
	Resource(int m, int g) :mineral(m), gas(g){}
};