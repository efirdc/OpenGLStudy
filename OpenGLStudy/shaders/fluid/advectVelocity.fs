#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D fluid;
uniform float timestep;
uniform vec2 pixelSize;

// Velocity is stored in the red and green channels. 
// Need to multiply by 2 and subtract 1 to convert from the [0:1] color range to [-1:1] velocity range
vec2 getVelocity(vec4 color) {return color.rg * 2.0 - 1.0;}

// Multiply by 0.5 and add 0.5 to shift back to color range
vec2 packVelocity(vec2 vel) {return vel * 0.5 + 0.5;}

void main()
{
  // Sample the fluid
  vec4 center = texture(fluid, TexCoords);
  vec2 centerVelocity = getVelocity(center);

  // Advection step
  // Follow the velocity at the center pixel backwards in time
  // Sample the fluid at this position to get the new velocity
  vec2 newVelocityPos = TexCoords - centerVelocity * timestep * pixelSize;
  vec2 newVelocity = getVelocity(texture(fluid, newVelocityPos));

  FragColor = vec4(packVelocity(newVelocity), center.b, 1.0);
}
