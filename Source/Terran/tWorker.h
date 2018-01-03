#pragma once
#include <BWAPI.h>
#include <iostream>

using namespace BWAPI;
using namespace std;

class tWorker{
private:
	vector<Unit> SCV; // MINING SCV
	vector<Unit> SCB; // BUILDING SCV; INCLUDES SCV ABOUT TO BUILD
public: // Unit bus
	void push(Unit);
	void pop(Unit);
public: // Manager functions
	void refresh();
private:
	void mine();

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