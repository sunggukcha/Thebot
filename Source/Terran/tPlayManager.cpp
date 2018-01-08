#include <iostream>
#include <BWAPI.h>
#include <string>
#include "tPlayManager.h"

using namespace BWAPI;
using namespace Filter;

Bus tPlaymanager::refresh(Resource r){
	return test(r);
}

Bus tPlaymanager::test(Resource r){
	Bus res;
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