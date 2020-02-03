int basicWindow();
int transformations();
int shaderTest();
int textures();
int sphereParticles();
int audioVisualizer();
int fluidSimulation();
int raymarchSpheres();
int raymarchNoise();

#include <list>
#include "GLFeedbackProgram.h"
#include "GLPlotProgram.h"
#include "GLReactionDiffusionProgram.h"
#include "GLReactionDiffusion3DProgram.h"

int main()
{
	GLReactionDiffusion3DProgram program;
	program.run();
}

