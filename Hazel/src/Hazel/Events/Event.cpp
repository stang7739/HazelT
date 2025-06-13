//
// Created by stang on 25-6-9.
//
#include <hzpch.h>
#include "Event.h"
namespace Hazel
{

/////////////////////////Event//////////////////////////////////////////////////////
   std::string Event::ToString() const {  return GetName(); }

    bool Event::IsInCategory(EventCategory category)
    {
        return GetCategoryFlags() & category;
    }
///////////////////////EventDispatcher/////////////////////////////////////////////
    EventDispatcher::EventDispatcher(Event& event) : m_Event(event) {}
}
