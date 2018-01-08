#include <iostream>
#include <BWAPI.h>
#include "tManager.h"

using namespace BWAPI;
using namespace Filter;
using namespace std;

void tManager::refresh(){
	Bus bus = playmanager.refresh(res());
	add(building.refresh(bus.bb));
	add(worker.refresh(bus.wb));
}

void tManager::push(Unit u){
	if (u->getType().isWorker() && IsOwned(u))
		worker.push(u);
}

void tManager::pop(Unit u){
	if (u->getType().isWorker() && IsOwned(u))
		worker.pop(u);
}