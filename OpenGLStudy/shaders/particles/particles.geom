#version 440

#include "particles_common.glsl"

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in VertexData
{
	flat int particleID;
	flat Particle particle;
	flat int sorted;
} inData[];

out GeometryData
{
	flat int particleID;
	flat Particle particle;
	flat int sorted;
    vec2 UV;
} outData;

uniform mat4 projection;
uniform mat4 view;
uniform float particleSize;

void main()
{
    outData.particleID = inData[0].particleID;
    outData.particle = inData[0].particle;
    outData.sorted = inData[0].sorted;

    //vec4 normal = view[2];

    //vec3 right = cross(normal.xyz, vec3(0, 1, 0));
    //vec3 up = cross(right, normal.xyz);

    vec3 center = gl_in[0].gl_Position.xyz;

    vec3 right = vec3(1, 0 , 0) * particleSize * 0.5;
    vec3 up = vec3(0, 1, 0) * particleSize * 0.5;

    //vec4 rightVector = projection * vec4(rightAxis.xyz, 1.0) * particleSize * 0.5;
    //vec4 upVector = projection * vec4(upAxis.xyz, 1.0) * particleSize * 0.5;
    
    gl_Position = projection * vec4(center + (-right - up) , 1.0);
    outData.UV = vec2(0, 0);
    EmitVertex();

    gl_Position = projection * vec4(center + (-right + up), 1.0);
    outData.UV = vec2(0, 1);
    EmitVertex();

    gl_Position = projection * vec4(center + (right - up), 1.0);
    outData.UV = vec2(1, 0);
    EmitVertex();

    gl_Position = projection * vec4(center + (right + up), 1.0);
    outData.UV = vec2(1, 1);
    EmitVertex();

    EndPrimitive(); 
}