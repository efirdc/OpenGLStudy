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
//#include "GLFeedbackProgram.h"
#include "GLPlotProgram.h"
#include "GLReactionDiffusionProgram.h"

int main()
{
	GLReactionDiffusionProgram program;
	program.run();
}

