#pragma once

class GameObject;

class GameEvent_Destroy : public Event
{
protected:
	GameObject * m_pGameObject;

public:
	GameEvent_Destroy(GameObject* gameobject)
	{
		m_EventType = GameEventType_Destroy;
		m_pGameObject = gameobject;
	}

	GameObject* GetGameObject() { return m_pGameObject; }
};