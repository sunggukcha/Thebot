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
	BuildBus(){ busno = 0; UT = UnitTypes::None; UpT = UpgradeTypes::None; TT = TechTypes::None; }
};

struct CentreBus{
	bool gas;	// refinery building
	bool gas2;	// gas mining
	unsigned busno;
	UnitType UT;
	CentreBus(){ busno = 0; gas = false; gas2 = false; }
	CentreBus(unsigned bno, UnitType ut, bool g1, bool g2) : busno(bno), UT(ut), gas(g1), gas2(g2) {}
};

struct WorkerBus : CentreBus{ // CENTRE -> WORKER
	unsigned short worker;
	WorkerBus(){}
	WorkerBus(CentreBus cb) : CentreBus(cb.busno, cb.UT, cb.gas, cb.gas2) {}
	tBuildLocation BL;
	vector<Unit> geyser;
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
	Resource(){ mineral = gas = 0; }
	Resource(int m, int g) :mineral(m), gas(g){}
};

struct Info{
	Unit SCV;
	UnitType UT;
	TilePosition tile;
	int frame;
	int POP;
	Info(){ POP = 0; }
	Info(Unit u, UnitType ut, TilePosition tp) : SCV(u), UT(ut), tile(tp){ POP = 0; }
	Info(Unit u, UnitType ut, TilePosition tp, int N) : SCV(u), UT(ut), tile(tp), frame(N){ POP = 0; }
};

struct PMBus{
	// BETWEEN PLAMANAGER
	unsigned short wk;			// WORKER REQUIRED NUMBER
	unsigned C;					// TRAINING BUILDINGS
	map<UnitType, int> number, _number; // number for complete number, _number for incomplete number of the UT
	map<UnitType, bool> idle;
	//
	Resource resource;			// FROM MANAGER
	map<UnitType, int> list;	// CONSTRUCT PLAN LIST from WORKER
	PMBus(){}
	PMBus(Resource res, map<UnitType, int> _info) :resource(res), list(_info){}
};