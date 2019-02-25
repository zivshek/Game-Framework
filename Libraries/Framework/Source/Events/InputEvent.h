#ifndef __InputEvent_H__
#define __InputEvent_H__

enum InputDeviceTypes
{
    InputDeviceType_Keyboard,
    InputDeviceType_Mouse,
    InputDeviceType_Gamepad,
    InputDeviceType_NumTypes,
};

enum InputStates
{
    InputState_Pressed,
    InputState_Released,
    InputState_Held,
    InputState_Moved,
    InputState_NumStates,
};

enum GamepadIDs
{
    GamepadID_LeftStick,
    GamepadID_RightStick,
};

class InputEvent : public Event
{
protected:
    InputDeviceTypes m_InputDeviceType;
    InputStates m_InputState;

    union
    {
        int m_ID;
        GamepadIDs m_GamepadID;
    };

    vec2 m_Position;

public:
    InputEvent(InputDeviceTypes devicetype, InputStates state, int id, vec2 pos)
    {
        m_EventType = EventType_Input;

        m_InputDeviceType = devicetype;
        m_InputState = state;
        m_ID = id;
        m_Position = pos;
    }

    InputDeviceTypes GetInputDeviceType() { return m_InputDeviceType; }
    InputStates GetInputState() { return m_InputState; }

    int GetID() { return m_ID; }
    GamepadIDs GetGamepadID() { return m_GamepadID; }

    vec2 GetPosition() { return m_Position; }
};

#endif //__InputEvent_H__