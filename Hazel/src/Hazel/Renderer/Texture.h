//
// Created by stang on 25-7-3.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include "Hazel/Core/Base.h"


namespace Hazel
{
    class HAZEL_API Texture {
    public:
        ~Texture() = default;
        virtual void Bind(uint32_t slot = 0) const = 0;
        virtual void Unbind() const = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;


    };
    class HAZEL_API Texture2D : public Texture
    {
    public:
        static Ref<Texture2D> Create(const std::string& path);
    };
}


#endif //TEXTURE_H
