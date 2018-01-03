#include <iostream>
#include <BWAPI.h>
#include "tWorker.h"

using namespace BWAPI;
using namespace Filter;
using namespace std;

void tWorker::refresh(){
	mine();
}

void tWorker::mine(){
	for (auto& u : SCV){
		if (u->isIdle()){ // IDLE -> GATHER CLOSEST MINERAL
			Unit M = u->getClosestUnit(IsMineralField);
			if (M){
				u->gather(M);
			}
		}
	}
}

void tWorker::push(Unit u)
{ SCV.push_back(u); }
void tWorker::pop(Unit u)
{ SCV.erase(find(SCV.begin(), SCV.end(), u)); } // tentative error: if no such 'u' is inside SCV || about SCB, it should be regarded