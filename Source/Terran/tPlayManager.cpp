#include <iostream>
#include <BWAPI.h>
#include <string>
#include "tPlayManager.h"

using namespace BWAPI;
using namespace Filter;

Bus tPlaymanager::refresh(Resource r){
	Bus b; b.busno = busno;
	map<UnitType, vector<Unit>> table;
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
		}
	}
	b.bb.table = table;
	return test(b, r);
}

Bus tPlaymanager::test(Bus res, Resource r){
	Broodwar->drawTextScreen(0, 0, "Resource reserved: %d %d", Broodwar->self()->minerals(), Broodwar->self()->gas());
	Broodwar->drawTextScreen(0, 15, "Remain resource: %d %d", Broodwar->self()->minerals() - r.mineral, Broodwar->self()->gas() - r.gas);
	Broodwar->drawTextScreen(0, 30, "Busno: %d", busno);
	if (Broodwar->self()->minerals() - r.mineral > 0 && r.mineral <= 500){
		res.bb.busno = ++busno;
		res.bb.UT = UnitTypes::Terran_SCV;
	}
	if (Broodwar->self()->supplyTotal() == Broodwar->self()->supplyUsed() && Broodwar->self()->supplyTotal() < 400 && r.mineral <= 600){
		res.cb.busno = ++busno;
		res.cb.UT = UnitTypes::Terran_Supply_Depot;
	}
	return res;
}