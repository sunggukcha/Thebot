#pragma once
#include <BWAPI.h>
#include <iostream>

using namespace BWAPI;
using namespace std;
using namespace Filter;

class Emperor_Junyoung{
private:
	enum armortype {small, medium, large};
	float damage_ratio(Unit, Unit);
public:
	bool isFight();
	void battle(vector<Unit> myarmy, vector<Unit> earmy);
};

class tSquad{
private:
	// Target Positioning
	bool search;
	vector<TilePosition> startlocations;
private:
	Emperor_Junyoung Junyoung;
	vector<Unit> army;
	vector<Unit> enemy;
	vector<TilePosition> targets;
	TilePosition target;
public:
	void start(){
		search = true;
		for (auto& base : Broodwar->getStartLocations())
			startlocations.push_back(base);
	}
	void refresh();
public:
	void discover(Unit u){ if (find(enemy.begin(), enemy.end(), u) == enemy.end()) enemy.push_back(u); }
	void discover(TilePosition tp){ if (find(targets.begin(), targets.end(), tp) == targets.end()) targets.push_back(tp); }
	void push(Unit u){ army.push_back(u); }
	void pop(Unit u){
		if (IsOwned(u)) army.erase(find(army.begin(), army.end(), u));
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