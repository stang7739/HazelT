 //
 // Created by stang on 25-6-18.
 //

 #ifndef WINDOWSINPUT_H
 #define WINDOWSINPUT_H
 #include "Hazel/Core/Input.h"


 namespace Hazel
 {
     class WindowsInput : public Input
     {
     protected:

         virtual bool IsKeyPressedImpl(HazelKey Keycode) override;
         virtual bool IsMouseButtonPressedImpl(HazelKey Button) override;
         virtual std::pair<float,float>GetMousePositionImpl() override;
         virtual float GetMouseXImpl() override;
         virtual float GetMouseYImpl() override;

     };
 }



 #endif //WINDOWSINPUT_H
