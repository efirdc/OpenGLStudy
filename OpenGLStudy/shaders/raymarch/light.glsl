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
    float diffuse = blinnPhongDiffuse(norm, dirLight.direction);
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

// PBR
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}  
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = kPi * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 physicalDirectionalLighting(DirectionalLight dirLight, vec3 norm, vec3 rayDir, PhysicalMaterial material)
{
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, material.albedo, material.metallic);

    vec3 radiance = dirLight.color;

    vec3 L = dirLight.direction;
    vec3 N = norm;
    vec3 V = rayDir;
    vec3 H = normalize(L + V);

    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, material.roughness);        
    float G   = GeometrySmith(N, V, L, material.roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);  

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - material.metallic;	  
        
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular     = numerator / max(denominator, 0.001);  
            
    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);                
    return (kD * material.albedo / kPi + specular) * radiance * NdotL; 
}

#endif
