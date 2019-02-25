//
// Copyright (c) 2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "FrameworkPCH.h"

// Initialize opengl window on windows, huge chunks taken from nehe
//    http://nehe.gamedev.net/tutorial/creating_an_opengl_window_%28win32%29/13001/

// Public methods

Framework::Framework()
{
    m_pGameCore = 0;

    // We now only quit the game when there is no active scenes
    m_EscapeButtonWillQuit = false;
    m_CloseProgramRequested = false;

    m_InitialWidth = -1;
    m_InitialHeight = -1;

    m_CurrentWidth = -1;
    m_CurrentHeight = -1;

    m_hWnd = 0;
    m_hRenderingContext = 0;
    m_hDeviceContext = 0;
    m_hInstance = 0;

    // Horrid key handling
    for( int i=0; i<256; i++ )
        m_KeyStates[i] = false;

    for( int i=0; i<3; i++ )
        m_MouseButtonStates[i] = false;

    m_WindowIsActive = false;
    m_FullscreenMode = false;
}

void Framework::Init(int width, int height)
{
    m_InitialWidth = width;
    m_InitialHeight = height;

    m_CurrentWidth = m_InitialWidth;
    m_CurrentHeight = m_InitialHeight;

    // Create Our OpenGL Window
    if( !CreateGLWindow( "OpenGL Window", width, height, 32, 31, 1, false ) )
    {
        return;
    }

    // Initialize OpenGL Extensions, must be done after OpenGL Context is created
    OpenGL_InitExtensions();
}

int Framework::Run(GameCore* pGameCore)
{
    m_pGameCore = pGameCore;
    m_pGameCore->OnSurfaceChanged( m_CurrentWidth, m_CurrentHeight );
    m_pGameCore->LoadContent();

    double lasttime = GetSystemTime();

    // Main loop
    MSG msg;
    bool done = false;

    while( !done )
    {
		ShowFPS(m_hWnd);

        if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
        {
            if( msg.message == WM_QUIT )
            {
                done = true;
            }
            else
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
        }
        else
        {
            double currtime = GetSystemTime();
            float deltatime = (float)(currtime - lasttime);
            lasttime = currtime;

            if( m_WindowIsActive )
            {
                if( m_CloseProgramRequested )
                {
                    done = true;
                }
                else
                {
                    m_pGameCore->Update( deltatime );

                    m_pGameCore->Draw();

                    SwapBuffers( m_hDeviceContext );
                }
            }
        }
    }

    return msg.wParam;
}

void Framework::Shutdown()
{
    KillGLWindow();
}

void Framework::SetWindowSize(int width, int height)
{
    int maxwidth = GetSystemMetrics( SM_CXFULLSCREEN );
    int maxheight = GetSystemMetrics( SM_CYFULLSCREEN );

    float aspect = (float)width / height;

    if( width > maxwidth )
    {
        width = maxwidth;
        height = (int)(maxwidth / aspect);
    }

    if( height > maxheight )
    {
        width = (int)(maxheight * aspect);
        height = maxheight;
    }

    DWORD dwStyle = GetWindowLongPtr( m_hWnd, GWL_STYLE ) ;
    DWORD dwExStyle = GetWindowLongPtr( m_hWnd, GWL_EXSTYLE ) ;
    HMENU menu = GetMenu( m_hWnd ) ;

    // Calculate the full size of the window needed to match our client area of width/height
    RECT WindowRect = { 0, 0, width, height } ;
    AdjustWindowRectEx( &WindowRect, dwStyle, menu ? TRUE : FALSE, dwExStyle );

    int windowwidth = WindowRect.right - WindowRect.left;
    int windowheight = WindowRect.bottom - WindowRect.top;

    SetWindowPos( m_hWnd, 0, 0, 0, windowwidth, windowheight, SWP_NOZORDER | SWP_NOMOVE ) ;

    ResizeWindow( width, height );
}

bool Framework::IsKeyDown(int value)
{
    assert( value >= 0 && value < 256 );
    return m_KeyStates[value];
}

bool Framework::IsMouseButtonDown(int id)
{
    assert( id >= 0 && id < 3 );
    return m_MouseButtonStates[id];
}

