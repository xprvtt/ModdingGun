#include "Core.h"

double getRandomDouble(double range)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> distrib(0.0, range);
    return distrib(gen);
}