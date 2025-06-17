//
// Created by stang on 25-6-6.
//

#ifndef CORE_H
#define CORE_H
#ifdef HZ_PLATFORM_WINDOWS
#ifdef HZ_BUILD_DLL
#define HAZEL_API __declspec(dllexport)
#else
#define HAZEL_API __declspec(dllimport)
#endif
#else
#error iGe only supports Windows!
#endif

//Core Log macros
#define HZ_CORE_TRACE(...) ::Hazel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HZ_CORE_INFO(...) ::Hazel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HZ_CORE_WARN(...) ::Hazel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HZ_CORE_ERROR(...) ::Hazel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HZ_CORE_FATAL(...) ::Hazel::Log::GetCoreLogger()->fatal(__VA_ARGS__)


//client Log
#define HZ_TRACE(...) ::Hazel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HZ_INFO(...) ::Hazel::Log::GetClientLogger()->info(__VA_ARGS__)
#define HZ_WARN(...) ::Hazel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HZ_ERROR(...) ::Hazel::Log::GetClientLogger()->error(__VA_ARGS__)
#define HZ_FATAL(...) ::Hazel::Log::GetClientLogger()->fatal(__VA_ARGS__)

#ifdef HZ_DEBUG
    #ifdef HZ_PLATFORM_WINDOWS
        #define HZ_DEBUGBREAK() __debugbreak()
    #else
        #ERROR "Platform don't support debugbreak yet!"
    #endif
    #define HZ_ENBLE_ASSERTS
#else
    #define HZ_ENBLE_ASSERT
#endif

#ifdef HZ_ENABLE_ASSERTS
    #define HZ_INTERNAL_ASSERT_IMPL(type,check,msg, ...)
    do
    {
        if(!(check))
        {
            HZ##type##ERROR(msg,__VA_ARGS__);
            HZ_DEBUGBREAK();
        }
    }while(0)
#define HZ_INTERNAL_ASSERT_WITH_MSG(type,check,...)
        HZ_INTERNAL_ASSERT_IMPL(type,check,"Assertion failed:{0}",__VA_ARGS__)
#define HZ_INTERNAL_ASSETR_NO_MSG(type,check)
        HZ_INTERNAL_ASSERT_IMPL(type,check,"Assert '{0} failed at {1}:{2}',#check,std::filesystem::path(__FILE__),filename().string(),__LINE__)
#define HZ_INTERNAL_ASSERT_GET_MACRO(_1,_2,_3,NAME,...) NAME
#define HZ_ASSERT(...) HZ_EXPAND_MACRO(HZ_INTERAL_ASSERT_GET_MACRO(__VA_ARGS__,HZ_INTERNAL_WITH_MSG,HZ_INTERNAL_ASSERT_NO_MSG(_,__VA_ARGS__))
#define HZ_CORE_ASSERT(...)                                                                                       \
IGE_EXPAND_MACRO(IGE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__, IGE_INTERNAL_ASSERT_WITH_MSG,                      \
IGE_INTERNAL_ASSERT_NO_MSG)(_CORE_, __VA_ARGS__))
        #else
#define HZ_ASSERT(...)
#define HZ_CORE_ASSERT(...)
#endif

#define BIT(x) (1 << x)
#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#endif //CORE_H
