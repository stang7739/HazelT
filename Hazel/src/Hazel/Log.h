//
// Created by stang on 25-6-8.
//

#ifndef LOG_H
#define LOG_H

#include<Hazel/Core.h>
#include<spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
namespace Hazel
{
    class HAZEL_API Log {
        public:
        static void Init();
        //Inline guarantees that when multiple CPPs are included, there is only one copy of this function
        //Smart pointer Automatically manage memory release Prevent memory leaks There is a reference count Prevent pointers from dangling
        //When multiple modules and functions want to use the same object, use smart pointers
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {return s_CoreLogger;}
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {return s_ClientLogger;}


    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}


#endif //LOG_H
