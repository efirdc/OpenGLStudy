#version 330 core
layout (location = 0) out vec4 FragColorFluid;
layout (location = 1) out vec4 FragColorDensity;

in vec2 TexCoords;

uniform sampler2D fluid;
uniform sampler2D density;
uniform float timestep;
uniform vec2 pixelSize;

uniform vec2 mousePosition;
uniform vec2 mouseDelta;
uniform float radius;
uniform float mouseForce;
uniform float leftMouseDown;
uniform float rightMouseDown;

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
  vec2 pixelCoords = TexCoords / pixelSize;
  vec2 splatVector = pixelCoords - mousePosition;
  float splat = gauss(splatVector, radius);

  // Sample the fluid
  vec4 fluidSample = texture(fluid, TexCoords);
  vec2 fluidVelocity = getVelocity(fluidSample);
  vec2 newVelocity = fluidVelocity + splat * mouseDelta * mouseForce * leftMouseDown;

  FragColorFluid = vec4(packVelocity(newVelocity), fluidSample.ba);

  float densitySample = texture(density, TexCoords).r;
  float newDensity = densitySample + splat * rightMouseDown;

  FragColorDensity = vec4(newDensity);
}
