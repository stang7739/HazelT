 //
 // Created by stang on 25-6-18.
 //

 #ifndef INPUT_H
 #define INPUT_H
 #include <utility>

#include "Base.h"
 #include "Hazel/Events/KeyCode.h"

 namespace Hazel
 {
     class HAZEL_API   Input
     {
         public:
         static bool IsKeyPressed(HazelKey keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
         static bool IsMouseButtonPressed(HazelKey button) { return s_Instance->IsMouseButtonPressedImpl(button); }
         static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
         static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
         static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
         static void SetInstance(Input* instance) { s_Instance = instance; }
         Ref<Input> Create();




     protected:
         virtual bool IsKeyPressedImpl(HazelKey Keycode) = 0;
         virtual bool IsMouseButtonPressedImpl(HazelKey Button) = 0;
         virtual std::pair<float,float>GetMousePositionImpl() = 0;
         virtual float GetMouseXImpl() = 0;
         virtual float GetMouseYImpl() = 0;

         static Input* s_Instance;

     };
 }
 #endif //INPUT_H
