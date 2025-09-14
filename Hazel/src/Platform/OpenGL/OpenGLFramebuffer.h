//
// Created by stang on 25-7-27.
//

#ifndef OPENGLFRAMEBUFFER_H
#define OPENGLFRAMEBUFFER_H
#include "Hazel/Core/Base.h"
#include "Hazel/Renderer/Framebuffer.h"

namespace Hazel
{
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        virtual ~OpenGLFramebuffer();

        void Invalidate();

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void Resize(uint32_t width, uint32_t height) override;

        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override
        {
            HZ_CORE_ASSERT(index < m_ColorAttachments.size()); return m_ColorAttachments[index];
        }
    virtual uint32_t GetDepthAttachmentRendererID() const override { return m_DepthAttachment; }

        virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
        virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

    private:
        uint32_t m_RendererID = 0;
        FramebufferSpecification m_Specification;

        std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
        FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

        std::vector<uint32_t> m_ColorAttachments;
        uint32_t m_DepthAttachment = 0;
    };
}

#endif //OPENGLFRAMEBUFFER_H
