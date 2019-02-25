
uniform sampler2D u_Texture;
uniform float u_Offset;
uniform bool u_Poisoned;
varying vec2 v_UV;

void main()
{
	vec2 uv = v_UV;
	vec4 finalcolor = texture2D( u_Texture, uv );
	if(u_Poisoned)
	{
		uv.x += sin(uv.y * 4 * 2 * 3.14159 + u_Offset) / 100;
		finalcolor = texture2D( u_Texture, uv );
	}
	// calculate saturation
	float saturation = 0.8;
	vec3 luminanceWeights = vec3(0.5, 0.5, 0.2);
	float luminance = dot(finalcolor.rgb, luminanceWeights);
	finalcolor = mix(vec4(luminance), finalcolor, saturation);

	gl_FragColor = finalcolor;
}
