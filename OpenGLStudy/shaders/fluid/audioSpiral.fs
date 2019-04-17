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
uniform float timestep;
uniform float utime;
uniform vec2 pixelSize;

// Velocity is stored in the red and green channels. 
// Need to multiply by 2 and subtract 1 to convert from the [0:1] color range to [-1:1] velocity range
vec2 getVelocity(vec4 color) {return color.rg * 2.0 - 1.0;}

// Multiply by 0.5 and add 0.5 to shift back to color range
vec2 packVelocity(vec2 vel) {return vel * 0.5 + 0.5;}

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
    float angle = (utime * 0.05) * k * PI - utime * 5.0;
    vec2 spiralDir = vec2(cos(angle), sin(angle));
    vec2 spiralPoint = spiralDir * radius;
    vec2 deltaPos = spiralPoint + vec2(0.5) - TexCoords;
    if (length(deltaPos) < length(smallestDeltaPos)) {
      smallestDeltaPos = deltaPos;
      smallestkPct = kPct;
      velocityDir = spiralDir;
    }
  }
  vec2 force = velocityDir * gauss(smallestDeltaPos, 0.0002) * 0.9 * texture(frequency, smallestkPct).r;
  vec4 fluidSample = texture(fluid, TexCoords);
  vec2 newVelocity = getVelocity(fluidSample) + force;
  float densitySample = texture(density, TexCoords).r;
  float newDensity = densitySample + length(force) * 0.5;
  
  FragColorFluid = vec4(packVelocity(newVelocity), fluidSample.ba);
  FragColorDensity = vec4(newDensity);
}
