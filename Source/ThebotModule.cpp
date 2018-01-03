#include "ThebotModule.h"
#include <BWAPI.h>

using namespace BWAPI;
using namespace BWTA;
using namespace Filter;
using namespace std;

bool debug = false;

void ThebotModule::onStart()
{
	Broodwar->enableFlag(Flag::UserInput);
	Broodwar->setCommandOptimizationLevel(2);

	BWTA::readMap();
	BWTA::analyze();

	//for (auto &base : BWTA::getBaseLocations()) manager.push(base->getTilePosition());
	//manager.start();
}
void ThebotModule::onFrame()
{
	//manager.refresh(debug);
}
void ThebotModule::onSendText(std::string text)
{
	if (text == "/debug"){
		debug = true;
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
	//manager.push(unit);
}
void ThebotModule::onUnitDestroy(BWAPI::Unit unit)
{
	//manager.pop(unit);
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
