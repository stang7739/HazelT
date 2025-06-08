
#include <Hazel.h>

class SandBox : public Hazel::Applicaton
{
    public:
    SandBox()
    {

    }
    ~SandBox()
    {

    }
};
//Create a SanBox Factory Pattern Abstraction of the application entry point
Hazel::Applicaton* Hazel::CreateApplication()
{
    return new SandBox;
}