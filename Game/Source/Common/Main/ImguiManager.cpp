//
// Copyright (c) 2016-2018 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GamePCH.h"

ImGuiManager* g_pImGuiManager = 0;

ImGuiManager::ImGuiManager()
{
    m_FontTexture = 0;
    m_ShaderHandle = 0; m_VertHandle = 0; m_FragHandle = 0;
    m_AttribLocationTex = 0; m_AttribLocationProjMtx = 0;
    m_AttribLocationPosition = 0; m_AttribLocationUV = 0; m_AttribLocationColor = 0;
    m_VboHandle = 0; m_VaoHandle = 0; m_ElementsHandle = 0;
}

ImGuiManager::~ImGuiManager()
{
    Shutdown();
}

void ImGuiManager::Init()
{
    //ImGui::StyleColorsClassic();

    CreateDeviceObjects();

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = 300;
    io.DisplaySize.y = 300;
    //io.IniFilename = "imgui.ini";
    
    // Set no render callback, we manually render in Draw.
    io.RenderDrawListsFn = 0;

    // Keyboard mapping.  ImGui will use those indices to peek into the io.KeyDown[] array.
    io.KeyMap[ImGuiKey_Tab] = VK_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
    io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
    io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
    io.KeyMap[ImGuiKey_Home] = VK_HOME;
    io.KeyMap[ImGuiKey_End] = VK_END;
    io.KeyMap[ImGuiKey_Insert] = VK_INSERT;
    io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
    io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
    io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
    io.KeyMap[ImGuiKey_A] = 'A';
    io.KeyMap[ImGuiKey_C] = 'C';
    io.KeyMap[ImGuiKey_V] = 'V';
    io.KeyMap[ImGuiKey_X] = 'X';
    io.KeyMap[ImGuiKey_Y] = 'Y';
    io.KeyMap[ImGuiKey_Z] = 'Z';
}

void ImGuiManager::Shutdown()
{
    ImGui::Shutdown();
}

void ImGuiManager::ClearInput()
{
    ImGuiIO& io = ImGui::GetIO();

    for( int i=0; i<5; i++ )
    {
        io.MouseDown[i] = false;
    }

    for( int i=0; i<512; i++ )
    {
        io.KeysDown[i] = false;
    }

    io.KeyCtrl = false;
    io.KeyShift = false;
    io.KeyAlt = false;
    io.KeySuper = false;
}

void ImGuiManager::OnFocusLost()
{
    ClearInput();
}

void ImGuiManager::OnEvent(Event* pEvent)
{
    ImGuiIO& io = ImGui::GetIO();

    if( pEvent->GetEventType() == EventType_Input )
    {
        InputEvent* pInputEvent = (InputEvent*)pEvent;

        if( pInputEvent->GetInputDeviceType() == InputDeviceType_Keyboard )
        {
            if( pInputEvent->GetInputState() == InputState_Pressed )
            {
                io.KeysDown[pInputEvent->GetID()] = true;
            }

            if( pInputEvent->GetInputState() == InputState_Released )
            {
                io.KeysDown[pInputEvent->GetID()] = false;
            }
        }

        if( pInputEvent->GetInputDeviceType() == InputDeviceType_Mouse )
        {
            io.MousePos.x = pInputEvent->GetPosition().x;
            io.MousePos.y = pInputEvent->GetPosition().y;

            if( pInputEvent->GetInputState() == InputState_Pressed )
            {
                io.MouseDown[pInputEvent->GetID()] = true;
            }
            if( pInputEvent->GetInputState() == InputState_Released )
            {
                io.MouseDown[pInputEvent->GetID()] = false;
            }
        }
    }

    //io.AddInputCharacter( (unsigned short)c );
}

void ImGuiManager::StartFrame(float width, float height, double TimePassed)
{
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = (float)TimePassed;
    io.DisplaySize.x = width;
    io.DisplaySize.y = height;

    io.KeyCtrl = io.KeysDown[VK_CONTROL] || io.KeysDown[VK_LCONTROL] || io.KeysDown[VK_RCONTROL];
    io.KeyShift = io.KeysDown[VK_SHIFT] || io.KeysDown[VK_LSHIFT] || io.KeysDown[VK_RSHIFT];
    io.KeyAlt = io.KeysDown[VK_MENU] || io.KeysDown[VK_LMENU] || io.KeysDown[VK_RMENU];
    io.KeySuper = io.KeysDown[VK_LWIN] || io.KeysDown[VK_RWIN];

    //ImGui::ShowDemoWindow();
}

void ImGuiManager::EndFrame()
{
    ImGui::Render();
    ImDrawData* data = ImGui::GetDrawData();
    RenderDrawLists( data );
}

