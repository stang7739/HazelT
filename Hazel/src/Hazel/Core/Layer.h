//
// Created by stang on 25-6-14.
//

#ifndef LAYER_H
#define LAYER_H
#include "Hazel/Events/Event.h"


namespace Hazel
{
    class Timestep;
}

namespace Hazel
{


    class HAZEL_API Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();
        virtual void OnAttach(); //Executed when the layer is loaded into the stack
        virtual void OnDetach();//Executed when the layer is removed from the stack
        virtual void OnUpdate(Timestep timestep);//Update logic every frame
        virtual void OnEvent(Event& event);//Respond to events that are distributed by the event system
        virtual void OnImGuiRender();//Draw the ImGui interface
        protected:
        std::string m_DebugName;

    };
}


#endif //LAYER_H
