#include <iostream>
#include <BWAPI.h>
#include "tWorker.h"

using namespace BWAPI;
using namespace Filter;
using namespace std;

Resource tWorker::refresh(WorkerBus wb){
	Broodwar->drawTextScreen(0, 60, "Busno received/had: %d/%d", wb.busno, busno);
	mine();
	return build(wb);
}

void tWorker::mine(){
	for (auto& u : SCV){
		if (u->isIdle() && !isWorking(u)){ // IDLE -> GATHER CLOSEST MINERAL
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
		if (I.POP >= 50){
			list[I.UT]--;
			continue;
		}
		else if (I.SCV->isConstructing()) I.POP++;
		else if (!Broodwar->canBuildHere(I.tile, I.UT, I.SCV)) continue;
		else if (Broodwar->self()->minerals() >= 0.8 * I.UT.mineralPrice() && Broodwar->self()->gas() >= 0.8 * I.UT.gasPrice()){ // ADD ECONOMY RELEVANT CONDITIONS
			I.POP = 0;
			Position p = (Position)(I.tile + I.UT.tileSize() / 2);
			if (I.SCV->getDistance(p) > 10) I.SCV->move(p);
			else I.SCV->build(I.UT, I.tile);

			Unit u = I.SCV;
			UnitType ut = I.UT;
			TilePosition tp1 = I.tile;
			TilePosition tp2 = I.tile + I.UT.tileSize();

			Broodwar->registerEvent([u, ut, tp1, tp2](Game*)
			{
				Broodwar->drawCircleMap(u->getPosition(), 4, Colors::Blue, true);
				Broodwar->drawBoxMap((Position)tp1, (Position)tp2, Colors::Orange);
				Broodwar->drawTextMap(u->getPosition(), "%s", ut.c_str());
			}, nullptr, Broodwar->getLatencyFrames());
		}
		tinfo.push_back(I);
	}
	info = tinfo;

	// ADD BUS ITEM
	if (wb.busno > busno){
		busno = wb.busno;
		if (wb.UT != UnitTypes::None){
			if (list.find(wb.UT) == list.end()) list[wb.UT] = 1;
			else list[wb.UT]++;
		}
	}

	// GEYSERS
	for (const auto& G : wb.geyser){
		bool flag = false;
		for (const auto& I : info){
			if (I.tile == G->getTilePosition()){
				flag = true;
				break;
			}
		}
		if (flag) continue;
		UnitType UT = UnitTypes::Terran_Refinery;
		TilePosition tile = G->getTilePosition();
		Unit SCV = G->getClosestUnit(IsOwned && IsGatheringMinerals && !IsCarryingSomething, 600);
		if (SCV){
			if (isWorking(SCV)) continue;
			Info BI(SCV, UT, tile);
			info.push_back(BI);
		}
	}

	// CENTRE // NOT DONE YET

	// LIST -> BUILD LIST
	Resource r;
	for (auto& L : list){
		r.mineral += L.first.mineralPrice() * L.second;
		r.gas += L.first.gasPrice() * L.second;
		if (L.second > howMany(L.first)){
			TilePosition tile;
			Unit SCV;
			tie(SCV, tile) = wb.BL.getBL();
			if (!tile || !SCV) continue;
			SCV = SCV->getClosestUnit(IsOwned && IsGatheringMinerals && !IsCarryingSomething, 400);
			if (SCV && tile){
				if (isWorking(SCV)) continue;
				Info BI(SCV, L.first, tile);
				info.push_back(BI);
			}
		}
	}


	return r;
}

int tWorker::howMany(UnitType UT){
	int res = 0;
	for (const auto& I : info){
		if (I.UT == UT) res++;
	}
	return res;
}

bool tWorker::isWorking(Unit u){
	for (auto& I : info){
		if (I.SCV == u) return true;
	}
	return false;
}

void tWorker::push(Unit u)
{ SCV.push_back(u); }
void tWorker::pop(Unit u)
{ SCV.erase(find(SCV.begin(), SCV.end(), u)); } // tentative error: if no such 'u' is inside SCV || about SCB, it should be regarded