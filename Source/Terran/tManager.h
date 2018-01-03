#pragma once
#include <BWAPI.h>
#include <iostream>
#include "tWorker.h"

using namespace BWAPI;
using namespace std;

class tManager{
private:
	tWorker worker;
public:
	void refresh();
	void push(Unit);
	void pop(Unit);
};