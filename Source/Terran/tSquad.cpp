#include <iostream>
#include <BWAPI.h>
#include <BWTA.h>
#include "tSquad.h"

using namespace BWAPI;
using namespace Filter;
using namespace std;

float Emperor_Junyoung::damage_ratio(Unit u, Unit target){
	static map<UnitType, int> armor;
	static bool once = true;
	if (once){
		armor[UnitTypes::Terran_SCV] = armor[UnitTypes::Terran_Marine] = armor[UnitTypes::Terran_Firebat] = armor[UnitTypes::Terran_Medic] = armor[UnitTypes::Terran_Ghost] = armor[UnitTypes::Zerg_Zergling] = armor[UnitTypes::Zerg_Broodling] = armor[UnitTypes::Zerg_Mutalisk] = armor[UnitTypes::Zerg_Scourge] = armor[UnitTypes::Zerg_Drone] = armor[UnitTypes::Protoss_Probe] = armor[UnitTypes::Protoss_Zealot] = armor[UnitTypes::Protoss_High_Templar] = armor[UnitTypes::Protoss_Dark_Templar] = armor[UnitTypes::Protoss_Interceptor] = 0;
		armor[UnitTypes::Terran_Vulture] = armor[UnitTypes::Zerg_Hydralisk] = armor[UnitTypes::Zerg_Lurker] = armor[UnitTypes::Zerg_Defiler] = armor[UnitTypes::Zerg_Queen] = armor[UnitTypes::Protoss_Corsair] = 1;
		armor[UnitTypes::Terran_Siege_Tank_Siege_Mode] = armor[UnitTypes::Terran_Siege_Tank_Tank_Mode] = armor[UnitTypes::Terran_Goliath] = armor[UnitTypes::Terran_Wraith] = armor[UnitTypes::Terran_Science_Vessel] = armor[UnitTypes::Terran_Battlecruiser] = armor[UnitTypes::Terran_Valkyrie] = armor[UnitTypes::Zerg_Guardian] = armor[UnitTypes::Zerg_Devourer] = armor[UnitTypes::Zerg_Ultralisk] = armor[UnitTypes::Zerg_Overlord] = armor[UnitTypes::Protoss_Dragoon] = armor[UnitTypes::Protoss_Archon] = armor[UnitTypes::Protoss_Dark_Archon] = armor[UnitTypes::Protoss_Shuttle] = armor[UnitTypes::Protoss_Reaver] = armor[UnitTypes::Protoss_Scout] = armor[UnitTypes::Protoss_Carrier] = armor[UnitTypes::Protoss_Arbiter] = 2;
		once = false;
	}
	DamageType a;
	if (target->isFlying())
		 a = u->getType().airWeapon().damageType();
	else
		a = u->getType().groundWeapon().damageType();
	if (a == DamageTypes::Normal || armor.find(target->getType()) == armor.end()) return 1.0;
	else if (a == DamageTypes::Concussive){
		switch (armor[target->getType()]){
		case 0:
			return 1.0;
		case 1:
			return 0.5;
		case 2:
			return 0.25;
		}
	}
	else{
		switch (armor[target->getType()]){
		case 0:
			return 0.5;
		case 1:
			return 0.75;
		case 2:
			return 1.0;
		}
	}
}

void Emperor_Junyoung::battle(vector<Unit> myarmy, vector<Unit> earmy){
	map<Unit, int> hp;
	for (auto& u : earmy)
		hp[u] = u->getHitPoints() + u->getShields();
	for (auto& u : myarmy){
		if (u->getGroundWeaponCooldown() + u->getAirWeaponCooldown() > 0){
			// move forward or backward
			continue;
		}
		bool attack = false;
		Unit target;
		int min = 0;
		bool kill = false;
		double value = 0.0;
		for (auto& e : u->getUnitsInWeaponRange(u->getType().airWeapon())){
			if (!IsEnemy(e)) continue;
			if (hp[e] <= 0) continue;
			UnitType mt, et;
			mt = u->getType();
			et = e->getType();
			int HP = hp[e] - (mt.airWeapon().damageAmount() + mt.airWeapon().damageBonus()
				* Broodwar->self()->getUpgradeLevel(mt.airWeapon().upgradeType()) - (et.armor() + Broodwar->enemy()->getUpgradeLevel(et.armorUpgrade()))
				* mt.airWeapon().damageFactor() * damage_ratio(u, e));
			// KILL
			if (HP <= 0){
				double _value = e->getType().mineralPrice() + e->getType().gasPrice() * 1.5;
				if (!kill || value < _value){
					kill = true;
					value = _value;
					target = e;
					min = HP;
				}
				else if (value == _value && abs(min) > abs(HP)){
					target = e;
					min = HP;
				}
			}
			// Min target
			else if (!attack || min > HP){
				attack = true;
				min = HP;
				target = e;
			}
		}
		for (auto& e : u->getUnitsInWeaponRange(u->getType().groundWeapon())){
			if (!IsEnemy(e)) continue;
			if (hp[e] <= 0) continue;
			UnitType mt, et;
			mt = u->getType();
			et = e->getType();
			int HP = hp[e] - (mt.groundWeapon().damageAmount() + mt.groundWeapon().damageBonus()
				* Broodwar->self()->getUpgradeLevel(mt.groundWeapon().upgradeType()) - (et.armor() + Broodwar->enemy()->getUpgradeLevel(et.armorUpgrade()))
				* mt.groundWeapon().damageFactor() * damage_ratio(u, e));
			// KILL
			if (HP <= 0){
				double _value = e->getType().mineralPrice() + e->getType().gasPrice() * 1.5;
				if (!kill || value < _value){
					kill = true;
					value = _value;
					target = e;
				}
				else if (value == _value && abs(min) > abs(HP)){
					target = e;
					min = HP;
				}
			}
			// Min target
			else if (!attack || min > HP){
				attack = true;
				min = HP;
				target = e;
			}
		}
		if (target){
			u->attack(target);
			kill ? hp[target] = 0 : hp[target] = min;
			Broodwar->registerEvent([u, target](Game*)
			{
				Broodwar->drawLineMap(u->getPosition(), target->getPosition(), Colors::Red);
			}, nullptr, Broodwar->getLatencyFrames() + 1);
		}
		else{ // No target in range
			//Move forward or backward
		}
	}
}