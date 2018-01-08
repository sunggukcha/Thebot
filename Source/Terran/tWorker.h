#pragma once
#include <BWAPI.h>
#include <iostream>
#include "tBus.h"

using namespace BWAPI;
using namespace std;

class tWorker{
private:
	unsigned busno;
	vector<Unit> SCV; // MINING SCV
	vector<Unit> SCB; // BUILDING SCV; INCLUDES SCV ABOUT TO BUILD
public: // Unit bus
	void push(Unit);
	void pop(Unit);
public: // Manager functions
	tWorker(){ busno = 0; }
	Resource refresh(WorkerBus);
private:
	void mine();
	Resource build(WorkerBus);

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