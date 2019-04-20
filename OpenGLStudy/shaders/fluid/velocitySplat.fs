#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D fluid;
uniform float timestep;
uniform vec2 pixelSize;

uniform vec2 mousePosition;
uniform vec2 mouseDelta;
uniform float radius;

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

  vec2 newVelocity = fluidVelocity.rg + splat * lineNormalized;

  FragColor = vec4(newVelocity, fluidSample.ba);
}
