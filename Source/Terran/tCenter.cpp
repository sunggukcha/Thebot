#include <iostream>
#include <BWAPI.h>
#include <string>
#include "tCenter.h"

using namespace BWAPI;
using namespace Filter;

WorkerBus Centre::refresh(CentreBus cb){
	WorkerBus* wb = new WorkerBus(cb);
	vector<Unit> geyser;
	if (centre.size() == 0) return *wb;
	for (auto& u : centre){
		bool defense = false;	// NOT IMPLEMENTED
		short M = 0;			// MINERAL#
		short S = 0;			// SCV@MINERAL
		short G = 0;			// SCV@GAS
		for (const auto& uu : u->getUnitsInRadius(400)){
			if (IsEnemy(uu)){
				if (uu->getType().airWeapon().damageAmount() > 0 || uu->getType().groundWeapon().damageAmount() > 0 || uu->getEnergy() > 0) defense = true;
			}
			if (uu->getType().isMineralField()) M++;
			if (uu->getType().isWorker() && IsOwned(uu) && uu->isGatheringMinerals()) S++;
			if (uu->getType().isWorker() && IsOwned(uu) && uu->isGatheringGas()) G++;
		}
		if (S > 6 && wb->gas){
			Unit geyser = u->getClosestUnit(GetType == UnitTypes::Resource_Vespene_Geyser && IsNeutral, 300);
			if (geyser){
				Broodwar->drawCircleMap(geyser->getPosition(), 5, Colors::Red, true);
				wb->geyser.push_back(geyser);
			}
		}
		Unit geyser = u->getClosestUnit(IsOwned && !IsBeingConstructed && GetType == UnitTypes::Terran_Refinery, 300);
		if (geyser){
			if (G > 3 || wb->gas2 == false){
				Unit come = u->getClosestUnit(GetType == u->getType().getRace().getWorker() &&
					IsGatheringGas && !IsCarryingSomething);
				if (come) come->stop();
			}
			else if (G < 2 && S > 6 && wb->gas2 == true){
				Unit ggg = u->getClosestUnit(GetType == u->getType().getRace().getWorker() &&
					(IsGatheringMinerals) && IsOwned && !IsCarryingSomething);
				if (ggg){
					if (ggg->canGather(geyser)){
						ggg->gather(geyser);
					}
				}
			}
		}
		/* SATISFACTORY
		if (M * 1.5 <= D){
			hatchery[u] = HatchData(true);
			Broodwar->drawCircleMap(u->getPosition(), 5, Colors::Green, hatchery[u].satisfied);
		}
		else{
			Broodwar->drawCircleMap(u->getPosition(), 5, Colors::Green);
			hatchery[u] = HatchData(false);
		}
		*/
	}
	buildlocation.Update(rcentre());
	wb->BL = buildlocation;
	return *wb;
}

Unit Centre::rcentre(){
	if (centre.size() == 0) return NULL;
	int N = Broodwar->getFrameCount() % centre.size();
	int K = 0;
	vector<Unit>::iterator CI;
	for (CI = centre.begin(); CI != centre.end(); ++CI){
		if (K == N) return *CI;
		K++;
	}
	return NULL;
}

void Centre::push(Unit u){
	vector<Unit>::iterator UI = find(centre.begin(), centre.end(), u);
	if (UI == centre.end()) centre.push_back(u);
}

void Centre::pop(Unit u){
	vector<Unit>::iterator UI = find(centre.begin(), centre.end(), u);
	if (UI == centre.end()) return;
	centre.erase(UI);
}