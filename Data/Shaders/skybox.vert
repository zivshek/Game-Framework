
attribute vec3 a_Position;

uniform mat4 u_WorldMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;

varying vec3 v_UV;

void main()
{
    vec4 objectpos = vec4(a_Position, 1);

	vec4 worldpos = u_WorldMatrix * objectpos;

    vec4 viewpos = u_ViewMatrix * vec4(objectpos.xyz, 0);
    vec4 clippos = u_ProjMatrix * vec4(viewpos.xyz, 1);

    gl_Position = clippos;

    v_UV = worldpos.xyz;
}
