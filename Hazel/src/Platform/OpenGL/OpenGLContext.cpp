//
// Created by stang on 25-6-21.
//

#include "OpenGLContext.h"

#include <glad/glad.h>

#include "GLFW/glfw3.h"

namespace Hazel
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle): m_windowHandle(windowHandle)
    {
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_windowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        HZ_CORE_ASSERT(status, "Could not initialize GLAD");


        // HZ_CORE_INFO("OpenGL Info:");
        // HZ_CORE_INFO("    Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        // HZ_CORE_INFO("    Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        // HZ_CORE_INFO("    Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        //
        // GLint numExtensions;
        // glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
        // HZ_CORE_INFO("OpenGL Extensions:");
        // for (GLint i = 0; i < numExtensions; ++i) {
        //     const char* ext = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
        //     HZ_CORE_INFO("    {0}", ext);
        // }

    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_windowHandle);
    }
}
