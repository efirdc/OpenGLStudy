#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D fluid;
uniform vec2 pixelSize;

void main()
{
  // Sample the fluid
  vec4 center = texture(fluid, TexCoords);
  vec2 upVelocity = texture(fluid, TexCoords + pixelSize * vec2(0.0, 1.0)).rg;
  vec2 downVelocity = texture(fluid, TexCoords - pixelSize * vec2(0.0, 1.0)).rg;
  vec2 rightVelocity = texture(fluid, TexCoords + pixelSize * vec2(1.0, 0.0)).rg;
  vec2 leftVelocity = texture(fluid, TexCoords - pixelSize * vec2(1.0, 0.0)).rg;

  float divergence = (rightVelocity.x - leftVelocity.x + upVelocity.y - downVelocity.y) * 0.5;

  FragColor = vec4(center.rgb, divergence);
}
