#include <iostream>
#include <BWAPI.h>
#include <string>
#include "tCenter.h"

using namespace BWAPI;
using namespace Filter;

Unit Centre::rcentre(){
	if (centre.size() == 0) return NULL;
	int N = Broodwar->getFrameCount() % centre.size();
	int K = 0;
	vector<Unit>::iterator CI;
	for (CI = centre.begin(); CI != centre.end(); ++CI){
		if (K == N) return *CI;
		K++;
	}
	return NULL;
}

void Centre::push(Unit u){
	vector<Unit>::iterator UI = find(centre.begin(), centre.end(), u);
	if (UI == centre.end()) centre.push_back(u);
}

void Centre::pop(Unit u){
	vector<Unit>::iterator UI = find(centre.begin(), centre.end(), u);
	if (UI == centre.end()) return;
	centre.erase(UI);
}