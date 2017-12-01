#ifndef EDPA20172_TIMEANDRNGFUNCTIONS_H
#define EDPA20172_TIMEANDRNGFUNCTIONS_H

#include <random>
#include <chrono>
#include <thread>

// Necessário apenas se usar INT_MAX e INT_MIN do C(32-bits precision)
#include <climits>

// Usando para não repetir INT_MAX e INT_MIN em todo lugar
const long RNG_MAX = INT16_MAX;
// Negativos não são mais necessários
const long RNG_MIN = 0;

using namespace std;

int geraNumeroRandomico();

double getCurrentTimeInMillis();

#endif //EDPA20172_TIMEANDRNGFUNCTIONS_H
