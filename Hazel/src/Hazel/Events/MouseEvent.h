//
// Created by stang on 25-6-9.
//

#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H


#include "Event.h"
namespace Hazel
{
    class HAZEL_API MouseMoveEvent : public Event {
    public:
        MouseMoveEvent(double x, double y) ;

        float GetX() const;
        float GetY() const;

        std::string ToString() const override ;

        EVENT_CLASS_TYPE(MouseMoved)

        virtual int GetCategoryFlags() const override;

        protected:
        float m_MouseX, m_MouseY ;
    };
    class HAZEL_API MouseScrolledEvent : public Event
    {
        public:
        MouseScrolledEvent(double offsetX, double offsetY) ;
        float GetOffsetX() const;
        float GetOffsetY() const;
        std::string ToString() const override ;
        EVENT_CLASS_TYPE(MouseScrolled)
        virtual int GetCategoryFlags() const override;
        protected:
        float m_OffsetX, m_OffsetY ;
    };
    class HAZEL_API MouseButtonEvent : public Event
    {
        public:
        MouseButtonEvent(HazelKey Mousebutton);
        HazelKey GetMouseButton() const;
        virtual int GetCategoryFlags() const override;
    protected:
        HazelKey m_MouseButton ;
    };
    class HAZEL_API MouseButtonPressedEvent : public MouseButtonEvent
    {
        public:
        MouseButtonPressedEvent(HazelKey Mousebutton);
        std::string ToString() const override ;
        EVENT_CLASS_TYPE(MouseButtonPressed)
    };
    class HAZEL_API MouseButtonReleasedEvent : public MouseButtonEvent
    {
        public:
        MouseButtonReleasedEvent(HazelKey Mousebutton);
        std::string ToString() const override ;
        EVENT_CLASS_TYPE(MouseButtonReleased)
    };
}



#endif //MOUSEEVENT_H
