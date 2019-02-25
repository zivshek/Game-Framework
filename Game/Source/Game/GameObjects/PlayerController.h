#ifndef __PlayerController_H__
#define __PlayerController_H__

enum PlayerControllerButtons
{
    PCB_Forward  = 0x00000001,
    PCB_Backward = 0x00000002,
    PCB_Left  =    0x00000004,
    PCB_Right =    0x00000008,
	PCB_Interact = 0x00000010
};

class PlayerController
{
protected:
    unsigned int m_ButtonFlags;

    vec2 m_MousePos;
    float m_MouseSensitivity;

public:
    PlayerController();
    virtual ~PlayerController();

    void Clear();

    void SetButtonPressed(PlayerControllerButtons button);
    void SetButtonReleased(PlayerControllerButtons button);

    bool IsButtonPressed(PlayerControllerButtons button);

    inline vec2 GetMousePosition()const { return m_MousePos; }

    void OnEvent(Event* pEvent);
};

#endif //__PlayerController_H__
