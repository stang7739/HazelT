//
// Created by stang on 25-6-7.
//

#ifndef APPLICATON_H
#define APPLICATON_H
#include <Hazel/Core/Core.h>
#include <Hazel/Events/Event.h>

#include "Window.h"

namespace Hazel
{
    // Symbolic macros everywhere
    class HAZEL_API Applicaton
    {
    public:
        Applicaton();
        //Virtual constructor function:
        //Support class polymorphism Ensure that the derived class object created by the base pointer
        //calls the corresponding destructor to properly free up memory
        virtual ~Applicaton();
        void run();

    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

    Applicaton* CreateApplication();
}
#endif //APPLICATON_H
