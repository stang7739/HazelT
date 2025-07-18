//
// Created by stang on 25-7-3.
//

#include "OpenGLTexture.h"

#include <glad/glad.h>

#include "stb_image.h"

namespace Hazel
{
    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : Texture2D(), m_Width(width), m_Height(height)
    {

        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;
        //Create 1 OpenGL texture object, of type GL_TEXTURE_2D
        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        //Allocate storage space to the texture object m_RendererID
        //1 is the number of layers of mipmap(there is only1 layer here)
        //GL_RGB is the internal pixel format(stored in RGB format)

        //m_Widthand m_Height are the width and height of the texture
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
        //When the texture is scaled down
        //(e.g. the rendered object is farther away from the camera and the texture map appears on fewer pixels),
        //the texex is sampled by linear interpolation.
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : Texture2D(), m_path(path)
    {
        int width, height, channels;
        //When loading the image, do you want to turn the image upside down in the Y direction
        stbi_set_flip_vertically_on_load(true);
        //Loads an image file into memory and returns a pixel data pointer.
        stbi_uc* data = nullptr;
        {
            HZ_INFO("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        }
        HZ_ASSERT(data, "Failed to load image: {0}");
        m_Width = width;
        m_Height = height;

        GLenum internalFormat = 0, dataFormat = 0;

        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }
        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;


        //Create 1 OpenGL texture object, of type GL_TEXTURE_2D
        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        //Allocate storage space to the texture object m_RendererID
        //1 is the number of layers of mipmap(there is only1 layer here)
        //GL_RGB is the internal pixel format(stored in RGB format)
        //m_Widthand m_Height are the width and height of the texture
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
        //When the texture is scaled down
        //(e.g. the rendered object is farther away from the camera and the texture map appears on fewer pixels),
        //the texex is sampled by linear interpolation.
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat,GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }
    void OpenGLTexture2D::setData(void* data, uint32_t size)
    {
        uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3; // Bytes per pixel
        HZ_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat,GL_UNSIGNED_BYTE, data);

    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        glBindTextureUnit(slot, m_RendererID);
    }

    void OpenGLTexture2D::Unbind() const
    {
    }
}
