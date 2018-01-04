// AUTHOR: Sungguk Cha, UNIST, Ulsan, South Korea
// e-Mail: navinad@naver.com
//
//
//


#include <BWAPI.h>
#include "ThebotModule.h"

using namespace BWAPI;
using namespace BWTA;
using namespace Filter;
using namespace std;


void ThebotModule::onStart()
{
	Broodwar->enableFlag(Flag::UserInput);
	Broodwar->setCommandOptimizationLevel(2);

	race = Broodwar->self()->getRace();

	BWTA::readMap();
	BWTA::analyze();

	//for (auto &base : BWTA::getBaseLocations()) manager.push(base->getTilePosition());
	//manager.start();
}
void ThebotModule::onFrame()
{
	refresh();
	//manager.refresh(debug);
}
void ThebotModule::onSendText(std::string text)
{
	if (text == "/debug"){
		Broodwar << "Debug mode on" << std::endl;
		Broodwar << Broodwar->enemy()->getRace().c_str() << endl;
	}
}
void ThebotModule::onReceiveText(BWAPI::Player player, std::string text)
{
	
}
void ThebotModule::onPlayerLeft(BWAPI::Player player)
{
	
}
void ThebotModule::onNukeDetect(BWAPI::Position target)
{

}
void ThebotModule::onUnitDiscover(BWAPI::Unit unit)
{
	if (IsEnemy(unit)){
		//if (IsBuilding(unit) && !IsRefinery(unit)) manager.push(make_tuple(unit, unit->getPosition()));
		//manager.push(unit);
	}
}
void ThebotModule::onUnitEvade(BWAPI::Unit unit)
{
}
void ThebotModule::onUnitShow(BWAPI::Unit unit)
{
}
void ThebotModule::onUnitHide(BWAPI::Unit unit)
{
}
void ThebotModule::onUnitCreate(BWAPI::Unit unit)
{
}
void ThebotModule::onUnitComplete(BWAPI::Unit unit)
{
	if (race == Races::Terran)
		terran.push(unit);
	/*
	else if (race == Races::Zerg)
	zerg.refresh();
	else if (race == Races::Protoss)
	protoss.refresh();
	*/
}
void ThebotModule::onUnitDestroy(BWAPI::Unit unit)
{
	if (race == Races::Terran)
		terran.pop(unit);
	/*
	else if (race == Races::Zerg)
	zerg.refresh();
	else if (race == Races::Protoss)
	protoss.refresh();
	*/
}
void ThebotModule::onUnitMorph(BWAPI::Unit unit)
{
	
}
void ThebotModule::onUnitRenegade(BWAPI::Unit unit)
{
}
void ThebotModule::onSaveGame(std::string gameName)
{
	
}
void ThebotModule::onEnd(bool isWinner)
{
	//manager.end(isWinner);
}

void ThebotModule::refresh(){
	if (race == Races::Terran)
		terran.refresh();
	/*
	else if (race == Races::Zerg)
		zerg.refresh();
	else if (race == Races::Protoss)
		protoss.refresh();
	*/
}