#pragma once
#include <BWAPI.h>
#include <iostream>
#include "tBus.h"

using namespace BWAPI;
using namespace std;

class tPlaymanager{
private:
	map<UnitType, int> all;
	unsigned busno;
private: // MAGICS //
	Bus test(Resource);
public:
	tPlaymanager(){ busno = 0; }
	Bus refresh(Resource);
};