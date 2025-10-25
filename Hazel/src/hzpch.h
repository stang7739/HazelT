//
// Created by stang on 25-6-10.
//

#ifndef HZPCH_H
#define HZPCH_H

// // 启用 GLM 实验特性（需在包含任何 glm 头之前）
// #ifndef GLM_ENABLE_EXPERIMENTAL
// #define GLM_ENABLE_EXPERIMENTAL
// #endif

#include <iostream>
#include <algorithm>
#include <functional>
#include <ctime>
#include <utility>
#include <memory>
#include <format>

#include <string>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <Hazel/Core/Log.h>
#include <Hazel/Debug/Instrumentor.h>
#include <random>

#ifdef HZ_PLATFORM_WINDOWS
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#endif

#endif //HZPCH_H
