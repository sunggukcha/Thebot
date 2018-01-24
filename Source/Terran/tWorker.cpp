#include <iostream>
#include <BWAPI.h>
#include "tWorker.h"

using namespace BWAPI;
using namespace Filter;
using namespace std;

PMBus tWorker::refresh(WorkerBus wb){
	mine();
	return build(wb);
}

void tWorker::mine(){
	for (auto& u : SCV){
		if (u->isIdle() && !isWorking(u)){ // IDLE -> GATHER CLOSEST MINERAL
			Unit centre = u->getClosestUnit(IsOwned && GetType == UnitTypes::Terran_Command_Center);
			Unit M = NULL;
			if (centre){
				int min = 0;
				for (auto& m : u->getUnitsInRadius(400)){
					if (!IsMineralField(m)) continue;
					if (min == 0 || min < m->getResources()){
						min = m->getResources();
						M = m;
					}
				}
			}
			else M = u->getClosestUnit(IsMineralField);

			if (M != NULL)
				u->gather(M);
		}
	}
}

// BUILDING PROCESS
// PLAY MANAGER CANNOT ORDER WHAT IMPOSSIBLE TO BUILD
// REFRESH(CHECK IF COMPLETED) -> ADD BUS ITEM -> GEYSERS -> CENTRE -> ACTUAL BUILD

PMBus tWorker::build(WorkerBus wb){
	PMBus r;
	vector<Info> tinfo;
	int _i = 0;
	int __i = 0;
	// REQUIRED SCV from CENTRE TO PM
	r.wk = wb.worker;
	// REFRESH & ORDER TO BUILD
	for (auto& I : info){
		// 2016 SUNGGUKCHA VERSION
		if (!I.SCV->exists()) continue; // SCV DEAD

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
		Broodwar->drawTextScreen(100, _i, "%s", ut.c_str());
		_i += 15;

		if (I.POP >= 50){
			if (I.UT != UnitTypes::Terran_Refinery) list[I.UT]--;
			continue;
		}
		else if (I.SCV->isConstructing()) I.POP++;
		else if (!Broodwar->canBuildHere(I.tile, I.UT, I.SCV)) continue;
		else{
			I.POP = 0;
			if (Broodwar->self()->minerals() >= 0.8 * I.UT.mineralPrice() && Broodwar->self()->gas() >= 0.8 * I.UT.gasPrice()){ // ADD ECONOMY RELEVANT CONDITIONS
				Position p = (Position)(I.tile + I.UT.tileSize() / 2);
				if (I.SCV->getDistance(p) > 10 && I.UT != UnitTypes::Terran_Refinery) I.SCV->move(p);
				else I.SCV->build(I.UT, I.tile);
			}
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
		if (!IsNeutral(G)) continue;
		bool flag = false;
		for (const auto& I : info){
			if (I.tile == G->getTilePosition()){
				flag = true;
				break;
			}
		}
		r.resource.mineral += 100;
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
	for (auto& L : list){
		Broodwar->drawTextScreen(200, __i, "%s %d", L.first.c_str(), L.second);
		__i += 15;
		r.resource.mineral += L.first.mineralPrice() * L.second;
		r.resource.gas += L.first.gasPrice() * L.second;
		if (L.second > howMany(L.first) || L.first == UnitTypes::Terran_Refinery){
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

	r.list = list;
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