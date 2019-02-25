#include "GamePCH.h"

#if _DEBUG
#pragma comment( lib, "../Output/Framework/Win32/Debug/Framework.lib" )
#pragma comment( lib, "../Output/Box2D/Win32/Debug/Box2D.lib" )
#pragma comment( lib, "../Output/BulletCollision/Win32/Debug/BulletCollision_Debug.lib" )
#pragma comment( lib, "../Output/BulletDynamics/Win32/Debug/BulletDynamics_Debug.lib" )
#pragma comment( lib, "../Output/LinearMath/Win32/Debug/LinearMath_Debug.lib" )

#else
#pragma comment( lib, "../Output/Framework/Win32/Release/Framework.lib" )
#pragma comment( lib, "../Output/Box2D/Win32/Release/Box2D.lib" )
#pragma comment( lib, "../Output/BulletCollision/Win32/Release/BulletCollision_Release.lib" )
#pragma comment( lib, "../Output/BulletDynamics/Win32/Release/BulletDynamics_Release.lib" )
#pragma comment( lib, "../Output/LinearMath/Win32/Release/LinearMath_Release.lib" )
#endif
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "XAudio2.lib" )

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MyMemory_ForgetAllPreviousAllocations();

    Framework* pFramework = new Framework();

    GameCore* pGame = new Game( pFramework );

    pFramework->Init( SCREEN_WIDTH, SCREEN_HEIGHT );
    pFramework->Run( pGame );
    pFramework->Shutdown();

    delete( pGame );
    delete( pFramework );

    MyMemory_ValidateAllocations( true );
}
