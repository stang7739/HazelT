//
// Created by stang on 25-8-9.
//

#ifndef SCRIPTABLEENTITY_H
#define SCRIPTABLEENTITY_H
#include "Entity.h"
#include "Hazel/Core/Timestep.h"

#include "Entity.h"

namespace Hazel {

    class ScriptableEntity
    {
    public:
        template<typename T>
        T& GetComponent()
        {
            return m_Entity.GetComponent<T>();
        }
    private:
        Entity m_Entity;
        friend class Scene;
    };

}
#endif //SCRIPTABLEENTITY_H
