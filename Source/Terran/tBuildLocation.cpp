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

bool tBuildLocation::ok(TilePosition T, UnitType UT = UnitTypes::Terran_Barracks){
	vector<TilePosition>::iterator TI;
	for (TI = multilocations.begin(); TI != multilocations.end(); TI++){
		TilePosition P1 = *TI;
		TilePosition P2 = P1 + UnitTypes::Terran_Barracks.tileSize();
		if (P1.x - UT.tileWidth() + unitX <= T.x && T.x <= P2.x - unitX &&
			P1.y - UT.tileHeight() + unitY <= T.y && T.y <= P2.y - unitY) return false;
	}
	queue<tuple<Unit, TilePosition> > tTP = buildlocations;
	while (!tTP.empty()){
		tuple<Unit, TilePosition> tt = tTP.front();
		tTP.pop();
		TilePosition ttt = get<1>(tt);
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
	int N = buildlocations.size();
	for (int i = 0; i < N; i++){ // filter inappropriate locations
		tuple<Unit, TilePosition> tupe = buildlocations.front();
		TilePosition tile;
		Unit c;
		tie(c, tile) = tupe;
		buildlocations.pop();
		if (!Broodwar->canBuildHere(tile, UnitTypes::Terran_Barracks) || !ok(tile) || !c) continue;
		if (!c->exists()) continue;
		buildlocations.push(tupe);
		Position a, b;
		a = (Position)tile;
		b = (Position)(tile + UnitTypes::Terran_Barracks.tileSize());
		Broodwar->registerEvent([a, b](Game*)
		{
			Broodwar->drawBoxMap(a, b, Colors::Blue);
		}, nullptr, Broodwar->getLatencyFrames() + 1);
	}
	if (buildlocations.size() == 0){
		TilePosition tile;
		if (center) tile = Broodwar->getBuildLocation(UnitTypes::Terran_Barracks, center->getTilePosition(), 10);
		if (tile){
			tuple<Unit, TilePosition> tupe = make_tuple(center, tile);
			buildlocations.push(tupe);

			Position a, b;
			a = (Position)tile;
			b = (Position)(tile + UnitTypes::Terran_Barracks.tileSize());
			Broodwar->registerEvent([a, b](Game*)
			{
				Broodwar->drawBoxMap(a, b, Colors::Red);
			}, nullptr, Broodwar->getLatencyFrames() + 1);
		}
	}
	else if(buildlocations.size() < 5){
		N = buildlocations.size();
		for (int i = 0; i < N; i++){
			tuple<Unit, TilePosition> tupe = buildlocations.front();
			TilePosition tile;
			Unit c;
			tie(c, tile) = tupe;
			buildlocations.pop();
			buildlocations.push(tupe);
			for (int j = 0; j < 4; j++){
				TilePosition A;
				A.x = tile.x + units[j][0] * 6 * unitX;
				A.y = tile.y + units[j][1] * 4 * unitY;
				if (Broodwar->canBuildHere(A, UnitTypes::Terran_Barracks) && ok(A)){
					buildlocations.push(make_tuple(c, A));

					Position a, b;
					a = (Position)tile;
					b = (Position)(tile + UnitTypes::Terran_Barracks.tileSize());
					Broodwar->registerEvent([a, b](Game*)
					{
						Broodwar->drawBoxMap(a, b, Colors::Red);
					}, nullptr, Broodwar->getLatencyFrames() + 1);
				}
			}
		}
		if (N == buildlocations.size()){
			tuple<Unit, TilePosition> tupe;
			TilePosition tile;
			if (center) tile = Broodwar->getBuildLocation(UnitTypes::Terran_Barracks, center->getTilePosition(), 10);
			tupe = make_tuple(center, tile);
			if (tile){
				buildlocations.push(tupe);

				Position a, b;
				a = (Position)tile;
				b = (Position)(tile + UnitTypes::Terran_Barracks.tileSize());
				Broodwar->registerEvent([a, b](Game*)
				{
					Broodwar->drawBoxMap(a, b, Colors::Red);
				}, nullptr, Broodwar->getLatencyFrames() + 1);
			}
		}
	}
}
/*
bool tBuildLocation::Coercion_Update(Unit center){
	int N = buildlocations.size();
	for (int i = 0; i < N; i++){ // filter inappropriate locations
		tuple<Unit, TilePosition> tupe = buildlocations.front();
		TilePosition tile;
		Unit c;
		tie(tile, c) = tupe;
		buildlocations.pop();
		if (!Broodwar->canBuildHere(tile, UnitTypes::Zerg_Defiler_Mound) || !ok(tile) || !c) continue;
		if (!c->exists()) continue;
		buildlocations.push(tupe);
		Position a, b;
		a = (Position)tile;
		b = (Position)(tile + UnitTypes::Zerg_Defiler_Mound.tileSize());
		Broodwar->registerEvent([a, b](Game*)
		{
			Broodwar->drawBoxMap(a, b, Colors::Blue);
		}, nullptr, Broodwar->getLatencyFrames() + 1);
	}
	if (buildlocations.size() == 0){
		TilePosition tile;
		if (center) tile = Broodwar->getBuildLocation(UnitTypes::Zerg_Defiler_Mound, center->getTilePosition(), 10);
		if (tile){
			tuple<Unit, TilePosition> tupe = make_tuple(tile, center);
			buildlocations.push(tupe);
			return true;
		}
	}
	queue<tuple<Unit, TilePosition> > tt;
	for (int i = 0; i < N; i++){
		tuple<Unit, TilePosition> tupe = tt.front();
		tt.pop();
		TilePosition t = get<0>(tupe);
		Unit u = get<1>(tupe); 
	}

	bool flag = false;

	if (buildlocations.size() < 5){
		N = buildlocations.size();
		for (int i = 0; i < N; i++){
			tuple<Unit, TilePosition> tupe = buildlocations.front();
			TilePosition tile;
			Unit c;
			tie(tile, c) = tupe;
			buildlocations.pop();
			buildlocations.push(tupe);
			for (int j = 0; j < 4; j++){
				TilePosition A;
				A.x = tile.x + units[j][0] * 4 * unitX;
				A.y = tile.y + units[j][1] * 3 * unitY;
				if (Broodwar->canBuildHere(A, UnitTypes::Zerg_Defiler_Mound) && ok(A)) buildlocations.push(make_tuple(A, c));
			}
		}
		if (N == buildlocations.size()){
			tuple<Unit, TilePosition> tupe;
			TilePosition tile;
			if (center) tile = Broodwar->getBuildLocation(UnitTypes::Zerg_Defiler_Mound, center->getTilePosition(), 10);
			tupe = make_tuple(tile, center);
			if (tile) buildlocations.push(tupe);
		}
	}
	return false;
}
*/

tuple<Unit, TilePosition> tBuildLocation::getBL(){
	if (buildlocations.size() == 0) return make_tuple(nullptr, Broodwar->self()->getStartLocation());
	tuple<Unit, TilePosition> tupe = buildlocations.front();
	buildlocations.pop();
	return tupe;
}