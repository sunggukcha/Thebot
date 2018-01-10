#pragma once
#include <BWAPI.h>
#include <iostream>
#include "tBus.h"

using namespace BWAPI;
using namespace std;

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

class tWorker{
private:
	unsigned busno;
	vector<Unit> SCV; // MINING SCV
	vector<Info> info;
	map<UnitType, int> list; // BUILD LIST, pop when the build is compensated
public: // Unit bus
	void push(Unit);
	void pop(Unit);
public: // Manager functions
	tWorker(){ busno = 0; }
	Resource refresh(WorkerBus);
private:
	void mine();
	Resource build(WorkerBus);
	bool isWorking(Unit);
	int howMany(UnitType);

	/*
public:
	//Order
	bool refresh(vector<Unit> All_units, vector<Unit> ML_hatcheries, vector<Unit> Unsatisfied_hatcheries, BuildLocations, BuildInfo, tuple<Unit, TilePosition> hatchery, vector<Unit> geysers);
	void mine(vector<Unit> h);
	bool build(vector<Unit> All_units, vector<Unit> ML_hatcheries, vector<Unit> Unsatisfied_hatcheries, BuildLocations, BuildInfo, tuple<Unit, TilePosition> hatchery, vector<Unit> geysers);

	//Sub-order
	vector<UnitType> Built;
	bool canIt(UnitType);

	//Unit Manage
	void push(Unit);
	void pop(Unit);
	*/
};