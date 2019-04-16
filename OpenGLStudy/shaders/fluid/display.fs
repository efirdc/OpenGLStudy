#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

#define ALL 0
#define VELOCITY 1
#define PRESSURE 2
#define DIVERGENCE 3
#define DENSITY 4

uniform int displayMode;
uniform sampler2D fluid;
uniform sampler2D density;

void main()
{
  vec4 fluidSample = texture(fluid, TexCoords);
  vec4 densitySample = texture(density, TexCoords);
  
  if (displayMode == ALL) {
    FragColor = vec4(fluidSample.rgb, 1.0);
  } else if (displayMode == VELOCITY) {
    FragColor = vec4(fluidSample.rg, 0.5, 1.0);
  } else if (displayMode == PRESSURE) {
    FragColor = vec4(vec3(fluidSample.b), 1.0);
  } else if (displayMode == DIVERGENCE) {
    FragColor = vec4(vec3(fluidSample.a), 1.0);
  } else if (displayMode == DENSITY) {
    FragColor = densitySample;
  }
  
}
