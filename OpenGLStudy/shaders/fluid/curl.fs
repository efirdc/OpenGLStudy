#version 330 core
layout (location = 2) out vec4 FragColorCurl;

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

  float curl = (right.y - left.y - up.x + down.x) * 0.5;

  FragColorCurl = vec4(curl, center.gba);
}
