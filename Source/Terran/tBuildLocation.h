#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <iostream>
#include <queue>

using namespace BWAPI;
using namespace std;
using namespace BWTA;

class tBuildLocation{
private:
	int playerno;
	vector<tuple<TilePosition, bool>> startlocations;
	vector<TilePosition> multilocations;
	queue<tuple<Unit, TilePosition>> buildlocations;
public:
	void start(){ // READING MAP SHOULD BE GUARANTEED SO RATHER THAN USING CONSTRUCTOR, IT IS USING FUNCTION
		for (auto &base : BWTA::getBaseLocations())
			multilocations.push_back(base->getTilePosition());
		for (auto& base : Broodwar->getStartLocations())
			startlocations.push_back(make_tuple(base, false));
	}
	TilePosition getML(); // get Random MultiLocation
	bool ok(TilePosition, UnitType); // whether is it okay to Build here
	bool isML(TilePosition);
	/*functions for buildlocations*/
	void Update(Unit);
	tuple<Unit, TilePosition> getBL(); // get Build Location
};