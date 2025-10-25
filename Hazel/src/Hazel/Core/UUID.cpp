//
// Created by stang on 25-10-25.
//
#include "hzpch.h"
#include "UUID.h"

namespace  Hazel
{
    static std::random_device s_RandomDevice; //获得系统中的非确定性熵，用作种子
    static std::mt19937_64 s_Engine(s_RandomDevice());//伪随机数引擎
    static std::uniform_int_distribution<uint64_t> s_UniformDistribution;//返回一个随机uint64_t
    UUID::UUID():m_UUID(s_UniformDistribution(s_Engine))
    {

    }
    UUID::UUID(uint64_t uuid):m_UUID(uuid)
    {

    }


}