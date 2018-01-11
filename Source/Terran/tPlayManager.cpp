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
	for (auto& u : Broodwar->self()->getUnits()){
		if (IsBuilding(u)){
			UnitType UT = u->getType();
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
		}
	}
	b.bb.table = table;
	return test(b, r);
}

Bus tPlaymanager::test(Bus res, PMBus r){
	static UnitType test_UT = UnitTypes::None;
	static int test_fr = 0;
	int C = 4;
	res.cb.gas = true;
	res.cb.gas2 = true;
	if (Broodwar->self()->minerals() > r.resource.mineral && r.resource.mineral <= 500 && !(test_fr + 10 > Broodwar->getFrameCount() && test_UT == UnitTypes::Terran_SCV)){
		test_UT = UnitTypes::Terran_SCV;
		test_fr = Broodwar->getFrameCount();
		res.bb.busno = ++busno;
		res.bb.UT = UnitTypes::Terran_SCV;
	}
	if (Broodwar->self()->supplyTotal() + building_depot * 16 - Broodwar->self()->supplyUsed() <= C && Broodwar->self()->supplyTotal() + building_depot < 400 &&
		!(test_fr + 10 > Broodwar->getFrameCount() && test_UT == UnitTypes::Terran_Supply_Depot)){ // C = PRODUCING BUILDINGS * 2
		Broodwar << "ORDER SET " << building_depot << endl;
		res.cb.busno = ++busno;
		res.cb.UT = UnitTypes::Terran_Supply_Depot;
	}
	return res;
}