#pragma once

#include "Core.h"

/// CLASS 

tuple<int, int, int, int, int, int, int> ParseInt7Block(const string& block);

void ParseLine3(const string& line, vector<tuple<int, int, tuple<int, int, int, int, int, int, int>>>& vec);
