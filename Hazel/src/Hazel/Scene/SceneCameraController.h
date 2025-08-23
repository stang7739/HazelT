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
            auto& transform = GetComponent<TransformComponent>().Translation;
              transform.x = rand() % 4 - 2.f; // Random position between -1 and 1
         }
         virtual void OnDestroy() override
         {

         }
        virtual void OnUpdate(Timestep ts) override
         {
             auto& transform = GetComponent<TransformComponent>().Translation;
             float speed = 5.f;
             if(Input::IsKeyPressed(HazelKey::A))
             {
                 transform.x -= speed * ts;
             }
             if(Input::IsKeyPressed(HazelKey::D))
             {
                  transform.x += speed * ts;
             }
             if(Input::IsKeyPressed(HazelKey::W))
             {
                  transform.x += speed * ts;
             }
             if(Input::IsKeyPressed(HazelKey::S))
             {
                  transform.x -= speed * ts;
             }
         }
     };
 }
 #endif //SCENECAMERACONTROLLER_H
