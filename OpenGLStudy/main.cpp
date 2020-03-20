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
#include "GLComputeParticlesProgram.h"
#include "GLFluid2DProgram.h"


//#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include "xtensor/xarray.hpp"
#include "xtensor/xnpy.hpp"

int main()
{
	xt::xarray<float> test{ 1, 1, 1, 1 };
	GLFluid2DProgram program;
	program.run();
}

