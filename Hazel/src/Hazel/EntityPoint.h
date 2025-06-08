//
// Created by stang on 25-6-8.
//

#ifndef ENTITYPONIT_H
#define ENTITYPONIT_H
#include <iostream>


#ifdef HZ_PLATFORM_WINDOWS

extern  Hazel::Applicaton* Hazel::CreateApplication();

int main(int argc, char** argv)
{
    Hazel::Log::Init();
    Hazel::Log::GetCoreLogger()->warn("Hazel Application started");
    Hazel::Log::GetClientLogger()->info("Hazel Application started");
    std::cout << "Hello Hzael!";
    auto app = Hazel::CreateApplication();
    app->run();
    delete app;

}


#endif

#endif //ENTITYPONIT_H
