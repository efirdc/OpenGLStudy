#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D fluid;
uniform float timestep;
uniform vec2 pixelSize;

void main()
{
  // Sample the fluid
  vec4 center = texture(fluid, TexCoords);
  vec4 up = texture(fluid, TexCoords + pixelSize * vec2(0.0, 1.0));
  vec4 down = texture(fluid, TexCoords - pixelSize * vec2(0.0, 1.0));
  vec4 right = texture(fluid, TexCoords + pixelSize * vec2(1.0, 0.0));
  vec4 left = texture(fluid, TexCoords - pixelSize * vec2(1.0, 0.0));
  
  float divergence = center.a;
  float pressure = (up.b + down.b + right.b + left.b - divergence) * 0.25;

  FragColor = vec4(center.rg, pressure, center.a);
}
