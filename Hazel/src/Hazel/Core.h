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


#endif

#endif //CORE_H
