/*
Modification of the Shader class from: https://learnopengl.com
Originally authored by Joey de Vries: https://twitter.com/JoeyDeVriez
Licensed under the terms of the CC BY-NC 4.0 license as published by Creative Commons: https://creativecommons.org/licenses/by/4.0/legalcode
*/

#ifndef SHADER_H
#define SHADER_H

#include "BaseShader.h"

class Shader : public BaseShader
{
public:
	Shader(const char * vertexPath, const char * fragmentPath, std::vector<std::string> extraCode = {});
	/*
	* Constructor
	* Pre:
	*   vertexPath and fragmentPath are the paths to a vertex and fragment shader file
	* Post:
	*	shader program is created and ready to be used
	*/
	
	bool update();


private:
	
	const char * vertexPath;
	const char * fragmentPath;
	long long int vertexModifiedTime;
	long long int fragmentModifiedTime;


	bool makeProgram(const char * vShaderCode, const char * fShaderCode, unsigned int & newProgramID);
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