//
// Created by stang on 25-6-12.
//

#ifndef RENDERER_H
#define RENDERER_H
#include"RendererAPI.h"

namespace Hazel
{
    class Renderer {
        public:
        static RendererAPI::API GetAPI(){return RendererAPI::GetAPI();}
    };
}



#endif //RENDERER_H
