#version 440
out vec4 FragColor;

in vec2 UV;

layout(binding = 0) uniform sampler2D rdSampler;
layout(binding = 1) uniform sampler1D lightGradientSampler;

#include "../raymarch/material.glsl"
#include "../raymarch/light.glsl"

uniform vec3 color1;
uniform vec3 color2;

uniform float aspect;

uniform ivec2 screenSize;
uniform ivec2 simulationSize;
uniform vec2 screenMousePos;


vec3 getNormal(out vec2 grad, float heightScale)
{	
	vec2 texelSize = 1.0 / simulationSize;
	texelSize.x *= aspect;
	vec4 L = texture(rdSampler, UV + texelSize * vec2(-1, 0));
	vec4 R = texture(rdSampler, UV + texelSize * vec2(1, 0));
	vec4 U = texture(rdSampler, UV + texelSize * vec2(0, 1));
	vec4 D = texture(rdSampler, UV + texelSize * vec2(0, -1));

	vec4 UL = texture(rdSampler, UV + texelSize * vec2(-1, 1));
	vec4 UR = texture(rdSampler, UV + texelSize * vec2(1, 1));
	vec4 DL = texture(rdSampler, UV + texelSize * vec2(-1, -1));
	vec4 DR = texture(rdSampler, UV + texelSize * vec2(1, -1));

	grad = vec2( 
		2.0 * (R.x - L.x) + (UR.x + DR.x - UL.x - DL.x),
		2.0 * (U.x - D.x) + (UL.x + UR.x - DL.x - DR.x)
	);

	grad *= heightScale;

	vec3 norm = normalize(cross(vec3(2.0 * texelSize.y, 0.0, -grad.x), vec3(0.0, 2.0 * texelSize.y, -grad.y)));
	return norm;
}

void main()
{
	vec4 chemicals = texture(rdSampler, UV);
	vec3 color = chemicals.x * color1 + chemicals.y * color2;

	float heightScale = 0.10;

	vec2 grad;
	vec3 norm = getNormal(grad, heightScale);

	//vec3 lightPos = vec3( 0.75, 1.25, 3.0);
	vec2 realMousePos = vec2(screenMousePos.x, screenSize.y - screenMousePos.y) / vec2(screenSize);
	vec3 fragPos = vec3(UV, chemicals.x * heightScale);

	DirectionalLight dl = DirectionalLight(normalize(vec3(-1, -1, -10)), vec3(3));

	PointLight pl = PointLight(vec3(realMousePos, 2), vec3(1.0, 0.09, 0.032), vec3(4.0));
	BlinnPhongMaterial material = BlinnPhongMaterial(color, 0.5);

	vec3 blinnPhong = blinnPhongPointLighting(pl, fragPos, norm, vec3(0,0,1), material);
	vec3 lightGradient = texture(lightGradientSampler, blinnPhong.x).xyz;
	//vec3 diffuse = blinnPhongDirectionalLighting(dl, norm, -vec3(0, 0, 1), material);
	vec3 ambient = vec3(0.1);
	vec3 lighting = (ambient + lightGradient) * material.color;

	FragColor = vec4(pow(lighting, vec3(1/2.2)), 1.0);
}