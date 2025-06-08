
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
//创建SanBox 工厂模式 应用程序入口点的抽象
Hazel::Applicaton* Hazel::CreateApplication()
{
    return new SandBox;
}