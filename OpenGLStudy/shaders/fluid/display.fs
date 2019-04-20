#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

#define ALL 0
#define VELOCITY 1
#define PRESSURE 2
#define DIVERGENCE 3
#define DENSITY 4
#define DENSITY_COLOR 5
#define CURL 6
#define VORTICITY 7

uniform int displayMode;
uniform sampler2D fluid;
uniform sampler2D density;
uniform sampler2D curl;
uniform sampler1D densityColorCurve;

void main()
{
  vec4 fluidSample = texture(fluid, TexCoords);
  fluidSample.rgb = fluidSample.rgb * 0.05 + 0.5;
  vec4 densitySample = texture(density, TexCoords);
  vec4 curlSample = texture(curl, TexCoords);
  curlSample.gb = curlSample.gb * 0.5 + 0.5;
  
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
  } else if (displayMode == DENSITY_COLOR) {
    FragColor = texture(densityColorCurve, densitySample.r);
  } else if (displayMode == CURL) {
    FragColor = vec4(vec3(curlSample.r), 1.0);
  } else if (displayMode == VORTICITY) {
    FragColor = vec4(curlSample.gb, 0.5, 1.0);
  }
  
}
