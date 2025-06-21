//
// Created by stang on 25-6-14.
//

#ifndef LAYERSTACK_H
#define LAYERSTACK_H
#include "Layer.h"


namespace Hazel
{
    class LayerStack {
        public:
        LayerStack();
        ~LayerStack();
        void PushLayer(Layer* layer);
        void PushOverLayer(Layer* layer);
        void PopLayer(Layer* layer);
        void PopOverLayer(Layer* layer);
        std::vector<Layer*>::iterator begin();
        std::vector<Layer*>::iterator end();
        private:
        std::vector<Layer*> m_Layers;
        // std::vector<Layer*>::iterator m_LayerInsert ;
        unsigned int m_LayerInsertIndex = 0;
    };
}



#endif //LAYERSTACK_H
