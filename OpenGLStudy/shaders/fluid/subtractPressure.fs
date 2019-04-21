#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D fluid;
uniform vec2 pixelSize;

void main()
{
  // Sample the fluid
  vec4 center = texture(fluid, TexCoords);
  vec4 up = texture(fluid, TexCoords + pixelSize * vec2(0.0, 1.0));
  vec4 down = texture(fluid, TexCoords - pixelSize * vec2(0.0, 1.0));
  vec4 right = texture(fluid, TexCoords + pixelSize * vec2(1.0, 0.0));
  vec4 left = texture(fluid, TexCoords - pixelSize * vec2(1.0, 0.0));

  vec2 pressureGradient = vec2(right.b - left.b, up.b - down.b) * 0.5;
  vec2 newVelocity = center.rg - pressureGradient;

  FragColor = vec4(newVelocity, center.ba);
}
