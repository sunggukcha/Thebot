#include <iostream>
#include <BWAPI.h>
#include <string>
#include "tCenter.h"

using namespace BWAPI;
using namespace Filter;

void Centre::push(Unit u){
	vector<Unit>::iterator UI = find(centre.begin(), centre.end(), u);
	if (UI == centre.end()) centre.push_back(u);
}

void Centre::pop(Unit u){
	vector<Unit>::iterator UI = find(centre.begin(), centre.end(), u);
	if (UI == centre.end()) return;
	centre.erase(UI);
}