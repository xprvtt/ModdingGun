#pragma once


#include "Core.h"

// используем флоат для экономии времени
extern vector<float> Average10000_RANDOM_GEN;
extern vector<float> Average10000;

extern mutex mtx;

extern int threadCount;

void add_results(const vector<float>& otherVec, int BOX);