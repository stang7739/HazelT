//
// Created by stang on 25-6-14.
//
#include <hzpch.h>
#include "Layer.h"

#include "Timestep.h"


namespace Hazel
{
    Layer::Layer(const std::string& name ):m_DebugName(name){}
    Layer::~Layer() {}
     void Layer::OnAttach(){} //Executed when the layer is loaded into the stack
     void Layer::OnDetach(){}//Executed when the layer is removed from the stack
     void Layer::OnUpdate(Timestep timestep){}//Update logic every frame
     void Layer::OnEvent(Event& event){}//Respond to events that are distributed by the event system
     void Layer::OnImGuiRender(){}//Draw the ImGui interface
}
