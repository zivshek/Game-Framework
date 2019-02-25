#include "GamePCH.h"

PlayerController::PlayerController()
{
    Clear();
	m_MousePos = vec2(400, 300);
    m_MouseSensitivity = 0.02f;
}

PlayerController::~PlayerController()
{
}

void PlayerController::Clear()
{
    m_ButtonFlags = 0;
}

void PlayerController::SetButtonPressed(PlayerControllerButtons button)
{
    m_ButtonFlags |= button;
}

void PlayerController::SetButtonReleased(PlayerControllerButtons button)
{
    m_ButtonFlags &= ~button;
}

bool PlayerController::IsButtonPressed(PlayerControllerButtons button)
{
    if( m_ButtonFlags & button )
        return true;

    return false;
}

void PlayerController::OnEvent(Event* pEvent)
{
    if (pEvent->GetEventType() == EventType_Input)
    {
        InputEvent* pInputEvent = (InputEvent*)pEvent;

        if (pInputEvent->GetInputDeviceType() == InputDeviceType_Keyboard)
        {
            if (pInputEvent->GetInputState() == InputState_Pressed)
            {
                if (pInputEvent->GetID() == VK_UP || pInputEvent->GetID() == 'W')
                    SetButtonPressed(PCB_Forward);
                if (pInputEvent->GetID() == VK_DOWN || pInputEvent->GetID() == 'S')
                    SetButtonPressed(PCB_Backward);
                if (pInputEvent->GetID() == VK_LEFT || pInputEvent->GetID() == 'A')
                    SetButtonPressed(PCB_Left);
                if (pInputEvent->GetID() == VK_RIGHT || pInputEvent->GetID() == 'D')
                    SetButtonPressed(PCB_Right);
				if (pInputEvent->GetID() == 'E')
					SetButtonPressed(PCB_Interact);
            }

            if (pInputEvent->GetInputState() == InputState_Released)
            {
                if (pInputEvent->GetID() == VK_UP || pInputEvent->GetID() == 'W')
                    SetButtonReleased(PCB_Forward);
                if (pInputEvent->GetID() == VK_DOWN || pInputEvent->GetID() == 'S')
                    SetButtonReleased(PCB_Backward);
                if (pInputEvent->GetID() == VK_LEFT || pInputEvent->GetID() == 'A')
                    SetButtonReleased(PCB_Left);
                if (pInputEvent->GetID() == VK_RIGHT || pInputEvent->GetID() == 'D')
                    SetButtonReleased(PCB_Right);
				if (pInputEvent->GetID() == 'E')
					SetButtonReleased(PCB_Interact);
            }
        }

        if (pInputEvent->GetInputDeviceType() == InputDeviceType_Mouse)
        {
            if (pInputEvent->GetInputState() == InputState_Moved)
            {
               m_MousePos = pInputEvent->GetPosition();
            }
        }
    }
}