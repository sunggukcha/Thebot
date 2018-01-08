// 3 functions
// 1. BASE DEFENSE
// 2. WORKER DISTRIBUTION
// 3. BUILDLOCATION MATTER

#pragma once
#include <BWAPI.h>
#include <iostream>
#include "tBuildLocation.h"
#include "tBus.h"

using namespace BWAPI;
using namespace std;

class Centre{
private:
	vector<Unit> centre;
	tBuildLocation buildlocation;
public:
	WorkerBus refresh(CentreBus);
	Unit rcentre();
	void start(){ buildlocation.start(); }
	void push(Unit);
	void pop(Unit);
};