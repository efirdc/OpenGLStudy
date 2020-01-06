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

uniform sampler3D sphereData1;
uniform sampler3D sphereData2;

uniform DirectionalLight dirLight = DirectionalLight(vec3(-1), vec3(1));

uniform BlinnPhongMaterial material1 = BlinnPhongMaterial(vec3(0.26, 0.95, 0.39), 5.0);
uniform BlinnPhongMaterial material2 = BlinnPhongMaterial(vec3(0.4, 0.24, 0.16), 0.3);
uniform BlinnPhongMaterial material3 = BlinnPhongMaterial(vec3(0.01, 0.2, 0.5), 1.5);

sdResult gridSphere(vec3 p, vec3 gridCell, float gridSize)
{
    vec3 texSize = vec3(64);
	vec3 texPos = gridCell / texSize + 0.5;
	vec4 d1 = texture(sphereData1, texPos);
	vec4 d2 = texture(sphereData2, texPos);

	vec3 spherePos = (d1.xyz + gridCell) * gridSize;
	float sphereRadius = d1.w;
	BlinnPhongMaterial material = BlinnPhongMaterial(d2.xyz, d2.w);
	return sdSphere(p - spherePos, sphereRadius, material);
	//return sdBox(p - spherePos, vec3(sphereRadius), material);
}

sdResult sdScene(vec3 p)
{
	const float gridSize = 100.0;

	vec3 gridCell = floor(p / gridSize);
	sdResult r = sdResult(1e10, material1);
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			for (int k = -1; k <= 1; k++)
				r = sdSmoothUnion(r, gridSphere(p, gridCell + vec3(i, j, k), gridSize), 50);
    return r;
}

SDSCENENORMAL2(sdScene)


vec3 lighting(vec3 rayPos, vec3 rayDir, vec3 norm, BlinnPhongMaterial material, float steps, float depth)
{
	float ambient = 0.7 * pow(1.02, -steps);
	vec3 directional = blinnPhongDirectionalLighting(dirLight, norm, rayDir, material) * 0.4;
	vec3 light = (directional + ambient) * material.color;
	float fog = exp(-depth*0.0005);
	
	return mix(fogColor, light, fog);
}

vec3 skybox(vec3 rayDir)
{
	return pal( -rayDir.y, vec3(0.5,0.5,0.5),vec3(0.5,0.5,0.5),vec3(1.0,0.7,0.4),vec3(0.0,0.15,0.20) );
}

void main()
{
	/*
	FragColor = texture(sphereData2, vec3(UV, fract(time * 0.01)));
	return;
	*/

    vec3 rayDir = normalize(FragPos - Eye);
    float depth = 0;

	depth = 0;
	int i;
    for (i = 0; i < 100; i++)
    {
        vec3 rayPos = Eye + depth * rayDir;
        sdResult result = sdScene(rayPos);
		float d = result.d;
        if (d < 0.001)
        {
			vec3 norm = sdSceneNormal(rayPos);
            vec3 color = lighting(rayPos, rayDir, norm, result.mat, i, depth);
			FragColor = vec4(pow(color, vec3(1.0 / gamma)), 1.0);
            return;
        }
        depth += max(0.2, d);
		if (depth > 500.0)
			break;
    }
	float steps = i;
	vec3 color = backgroundColor * pow(1.05, -steps);
    FragColor = vec4(pow(color, vec3(1.0 / gamma)), 1);
}
