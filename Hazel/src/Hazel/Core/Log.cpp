//
// Created by stang on 25-6-8.
//
#include <hzpch.h>
#include "Log.h"

namespace Hazel
{
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
    void Log::Init()
    {
        // spdlog::set_pattern("%^[%Y/%m/%d %H:%M:%S] [%l] %n: %v%$");
        spdlog::set_pattern("%^[%m/%d %H:%M:%S] [%l] %n: %v%$");
        s_CoreLogger = spdlog::stdout_color_mt("H");
        s_CoreLogger->set_level(spdlog::level::trace);
        s_ClientLogger = spdlog::stdout_color_mt("A");
        s_ClientLogger->set_level(spdlog::level::trace);

    }

}