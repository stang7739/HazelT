//
// Created by stang on 25-6-8.
//

#ifndef ENTITYPONIT_H
#define ENTITYPONIT_H



#ifdef HZ_PLATFORM_WINDOWS

extern  Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
    Hazel::Log::Init();

    HZ_CORE_INFO("Hazel Application started");

    auto app = Hazel::CreateApplication();
    app->run();
    delete app;
    return 0;
}


#endif

#endif //ENTITYPONIT_H
