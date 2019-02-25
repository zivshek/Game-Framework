uniform Light u_Lights[MAX_LIGHTS];
uniform Material u_Material;

uniform vec3 u_CameraPosition;
uniform float u_Time;
uniform int u_NumOfLights;

varying vec3 v_SurfaceWorldPosition;
varying vec3 v_ViewWorldPosition;
varying vec2 v_UV;
varying vec3 v_Normal;

void main()
{
	// fog, http://in2gpu.com/2014/07/22/create-fog-shader/
	const vec3 fogColor = vec3(0.5, 0.5, 0.5);
	const float fogDensity = 0.05;
	float dist = length(v_ViewWorldPosition);
	//// 20 - fog starts; 80 - fog ends
    //fogFactor = (80 - dist)/(80 - 20);
	float fogFactor = 1.0 /exp( (dist * fogDensity)* (dist * fogDensity));
    fogFactor = clamp( fogFactor, 0.0, 1.0 );

	// Get the surface color of the material
	vec3 surfaceColor = texture2D(u_Material.texture, v_UV).rgb;

	vec3 lightColor = vec3(0,0,0);

	// Loop through all lights
	for (int i = 0; i < u_NumOfLights; i++)
	{
		// Only apply lighting if the light is turned on
		if(u_Lights[i].on)
		{
			lightColor += CalculateLightColor(u_Lights[i], u_Material, surfaceColor, v_SurfaceWorldPosition, u_CameraPosition, v_Normal, u_Time);
		}
	}
    
	// Mix fog and light colors
	vec3 finalColor = mix(fogColor, lightColor, fogFactor);

	gl_FragColor = vec4(finalColor, 1);
}
