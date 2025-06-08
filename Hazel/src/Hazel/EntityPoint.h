//
// Created by stang on 25-6-8.
//

#ifndef ENTITYPONIT_H
#define ENTITYPONIT_H


#ifdef HZ_PLATFORM_WINDOWS

extern  Hazel::Applicaton* Hazel::CreateApplication();

int main(int argc, char** argv)
{
    std::cout << "Hello Hzael!\n";
    auto app = Hazel::CreateApplication();
    app->run();
    delete app;

}


#endif

#endif //ENTITYPONIT_H
