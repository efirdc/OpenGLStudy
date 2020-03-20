#version 440
out vec4 FragColor;

#include "fluid_common.glsl"
#include "../raymarch/material.glsl"
#include "../raymarch/light.glsl"
#include "../color/colorconvert.glsl"

in vec2 UV;

uniform vec3 color1;
uniform vec3 color2;

uniform float aspect;

uniform int renderMode;

uniform bool velocityVectors;
uniform bool densityGradientVectors;
uniform bool densityFluxGradientVectors;

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

const float ARROW_TILE_SIZE = 28.0;

vec2 arrowTileCenterCoord(vec2 pos) 
{
	return (floor(pos / ARROW_TILE_SIZE) + 0.5) * ARROW_TILE_SIZE;
}

float line(vec2 p, vec2 p1, vec2 p2) 
{
	vec2 center = (p1 + p2) * 0.5;
	float len = length(p2 - p1);
	vec2 dir = (p2 - p1) / len;
	vec2 rel_p = p - center;
	float dist1 = abs(dot(rel_p, vec2(dir.y, -dir.x)));
	float dist2 = abs(dot(rel_p, dir)) - 0.5*len;
	return max(dist1, dist2);
}

float arrow(vec2 p, vec2 v) 
{
	// Make everything relative to the center, which may be fractional
	p -= arrowTileCenterCoord(p);
		
	float mag_v = length(v), mag_p = length(p);
	
	if (mag_v > 0.0) {
		// Non-zero velocity case
		vec2 dir_v = v / mag_v;
		
		// We can't draw arrows larger than the tile radius, so clamp magnitude.
		// Enforce a minimum length to help see direction
		mag_v = clamp(mag_v, 0.0, ARROW_TILE_SIZE * 0.5);

		// Arrow tip location
		v = dir_v * mag_v;

		// Signed distance from shaft
		float shaft = line(p, v, -v);
		// Signed distance from head
		float head = min(line(p, v, 0.4*v + 0.2*vec2(-v.y, v.x)),
		                 line(p, v, 0.4*v + 0.2*vec2(v.y, -v.x)));

		return min(shaft, head);
	} else {
		// Signed distance from the center point
		return mag_p;
	}
}

vec3 quiverPlot(vec2 vectorSnapped, vec3 arrowColor, vec3 inColor)
{
	float arrow_dist = arrow(UV * screenSize, vectorSnapped * ARROW_TILE_SIZE * 0.5);
	arrow_dist = clamp(arrow_dist, 0.0, 1.0);

	float lengthClamped = clamp(length(vectorSnapped), 0.0, 1.0);
	return mix(inColor, arrowColor, smoothstep(0.1, 0.6, 1 - arrow_dist));
}

vec2 powLength(vec2 v, float a)
{
    float len = length(v);
	return v / len * pow(len, a);
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

	vec4 densityHist = texture(densitySampler, UV);
	float density = densityHist.r;

	vec3 color = mix(color1, color2, density);
	vec2 velocity = texture(velocitySampler, UV).xy;
	vec2 pressure = texture(pressureSampler, UV).xy;

	//FragColor = vec4(pow(vec3(velocity * 2 - 1, 0.5), vec3(1/2.2)), 1.0); return;

	color = pow(color, vec3(1/2.2));

	if (renderMode == 1)
	{
		color = vec3(velocity * 0.3 + 0.5, 0.5);
	}
	
	if (renderMode == 2)
	{
		float divergence = pressure.g;
		vec3 divcol = vec3(1.0);
		if (divergence > 0)
			divcol = mix(divcol, vec3(0.0, 0.0, 1.0), divergence * 10.0);
		else
			divcol = mix(divcol, vec3(1.0, 0.0, 0.0), abs(divergence * 10.0));
		color = divcol;
	}
	
	if (renderMode == 3)
	{
		float pressure = pressure.r;
		vec3 pressureColor = vec3(1.0);
		if (pressure > 0)
			pressureColor = mix(pressureColor, vec3(1.0, 0.0, 0.0), pressure * 0.25);
		else
			pressureColor = mix(pressureColor, vec3(0.0, 0.0, 1.0), abs(pressure * 0.25));
		color = pressureColor;
	}

	if (renderMode == 4)
	{
		float flux = densityFlux(densityHist);
		float fluxPow = 0.2;
		vec3 fluxColor = vec3(1.0);
		if (flux > 0)
			fluxColor = mix(fluxColor, vec3(1.0, 0.0, 0.0), pow(flux, fluxPow));
		else
			fluxColor = mix(fluxColor, vec3(0.0, 0.0, 1.0), pow(abs(flux), fluxPow));
		color = fluxColor;
	}

	vec2 gridSize = 1 / simulationSize;
	vec4 rightDensity = texture(densitySampler, UV + vec2(1, 0) * gridSize);
	vec4 leftDensity = texture(densitySampler, UV + vec2(1, 0) * gridSize);
	vec4 upDensity = texture(densitySampler, UV + vec2(0, 1) * gridSize);
	vec4 downDensity = texture(densitySampler, UV + vec2(0, -1) * gridSize);

	if (renderMode == 5)
	{
		float tLaplacian = temporalLaplacian(densityHist) * 500;
		vec3 tLaplacianColor = vec3(1.0);
		if (tLaplacian > 0.0)
			tLaplacianColor = mix(tLaplacianColor, vec3(1.0, 0.0, 0.0), tLaplacian);
		else
			tLaplacianColor = mix(tLaplacianColor, vec3(0.0, 0.0, 1.0), abs(tLaplacian));
		color = tLaplacianColor;
	}


	if (velocityVectors)
	{
		vec2 velocitySample = texture(velocitySampler, arrowTileCenterCoord(UV * screenSize) / screenSize).xy;
		color = quiverPlot(velocitySample, hsb2rgb(vec3(0.05, 1.0, 1.0)), color);
	}
	if (densityGradientVectors)
	{
		vec2 gradientSample = texture(gradientsSampler, arrowTileCenterCoord(UV * screenSize) / screenSize).xy;
		gradientSample = powLength(gradientSample, 0.1);
		color = quiverPlot(gradientSample, hsb2rgb(vec3(0.3, 1.0, 1.0)), color);
	}
	if (densityFluxGradientVectors)
	{
		vec2 gradientSample = texture(gradientsSampler, arrowTileCenterCoord(UV * screenSize) / screenSize).zw;
		gradientSample = powLength(gradientSample, 0.2) * 4;
		color = quiverPlot(gradientSample, hsb2rgb(vec3(0.8, 1.0, 1.0)), color);
	}

	if (autoCaptureToggle && captureState == CAPTURE_STATE_MIX)
	{
		vec2 deltaMixingPos = UV - mixingPos;
		if (length(deltaMixingPos) < 0.01)
		{
			color = vec3(1.0, 0.0, 0.0);
		}
	}
		
	FragColor = vec4(color, 1.0);
}