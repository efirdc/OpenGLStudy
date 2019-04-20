#version 330 core
layout (location = 0) out vec4 FragColorFluid;
layout (location = 2) out vec4 FragColorCurl;

in vec2 TexCoords;

uniform sampler2D fluid;
uniform sampler2D curl;
uniform float timestep;
uniform vec2 pixelSize;
uniform float vorticityScalar;

void main()
{
  // Sample the fluid and curl
  vec4 centerFluid = texture(fluid, TexCoords);
  vec4 centerCurl = texture(curl, TexCoords);
  vec4 upCurl = texture(curl, TexCoords + pixelSize * vec2(0.0, 1.0));
  vec4 downCurl = texture(curl, TexCoords - pixelSize * vec2(0.0, 1.0));
  vec4 rightCurl = texture(curl, TexCoords + pixelSize * vec2(1.0, 0.0));
  vec4 leftCurl = texture(curl, TexCoords - pixelSize * vec2(1.0, 0.0));
  
  vec2 vorticity = vec2(abs(upCurl.r) - abs(downCurl.r), abs(rightCurl.r) - abs(leftCurl.r)) * 0.5;
  vorticity /= length(vorticity) + 0.0001;
  vorticity *= vorticityScalar * centerCurl.r;
  vorticity.y *= -1.0;

  vec2 newVelocity = centerFluid.rg + vorticity * timestep;

  FragColorFluid = vec4(newVelocity, centerFluid.ba);
  FragColorCurl = vec4(centerCurl.x, vorticity, centerCurl.a);
}
