#include "tBuildLocation.h"
#include <iostream>
#include <BWAPI.h>
#include <queue>
#include <BWTA.h>

using namespace BWAPI;
using namespace Filter;
using namespace std;

#define unitX UnitTypes::Terran_Supply_Depot.tileWidth() / 3
#define unitY UnitTypes::Terran_Supply_Depot.tileHeight() / 2
int units[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };

TilePosition tBuildLocation::getML(){
	int k = Broodwar->getFrameCount() % multilocations.size();
	vector<TilePosition>::iterator TI = multilocations.begin();
	for (int i = 0; i < k; i++, TI++);
	return *TI;
}

bool tBuildLocation::ok(TilePosition T, UnitType UT = UnitTypes::Zerg_Defiler_Mound){
	vector<TilePosition>::iterator TI;
	for (TI = multilocations.begin(); TI != multilocations.end(); TI++){
		TilePosition P1 = *TI;
		TilePosition P2 = P1 + UnitTypes::Zerg_Defiler_Mound.tileSize();
		if (P1.x - UT.tileWidth() + unitX <= T.x && T.x <= P2.x - unitX &&
			P1.y - UT.tileHeight() + unitY <= T.y && T.y <= P2.y - unitY) return false;
	}
	queue<tuple<TilePosition, Unit> > tTP = tilepositions;
	while (!tTP.empty()){
		tuple<TilePosition, Unit> tt = tTP.front();
		tTP.pop();
		TilePosition ttt = get<0>(tt);
		if (ttt == T) return false;
	}
	return true;
}

bool tBuildLocation::isML(TilePosition T){
	for (const auto& ML : multilocations){
		if (ML == T) return true;
	}
	return false;
}

/*functions for tBuildLocation*/
void tBuildLocation::Update(Unit center){ // Updating Bulidable Locations with BFS
	int N = tilepositions.size();
	for (int i = 0; i < N; i++){ // filter inappropriate locations
		tuple<TilePosition, Unit> tupe = tilepositions.front();
		TilePosition tile;
		Unit c;
		tie(tile, c) = tupe;
		tilepositions.pop();
		if (!Broodwar->canBuildHere(tile, UnitTypes::Zerg_Defiler_Mound) || !ok(tile) || !c) continue;
		if (!c->exists()) continue;
		tilepositions.push(tupe);
		Position a, b;
		a = (Position)tile;
		b = (Position)(tile + UnitTypes::Zerg_Defiler_Mound.tileSize());
		Broodwar->registerEvent([a, b](Game*)
		{
			Broodwar->drawBoxMap(a, b, Colors::Blue);
		}, nullptr, Broodwar->getLatencyFrames() + 1);
	}
	if (tilepositions.size() == 0){
		TilePosition tile;
		if (center) tile = Broodwar->getBuildLocation(UnitTypes::Zerg_Spawning_Pool, center->getTilePosition(), 10);
		if (tile){
			tuple<TilePosition, Unit> tupe = make_tuple(tile, center) ;
			tilepositions.push(tupe);
		}
	}
	else if(tilepositions.size() < 5){
		N = tilepositions.size();
		for (int i = 0; i < N; i++){
			tuple<TilePosition, Unit> tupe = tilepositions.front();
			TilePosition tile;
			Unit c;
			tie(tile, c) = tupe;
			tilepositions.pop();
			tilepositions.push(tupe);
			for (int j = 0; j < 4; j++){
				TilePosition A;
				A.x = tile.x + units[j][0] * 4 * unitX;
				A.y = tile.y + units[j][1] * 3 * unitY;
				if (Broodwar->canBuildHere(A, UnitTypes::Zerg_Defiler_Mound) && ok(A)) tilepositions.push(make_tuple(A, c));
			}
		}
		if (N == tilepositions.size()){
			tuple<TilePosition, Unit> tupe;
			TilePosition tile;
			if (center) tile = Broodwar->getBuildLocation(UnitTypes::Zerg_Defiler_Mound, center->getTilePosition(), 10);
			tupe = make_tuple(tile, center);
			if (tile) tilepositions.push(tupe);
		}
	}
}
/*
bool tBuildLocation::Coercion_Update(Unit center){
	int N = tilepositions.size();
	for (int i = 0; i < N; i++){ // filter inappropriate locations
		tuple<TilePosition, Unit> tupe = tilepositions.front();
		TilePosition tile;
		Unit c;
		tie(tile, c) = tupe;
		tilepositions.pop();
		if (!Broodwar->canBuildHere(tile, UnitTypes::Zerg_Defiler_Mound) || !ok(tile) || !c) continue;
		if (!c->exists()) continue;
		tilepositions.push(tupe);
		Position a, b;
		a = (Position)tile;
		b = (Position)(tile + UnitTypes::Zerg_Defiler_Mound.tileSize());
		Broodwar->registerEvent([a, b](Game*)
		{
			Broodwar->drawBoxMap(a, b, Colors::Blue);
		}, nullptr, Broodwar->getLatencyFrames() + 1);
	}
	if (tilepositions.size() == 0){
		TilePosition tile;
		if (center) tile = Broodwar->getBuildLocation(UnitTypes::Zerg_Defiler_Mound, center->getTilePosition(), 10);
		if (tile){
			tuple<TilePosition, Unit> tupe = make_tuple(tile, center);
			tilepositions.push(tupe);
			return true;
		}
	}
	queue<tuple<TilePosition, Unit> > tt;
	for (int i = 0; i < N; i++){
		tuple<TilePosition, Unit> tupe = tt.front();
		tt.pop();
		TilePosition t = get<0>(tupe);
		Unit u = get<1>(tupe); 
	}

	bool flag = false;

	if (tilepositions.size() < 5){
		N = tilepositions.size();
		for (int i = 0; i < N; i++){
			tuple<TilePosition, Unit> tupe = tilepositions.front();
			TilePosition tile;
			Unit c;
			tie(tile, c) = tupe;
			tilepositions.pop();
			tilepositions.push(tupe);
			for (int j = 0; j < 4; j++){
				TilePosition A;
				A.x = tile.x + units[j][0] * 4 * unitX;
				A.y = tile.y + units[j][1] * 3 * unitY;
				if (Broodwar->canBuildHere(A, UnitTypes::Zerg_Defiler_Mound) && ok(A)) tilepositions.push(make_tuple(A, c));
			}
		}
		if (N == tilepositions.size()){
			tuple<TilePosition, Unit> tupe;
			TilePosition tile;
			if (center) tile = Broodwar->getBuildLocation(UnitTypes::Zerg_Defiler_Mound, center->getTilePosition(), 10);
			tupe = make_tuple(tile, center);
			if (tile) tilepositions.push(tupe);
		}
	}
	return false;
}
*/

tuple<TilePosition, Unit> tBuildLocation::getBL(){
	if (tilepositions.size() == 0) return make_tuple(Broodwar->self()->getStartLocation(), nullptr);
	tuple<TilePosition, Unit> tupe = tilepositions.front();
	tilepositions.pop();
	return tupe;
}