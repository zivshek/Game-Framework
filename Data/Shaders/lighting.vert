
attribute vec3 a_Position;
attribute vec2 a_UV;
attribute vec4 a_Color;
attribute vec3 a_Normal;

uniform mat4 u_WorldMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;

varying vec2 v_UV;
varying vec3 v_Normal;
varying vec3 v_SurfaceWorldPosition;
varying vec3 v_ViewWorldPosition;

void main()
{
    vec4 objectpos = vec4(a_Position, 1);
    vec4 worldpos = u_WorldMatrix * objectpos;
	v_SurfaceWorldPosition = worldpos.xyz;
    vec4 viewpos = u_ViewMatrix * worldpos;
	v_ViewWorldPosition = viewpos.xyz;
    vec4 clippos = u_ProjMatrix * viewpos;

    gl_Position = clippos;

    v_UV = a_UV;

	//v_Normal = mat3(transpose(inverse(u_WorldMatrix))) * a_Normal;
	v_Normal = u_WorldMatrix * vec4( a_Normal, 0 );
}
