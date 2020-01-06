#version 330

out vec4 FragColor;

in vec2 UV;
in vec3 FragPos;
in vec3 Eye;

#include "../noise/noise3d.glsl"
#include "../noise/noise3dgrad.glsl"
#include "../noise/hash.glsl"
#include "../color/colorconvert.glsl"

#include "material.glsl"
#include "sdf.glsl"
#include "light.glsl"

uniform float time;
uniform float gamma;

uniform vec3 fogColor;
uniform vec3 backgroundColor;

uniform sampler3D noiseData;
uniform sampler1D cloudColorCurve;

uniform DirectionalLight dirLight = DirectionalLight(vec3(-1), vec3(1));

uniform BlinnPhongMaterial material1 = BlinnPhongMaterial(vec3(0.26, 0.95, 0.39), 5.0);
uniform BlinnPhongMaterial material2 = BlinnPhongMaterial(vec3(0.4, 0.24, 0.16), 0.3);
uniform BlinnPhongMaterial material3 = BlinnPhongMaterial(vec3(0.01, 0.2, 0.5), 1.5);

vec3 lighting(vec3 rayPos, vec3 rayDir, vec3 norm, BlinnPhongMaterial material, float steps, float depth)
{
	float ambient = 0.7 * pow(1.02, -steps);
	vec3 directional = blinnPhongDirectionalLighting(dirLight, norm, rayDir, material) * 0.4;
	vec3 light = (directional + ambient) * material.color;
	float fog = exp(-depth*0.0005);
	
	return mix(fogColor, light, fog);
}

void main()
{
    vec3 rayDir = normalize(FragPos - Eye);

	const float stepSize = 0.2f;
	const float maxDepth = 500f;
	const int maxSteps = 200;

    float depth = 0;
	float density = 0;
	
    for (int i = 0; i < maxSteps; i++)
    {
		if (depth > maxDepth)
			break;

        vec3 rayPos = Eye + depth * rayDir;
		const float volumeSize = 20.0;
		sdResult r = sdBox(rayPos, vec3(volumeSize), material1);
		if (r.d > 0.4)
		{
			depth += r.d;
			continue;
		}
		vec4 data = texture(noiseData, (rayPos / volumeSize) * 0.5 + 0.5);
		density += max(0.0, data.y  - 0.6) * stepSize;
        depth += stepSize;
    }
	density *= 1;

	if (density < 0.0001)
	{
		FragColor = vec4(backgroundColor, 1.0);
		return;
	}

	float v = 2 * ( 1 - exp(-2 * density)) * exp(-density) ;
	vec3 color = pow(texture(cloudColorCurve, v).xyz, vec3(1 ));//vec3(v *0.7 + 0.1);
	//vec3 color = pal( v , vec3(0.5,0.5,0.5),vec3(0.5,0.5,0.5),vec3(2.0,1.0,0.0),vec3(0.5,0.20,0.25));
    FragColor = vec4(color + backgroundColor, 1);
}
