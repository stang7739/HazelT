//
// Created by stang on 25-6-6.
//

#ifndef CORE_H
#define CORE_H
#include <filesystem>
#ifdef HZ_PLATFORM_WINDOWS
#ifdef HZ_BUILD_DLL
#define HAZEL_API __declspec(dllexport)
#else
#define HAZEL_API __declspec(dllimport)
#endif
#elif defined(HZ_PLATFORM_LINUX)
#define HAZEL_API
#else
#error Hazel only supports Windows and Linux!
#endif

//Core Log macros
// #define HZ_CORE_TRACE(...) ::Hazel::Log::GetCoreLogger()->trace(__VA_ARGS__)
// #define HZ_CORE_INFO(...) ::Hazel::Log::GetCoreLogger()->info(__VA_ARGS__)
// #define HZ_CORE_WARN(...) ::Hazel::Log::GetCoreLogger()->warn(__VA_ARGS__)
// #define HZ_CORE_ERROR(...) ::Hazel::Log::GetCoreLogger()->error(__VA_ARGS__)
// #define HZ_CORE_FATAL(...) ::Hazel::Log::GetCoreLogger()->fatal(__VA_ARGS__)
//
//
// //client Log
// #define HZ_TRACE(...) ::Hazel::Log::GetClientLogger()->trace(__VA_ARGS__)
// #define HZ_INFO(...) ::Hazel::Log::GetClientLogger()->info(__VA_ARGS__)
// #define HZ_WARN(...) ::Hazel::Log::GetClientLogger()->warn(__VA_ARGS__)
// #define HZ_ERROR(...) ::Hazel::Log::GetClientLogger()->error(__VA_ARGS__)
// #define HZ_FATAL(...) ::Hazel::Log::GetClientLogger()->fatal(__VA_ARGS__)

// #define HZ_CORE_TRACE(fmt, ...) ::Hazel::Log::GetCoreLogger()->trace("[{}:{}] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
// #define HZ_CORE_INFO(fmt, ...)  ::Hazel::Log::GetCoreLogger()->info("[{}:{}] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
// #define HZ_CORE_WARN(fmt, ...)  ::Hazel::Log::GetCoreLogger()->warn("[{}:{}] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
// #define HZ_CORE_ERROR(fmt, ...) ::Hazel::Log::GetCoreLogger()->error("[{}:{}] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
// #define HZ_CORE_FATAL(fmt, ...) ::Hazel::Log::GetCoreLogger()->fatal("[{}:{}] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
//
// #define HZ_TRACE(fmt, ...) ::Hazel::Log::GetClientLogger()->trace("[{}:{}] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
// #define HZ_INFO(fmt, ...) ::Hazel::Log::GetClientLogger()->info("[{}:{}] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
// #define HZ_WARN(fmt, ...)  ::Hazel::Log::GetClientLogger()->warn("[{}:{}] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
// #define HZ_ERROR(fmt, ...) ::Hazel::Log::GetClientLogger()->error("[{}:{}] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
// #define HZ_FATAL(fmt, ...) ::Hazel::Log::GetClientLogger()->fatal("[{}:{}] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

#define HZ_CORE_TRACE(fmt, ...) ::Hazel::Log::GetCoreLogger()->trace("[{}:{}] " fmt, std::filesystem::path(__FILE__).filename().string(), __LINE__, ##__VA_ARGS__)
#define HZ_CORE_INFO(fmt, ...)  ::Hazel::Log::GetCoreLogger()->info("[{}:{}] " fmt, std::filesystem::path(__FILE__).filename().string(), __LINE__, ##__VA_ARGS__)
#define HZ_CORE_WARN(fmt, ...)  ::Hazel::Log::GetCoreLogger()->warn("[{}:{}] " fmt, std::filesystem::path(__FILE__).filename().string(), __LINE__, ##__VA_ARGS__)
#define HZ_CORE_ERROR(fmt, ...) ::Hazel::Log::GetCoreLogger()->error("[{}:{}] " fmt, std::filesystem::path(__FILE__).filename().string(), __LINE__, ##__VA_ARGS__)
#define HZ_CORE_FATAL(fmt, ...) ::Hazel::Log::GetCoreLogger()->fatal("[{}:{}] " fmt, std::filesystem::path(__FILE__).filename().string(), __LINE__, ##__VA_ARGS__)

#define HZ_TRACE(fmt, ...) ::Hazel::Log::GetClientLogger()->trace("[{}:{}] " fmt, std::filesystem::path(__FILE__).filename().string(), __LINE__, ##__VA_ARGS__)
#define HZ_INFO(fmt, ...)  ::Hazel::Log::GetClientLogger()->info("[{}:{}] " fmt, std::filesystem::path(__FILE__).filename().string(), __LINE__, ##__VA_ARGS__)
#define HZ_WARN(fmt, ...)  ::Hazel::Log::GetClientLogger()->warn("[{}:{}] " fmt, std::filesystem::path(__FILE__).filename().string(), __LINE__, ##__VA_ARGS__)
#define HZ_ERROR(fmt, ...) ::Hazel::Log::GetClientLogger()->error("[{}:{}] " fmt, std::filesystem::path(__FILE__).filename().string(), __LINE__, ##__VA_ARGS__)
#define HZ_FATAL(fmt, ...) ::Hazel::Log::GetClientLogger()->fatal("[{}:{}] " fmt, std::filesystem::path(__FILE__).filename().string(), __LINE__, ##__VA_ARGS__)

#ifdef HZ_DEBUG
    #ifdef HZ_PLATFORM_WINDOWS
        #define HZ_DEBUGBREAK() __debugbreak()
    #elif defined(HZ_PLATFORM_LINUX)
        #include <signal.h>
        #define HZ_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform don't support debugbreak yet!"
    #endif
    #define HZ_ENABLE_ASSERTS
#else
    #define HZ_ENABLE_ASSERTS
#endif

#ifdef HZ_ENABLE_ASSERTS
    #define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); HZ_DEBUGBREAK(); } }
    #define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); HZ_DEBUGBREAK(); } }
#else
    #define HZ_ASSERT(...)
    #define HZ_CORE_ASSERT(...)
#endif

#define BIT(x) (1 << x)
#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#endif //CORE_H
