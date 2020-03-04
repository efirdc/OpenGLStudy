#version 440
out vec4 FragColor;

#include "fluid_common.glsl"
#include "../raymarch/material.glsl"
#include "../raymarch/light.glsl"

in vec2 UV;

uniform vec3 color1;
uniform vec3 color2;

uniform float aspect;

uniform int renderMode;

vec3 getNormal(out vec2 grad, float heightScale)
{	
	vec2 texelSize = 1.0 / simulationSize;
	texelSize.x *= aspect;
	vec4 L = texture(densitySampler, UV + texelSize * vec2(-1, 0));
	vec4 R = texture(densitySampler, UV + texelSize * vec2(1, 0));
	vec4 U = texture(densitySampler, UV + texelSize * vec2(0, 1));
	vec4 D = texture(densitySampler, UV + texelSize * vec2(0, -1));

	vec4 UL = texture(densitySampler, UV + texelSize * vec2(-1, 1));
	vec4 UR = texture(densitySampler, UV + texelSize * vec2(1, 1));
	vec4 DL = texture(densitySampler, UV + texelSize * vec2(-1, -1));
	vec4 DR = texture(densitySampler, UV + texelSize * vec2(1, -1));

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
	/*vec3 color = chemicals.x * color1 + chemicals.y * color2;

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
	*/
	float density = texture(densitySampler, UV).r;

	vec3 col = mix(color1, color2, density);
	vec2 velocity = texture(velocitySampler, UV).xy;
	vec2 pressure = texture(pressureSampler, UV).xy;

	//FragColor = vec4(pow(vec3(velocity * 2 - 1, 0.5), vec3(1/2.2)), 1.0); return;
	FragColor = vec4(pow(col, vec3(1/2.2)), 1.0);

	if (renderMode == 1)
	{
		FragColor = vec4(velocity * 0.3 + 0.5, 0.5, 1.0);
		return;
	}
	
	if (renderMode == 2)
	{
		float divergence = pressure.g;
		vec3 divcol = vec3(1.0);
		if (divergence > 0)
			divcol = mix(divcol, vec3(0.0, 0.0, 1.0), divergence * 10.0);
		else
			divcol = mix(divcol, vec3(1.0, 0.0, 0.0), abs(divergence * 10.0));
		FragColor = vec4(divcol, 1.0);
		return;
	}
	
	if (renderMode == 3)
	{
		float divergence = pressure.r;
		vec3 divcol = vec3(1.0);
		if (divergence > 0)
			divcol = mix(divcol, vec3(1.0, 0.0, 0.0), divergence * 0.05);
		else
			divcol = mix(divcol, vec3(0.0, 0.0, 1.0), abs(divergence * 0.05));
		FragColor = vec4(divcol, 1.0);
		return;
	}

	if (renderMode == 4)
	{
		vec2 pressureGrad = texture(velocitySampler, UV).zw;
		FragColor = vec4(pressureGrad * 0.5 + 0.5, 0.5, 1.0);
	}
		
}