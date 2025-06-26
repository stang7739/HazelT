//
// Created by stang on 25-6-21.
//

#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H


#include "Hazel/Renderer/GraphicsContext.h"

struct GLFWwindow;
namespace Hazel
{

    class OpenGLContext : public GraphicsContext {
    public:
        OpenGLContext(GLFWwindow* windowHandle);
        virtual void Init() override;
        virtual void SwapBuffers() override;
    private:
       GLFWwindow* m_windowHandle;
    };
}


#endif //OPENGLCONTEXT_H
