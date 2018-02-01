#pragma once
#include <BWAPI.h>
#include <iostream>
#include "tBus.h"

using namespace BWAPI;
using namespace std;

class tPlaymanager{
private:
	vector<UnitType> kinds;
	map<UnitType, int> all;
	unsigned busno;
private: // MAGICS //
	Bus test(Bus, PMBus);
	int building_depot;
public:
	tPlaymanager(){
		busno = 0; building_depot = 0;
		kinds.push_back(UnitTypes::Terran_Command_Center);
		kinds.push_back(UnitTypes::Terran_Supply_Depot);
		kinds.push_back(UnitTypes::Terran_Barracks);
		kinds.push_back(UnitTypes::Terran_Factory);
		kinds.push_back(UnitTypes::Terran_Machine_Shop);
		kinds.push_back(UnitTypes::Terran_Siege_Tank_Tank_Mode);
		kinds.push_back(UnitTypes::Terran_Starport);
		kinds.push_back(UnitTypes::Terran_Control_Tower);
		kinds.push_back(UnitTypes::Terran_Dropship);
		kinds.push_back(UnitTypes::Terran_Science_Vessel);
		kinds.push_back(UnitTypes::Terran_Valkyrie);
		kinds.push_back(UnitTypes::Terran_Battlecruiser);
	}
	Bus refresh(PMBus);
	bool ok(PMBus, UnitType); // RETURNS IF THE UT IS TRAINABLE
	bool ok(PMBus, TechType);
	bool ok(PMBus, UpgradeType);
};