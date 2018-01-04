#pragma once
#include <BWAPI.h>
#include <iostream>
#include "tBus.h"

using namespace BWAPI;
using namespace std;

class tBuilding{
private:
	unsigned busno;
	map<UnitType, vector<Unit>> table; // Hash Table that saves buildings
	vector<UnitType> uorder;
	vector<UpgradeType> uporder;
	vector<TechType> rorder;
public:
	void refresh(BuildBus);
	void train();
	void upgrade();
	void push(Unit);		// COMPLETE
	void pop(Unit);			// DESTROY
public:
	tBuilding(){ busno = 0; }
};