//
// Created by stang on 25-6-7.
//

#ifndef APPLICATON_H
#define APPLICATON_H
#include <Hazel/Core.h>
namespace Hazel
{
 //到处符号宏
    class HAZEL_API Applicaton
    {
        public:
        Applicaton();
        //虚构造函数作用： 支持类多态 确保基指针创建的派生类对象调用对应的析构函数，正确释放内存
        virtual ~Applicaton();
        void run();
    };

    Applicaton* CreateApplication();
}
#endif //APPLICATON_H
