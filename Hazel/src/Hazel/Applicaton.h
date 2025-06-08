//
// Created by stang on 25-6-7.
//

#ifndef APPLICATON_H
#define APPLICATON_H
#include <Hazel/Core.h>
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
    };

    Applicaton* CreateApplication();
}
#endif //APPLICATON_H
