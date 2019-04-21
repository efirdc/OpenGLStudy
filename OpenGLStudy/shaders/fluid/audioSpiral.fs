#version 330 core
layout (location = 0) out vec4 FragColorFluid;
layout (location = 1) out vec4 FragColorDensity;

#define SPREAD 0.05
#define SPIRALYNESS 1.46
#define NUM_POINTS 100

#define PI 3.1415926

in vec2 TexCoords;

uniform sampler2D fluid;
uniform sampler2D density;
uniform sampler1D frequency;
uniform float time;
uniform vec2 pixelSize;

uniform float spiralCurl;
uniform float spiralSpin;
uniform float splatRadius;
uniform float velocityAddScalar;
uniform float pressureAddScalar;
uniform float densityAddScalar;

float gauss(vec2 p, float r)
{
  return exp(-dot(p, p) / r);
}

void main()
{
  vec2 smallestDeltaPos = vec2(100000.0);
  float smallestkPct = 0.0;
  vec2 velocityDir = vec2(0.0);
  for (int i = 0; i < NUM_POINTS; i++) {
    float k = float(i);
    float kPct = k / float(NUM_POINTS);
    float radius = SPREAD * sqrt(k);
    float angle = (SPIRALYNESS + time * spiralCurl) * k * PI - time * spiralSpin;
    vec2 spiralDir = vec2(cos(angle), sin(angle));
    vec2 spiralPoint = spiralDir * radius;
    vec2 deltaPos = spiralPoint + vec2(0.5) - TexCoords;
    if (length(deltaPos) < length(smallestDeltaPos)) {
      smallestDeltaPos = deltaPos;
      smallestkPct = kPct;
      velocityDir = spiralDir;
    }
  }

  float splat = gauss(smallestDeltaPos, splatRadius);
  float frequencySample = texture(frequency, smallestkPct).r;

  vec2 velocityAdd = velocityDir * splat * frequencySample * velocityAddScalar;
  float pressureAdd = splat * frequencySample * pressureAddScalar;
  float densityAdd = splat * frequencySample * densityAddScalar;

  vec4 fluidSample = texture(fluid, TexCoords);
  float densitySample = texture(density, TexCoords).r;

  vec2 newVelocity = fluidSample.rg + velocityAdd;
  float newPressure = fluidSample.b + pressureAdd;
  float newDensity = densitySample + densityAdd;
  
  FragColorFluid = vec4(newVelocity, newPressure, fluidSample.a);
  FragColorDensity = vec4(newDensity);
}
