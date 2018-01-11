// Heo Junyoung (1995~)
// BATTLE ALGORITHM INVENTOR

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
	Emperor_Junyoung Junyoung;
	vector<Unit> army;
public:
	void push(Unit u){ army.push_back(u); }
	void pop(Unit u){ army.erase(find(army.begin(), army.end(), u)); }
};