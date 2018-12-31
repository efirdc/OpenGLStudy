#version 330 core
out vec4 FragColor;  
in vec3 ourColor;

uniform float time;
uniform vec4 modulate;
  
void main()
{
	float wave = sin(time) * 0.5 + 0.5;
    FragColor = vec4(ourColor * wave, 1.0);
}