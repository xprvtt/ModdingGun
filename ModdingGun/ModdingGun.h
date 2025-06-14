#pragma once

#include "Core.h"

#include "CharacteristicGun.h"

#include "GunStats.h"

#include "Thread.h"

bool RunSingleAssembly(
    size_t it_index,
    CharacteristicGun CurrentCharacteristicGun,
    vector<int> method,
    vector<int> CharacteristicsInitial,
    vector<int> CharacteristicsModded,
    bool Initial_OFF,
    double TOOL,
    double KIT,
    double SKILL

);

CharacteristicGun RunSingleAssembly(
    int it_index,
    CharacteristicGun CurrentCharacteristicGun,
    vector<int> method,
    vector<int> CharacteristicsInitial,
    vector<int> CharacteristicsModded,
    bool Initial_OFF,
    double TOOL,
    double KIT,
    double SKILL
);
