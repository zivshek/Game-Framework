#ifndef __FrameworkPCH_H__
#define __FrameworkPCH_H__

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>

#include <map>
#include <vector>

#include <GL/GL.h>
#include "GL/glext.h"
#include "GL/wglext.h"

#include "../Libraries/imgui/imgui.h"
#include "../Libraries/Box2D/Box2D/Box2D.h"
#include "../Libraries/lodepng/lodepng.h"
#include "../Libraries/cJSON/cJSON.h"

#include "Utility/Helpers.h"
#include "Utility/ShaderProgram.h"

#include "Math/MathHelpers.h"
#include "Math/Vector.h"
#include "Math/MyMatrix.h"

#include "GL/GLHelpers.h"
#include "GL/GLExtensions.h"
#include "GL/WGLExtensions.h"

typedef Vector2 vec2;
typedef Vector3 vec3;
typedef Vector4 vec4;
typedef Vector2Int ivec2;
typedef Vector3Int ivec3;
typedef Vector4Int ivec4;
typedef MyMatrix mat4;

#include "Events/Event.h"
#include "Events/InputEvent.h"
#include "GameCore/GameCore.h"
#include "FrameworkMain.h"

#endif //__FrameworkPCH_H__
