//? #version 330

#ifndef _LIGHT
#define _LIGHT

#include "material.glsl"

uniform float kPi = 3.14159265;

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
};

struct PointLight 
{
    vec3 position;
    vec3 attenuation;
    vec3 color;
};

float blinnPhongSpecularity(float shininess, vec3 lightDir, vec3 norm, vec3 rayDir)
{
    float energyConservation = ( 8.0 + shininess ) / ( 8.0 * kPi );
    vec3 halfwayDir = normalize(lightDir + rayDir);
    return energyConservation * pow(max(dot(norm, halfwayDir), 0.0), shininess);
	
	/* no blinn, just phong
    float energyConservation = ( 2.0 + shininess ) / ( 2.0 * kPi ); 
    vec3 reflectDir = reflect(-lightDir, norm);
    return energyConservation * pow(max(dot(rayDir, reflectDir), 0.0), shininess);
	*/
}

float blinnPhongDiffuse(vec3 norm, vec3 lightDir)
{
	return max(dot(norm, lightDir), 0.0) / kPi;
}

float pointLightAttenuation(vec3 attenuation, float dist)
{
	return 1.0 / (attenuation.x + attenuation.y * dist + attenuation.z * dist * dist);
}

vec3 blinnPhongDirectionalLighting(DirectionalLight dirLight, vec3 norm, vec3 rayDir, BlinnPhongMaterial material)
{
    float diffuse = blinnPhongDiffuse(norm, -dirLight.direction);
    float specular = blinnPhongSpecularity(material.shininess, dirLight.direction, norm, rayDir);
    return (diffuse + specular) * dirLight.color;
}

vec3 blinnPhongPointLighting(PointLight pointLight, vec3 rayPos, vec3 norm, vec3 rayDir, BlinnPhongMaterial material)
{
	vec3 deltaPos = pointLight.position - rayPos;
	float dist = length(deltaPos);
	vec3 lightDir = deltaPos / dist;

    float diffuse = blinnPhongDiffuse(norm, lightDir);
	float specular = blinnPhongSpecularity(material.shininess, lightDir, norm, rayDir);
	float attenuation = pointLightAttenuation(pointLight.attenuation, dist);

    return (diffuse + specular) * attenuation * pointLight.color;
}

#endif
