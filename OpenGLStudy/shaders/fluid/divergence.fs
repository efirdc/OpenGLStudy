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
  vec4 upVelocity = getVelocity(texture(fluid, TexCoords + pixelSize * vec2(0.0, 1.0)));
  vec4 downVelocity = getVelocity(texture(fluid, TexCoords - pixelSize * vec2(0.0, 1.0)));
  vec4 rightVelocity = getVelocity(texture(fluid, TexCoords + pixelSize * vec2(1.0, 0.0)));
  vec4 leftVelocity = getVelocity(texture(fluid, TexCoords - pixelSize * vec2(1.0, 0.0)));

  float divergence = (rightVelocity.x - leftVelocity.x + upVelocity.y - downVelocity.y) * 0.5;

  FragColor = vec4(center.rgb, divergence);
}
