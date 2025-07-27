//
// Created by stang on 25-7-27.
//

#ifndef OPENGLFRAMEBUFFER_H
#define OPENGLFRAMEBUFFER_H
#include "Hazel/Core/Base.h"
#include "Hazel/Renderer/Framebuffer.h"

namespace Hazel
{

    class OpenGLFramebuffer : public Framebuffer{
    public:
        OpenGLFramebuffer(FramebufferSpecification& spec);
        ~OpenGLFramebuffer();
        void Invalidate();

        virtual void Bind() override;
        virtual void Unbind()override;

        virtual const FramebufferSpecification& GetSpecification() const override{return m_Specification; }
        virtual uint32_t GetColorAttachmentRendererID() const override{return m_ColorAttachment; }
        virtual uint32_t GetDepthAttachmentRendererID() const override{return m_DepthAttachment; }


    private:
        FramebufferSpecification m_Specification;
        uint32_t m_RendererID ;
        uint32_t m_ColorAttachment , m_DepthAttachment;

    };
}

#endif //OPENGLFRAMEBUFFER_H
