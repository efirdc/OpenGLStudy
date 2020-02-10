#version 440

#include "particles_common.glsl"

out VertexData
{
	flat int particleID;
	flat Particle particle;
	flat int sorted;
} outData;

uniform mat4 view;
uniform mat4 projection;

void main()
{ 
    outData.particleID = gl_VertexID + 1;
	outData.particle = destParticles[gl_VertexID];
	outData.sorted = isSorted(outData.particle.position, outData.particleID);
	gl_Position = view * vec4(outData.particle.position, 1.0); 
}