void Framework::GetMouseCoordinates(int* mx, int* my)
{
    POINT p;
    if( GetCursorPos( &p ) )
    {
        if( ScreenToClient( m_hWnd, &p ) )
        {
            *mx = p.x;
            *my = p.y;
        }
    }
}

void Framework::SetCursorPosition(int x, int y)
{
    POINT p;
    p.x = x;
    p.y = y;
    ClientToScreen(m_hWnd, &p);
	SetCursorPos(p.x, p.y);
}

void Framework::ShowCursorWrapper(bool show)
{
    ShowCursor(show);
}

void Framework::RequestCloseProgram()
{
    m_CloseProgramRequested = true;
}

// Protected methods
void Framework::ResizeWindow(int width, int height)
{
    if( height <= 0 ) height = 1;
    if( width <= 0 ) width = 1;

    m_CurrentWidth = width;
    m_CurrentHeight = height;

    if( m_pGameCore )
        m_pGameCore->OnSurfaceChanged( width, height );
}

bool Framework::CreateGLWindow(char* title, int width, int height, char colorbits, char zbits, char stencilbits, bool fullscreenflag)
{
    GLuint PixelFormat;

    WNDCLASS wc;
    DWORD dwExStyle;
    DWORD dwStyle;

    RECT WindowRect;
    WindowRect.left = (long)0;
    WindowRect.right = (long)width;
    WindowRect.top = (long)0;
    WindowRect.bottom = (long)height;

    m_FullscreenMode = fullscreenflag;

    m_hInstance = GetModuleHandle( 0 );             // Grab an instance for our window
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // Redraw on move, and own dc for window
    wc.lpfnWndProc = (WNDPROC)WndProc;              // Wndproc handles messages
    wc.cbClsExtra = 0;                              // No extra window data
    wc.cbWndExtra = 0;                              // No extra window data
    wc.hInstance = m_hInstance;                     // Set the instance
    wc.hIcon = LoadIcon( 0, IDI_WINLOGO );          // Load the default icon
    wc.hCursor = LoadCursor( 0, IDC_ARROW );        // Load the arrow pointer
    wc.hbrBackground = 0;                           // No background required for GL
    wc.lpszMenuName = 0;                            // We don't want a menu
    wc.lpszClassName = "OpenGL";                    // Set the class name

	ShowCursor(false);
	SetCursorPosition(400, 300);

    if( !RegisterClass( &wc ) )                     // Attempt to register the window class
    {
        MessageBox( 0, "Failed To Register The Window Class.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return false;
    }

    if( m_FullscreenMode )
    {
        DEVMODE dmScreenSettings;                                   // Device mode
        memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) ); // Makes sure memory's cleared
        dmScreenSettings.dmSize = sizeof( dmScreenSettings );       // Size of the devmode structure
        dmScreenSettings.dmPelsWidth  = width;                      // Selected screen width
        dmScreenSettings.dmPelsHeight = height;                     // Selected screen height
        dmScreenSettings.dmBitsPerPel = colorbits;                  // Selected bits per pixel
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Try to set selected mode and get results.  NOTE: CDS_FULLSCREEN gets rid of start bar.
        if( ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
        {
            // If the mode fails, offer two options.  Quit or run in a window.
            if( MessageBox( 0, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "", MB_YESNO|MB_ICONEXCLAMATION ) == IDYES )
            {
                m_FullscreenMode = false;
            }
            else
            {
                MessageBox( 0, "Program Will Now Close.", "ERROR", MB_OK|MB_ICONSTOP );
                return false;
            }
        }
    }

    if( m_FullscreenMode )
    {
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle = WS_POPUP;
        ShowCursor( false );
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle = WS_OVERLAPPEDWINDOW;
    }

    AdjustWindowRectEx( &WindowRect, dwStyle, false, dwExStyle );   // Adjust window to true requested size

    if( !( CreateWindowEx( dwExStyle,           // Extended style for the window
        "OpenGL",                               // Class name
        title,                                  // Window title
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN |     // Required window style
        dwStyle,                                // Selected window style
        0, 0,                                   // Window position
        WindowRect.right-WindowRect.left,       // Calculate adjusted window width
        WindowRect.bottom-WindowRect.top,       // Calculate adjusted window height
        0,                                      // No parent window
        0,                                      // No menu
        m_hInstance,                            // Instance
        this)))                                 // Pass a pointer to this framework object to WM_NCCREATE
    {
        KillGLWindow();
        MessageBox( 0, "Window Creation Error.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return false;
    }

    PIXELFORMATDESCRIPTOR pfd =  // pfd tells Windows how we want things to be
    {
        sizeof(PIXELFORMATDESCRIPTOR),  // Size of this pixel format descriptor
        1,                              // Version number
        PFD_DRAW_TO_WINDOW |            // Format must support window
        PFD_SUPPORT_OPENGL |            // Format must support opengl
        PFD_DOUBLEBUFFER,               // Must support double buffering
        PFD_TYPE_RGBA,                  // Request an rgba format
        (BYTE)colorbits,                // Select our color depth
        0, 0, 0, 0, 0, 0,               // Color bits ignored
        0,                              // No alpha buffer
        0,                              // Shift bit ignored
        0,                              // No accumulation buffer
        0, 0, 0, 0,                     // Accumulation bits ignored
        (BYTE)zbits,                    // Bits for z-buffer (depth buffer)
        (BYTE)stencilbits,              // Stencil bits
        0,                              // No auxiliary buffer
        PFD_MAIN_PLANE,                 // Main drawing layer
        0,                              // Reserved
        0, 0, 0                         // Layer masks ignored
    };

    if( !( m_hDeviceContext = GetDC( m_hWnd ) ) ) // Did we get a device context?
    {
        KillGLWindow();
        MessageBox( 0, "Can't Create A GL Device Context.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return 0;
    }

    if( !( PixelFormat = ChoosePixelFormat( m_hDeviceContext, &pfd ) ) ) // Did Windows find a matching pixel format?
    {
        KillGLWindow();
        MessageBox( 0, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return 0;
    }

    if( !SetPixelFormat( m_hDeviceContext, PixelFormat, &pfd ) ) // Are we able to set the pixel format?
    {
        KillGLWindow();
        MessageBox( 0, "Can't Set The PixelFormat.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return 0;
    }

    if( !( m_hRenderingContext = wglCreateContext( m_hDeviceContext ) ) ) // Are we able to get a rendering context?
    {
        KillGLWindow();
        MessageBox( 0, "Can't Create A GL Rendering Context.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return 0;
    }

    if( !wglMakeCurrent( m_hDeviceContext, m_hRenderingContext ) ) // Try to activate the rendering context.
    {
        KillGLWindow();
        MessageBox( 0, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
        return 0;
    }

    ShowWindow( m_hWnd, SW_SHOW );   // Show the window.
    SetForegroundWindow( m_hWnd );   // Slightly higher priority.
    SetFocus( m_hWnd );              // Sets keyboard focus to the window.
    ResizeWindow( width, height );   // Tells our GameCore object the window size.

    return true;
}

void Framework::KillGLWindow()
{
    if( m_FullscreenMode )
    {
        ChangeDisplaySettings( 0, 0 );
        ShowCursor( true );
    }

    if( m_hRenderingContext )
    {
        if( !wglMakeCurrent( 0, 0 ) )
        {
            MessageBox( 0, "Release Of Device Context And Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        }

        if( !wglDeleteContext( m_hRenderingContext ) )
        {
            MessageBox( 0, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        }

        m_hRenderingContext = 0;
    }

    if( m_hDeviceContext && !ReleaseDC( m_hWnd, m_hDeviceContext ) )
    {
        MessageBox( 0, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        m_hDeviceContext = 0;
    }

    if( m_hWnd && !DestroyWindow( m_hWnd ) )
    {
        MessageBox( 0, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        m_hWnd = 0;
    }

    if( !UnregisterClass( "OpenGL", m_hInstance ) )
    {
        MessageBox( 0, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        m_hInstance = 0;
    }
}

// This is a static method.
LRESULT CALLBACK Framework::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Get a pointer to the framework object associated with this window.
    Framework* pFramework = (Framework*)GetWindowLongPtr( hWnd, GWLP_USERDATA );

    switch( uMsg )
    {
    case WM_NCCREATE:
        {
            // Set the user data for this hWnd to the Framework* we passed in, used on first line of this method above.
            CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
            Framework* pFramework = (Framework*)pcs->lpCreateParams;
            SetWindowLongPtr( hWnd, GWLP_USERDATA, (LONG)pFramework );

            pFramework->m_hWnd = hWnd;
        }
        return 1;

    case WM_DESTROY:
        {
            pFramework->m_hWnd = 0;
        }
        return 0;

    case WM_ACTIVATE:
        {
            if( !HIWORD(wParam) )
            {
                pFramework->m_WindowIsActive = true;
            }
            else
            {
                pFramework->m_WindowIsActive = false;
            } 
        }
        return 0;

    case WM_SYSCOMMAND:
        {
            switch( wParam )
            {
            // Don't let screensaver or monitor power save mode kick in.
            case SC_SCREENSAVE:
            case SC_MONITORPOWER:
                return 0;
            }
        }
        break;

    case WM_CLOSE:
        {
            PostQuitMessage(0);
        }
        return 0;

    case WM_KEYDOWN:
        {
			bool keyWasPressedLastTimeMessageArrived = lParam & (1 << 30);

			if( keyWasPressedLastTimeMessageArrived == false )
			{
				if( wParam == VK_ESCAPE && pFramework->m_EscapeButtonWillQuit )
					pFramework->m_CloseProgramRequested = true;

				pFramework->m_KeyStates[wParam] = true;

				InputEvent* pEvent = new InputEvent( InputDeviceType_Keyboard, InputState_Pressed, wParam, vec2(0,0) );
				pFramework->m_pGameCore->QueueEvent( pEvent );
			}
        }
        return 0;

    case WM_KEYUP:
        {
            pFramework->m_KeyStates[wParam] = false;

            InputEvent* pEvent = new InputEvent( InputDeviceType_Keyboard, InputState_Released, wParam, vec2(0, 0));
            pFramework->m_pGameCore->QueueEvent( pEvent );
        }
        return 0;

    case WM_MOUSEMOVE:
        {
            int x = GET_X_LPARAM( lParam );
            int y = GET_Y_LPARAM( lParam );
    
            InputEvent* pEvent = new InputEvent( InputDeviceType_Mouse, InputState_Moved, -1, vec2((float)x, (float)y) );
			pFramework->m_pGameCore->QueueEvent(pEvent);			
        }
        return 0;

    case WM_LBUTTONDOWN:
        {
            pFramework->m_MouseButtonStates[0] = true;

            int x = GET_X_LPARAM( lParam );
            int y = GET_Y_LPARAM( lParam );

            InputEvent* pEvent = new InputEvent( InputDeviceType_Mouse, InputState_Pressed, 0, vec2((float)x, (float)y) );
            pFramework->m_pGameCore->QueueEvent( pEvent );
        }
        return 0;

    case WM_LBUTTONUP:
        {
            pFramework->m_MouseButtonStates[0] = false;

            int x = GET_X_LPARAM( lParam );
            int y = GET_Y_LPARAM( lParam );

            InputEvent* pEvent = new InputEvent( InputDeviceType_Mouse, InputState_Released, 0, vec2((float)x, (float)y) );
            pFramework->m_pGameCore->QueueEvent( pEvent );
        }
        return 0;

    case WM_SIZE:
        {
            pFramework->ResizeWindow( LOWORD(lParam), HIWORD(lParam) );
        }
        return 0;
    }

    // Pass all unhandled messages to DefWindowProc
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

void Framework::ShowFPS(HWND hWnd)
{
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = GetSystemTime();

	elapsedSeconds = currentSeconds - previousSeconds;

	// Only update fps every 1/4s
	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;

		std::string title;
		title += "Eternal Darkness    -    ";
		title += std::to_string((int)fps);
		title += " fps";

		title += "    -    ";
		title += "GL Version: ";
		title += (char*)glGetString(GL_VERSION);

		SetWindowText(m_hWnd, title.c_str());
		frameCount = 0;
	}
	frameCount++;
}