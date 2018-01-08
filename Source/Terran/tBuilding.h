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
private:
	unsigned mineral;
	unsigned gas;
	void train();
	void upgrade();
	void research();
public:
	Resource refresh(BuildBus);
	/*
	void push(Unit);		// COMPLETE
	void pop(Unit);			// DESTROY
	*/
public:
	tBuilding(){ busno = 0; }
};