//
// Created by stang on 25-7-3.
//

#ifndef OpenGLTEXTURE_H
#define OpenGLTEXTURE_H
#include "Hazel/Renderer/Texture.h"


namespace Hazel
{
    class OpenGLTexture : public Texture2D{
    public:
        OpenGLTexture(const std::string& path);
        virtual ~OpenGLTexture();
        virtual void Bind(uint32_t slot =0 )const override;
        virtual void Unbind()const override;
        inline virtual uint32_t GetWidth() const override { return m_Width; }
        inline virtual uint32_t GetHeight() const override { return m_Height; }
    private:
        std::string m_path;
        uint32_t m_Width,m_Height;
        uint32_t m_RendererID;

    };
}




#endif //OpenGLTexture_H
