#version 440 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 UV;
out vec3 FragPos;
out vec3 Eye;

uniform mat4 view;
uniform float aspect;

void main()
{
    UV = aTexCoords;
    Eye = view[3].xyz;
    vec2 pos = aPos;
    pos.x *= aspect;
    FragPos = vec3(view * vec4(pos, -2, 1.0));
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);  
}
