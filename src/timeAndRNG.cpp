#include "timeAndRNG.h"

extern const long RNG_MAX;
extern const long RNG_MIN;

int geraNumeroRandomico() {
    this_thread::sleep_for(chrono::milliseconds(1));
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_int_distribution<int> distribution(RNG_MIN, RNG_MAX);
    return distribution(generator);
}

double getCurrentTimeInMillis() {
    return chrono::duration_cast<chrono::milliseconds>(
            chrono::steady_clock::now().time_since_epoch()
    ).count();
}

