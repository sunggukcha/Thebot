#include <iostream>
#include <BWAPI.h>
#include "tWorker.h"

using namespace BWAPI;
using namespace Filter;
using namespace std;

Resource tWorker::refresh(WorkerBus wb){
	mine();
	return build(wb);
}

void tWorker::mine(){
	for (auto& u : SCV){
		if (u->isIdle()){ // IDLE -> GATHER CLOSEST MINERAL
			Unit M = u->getClosestUnit(IsMineralField);
			if (M){
				u->gather(M);
			}
		}
	}
}

// BUILDING PROCESS
// PLAY MANAGER CANNOT ORDER WHAT IMPOSSIBLE TO BUILD
// REFRESH(CHECK IF COMPLETED) -> ADD BUS ITEM -> GEYSERS -> CENTRE -> ACTUAL BUILD

Resource tWorker::build(WorkerBus wb){
	vector<Info> tinfo;

	// REFRESH & ORDER TO BUILD
	for (auto& I : info){
		// 2016 SUNGGUKCHA VERSION
		if (!I.SCV->exists()) continue; // SCV DEAD
		if (I.POP >= 50) continue;
		else if (I.SCV->isConstructing() || !Broodwar->canBuildHere(I.tile, I.UT, I.SCV)) I.POP++;
		else{ // ADD ECONOMY RELEVANT CONDITIONS
			I.POP = 0;
			Broodwar->drawCircleMap(I.SCV->getPosition(), 4, Colors::Blue, true);
			Position p = (Position)(I.tile + I.UT.tileSize() / 2);
			if (I.SCV->getDistance(p) > 10) I.SCV->move(p);
			else I.SCV->build(I.UT, I.tile);
		}
		tinfo.push_back(I);
	}
	info = tInfo;

	vector<Info> tinfo;
	if (wb.busno > busno){
		busno = wb.busno;

	}
	Resource r;
	return r;
}

void tWorker::push(Unit u)
{ SCV.push_back(u); }
void tWorker::pop(Unit u)
{ SCV.erase(find(SCV.begin(), SCV.end(), u)); } // tentative error: if no such 'u' is inside SCV || about SCB, it should be regarded