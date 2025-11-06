//
// Created by stang on 25-8-26.
//

#ifndef PLATFORMUTILS_H
#define PLATFORMUTILS_H


namespace Hazel
{
    class FileDialogs {
    public:
        static std::string OpenFile(const char* filter);
        static std::string SaveFile(const char* filter);
    };
    class Time
    {
    public:
        static float GetTime();
    };
}



#endif //PLATFORMUTILS_H
