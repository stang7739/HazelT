//
// Created by stang on 25-6-21.
//

#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H

#endif //GRAPHICSCONTEXT_H

namespace Hazel
{
    class GraphicsContext
    {
    public:
        virtual void Init() =0;
        virtual void SwapBuffers() =0;
    };
}
