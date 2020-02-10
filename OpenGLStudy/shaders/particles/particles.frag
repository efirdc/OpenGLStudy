#version 440

#include "particles_common.glsl"
#include "../noise/hash.glsl"

out vec4 FragColor;

in GeometryData
{
	flat int particleID;
	flat Particle particle;
	flat int sorted;
    vec2 UV;
};

float gauss(vec2 p, float r)
{
	return exp(-dot(p, p) / r);
}

void main()
{	
	float splatSize = 0.01;
	vec3 col = vec3(0.0);
	if (sorted == OUT_OF_BOUNDS)
	{
		splatSize = 0.02;
		col = vec3(1.0, 1.0, 0.0);
	}
	else if (sorted == UNSORTED)
	{
		splatSize = 0.02;
		col = vec3(1.0, 0.0, 0.0);
	}
	else 
		col = vec3(0.0, 1.0, 0.0);

	vec2 p = UV - 0.5;
	float splat = gauss(p, splatSize);

	FragColor = vec4(col * splat, 1.0);
}