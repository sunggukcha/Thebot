#pragma once
#include <BWAPI.h>
#include <iostream>

using namespace BWAPI;
using namespace std;
using namespace Filter;

class Emperor_Junyoung{
private:
	map<Unit, int> hp;
	unsigned frame;
	enum armortype {small, medium, large};
	float damage_ratio(Unit, Unit);
public:
	Emperor_Junyoung(){ frame = 0; }
	bool isFight();
	TilePosition battle(vector<Unit> myarmy, vector<Unit> earmy, Position ave, unsigned short interval);
	bool fight(Unit, unsigned short the_number_of_the_UT, Position); // true if the unit is ordered
};

class tSquad{
private:
	// Target Positioning
	unsigned short interval;
	bool search;
	bool targ;
	vector<TilePosition> startlocations;
	vector<TilePosition> multilocations;
private:
	Emperor_Junyoung Junyoung;
	vector<Unit> army;
	vector<Unit> enemy;
	vector<TilePosition> targets;
	TilePosition target;
public:
	void start(){
		interval = 0;
		search = true;
		for (auto& base : Broodwar->getStartLocations())
			startlocations.push_back(base);
		for (auto &base : BWTA::getBaseLocations())
			multilocations.push_back(base->getTilePosition());
	}
	void refresh();
public:
	void discover(Unit u){ if (find(enemy.begin(), enemy.end(), u) == enemy.end()) enemy.push_back(u); }
	void discover(TilePosition tp){ if (find(targets.begin(), targets.end(), tp) == targets.end()) targets.push_back(tp); }
	void push(Unit u){
		army.push_back(u);
		if (u->getType().groundWeapon().damageCooldown() > 0 && u->getType().airWeapon().damageCooldown() > 0)
			interval += (u->getType().groundWeapon().damageCooldown() + u->getType().airWeapon().damageCooldown()) / 2;
		else if (u->getType().groundWeapon().damageCooldown() == 0 && u->getType().airWeapon().damageCooldown() > 0)
			interval += u->getType().airWeapon().damageCooldown();
		else if (u->getType().airWeapon().damageCooldown() == 0 && u->getType().groundWeapon().damageCooldown() > 0)
			interval += u->getType().groundWeapon().damageCooldown();
	}
	void pop(Unit u){
		if (IsOwned(u)){
			army.erase(find(army.begin(), army.end(), u));
			if (u->getType().groundWeapon().damageCooldown() > 0 && u->getType().airWeapon().damageCooldown() > 0)
				interval -= (u->getType().groundWeapon().damageCooldown() + u->getType().airWeapon().damageCooldown()) / 2;
			else if (u->getType().groundWeapon().damageCooldown() == 0 && u->getType().airWeapon().damageCooldown() > 0)
				interval -= u->getType().airWeapon().damageCooldown();
			else if (u->getType().airWeapon().damageCooldown() == 0 && u->getType().groundWeapon().damageCooldown() > 0)
				interval -= u->getType().groundWeapon().damageCooldown();
		}
		else{
			for (auto &e : enemy){
				if (u->getID() == e->getID()){
					enemy.erase(find(enemy.begin(), enemy.end(), e));
					return;
				}
			}
		}
	}
};