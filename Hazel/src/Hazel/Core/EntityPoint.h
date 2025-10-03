//
// Created by stang on 25-6-8.
//

#ifndef ENTITYPONIT_H
#define ENTITYPONIT_H
#include <Hazel/Debug/Instrumentor.h>

#include "Application.h"


#ifdef HZ_PLATFORM_WINDOWS

extern  Hazel::Application* Hazel::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
//需要接收命令行参数
{
    Hazel::Log::Init();

   HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");

    // auto app = Hazel::CreateApplication();
    auto app = Hazel::CreateApplication({argc,argv});
    HZ_PROFILE_END_SESSION();

    HZ_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
    app->run();
    HZ_PROFILE_END_SESSION();

    HZ_PROFILE_BEGIN_SESSION("end", "HazelProfile-End.json");
    delete app;
    HZ_PROFILE_END_SESSION();
    return 0;
}


#endif

#endif //ENTITYPONIT_H
