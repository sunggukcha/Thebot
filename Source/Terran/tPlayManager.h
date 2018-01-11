#pragma once
#include <BWAPI.h>
#include <iostream>
#include "tBus.h"

using namespace BWAPI;
using namespace std;

class tPlaymanager{
private:
	map<UnitType, int> all;
	unsigned busno;
private: // MAGICS //
	Bus test(Bus, PMBus);
	int building_depot;
public:
	tPlaymanager(){ busno = 0; building_depot = 0; }
	Bus refresh(PMBus);
};