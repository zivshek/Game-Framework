#ifndef __GamePCH_H__
#define __GamePCH_H__

#include "../../Libraries/Framework/Source/FrameworkPCH.h"

#pragma warning( push )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4263 )
#pragma warning( disable : 4264 )
#pragma warning( disable : 4640 )
#pragma warning( disable : 4305 )
#include "../../Libraries/bullet3/src/btBulletDynamicsCommon.h"
#include "Game/Base/MyDebugDraw.h"
#pragma warning( pop )
#include <XAudio2.h>

#include <queue>

#include "Physics/BulletWorld.h"

#include "Common/Memory/CPPList.h"
#include "Common/Memory/Memory.h"

#include "Game/Audio/WaveLoader.h"
#include "Game/Audio/SoundPlayerXAudio.h"
#include "Game/Audio/SoundQueue.h"
#include "Common/Main/ImGuiManager.h"
#include "Game/Base/EulerAngle.h"
#include "Game/Mesh/VertexFormat.h"
#include "Game/Mesh/Material.h"
#include "Game/GameObjects/Light.h"
#include "Game/Mesh/Mesh.h"

#include "ParticleSystem/Particle.h"
#include "ParticleSystem/ParticleEmitter.h"
#include "ParticleSystem/ParticleRenderer.h"


#include "Game/GameObjects/PlayerController.h"
#include "Game/GameObjects/GameObject.h"
#include "Game/GameObjects/CameraObject.h"
#include "Game/GameObjects/PlayerObject.h"

#include "Game/GameObjects/Interactable.h"
#include "Game/GameObjects/Pickup.h"
#include "Game/GameObjects/Pickup_Apple.h"
#include "Game/GameObjects/Pickup_Mushroom.h"
#include "Game/GameObjects/Breakable.h"
#include "Game/GameObjects/Breakable_Tree.h"
#include "Game/Events/GameEvents.h"
#include "Game/Events/GameEvent_Destroy.h"

#include "Game/Base/FBODefinition.h"
#include "Game/Base/ResourceManager.h"
#include "Game/Base/SceneManager.h"
#include "Game/Base/Scene.h"
#include "Game/Base/TweenFuncs.h"
#include "Game/Base/TweenObject.h"
#include "Game/Scenes/Scene_MainMenu.h"
#include "Game/Scenes/GameScene_Level1.h"
#include "Game/Scenes/GameScene_Level2.h"
#include "Game/Scenes/Scene_Option.h"
#include "Game/Scenes/Scene_Hud.h"
#include "Game/Scenes/Scene_Inventory.h"
#include "Game/Scenes/Dead_Scene.h"
#include "Game/Scenes/Scene_Level.h"
#include "Game/Base/TextRenderer.h"
#include "Game/Base/Game.h"

#endif //__GamePCH_H__
