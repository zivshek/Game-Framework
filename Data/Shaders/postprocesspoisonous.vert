
attribute vec2 a_Position;
attribute vec2 a_UV;
attribute vec3 a_Color;

varying vec2 v_UV;

void main()
{
	// Since our mesh is only 1 * 1, we need to multiply it by 2 to fit into the opengl clip space
	vec4 objectpos = vec4(a_Position * 2, 1, 1);

	// Store final position in gl_Position
    gl_Position = objectpos;

	v_UV = a_UV;
}
