 //
 // Created by stang on 25-8-9.
 //

 #ifndef SCENECAMERACONTROLLER_H
 #define SCENECAMERACONTROLLER_H
 #include "ScriptableEntity.h"
 namespace Hazel
 {
     class HAZEL_API CameraController: public ScriptableEntity
     {
     public:
         virtual void OnCreate() override
         {
            auto& transform = GetComponent<TransformComponent>().Transform;
             transform[3][0] = rand() % 10 - 5.f; // Random position between -5 and 5
         }
         virtual void OnDestroy() override
         {

         }
        virtual void OnUpdate(Timestep ts) override
         {
             auto& transform = GetComponent<TransformComponent>().Transform;
             float speed = 5.f;
             if(Input::IsKeyPressed(HazelKey::A))
             {
                 transform[3][0] -= speed * ts;
             }
             if(Input::IsKeyPressed(HazelKey::D))
             {
                 transform[3][0] += speed * ts;
             }
             if(Input::IsKeyPressed(HazelKey::W))
             {
                 transform[3][1] += speed * ts;
             }
             if(Input::IsKeyPressed(HazelKey::S))
             {
                 transform[3][1] -= speed * ts;
             }
         }
     };
 }
 #endif //SCENECAMERACONTROLLER_H
