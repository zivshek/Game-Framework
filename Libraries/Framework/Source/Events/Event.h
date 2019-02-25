#ifndef __Event_H__
#define __Event_H__

enum EventTypes
{
    EventType_Input,
    GameEventType_Destroy,
    EventType_NotSet,
};

class Event
{
protected:
    EventTypes m_EventType;

public:
    Event()
    {
        m_EventType = EventType_NotSet;
    }

    EventTypes GetEventType() { return m_EventType; }
};

#endif //__Event_H__