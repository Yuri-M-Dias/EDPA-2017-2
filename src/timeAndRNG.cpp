#include "timeAndRNG.h"

extern const long RNG_MAX;
extern const long RNG_MIN;

using namespace std;

int geraNumeroRandomico() {
    this_thread::sleep_for(chrono::microseconds(1));
    unsigned seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_int_distribution<int> distribution(RNG_MIN, RNG_MAX);
    return distribution(generator);
}

double getCurrentTime() {
    return chrono::duration_cast<chrono::microseconds>(
            chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}

