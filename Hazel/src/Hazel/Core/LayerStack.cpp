//
// Created by stang on 25-6-14.
//

#include "LayerStack.h"

namespace Hazel
{
    LayerStack::LayerStack(){

    }
    LayerStack::~LayerStack(){
        for(Layer* layer : m_Layers){delete layer;}
    }
    void LayerStack::PushLayer(Layer* layer){
        // m_LayerInsert = m_Layers.emplace(m_LayerInsert,layer);
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        m_LayerInsertIndex++;
    }
    //overLayer always in the back end
    void LayerStack::PushOverLayer(Layer* layer){
        m_Layers.emplace_back(layer);
    }
    void LayerStack::PopLayer(Layer* layer){
        auto it = std::find(m_Layers.begin(),m_Layers.end(),layer);
        if(it != m_Layers.end())
        {
            m_Layers.erase(it);
            m_LayerInsertIndex--;
        }
    }
    void LayerStack::PopOverLayer(Layer* layer)
    {
        auto it = std::find(m_Layers.begin(),m_Layers.end(),layer);
        if(it != m_Layers.end())
        {
            m_Layers.erase(it);

        }

    }
    std::vector<Layer*>::iterator LayerStack::begin(){
        return m_Layers.begin();
    }
    std::vector<Layer*>::iterator LayerStack::end(){
        return m_Layers.end();
    }
}