#include <iostream>
#include <BWAPI.h>
#include <string>
#include "tPlayManager.h"

using namespace BWAPI;
using namespace Filter;

Bus tPlaymanager::refresh(PMBus r){
	Broodwar->drawTextScreen(0, 30, "%d	%d", Broodwar->self()->minerals() - r.resource.mineral, Broodwar->self()->gas() - r.resource.gas);
	Bus b; b.busno = busno;
	map<UnitType, vector<Unit>> table;
	r.list.find(UnitTypes::Terran_Supply_Depot) != r.list.end() ? building_depot = r.list[UnitTypes::Terran_Supply_Depot] : building_depot = 0;
	map<UnitType, int> n, _n;
	map<UnitType, bool> idle;
	unsigned C = 0;
	for (auto& u : Broodwar->self()->getUnits()){
		UnitType UT = u->getType();
		if (IsBuilding(u)){
			idle.find(UT) != idle.end() ? 0 : idle[UT] = false;
			if (table.find(UT) != table.end())
				table[UT].push_back(u);
			else{
				vector<Unit> UV;
				UV.push_back(u);
				table[UT] = UV;
			}
			if (UT == UnitTypes::Terran_Supply_Depot){
				if (!u->isCompleted()) building_depot++;
			}
			if (u->isTraining()) C++;
			else idle[UT] = true;
		}
		n.find(UT) != n.end() ? n[UT]++ : n[UT] = 0;
		if (!u->isCompleted()) _n.find(UT) != _n.end() ? _n[UT]++ : _n[UT] = 0;
	}
	b.bb.table = table;
	r.C = C;
	r.number = n;
	r._number = _n;
	r.idle = idle;
	return test(b, r);
}

Bus tPlaymanager::test(Bus res, PMBus r){	
	static UnitType test_UT = UnitTypes::None;
	static int test_fr = 0;
	int C = r.C * 2 + 2;
	res.cb.gas = true;
	res.cb.gas2 = true;
	// GATE CONDITION
	if (Broodwar->self()->minerals() <= r.resource.mineral) return res;
	// SCV
	if (r.number[UnitTypes::Terran_SCV] < r.wk){
		test_UT = UnitTypes::Terran_SCV;
		test_fr = Broodwar->getFrameCount();
		res.bb.busno = ++busno;
		res.bb.UT = UnitTypes::Terran_SCV;
		return res;
	}
	// DEPOT
	if (Broodwar->self()->supplyTotal() + building_depot * 16 - Broodwar->self()->supplyUsed() <= C && Broodwar->self()->supplyTotal() + building_depot < 400 &&
		!(test_fr + 10 > Broodwar->getFrameCount() && test_UT == UnitTypes::Terran_Supply_Depot)){ // C = PRODUCING BUILDINGS * 2
		res.cb.busno = ++busno;
		res.cb.UT = UnitTypes::Terran_Supply_Depot;
		return res;
	}

	// BARRACK

	// FACTORY
}