#include <iostream>
#include <BWAPI.h>
#include <string>
#include "tPlayManager.h"

using namespace BWAPI;
using namespace Filter;

Bus tPlaymanager::refresh(PMBus r){
	Broodwar->drawTextScreen(0, 30, "%d	%d", Broodwar->self()->minerals() - r.resource.mineral, Broodwar->self()->gas() - r.resource.gas);
	Bus b; b.busno = busno;
	map<UnitType, vector<Unit>> table;
	r.list.find(UnitTypes::Terran_Supply_Depot) != r.list.end() ? building_depot = r.list[UnitTypes::Terran_Supply_Depot] : building_depot = 0;
	map<UnitType, int> n, _n;
	map<UnitType, bool> idle;
	unsigned C = 0;
	for (auto& u : kinds){
		n[u] = 0;
		_n[u] = 0;
		idle[u] = false;
	}
	for (auto& u : r.list){
		if (_n.find(u.first) == _n.end())
			idle[u.first] = false;
		//n[u.first] = u.second;
		_n[u.first] = u.second;
	}
	for (auto& u : r.addons){
		if (_n.find(u.first) == _n.end())
			idle[u.first] = false;
		_n[u.first] = u.second;
	}
	for (auto& u : Broodwar->self()->getUnits()){
		UnitType UT = u->getType();
		if (IsBuilding(u)){
			idle.find(UT) != idle.end() ? 0 : idle[UT] = false;
			if (table.find(UT) != table.end())
				table[UT].push_back(u);
			else{
				vector<Unit> UV;
				UV.push_back(u);
				table[UT] = UV;
			}
			if (UT == UnitTypes::Terran_Supply_Depot){
				if (!u->isCompleted()) building_depot++;
			}
			if (u->isTraining()) C++;
			else if (u->isCompleted()) idle[UT] = true;
		}
		if (!u->isCompleted()) _n.find(UT) != _n.end() ? _n[UT]++ : _n[UT] = 0;
		else n.find(UT) != n.end() ? n[UT]++ : n[UT] = 0;
	}
	b.bb.table = table;
	r.C = C;
	r.number = n;
	r._number = _n;
	r.idle = idle;
	return test(b, r);
}

Bus tPlaymanager::test(Bus res, PMBus r){
	int C = r.C * 2 + 2;
	res.cb.gas2 = res.cb.gas = r.number[UnitTypes::Terran_Barracks] + r._number[UnitTypes::Terran_Barracks] > 0;
	// GATE CONDITION
	if (Broodwar->self()->minerals() <= r.resource.mineral) return res;

	// SCV
	if (r.number[UnitTypes::Terran_SCV] < r.wk && ok(r, UnitTypes::Terran_SCV)){
		res.bb.busno = ++busno;
		res.bb.UT = UnitTypes::Terran_SCV;
		return res;
	}

	// DEPOT
	if (Broodwar->self()->supplyTotal() + building_depot * 16 - Broodwar->self()->supplyUsed() <= C && Broodwar->self()->supplyTotal() + building_depot < 400 ){ // C = PRODUCING BUILDINGS * 2
		res.cb.busno = ++busno;
		res.cb.UT = UnitTypes::Terran_Supply_Depot;
		return res;
	}
	// BARRACK
	if (r.number[UnitTypes::Terran_Supply_Depot] > 0 && r.number[UnitTypes::Terran_Barracks] + r._number[UnitTypes::Terran_Barracks] == 0){
		res.cb.busno = ++busno;
		res.cb.UT = UnitTypes::Terran_Barracks;
		return res;
	}

	// FACTORY
	if (r.number[UnitTypes::Terran_Factory] + r._number[UnitTypes::Terran_Factory] < 2 && r.number[UnitTypes::Terran_Barracks] > 0){
		res.cb.busno = ++busno;
		res.cb.UT = UnitTypes::Terran_Factory;
		return res;
	}

	// MACHINE_SHOP
	if (r.number[UnitTypes::Terran_Machine_Shop] + r._number[UnitTypes::Terran_Machine_Shop] < (r.number[UnitTypes::Terran_Factory] + r._number[UnitTypes::Terran_Factory]) / 2 &&
		r.number[UnitTypes::Terran_Factory] > 0){
		res.bb.busno = ++busno;
		res.bb.UT = UnitTypes::Terran_Machine_Shop;
		return res;
	}

	///		SQUAD

	// Marine
	if (r.number[UnitTypes::Terran_Factory] == 0 && ok(r, UnitTypes::Terran_Marine)){
		res.bb.busno = ++busno;
		res.bb.UT = UnitTypes::Terran_Marine;
		return res;
	}

	// Tank
	if (ok(r, UnitTypes::Terran_Siege_Tank_Tank_Mode) && r.number[UnitTypes::Terran_Machine_Shop] > 0){
		res.bb.busno = ++busno;
		res.bb.UT = UnitTypes::Terran_Siege_Tank_Tank_Mode;
		return res;
	}

	// Vulture
	if (ok(r, UnitTypes::Terran_Vulture)){
		res.bb.busno = ++busno;
		res.bb.UT = UnitTypes::Terran_Vulture;
		return res;
	}

	///		RESEARCH

	// VULTURE
	// MINE & BOOST
	if (r.number[UnitTypes::Terran_Machine_Shop] > 0 && ok(r, TechTypes::Spider_Mines)){
		res.bb.busno = ++busno;
		res.bb.TT = TechTypes::Spider_Mines;
		return res;
	}
	else if (r.number[UnitTypes::Terran_Machine_Shop] > 0 && ok(r, UpgradeTypes::Ion_Thrusters)){
		res.bb.busno = ++busno;
		res.bb.UpT = UpgradeTypes::Ion_Thrusters;
		return res;
	}
	else if (r.number[UnitTypes::Terran_Machine_Shop] > 0 && ok(r, TechTypes::Tank_Siege_Mode)){
		res.bb.busno = ++busno;
		res.bb.TT = TechTypes::Tank_Siege_Mode;
		return res;
	}

	return res;
}

bool tPlaymanager::ok(PMBus r, UnitType UT){
	if (Broodwar->self()->minerals() >= r.resource.mineral + UT.mineralPrice() &&
		Broodwar->self()->gas() >= r.resource.gas + UT.gasPrice() &&
		r.idle[UT.whatBuilds().first]) return true;
	return false;
}

bool tPlaymanager::ok(PMBus r, TechType UT){
	if (Broodwar->self()->hasResearched(UT)) return false;
	if (Broodwar->self()->minerals() >= r.resource.mineral + UT.mineralPrice() &&
		Broodwar->self()->gas() >= r.resource.gas + UT.gasPrice() &&
		r.idle[UT.whatResearches()]) return true;
	return false;
}

bool tPlaymanager::ok(PMBus r, UpgradeType UT){
	if (Broodwar->self()->getUpgradeLevel(UT) == Broodwar->self()->getMaxUpgradeLevel(UT)) return false;
	if (Broodwar->self()->minerals() >= r.resource.mineral + UT.mineralPrice() &&
		Broodwar->self()->gas() >= r.resource.gas + UT.gasPrice() &&
		r.idle[UT.whatUpgrades()]) return true;
	return false;
}