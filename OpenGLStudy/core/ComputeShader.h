#ifndef COMPUTE_SHADER_H
#define COMPUTE_SHADER_H

#include "BaseShader.h"

class ComputeShader : public BaseShader
{
public:
	static void printSizes();

	ComputeShader(const char * path);

	bool update();

private:

	const char * path;
	long long int modifiedTime;

	bool makeProgram(const char * shaderCode, unsigned int & newProgramID);
	/*
	* Creates a new shader program
	* Pre:
	*	vShaderCode and fShaderCode are c strings containing the vertex and fragment shader code
	*	newProgramID is an integer that can contain the new ID of the program
	* Post:
	*	returns true if the program successfully compiles, false otherwise.
	*	newProgramID is set to the new ID of the program
	*/
};

#endif