#pragma once
#include <BWAPI.h>
#include <iostream>
#include "tPlayManager.h"
#include "tBuilding.h"
#include "tCenter.h"
#include "tWorker.h"
#include "tBus.h"

using namespace BWAPI;
using namespace std;

class tManager{
private:
	unsigned mineral;
	unsigned gas;
private:
	tPlaymanager playmanager;
	Centre centre;
	tWorker worker;
	tBuilding building;
public:
	tManager(){ mineral = gas = 0; }
	void start(){ centre.start(); }
	void refresh();
	void push(Unit);
	void pop(Unit);
	void add(Resource r){ mineral += r.mineral; gas += r.gas; }
	Resource res(){ Resource r(mineral, gas); mineral = gas = 0; return r; }
};