
attribute vec2 a_Position;
attribute vec2 a_UV;

uniform vec2 u_ScreenSize;

varying vec2 v_UV;

void main()
{
	vec2 finalPos = a_Position - u_ScreenSize / 2; 
	finalPos /= u_ScreenSize/2;

	gl_Position =  vec4(finalPos,0,1);
	
	v_UV = a_UV;
}

