#version 330 core
layout (location = 2) out vec4 FragColorCurl;

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
  vec4 up = texture(fluid, TexCoords + pixelSize * vec2(0.0, 1.0));
  vec4 down = texture(fluid, TexCoords - pixelSize * vec2(0.0, 1.0));
  vec4 right = texture(fluid, TexCoords + pixelSize * vec2(1.0, 0.0));
  vec4 left = texture(fluid, TexCoords - pixelSize * vec2(1.0, 0.0));

  float curl = (right.y - left.y - up.x + down.x) * 0.5;

  FragColorCurl = vec4(curl, center.gba);
}
