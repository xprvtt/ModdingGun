#include "Thread.h"


vector<float> Average10000_RANDOM_GEN;

vector<float> Average10000;

//мью
mutex mtx;

//потоки
int threadCount = thread::hardware_concurrency();
// int threadCount = 1;



void add_results(const vector<float>& otherVec, int BOX)
{
    lock_guard<mutex> lock(mtx);
    Average10000_RANDOM_GEN.insert(Average10000_RANDOM_GEN.end(), otherVec.begin(), otherVec.end());
    Average10000.push_back(BOX);
}