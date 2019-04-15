#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

#define ALL 0
#define VELOCITY 1
#define PRESSURE 2
#define DIVERGENCE 3

uniform int displayMode;
uniform sampler2D fluid;

void main()
{
  vec4 fluidSample = texture(fluid, TexCoords);
  
  if (displayMode == ALL) {
    FragColor = vec4(fluidSample.rgb, 1.0);
  } else if (displayMode == VELOCITY) {
    FragColor = vec4(fluidSample.rg, 0.5, 1.0);
  } else if (displayMode == PRESSURE) {
    FragColor = vec4(vec3(fluidSample.b), 1.0);
  } else if (displayMode == DIVERGENCE) {
    FragColor = vec4(vec3(fluidSample.a), 1.0);
  }
  
}
