//?#version 440

#define LOCAL_SIZE_X 16
#define LOCAL_SIZE_Y 16

#define IMAGE_QUALIFIERS writeonly restrict

layout(binding = 0, rgba32f) IMAGE_QUALIFIERS uniform image2D velocityImage;
layout(binding = 1, rg32f) IMAGE_QUALIFIERS uniform image2D pressureImage;
layout(binding = 2, rgba32f) IMAGE_QUALIFIERS uniform image2D densityImage;
layout(binding = 3, rgba32f) IMAGE_QUALIFIERS uniform image2D gradientsImage;

layout(binding = 0) uniform sampler2D velocitySampler;
layout(binding = 1) uniform sampler2D pressureSampler;
layout(binding = 2) uniform sampler2D densitySampler;
layout(binding = 3) uniform sampler2D gradientsSampler;

uniform float time;
uniform float deltaTime;
uniform float timestep;

uniform ivec2 screenSize;
uniform ivec2 simulationSize;
uniform int numPressureIterations;
uniform float velocityDissipation;
uniform float densityDissipation;

uniform vec2 screenMousePos;
uniform vec2 prevScreenMousePos;

uniform bool mouseSplatActive;
uniform bool mouseOnUI;
uniform bool leftMouseDown;
uniform bool rightMouseDown;

float densityFlux(vec4 density)
{
	return ( density[0] * 2 + density[1] * 1 + density[2] * -1 + density[3] * -2 ) / 6;
}

float temporalLaplacian(vec4 d)
{
	return ( d.x * 1 + d.y * -2 + d.z * 1);
}

vec2 samplePressure(ivec2 icoords)
{
	return texelFetch(pressureSampler, icoords, 0).xy;
}

void texelFetch9(sampler2D s, ivec2 icoords, out vec4 texels[3][3])
{
	for (int i = -1; i < 2; i++)
	for (int j = -1; j < 2; j++)
		texels[i + 1][j + 1] = texelFetch(s, icoords + ivec2(i, j), 0);
}

#define TEXEL_FETCH_9_FUNC(funcName, sSampler, outType, components, sampleOp)\
void funcName(ivec2 icoords, out outType texels[3][3])\
{\
	for (int i = -1; i < 2; i++)\
	for (int j = -1; j < 2; j++)\
		texels[i + 1][j + 1] = texelFetch(sSampler, sampleOp(icoords + ivec2(i, j)), 0).components;\
}

#define clampSampleOp(pos) clamp(pos, ivec2(0), simulationSize)
#define nullSampleOp(pos) (pos)

TEXEL_FETCH_9_FUNC(texelFetchPressureDivergence9, pressureSampler, vec2, xy, nullSampleOp)
TEXEL_FETCH_9_FUNC(texelFetchPressure9, pressureSampler, float, x, nullSampleOp)
TEXEL_FETCH_9_FUNC(texelFetchVelocity9, velocitySampler, vec2, xy, nullSampleOp)

float computeDivergence(in vec2 texels[3][3])
{
	#define sin45 0.70710678
    const float kernel[3][3] = {
		{-sin45, 0, sin45},
		{-1, 0, 1},
		{-sin45, 0, sin45}
	};

	float divergence = 0.0;
	for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++)
		divergence += dot( vec2(kernel[i][j], kernel[j][i]), texels[i][j] );
	return divergence / 3;
}

vec2 computeGradient(in float texels[3][3])
{
	const float kernel[3][3] = {
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
	};

	vec2 gradient = vec2(0.0);
	for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++)
		gradient += texels[i][j] * vec2(kernel[i][j], kernel[j][i]);

	return gradient * 0.01;
}

float computeLaplacian(in float texels[3][3])
{
	const float kernel[3][3] = {
		{0.25, 0.5, 0.25},
		{0.5, -3.0, 0.5},
		{0.25, 0.5, 0.25}
	};
	const float kernel2[3][3] = {
		{.05, .2, .05},
		{.2, -1.0, .2},
		{.05, .2, .05}
	};

	float laplacian = 0.0;
	for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++)
		laplacian += texels[i][j] * kernel2[i][j];

	return laplacian;
}