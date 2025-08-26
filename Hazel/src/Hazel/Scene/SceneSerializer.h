//
// Created by stang on 25-8-24.
//

#ifndef SCENESERIALIZER_H
#define SCENESERIALIZER_H
#include "Scene.h"
#include "Hazel/Core/Base.h"

namespace Hazel{
    class SceneSerializer {
      public:
      SceneSerializer(const Ref<Scene>& scene);
        void Serialize(const std::string& filepath);
        void SerializeRuntime(const std::string& filepath);

        bool Deserialize(const std::string& filepath);
        bool DeserializeRuntime(const std::string& filepath);
    private:
        Ref<Scene> m_Scene;
    };
}




#endif //SCENESERIALIZER_H
