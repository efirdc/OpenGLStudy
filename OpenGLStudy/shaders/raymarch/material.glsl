#ifndef _MATERIAL
#define _MATERIAL

#include "../color/colorconvert.glsl"

struct BlinnPhongMaterial 
{
    vec3 color;  
    float shininess;
};

BlinnPhongMaterial materialMix(BlinnPhongMaterial a, BlinnPhongMaterial b, float t)
{
	return BlinnPhongMaterial(
		mix(a.color, b.color, t),
		mix(a.shininess, b.shininess, t)
	);
}

struct PhysicalMaterial
{
	vec3 albedo;
	float metallic;
	float roughness;
	float ao;
};

#endif
