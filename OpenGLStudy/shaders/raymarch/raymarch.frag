#version 330 core
out vec4 FragColor;

in vec2 UV;
in vec3 FragPos;
in vec3 Eye;

#include "../noise/noise3d.glsl"
#include "../noise/noise3dgrad.glsl"

#include "material.glsl"
#include "sdf.glsl"
#include "light.glsl"

uniform float time;
uniform DirLight dirLight = DirLight(vec3(-1), vec3(0.88, 0.86, 0.48), 0.5, 0.6);

uniform Material material1 = Material(vec3(0.26, 0.95, 0.39), 5.0, true);
uniform Material material2 = Material(vec3(0.4, 0.24, 0.16), 0.3, true);
uniform Material material3 = Material(vec3(0.01, 0.2, 0.5), 1.5, true);

sdResult sdScene(vec3 p)
{
    sdResult r = sdResult(1e10, material1);
    r = sdUnion(r, sdSphere(p - vec3(0.0, 0.0, -4.0), 0.4, material1));
    r = sdUnion(r, sdSphere(p - vec3(0.5, 0.0, -2.0), 0.4, material2));
    return r;
}

vec3 sdSceneNormal(vec3 p)
{
    vec2 epsilon = vec2(0.001, 0);
    return normalize(vec3(
        sdScene(p + epsilon.xyy).d - sdScene(p - epsilon.xyy).d,
        sdScene(p + epsilon.yxy).d - sdScene(p - epsilon.yxy).d,
        sdScene(p + epsilon.yyx).d - sdScene(p - epsilon.yyx).d
    ));
}




vec3 pal( in float t, in vec3 a, in vec3 b, in vec3 c, in vec3 d )
{
    return a + b*cos( 6.28318*(c*t+d) );
}

vec3 skybox(vec3 rayDir)
{
	return pal( -rayDir.y, vec3(0.5,0.5,0.5),vec3(0.5,0.5,0.5),vec3(1.0,0.7,0.4),vec3(0.0,0.15,0.20) );
}


void main()
{
    vec3 rayDir = normalize(FragPos - Eye);
    float depth = 0;

	/*
	float density = 0;
	for (int i = 0; i < 300; i++)
	{
		vec3 rayPos = Eye + depth * rayDir;

		density += max(0, snoise(rayPos)) * 0.41;

		if (density > 1)
		{
			vec3 grad;
			snoisegrad(rayPos, grad);
			FragColor = vec4(skybox(grad*0.15), 1);
			//FragColor = vec4(lighting(rayPos, rayDir, grad, material3), 1);
			return;
		}
		depth += max(0.01, depth*0.02);
	}*/

	depth = 0;
    for (int i = 0; i < 100; i++)
    {
        vec3 rayPos = Eye + depth * rayDir;
        sdResult result = sdScene(rayPos);
        if (result.d < 0.001)
        {
			vec3 norm = sdSceneNormal(rayPos);
            FragColor = vec4(lighting(rayPos, rayDir, norm, result.mat), 1);
            return;
        }
        depth += result.d;
    }
	
    FragColor = vec4(vec3(0.1), 1);
}
