#include <iostream>
#include <BWAPI.h>
#include "tManager.h"

using namespace BWAPI;
using namespace Filter;
using namespace std;

void tManager::refresh(){
	Broodwar->drawTextScreen(0, 0, "Frame: %d", Broodwar->getFrameCount());
	Broodwar->drawTextScreen(0, 15, "APM: %d", Broodwar->getAPM());
	// PM -> BUILD -> CENTRE -> WORKER -> SQUAD
	Bus bus = playmanager.refresh(res()); // PM
	add(building.refresh(bus.bb)); // BUILD
	add(worker.refresh(centre.refresh(bus.cb))); // CENTRE -> WORKER
}

void tManager::push(Unit u){
	if (u->getType().isWorker() && IsOwned(u))
		worker.push(u);
	if (u->getType() == UnitTypes::Terran_Command_Center && IsOwned(u))
		centre.push(u);
}

void tManager::pop(Unit u){
	if (u->getType().isWorker() && IsOwned(u))
		worker.pop(u);
	if (u->getType() == UnitTypes::Terran_Command_Center && IsOwned(u))
		centre.pop(u);
}