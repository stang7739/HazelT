//
// Created by stang on 25-6-9.
//

#ifndef EVENT_H
#define EVENT_H

#include <functional>

#include<Hazel/Core/Core.h>
#include "KeyCode.h"
namespace Hazel
{
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        AppTick,
        AppUpdate,
        AppRender,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };
    enum EventCategory
    {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyBoard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseBotton = BIT(4),
    };

    class HAZEL_API Event
    {
        friend class EventDispatcher;
    public:
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const;
        bool IsInCategory(EventCategory category);
        protected:
        bool m_Handled = false;

    };

    class HAZEL_API EventDispatcher
    {
        public:
        EventDispatcher(Event& event);

        template<class T>
        bool Dispatch(std::function<bool(T&)> func)
        {
            if(m_Event.GetEventType() == T::GetEventType())
            {
                m_Event.m_Handled = func(*(T*)&m_Event);
            }
        }
        protected:
        Event& m_Event;
    };
    //It is possible to leave ## here, ## is redundant
#define EVENT_CLASS_TYPE(type) \
                                static EventType GetStaticType() {return EventType::type;} \
                                virtual EventType GetEventType() const override {return GetStaticType();} \
                                virtual const char* GetName() const override {return #type;}
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override{return category;}

}


#endif //EVENT_H
