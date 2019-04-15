#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D fluid;
uniform float timestep;
uniform vec2 pixelSize;

uniform vec2 mousePosition;
uniform vec2 mouseDelta;
uniform float radius;

// Velocity is stored in the red and green channels. 
// Need to multiply by 2 and subtract 1 to convert from the [0:1] color range to [-1:1] velocity range
vec2 getVelocity(vec4 color) {return color.rg * 2.0 - 1.0;}

// Multiply by 0.5 and add 0.5 to shift back to color range
vec2 packVelocity(vec2 vel) {return vel * 0.5 + 0.5;}

float gauss(vec2 p, float r)
{
  return exp(-dot(p, p) / r);
}

void main()
{
  
  //
  vec2 pixelPos = TexCoords / pixelSize;
  vec2 lineEndPos = mousePosition;
  vec2 lineStartPos = mousePosition - mouseDelta;
  vec2 lineNormalized = normalize(mouseDelta);
  float lineLength = length(mouseDelta);
  
  //
  vec2 linePixelPos = pixelPos - lineStartPos;
  float pixelPosProjection = dot(lineNormalized, linePixelPos);
  vec2 splatVector = vec2(0.0);
  if (pixelPosProjection < 0.0) {
    splatVector = pixelPos - lineStartPos;
  } else if (pixelPosProjection > lineLength) {
    splatVector = pixelPos - lineEndPos;
  } else {
    splatVector = pixelPos - pixelPosProjection * lineNormalized;
  }
  
  splatVector = vec2(5.0, 2.5);
  float splat = gauss(splatVector, radius);

  // Sample the fluid
  vec4 fluidSample = texture(fluid, TexCoords);
  vec2 fluidVelocity = getVelocity(fluidSample);

  vec2 newVelocity = fluidVelocity + splat * lineNormalized;

  FragColor = vec4(packVelocity(newVelocity), fluidSample.ba);
}
