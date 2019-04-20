#version 330 core
layout (location = 0) out vec4 FragColorFluid;
layout (location = 1) out vec4 FragColorDensity;

in vec2 TexCoords;

uniform sampler2D fluid;
uniform sampler2D density;
uniform float timestep;
uniform vec2 pixelSize;
uniform float velocityDissipation;
uniform float densityDissipation;

void main()
{
  // Sample the fluid
  vec4 center = texture(fluid, TexCoords);

  // Advection step
  // Follow the velocity at the center pixel backwards in time
  // Sample the fluid at this position to get the new velocity
  vec2 newVelocityPos = TexCoords - center.rg * timestep * pixelSize;
  vec2 newVelocity = texture(fluid, newVelocityPos).rg * velocityDissipation;
  vec4 newDensity = texture(density, newVelocityPos) * densityDissipation;

  FragColorFluid = vec4(newVelocity, center.ba);
  FragColorDensity = newDensity;
}
