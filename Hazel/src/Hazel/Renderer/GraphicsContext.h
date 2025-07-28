//
// Created by stang on 25-6-21.
//

#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H
#include "Hazel/Core/Base.h"



namespace Hazel
{
    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;
        virtual void Init() =0;
        virtual void SwapBuffers() =0;
        static  Scope<GraphicsContext> Create(void* window);
    };
}
#endif //GRAPHICSCONTEXT_H