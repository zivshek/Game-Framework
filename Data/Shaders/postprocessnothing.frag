
uniform sampler2D u_Texture;
varying vec2 v_UV;

void main()
{
	vec4 finalcolor = texture2D( u_Texture, v_UV );

	gl_FragColor = finalcolor;
}
