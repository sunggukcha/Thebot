#include <iostream>
#include <BWAPI.h>
#include <string>
#include "tBuilding.h"

using namespace BWAPI;
using namespace Filter;

void tBuilding::refresh(BuildBus bb){
	if (bb.busno > busno){
		if (bb.UpT != UpgradeTypes::None)
			uporder.push_back(bb.UpT);
		if (bb.UT != UnitTypes::None)
			uorder.push_back(bb.UT);
		if (bb.TT != TechTypes::None)
			rorder.push_back(bb.TT);
		busno = bb.busno;
	} // ONLY NEW PLAN SHOULD BE REQURESTED BY PLAYMANAGER
	train();
	upgrade();
	//must return reserved orders to playmanager 
}

void tBuilding::train(){
	vector<UnitType> torder;
	for (auto& ut : uorder){
		UnitType UT = ut.whatBuilds().first; // <UnitType, int>:: 0:cannot, 2:Archons, 1: others
		if (table.find(UT) != table.end()){ // if there are buildings that can produce
			bool made = false;
			for (auto& u : table[UT]){
				if (u->canTrain(ut)){
					u->train(ut);
					made = true;
					break;
				}
			}
			if (!made) torder.push_back(ut);
		}
	}
	uorder = torder;
}

void tBuilding::upgrade(){
	vector<UpgradeType> torder;
	for (auto& ut : uporder){
		UnitType UT = ut.whatUpgrades();
		if (table.find(UT) != table.end()){ // if there are buildings that can produce
			bool made = false;
			for (auto& u : table[UT]){
				if (u->canUpgrade(ut)){
					u->upgrade(ut);
					made = true;
					break;
				}
			}
			if (!made) torder.push_back(ut);
		}
	}
	uporder = torder;
}

void tBuilding::push(Unit u){
	UnitType UT = u->getType();
	if (table.find(UT) == table.end()){
		vector<Unit> neo;
		neo.push_back(u);
		table[UT] = neo;
	}
	else
		table[UT].push_back(u);		
}

void tBuilding::pop(Unit u){
	UnitType UT = u->getType();
	vector<Unit>::iterator UI = find(table[UT].begin(), table[UT].end(), u);
	if (UI != table[UT].end())
		table[UT].erase(UI);
}