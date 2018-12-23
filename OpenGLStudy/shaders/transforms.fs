#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
  vec4 col1 = texture(texture1, TexCoord);
  vec4 col2 = texture(texture2, TexCoord);
  FragColor = mix(col1, col2, col2.a * 0.2);
}
