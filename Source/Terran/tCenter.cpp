#include <iostream>
#include <BWAPI.h>
#include <string>
#include "tCenter.h"

using namespace BWAPI;
using namespace Filter;

WorkerBus Centre::refresh(CentreBus cb){
	WorkerBus* wb = new WorkerBus(cb);
	vector<Unit> geyser;
	if (centre.size() == 0) return *wb;
	for (auto& u : centre){
		bool defense = false;	// NOT IMPLEMENTED
		short M = 0;			// MINERAL#
		short S = 0;			// SCV@MINERAL
		short G = 0;			// SCV@GAS

	}
	buildlocation.Update(rcentre());
	wb->BL = buildlocation;
	return *wb;
}

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