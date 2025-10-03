//
// Created by stang on 25-9-20.
//

#ifndef OPENGLUNIFORMBUFFER_H
#define OPENGLUNIFORMBUFFER_H
#include "Hazel/Renderer/UniformBuffer.h"


namespace Hazel{
class OpenGLUniformBuffer : public UniformBuffer
{
public:
    OpenGLUniformBuffer(uint32_t size,uint32_t binding) ;
    virtual  ~OpenGLUniformBuffer() ;
    virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
private:
    uint32_t m_RendererID = 0;
};
}


#endif //OPENGLUNIFORMBUFFER_H
