#pragma once
#include <BWAPI.h>
#include <iostream>
#include "tWorker.h"
#include "tBuilding.h"
#include "tBus.h"

using namespace BWAPI;
using namespace std;

class tManager{
private:
	unsigned mineral;
	unsigned gas;
private:
	tWorker worker;
	tBuilding building;
public:
	void refresh();
	void push(Unit);
	void pop(Unit);
	void add(Resource r){ mineral += r.mineral; gas += r.gas; }
	Resource res(){ Resource r(mineral, gas); return r; }
};