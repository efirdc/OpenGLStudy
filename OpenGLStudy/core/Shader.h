/*
Modification of the Shader class from: https://learnopengl.com
Originally authored by Joey de Vries: https://twitter.com/JoeyDeVriez
Licensed under the terms of the CC BY-NC 4.0 license as published by Creative Commons: https://creativecommons.org/licenses/by/4.0/legalcode
*/

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
//#include <unistd.h>
#endif

#ifdef WIN32
#define stat _stat
#endif

#include "glad/glad.h"
#include "glm/glm.hpp"

class Shader
{
public:
	

	Shader(const char * vertexPath, const char * fragmentPath);
	/*
	* Constructor
	* Pre:
	*   vertexPath and fragmentPath are the paths to a vertex and fragment shader file
	* Post:
	*	shader program is created and ready to be used
	*/

	bool update();
	/*
	* Recompiles the shader program if there are any changes to the vertex or fragment shader files
	* Pre:
	*	none. can be called every frame
	* Post:
	*	If the files at vertexPath or fragmentPath have been updated, then the shader program will be recompiled.
	*	If recompilation is successful, then the old shader program will be destroyed and the ID overwritten. Returns true
	*	This will reset any shader uniforms. Make sure to set uniforms after calling update()
	*	Setting a texture uniform has a large performance impact, so only do this if update returns true.
	*	If recompilation fails, the new shader program gets destroyed and the shader ID is not changed. Returns false
	*/
	
	void use();
	/*
	* binds the shader program with glUseProgram()
	*/

	void setUniform(const std::string & name, bool value);
	void setUniform(const std::string & name, int value);
	void setUniform(const std::string & name, unsigned int value);
	void setUniform(const std::string & name, float value);
	void setUniform(const std::string & name, const glm::vec2 & value);
	void setUniform(const std::string & name, const glm::vec3 & value);
	void setUniform(const std::string & name, const glm::vec4 & value);
	void setUniform(const std::string & name, float x, float y);
	void setUniform(const std::string & name, float x, float y, float z);
	void setUniform(const std::string & name, float x, float y, float z, float w);
	void setUniform(const std::string & name, const glm::mat2 & mat);
	void setUniform(const std::string & name, const glm::mat3 & mat);
	void setUniform(const std::string & name, const glm::mat4 & mat);

private:
	unsigned int ID;
	const char * vertexPath;
	const char * fragmentPath;
	long long int vertexModifiedTime;
	long long int fragmentModifiedTime;

	struct Uniform 
	{
		std::string name;
		int location;
		GLenum type;
		int size;
	};
	std::map<std::string, Uniform> uniforms;

	const char * getCode(const char * codePath, std::string & code);
	/*
	* Gets shader code from a file as a c string
	* Pre:
	*	code is a new empty string object
	*	codePath is a path to the shader file
	* Post:
	*	the shader files code is copied into the code string
	*	returns the code as a c string
	*/

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

	void getUniforms();
	/*
	* Populates uniforms with this shaders uniform data
	* Pre:
	*	uniforms is empty
	* Post:
	*	uniforms is populated
	*/

	long long int getModificationTime(const char * filePath);
	/*
	* Returns time the file was last modified.
	*/
};

#endif