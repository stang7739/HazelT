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
         void OnCreate()
         {

         }
         void OnDestory()
         {

         }
         void OnUpdate(Timestep ts)
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
