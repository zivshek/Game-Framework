
uniform Material u_Material;
uniform vec4 u_AddColor;

varying vec2 v_UV;

void main()
{
	gl_FragColor = texture2D( u_Material.texture, v_UV ) + u_AddColor;
}
