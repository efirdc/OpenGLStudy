#version 330 core
out vec4 FragColor;

in vec3 Norm;
in vec2 UV;
in vec3 FragPos;

uniform vec3 lightColor;

void main()
{
  FragColor = vec4(lightColor, 1.0);
}
