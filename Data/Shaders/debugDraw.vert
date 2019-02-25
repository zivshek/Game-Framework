
attribute vec3 a_Position;
uniform vec4 u_Color;
varying vec4 v_Color;

uniform mat4 u_matWorld;
uniform mat4 u_matView;
uniform mat4 u_matProj;

void main()
{
	// Store object space position.
	vec4 objectpos = vec4(a_Position, 1);

	// Calculate world space position.
	vec4 worldpos = u_matWorld * objectpos;

	// Calculate view space position.
	vec4 viewpos = u_matView * worldpos;

	// Calculate clip space position.
	vec4 clippos = u_matProj * viewpos;

	// Store final position in gl_Position
    gl_Position = clippos;

	v_Color = u_Color;
}
