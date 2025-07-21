//
// Created by stang on 25-7-18.
//

#include "Random.h"
 std::mt19937 Random::s_RandomEngine; // Mersenne Twister random number generator
 std::uniform_int_distribution<int> Random::s_IntDistribution; // Uniform distribution for integers