// This is the main rendering function
// If text or lines are blurry when integrating ImGui in your engine:
// - in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
void ImGuiManager::RenderDrawLists(ImDrawData* draw_data)
{
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    ImGuiIO& io = ImGui::GetIO();
    int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if( fb_width == 0 || fb_height == 0 )
        return;

    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    // Backup GL state
    GLenum last_active_texture; glGetIntegerv( GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture );
    glActiveTexture( GL_TEXTURE0 );
    GLint last_program; glGetIntegerv( GL_CURRENT_PROGRAM, &last_program );
    GLint last_texture; glGetIntegerv( GL_TEXTURE_BINDING_2D, &last_texture );
    //GLint last_sampler; glGetIntegerv( GL_SAMPLER_BINDING, &last_sampler );
    GLint last_array_buffer; glGetIntegerv( GL_ARRAY_BUFFER_BINDING, &last_array_buffer );
    GLint last_element_array_buffer; glGetIntegerv( GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer );
    GLint last_vertex_array; glGetIntegerv( GL_VERTEX_ARRAY_BINDING, &last_vertex_array );
    GLint last_polygon_mode[2]; glGetIntegerv( GL_POLYGON_MODE, last_polygon_mode );
    GLint last_viewport[4]; glGetIntegerv( GL_VIEWPORT, last_viewport );
    GLint last_scissor_box[4]; glGetIntegerv( GL_SCISSOR_BOX, last_scissor_box );
    GLenum last_blend_src_rgb; glGetIntegerv( GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb );
    GLenum last_blend_dst_rgb; glGetIntegerv( GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb );
    GLenum last_blend_src_alpha; glGetIntegerv( GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha );
    GLenum last_blend_dst_alpha; glGetIntegerv( GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha );
    GLenum last_blend_equation_rgb; glGetIntegerv( GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb );
    GLenum last_blend_equation_alpha; glGetIntegerv( GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha );
    GLboolean last_enable_blend = glIsEnabled( GL_BLEND );
    GLboolean last_enable_cull_face = glIsEnabled( GL_CULL_FACE );
    GLboolean last_enable_depth_test = glIsEnabled( GL_DEPTH_TEST );
    GLboolean last_enable_scissor_test = glIsEnabled( GL_SCISSOR_TEST );

    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
    glEnable( GL_BLEND );
    //glBlendEquation( GL_FUNC_ADD );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glDisable( GL_CULL_FACE );
    glDisable( GL_DEPTH_TEST );
    glEnable( GL_SCISSOR_TEST );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    // Setup viewport, orthographic projection matrix
    glViewport( 0, 0, (GLsizei)fb_width, (GLsizei)fb_height );
    const float ortho_projection[4][4] =
    {
        { 2.0f/io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
        { 0.0f,                  2.0f/-io.DisplaySize.y, 0.0f, 0.0f },
        { 0.0f,                  0.0f,                  -1.0f, 0.0f },
        {-1.0f,                  1.0f,                   0.0f, 1.0f },
    };
    glUseProgram( m_ShaderHandle );
    glUniform1i( m_AttribLocationTex, 0 );
    glUniformMatrix4fv( m_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0] );
    glBindVertexArray( m_VaoHandle );
    //glBindSampler( 0, 0 ); // Rely on combined texture/sampler state.

    for( int n = 0; n < draw_data->CmdListsCount; n++ )
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawIdx* idx_buffer_offset = 0;

        glBindBuffer( GL_ARRAY_BUFFER, m_VboHandle );
        glBufferData( GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ElementsHandle );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW );

        for( int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++ )
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if( pcmd->UserCallback )
            {
                pcmd->UserCallback( cmd_list, pcmd );
            }
            else
            {
                glBindTexture( GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId );
                glScissor( (int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y) );
                glDrawElements( GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset );
            }
            idx_buffer_offset += pcmd->ElemCount;
        }
    }

    // Restore modified GL state
    glUseProgram( last_program );
    glBindTexture( GL_TEXTURE_2D, last_texture );
    //glBindSampler( 0, last_sampler );
    glActiveTexture( last_active_texture );
    glBindVertexArray( last_vertex_array );
    glBindBuffer( GL_ARRAY_BUFFER, last_array_buffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer );
    //glBlendEquationSeparate( last_blend_equation_rgb, last_blend_equation_alpha );
    glBlendFuncSeparate( last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha );
    if( last_enable_blend ) glEnable( GL_BLEND ); else glDisable( GL_BLEND );
    if( last_enable_cull_face ) glEnable( GL_CULL_FACE ); else glDisable( GL_CULL_FACE );
    if( last_enable_depth_test ) glEnable( GL_DEPTH_TEST ); else glDisable( GL_DEPTH_TEST );
    if( last_enable_scissor_test ) glEnable( GL_SCISSOR_TEST ); else glDisable( GL_SCISSOR_TEST );
    glPolygonMode( GL_FRONT_AND_BACK, last_polygon_mode[0] );
    glViewport( last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3] );
    glScissor( last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3] );
}

