//
// Created by stang on 25-6-9.
//

#ifndef KEYEVENT_H
#define KEYEVENT_H

#include "Event.h"

namespace Hazel
{
    class HAZEL_API KeyEvent : public Event
    {
    public:
        //
        inline HazelKey GetKeyCode() const;
         HazelKey GetKeyCode(int a,int b) const;
        // EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyBoard)

        virtual int GetCategoryFlags() const override;
    protected:
        KeyEvent(HazelKey keyCode);

        HazelKey m_keyCode;
    };

    class HAZEL_API KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(HazelKey keycode, int repeatCount);
        inline int GetRepeatCount() const;
        std::string ToString() const override;
        EVENT_CLASS_TYPE(KeyPressed)
        protected:
        int m_RepeatCount;
    };

    class HAZEL_API KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(HazelKey keyCode);
        std::string ToString() const override;
        EVENT_CLASS_TYPE(KeyReleased)
    };

    class HAZEL_API KeyTypedEvent : public Event
    {
    public:
        KeyTypedEvent( unsigned int  codepoint);
        inline unsigned int GetCodePoint() const;
        std::string ToString() const override;
        EVENT_CLASS_TYPE(KeyTyped)
        // EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyBoard)

       virtual int GetCategoryFlags() const override;
        protected:
        unsigned int m_codePoint;
    };
}


#endif //KEYEVENT_H
