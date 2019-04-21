#version 330 core
layout (location = 0) out vec4 FragColorFluid;
layout (location = 1) out vec4 FragColorDensity;

in vec2 TexCoords;

uniform sampler2D fluid;
uniform sampler2D density;
uniform vec2 pixelSize;

uniform vec2 mousePosition;
uniform vec2 mouseDelta;
uniform float radius;
uniform float velocityAddScalar;
uniform float pressureAddScalar;
uniform float densityAddScalar;
uniform float leftMouseDown;
uniform float rightMouseDown;

float gauss(vec2 p, float r)
{
  return exp(-dot(p, p) / r);
}

void main()
{
  vec2 pixelCoords = TexCoords / pixelSize;
  vec2 splatVector = pixelCoords - mousePosition;
  float splat = gauss(splatVector, radius);

  // Sample the fluid
  vec4 fluidSample = texture(fluid, TexCoords);
  float densitySample = texture(density, TexCoords).r;

  vec2 newVelocity = fluidSample.rg + splat * mouseDelta * velocityAddScalar * leftMouseDown;
  float newPressure = fluidSample.b + splat * pressureAddScalar * leftMouseDown;
  float newDensity = densitySample + splat * densityAddScalar * rightMouseDown;

  FragColorFluid = vec4(newVelocity, newPressure, fluidSample.a);
  FragColorDensity = vec4(newDensity);
}