bool ImGuiManager::CreateFontsTexture()
{
    // Build texture atlas
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int width, height;
    // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.
    io.Fonts->GetTexDataAsRGBA32( &pixels, &width, &height );

    // Upload texture to graphics system
    GLint last_texture;
    glGetIntegerv( GL_TEXTURE_BINDING_2D, &last_texture );
    glGenTextures( 1, &m_FontTexture);
    glBindTexture( GL_TEXTURE_2D, m_FontTexture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

    // Store our identifier
    io.Fonts->TexID = (void*)m_FontTexture;

    // Restore state
    glBindTexture( GL_TEXTURE_2D, last_texture );

    return true;
}

bool ImGuiManager::CreateDeviceObjects()
{
    // Backup GL state
    GLint last_texture, last_array_buffer, last_vertex_array;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

    const GLchar *vertex_shader =
        "#version 150\n"
        "uniform mat4 ProjMtx;\n"
        "in vec2 Position;\n"
        "in vec2 UV;\n"
        "in vec4 Color;\n"
        "out vec2 Frag_UV;\n"
        "out vec4 Frag_Color;\n"
        "void main()\n"
        "{\n"
        "	Frag_UV = UV;\n"
        "	Frag_Color = Color;\n"
        "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
        "}\n";

    const GLchar* fragment_shader =
        "#version 150\n"
        "uniform sampler2D Texture;\n"
        "in vec2 Frag_UV;\n"
        "in vec4 Frag_Color;\n"
        "out vec4 Out_Color;\n"
        "void main()\n"
        "{\n"
        "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
        "}\n";

    m_ShaderHandle = glCreateProgram();
    m_VertHandle = glCreateShader( GL_VERTEX_SHADER );
    m_FragHandle = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( m_VertHandle, 1, &vertex_shader, 0 );
    glShaderSource( m_FragHandle, 1, &fragment_shader, 0 );
    glCompileShader( m_VertHandle );
    glCompileShader( m_FragHandle );
    glAttachShader( m_ShaderHandle, m_VertHandle );
    glAttachShader( m_ShaderHandle, m_FragHandle );
    glLinkProgram( m_ShaderHandle );

    m_AttribLocationTex = glGetUniformLocation( m_ShaderHandle, "Texture" );
    m_AttribLocationProjMtx = glGetUniformLocation( m_ShaderHandle, "ProjMtx" );
    m_AttribLocationPosition = glGetAttribLocation( m_ShaderHandle, "Position" );
    m_AttribLocationUV = glGetAttribLocation( m_ShaderHandle, "UV" );
    m_AttribLocationColor = glGetAttribLocation( m_ShaderHandle, "Color" );

    glGenBuffers( 1, &m_VboHandle );
    glGenBuffers( 1, &m_ElementsHandle );

    glGenVertexArrays( 1, &m_VaoHandle );
    glBindVertexArray( m_VaoHandle );
    glBindBuffer( GL_ARRAY_BUFFER, m_VboHandle );
    glEnableVertexAttribArray( m_AttribLocationPosition );
    glEnableVertexAttribArray( m_AttribLocationUV );
    glEnableVertexAttribArray( m_AttribLocationColor );

    glVertexAttribPointer( m_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos) );
    glVertexAttribPointer( m_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv) );
    glVertexAttribPointer( m_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col) );

    CreateFontsTexture();

    // Restore modified GL state
    glBindTexture( GL_TEXTURE_2D, last_texture );
    glBindBuffer( GL_ARRAY_BUFFER, last_array_buffer );
    glBindVertexArray( last_vertex_array );

    return true;
}

void ImGuiManager::InvalidateDeviceObjects()
{
    if( m_VaoHandle ) glDeleteVertexArrays( 1, &m_VaoHandle );
    if( m_VboHandle ) glDeleteBuffers( 1, &m_VboHandle );
    if( m_ElementsHandle ) glDeleteBuffers( 1, &m_ElementsHandle );
    m_VaoHandle = m_VboHandle = m_ElementsHandle = 0;

    if( m_ShaderHandle && m_VertHandle ) glDetachShader( m_ShaderHandle, m_VertHandle );
    if( m_VertHandle ) glDeleteShader( m_VertHandle );
    m_VertHandle = 0;

    if( m_ShaderHandle && m_FragHandle ) glDetachShader( m_ShaderHandle, m_FragHandle );
    if( m_FragHandle ) glDeleteShader( m_FragHandle );
    m_FragHandle = 0;

    if( m_ShaderHandle ) glDeleteProgram( m_ShaderHandle );
    m_ShaderHandle = 0;

    if( m_FontTexture )
    {
        glDeleteTextures( 1, &m_FontTexture );
        ImGui::GetIO().Fonts->TexID = 0;
        m_FontTexture = 0;
    }
}
