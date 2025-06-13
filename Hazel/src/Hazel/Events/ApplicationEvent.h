//
// Created by stang on 25-6-9.
//

#ifndef APPLICATIONEVENT_H
#define APPLICATIONEVENT_H


#include "Event.h"


namespace Hazel
{
    class HAZEL_API WindowResizeEvent : public Event {
        public:
        WindowResizeEvent(int width, int height);
         unsigned int GetWidth() const ;
         unsigned int GetHeight() const ;

        std::string ToString() const override;

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)

        protected:
        unsigned int m_width, m_height;
    };

    class HAZEL_API WindowCloseEvent : public Event
    {
        public:
        WindowCloseEvent();
        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
    class HAZEL_API AppTickEvent : public Event
    {
        public:
        AppTickEvent();
        EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
    class HAZEL_API AppUpdateEvent : public Event
    {
        public:
        AppUpdateEvent();
        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
    class HAZEL_API AppRenderEvent : public Event
    {
        public:
        AppRenderEvent();
        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
}



#endif //APPLICATIONEVENT_H
