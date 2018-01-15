#pragma once
#include <BWAPI.h>
#include <iostream>
#include "tPlayManager.h"
#include "tBuilding.h"
#include "tCenter.h"
#include "tWorker.h"
#include "tSquad.h"
#include "tBus.h"

using namespace BWAPI;
using namespace std;

class tManager{
private:
	PMBus pmbus;
	unsigned mineral;
	unsigned gas;
private:
	tPlaymanager playmanager;
	Centre centre;
	tWorker worker;
	tBuilding building;
	tSquad squad;
public:
	tManager(){ mineral = gas = 0; }
	void start(){ centre.start(); }
	void refresh();
	void discover(Unit u){ if (IsBuilding(u)) squad.discover(u->getTilePosition()); else squad.discover(u); }
	void push(Unit);
	void pop(Unit);
	void add(Resource r){ mineral += r.mineral; gas += r.gas; }
	void add(PMBus pb){ mineral += pb.resource.mineral; gas += pb.resource.gas; pmbus = pb; }
	Resource res(){ Resource r(mineral, gas); mineral = gas = 0; return r; }
};