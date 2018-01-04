#include <iostream>
#include <BWAPI.h>
#include <string>
#include "tPlayManager.h"

using namespace BWAPI;
using namespace Filter;

Bus tPlaymanager::refresh(Resource r){
	return test(r);
}
/*
if (Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed() + (BUILD.getsup() + Building_depot_number) * 16  <
	4 * (Building_centres - Saturated_centres + Building_factories) &&
	Broodwar->self()->supplyTotal() + (BUILD.getsup() + Building_depot_number) * 16 < 400 && (!first_depot || Building_centres >= 1)){
	*/
Bus tPlaymanager::test(Resource r){
	Bus res;
	if (Broodwar->self()->minerals() - r.mineral > 0 && r.mineral <= 500){
		res.bb.busno = ++busno;
		res.bb.UT = UnitTypes::Terran_SCV;
	}
	if (Broodwar->self()->supplyTotal() == Broodwar->self()->supplyUsed() && Broodwar->self()->supplyTotal() < 400 && r.mineral <= 600){
		res.wb.busno = ++busno;
		res.wb.UT = UnitTypes::Terran_Supply_Depot;
	}
	return res;
}