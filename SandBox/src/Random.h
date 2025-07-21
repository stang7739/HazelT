//
// Created by stang on 25-7-18.
//

#ifndef RANDOM_H
#define RANDOM_H
#include <random>


class Random {
public:
    static void Init()
    {
     s_RandomEngine.seed(std::random_device()());
    }
    static float Float()
    {
        return (float)s_IntDistribution(s_RandomEngine)/(float)std::numeric_limits<uint32_t>::max();
    }

private:
    static std::mt19937 s_RandomEngine; // Mersenne Twister random number generator
    static std::uniform_int_distribution<int> s_IntDistribution; // Uniform distribution for integers
};



#endif //RANDOM_H
