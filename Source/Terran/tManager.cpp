#include <iostream>
#include <BWAPI.h>
#include "tManager.h"

using namespace BWAPI;
using namespace Filter;
using namespace std;

void tManager::refresh(){
	worker.refresh();
}

void tManager::push(Unit u){
	if (u->getType().isWorker() && IsOwned(u))
		worker.push(u);
}

void tManager::pop(Unit u){
	if (u->getType().isWorker() && IsOwned(u))
		worker.pop(u);
}