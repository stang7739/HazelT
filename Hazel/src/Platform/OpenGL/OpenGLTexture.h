//
// Created by stang on 25-7-3.
//

#ifndef OpenGLTexture2D_H
#define OpenGLTexture2D_H
#include "Hazel/Renderer/Texture.h"
#include "glad/glad.h"


namespace Hazel
{
    class OpenGLTexture2D : public Texture2D{
    public:
       explicit OpenGLTexture2D(const std::string& path);
        OpenGLTexture2D(uint32_t width, uint32_t height);
        virtual ~OpenGLTexture2D();
        virtual void Bind(uint32_t slot =0 )const override;
        virtual bool IsLoaded() const override {return m_IsLoaded;}
        virtual bool operator==(const Texture& other) const override
        {
            return m_RendererID == other.GetRendererID();
        }
        virtual void Unbind()const override;
        virtual void setData(void* data, uint32_t size)  override;

        inline virtual uint32_t GetWidth() const override { return m_Width; }
        inline virtual uint32_t GetHeight() const override { return m_Height; }
        virtual uint32_t GetRendererID() const override { return m_RendererID; }
        virtual const std::string& GetPath() const override {return m_path;}

    private:
        std::string m_path;
        bool m_IsLoaded = false;
        uint32_t m_Width,m_Height;
        uint32_t m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;


    };
}




#endif //OpenGLTexture2D_H
