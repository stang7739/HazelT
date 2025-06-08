//
// Created by stang on 25-6-8.
//

#include "Log.h"

namespace Hazel
{
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
    void Log::Init()
    {
        spdlog::set_pattern("%^[%Y-%m-%d %H:%M:%S] [%l] %n: %v%$");
        s_CoreLogger = spdlog::stdout_color_mt("Hazel");
        s_CoreLogger->set_level(spdlog::level::trace);
        s_ClientLogger = spdlog::stdout_color_mt("App");
        s_ClientLogger->set_level(spdlog::level::trace);

    }



}