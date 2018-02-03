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
	return 1.0;
}

TilePosition Emperor_Junyoung::battle(vector<Unit> myarmy, vector<Unit> earmy, Position ave, unsigned short interval){
	TilePosition res = (TilePosition)ave;

	if (myarmy.size() == 0) return res;
	if (frame + interval / myarmy.size() < Broodwar->getFrameCount()){
		frame = Broodwar->getFrameCount();
		for (auto& u : earmy)
			hp[u] = u->getHitPoints() + u->getShields();
	}
	for (auto& u : myarmy){
		Unit eb = u->getClosestUnit(IsEnemy && IsBuilding, u->getType().sightRange());
		if (u->getDistance(ave) < 10){
			if (eb) res = eb->getTilePosition();
			else {
				Unit eb2 = u->getClosestUnit(IsEnemy && IsBuilding);
				if (eb2) res = eb2->getTilePosition();
				else res = TilePositions::None;
			}
		}
		else if (eb){
			res = eb->getTilePosition();
		}
		if (fight(u, interval, ave)) continue;
		if (u->getGroundWeaponCooldown() + u->getAirWeaponCooldown() > 0){
			// move forward or backward
			// if weapon in cooldown / 2, position in maximum range
			// otherwise, evade. SEE HOW MARINE, VULTURE, OR THAT KINDS WORKS
			if (u->getGroundWeaponCooldown() >= u->getType().groundWeapon().damageCooldown() / 2){
				Unit e = u->getClosestUnit(IsEnemy && !IsFlying);
				if (e){
					Position ppp;
					ppp.x = 7 * u->getPosition().x - 6 * e->getPosition().x;
					ppp.y = 7 * u->getPosition().y - 6 * e->getPosition().y;
					ppp.makeValid();
					u->attack(ppp);
				}
			}
			else if (u->getAirWeaponCooldown() >= u->getType().airWeapon().damageCooldown() / 2){
				Unit e = u->getClosestUnit(IsEnemy && IsFlying);
				if (e){
					Position ppp;
					ppp.x = 7 * u->getPosition().x - 6 * e->getPosition().x;
					ppp.y = 7 * u->getPosition().y - 6 * e->getPosition().y;
					ppp.makeValid();
					u->attack(ppp);
				}
			}
			continue;
		}
		bool attack = false;
		Unit target = NULL;
		int min = 0;
		bool kill = false;
		double value = 0.0;
		for (auto& e : u->getUnitsInRadius(u->getType().airWeapon().maxRange())){
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
		for (auto& e : u->getUnitsInRadius(u->getType().sightRange())){
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
			u->attack(ave);
		}
	}
	return res;
}

bool Emperor_Junyoung::fight(Unit u, unsigned short N, Position ave){
	UnitType UT = u->getType();
	if (UT == UnitTypes::Terran_Vulture){
		// if (u->getLastCommand().getType() == UnitCommandTypes::Use_Tech && u->getLastCommandFrame() + 100 > Broodwar->getFrameCount()) return true;

//		Unit e = u->getClosestUnit(IsEnemy && !IsFlying, UT.sightRange());
		if (u->getSpiderMineCount() > 0 && Broodwar->self()->hasResearched(TechTypes::Spider_Mines)){
			if (u->getLastCommandFrame() + 20 > Broodwar->getFrameCount()) return true;
			Unit mine = u->getClosestUnit(IsOwned && GetType == UnitTypes::Terran_Vulture_Spider_Mine, 100);
			Unit enemy = u->getClosestUnit(IsEnemy, UT.sightRange());
			if ((mine || enemy)){
				if (mine){
					if (u->getDistance(mine) < 50) return false;
				}
				Position p = (Position)u->getTilePosition();
				Position _p = u->getPosition();
				u->useTech(TechTypes::Spider_Mines, _p);
				Broodwar->registerEvent([u](Game*)
				{
					Broodwar->drawCircleMap(u->getPosition(), 3, Colors::Red, true);
				}, nullptr, Broodwar->getLatencyFrames() + 1);
				return true;
			}
		}
		//return true;
	}
	else if (UT == UnitTypes::Terran_Siege_Tank_Tank_Mode){
		Unit e = u->getClosestUnit(IsEnemy && !IsFlying, 196);
		if (u->canSiege() && e){
			u->siege();
			return true;
		}
	}
	else if (UT == UnitTypes::Terran_Siege_Tank_Siege_Mode){
		Unit e = u->getClosestUnit(IsEnemy && !IsFlying, 196);
		if (!e && u->getGroundWeaponCooldown() == 0){
			u->unsiege();
		}
	}
	return false;
}

void tSquad::refresh(){
	// TARGET POSITIONING
	vector<TilePosition> ts;
	for (auto& base : startlocations){
		if (Broodwar->isExplored(base)) continue;
		ts.push_back(base);
	}
	startlocations = ts;
	bool yes = targets.empty();
	if (yes){
		if (!startlocations.empty()){
			targ = true;
			target = startlocations.front();
		}
		else{
			targ = false;
			unsigned k = Broodwar->getFrameCount() % multilocations.size();
			int i = 0;
			for (auto& base : multilocations){
				if (i++ != k) continue;
				target = base;
				break;
			}
		}
	}
	Position targe = (Position) target;
	bool tarG = targ;
	Broodwar->registerEvent([yes, tarG, targe](Game*)
	{
		if (yes){
			Broodwar->drawCircleMap(targe, 5, Colors::Green, tarG);
			Broodwar->drawCircleMap(targe, 150, Colors::Green);
		}
		else{
			Broodwar->drawCircleMap(targe, 5, Colors::Red, tarG);
			Broodwar->drawCircleMap(targe, 150, Colors::Red);
		}
	}, nullptr, Broodwar->getLatencyFrames());

	target = Junyoung.battle(army, enemy, (Position) target, interval);
	if (target == TilePositions::None)
		search = true;
	else
		search = false;
}