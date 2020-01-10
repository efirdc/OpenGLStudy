/*
Modification of the Shader class from: https://learnopengl.com
Originally authored by Joey de Vries: https://twitter.com/JoeyDeVriez
Licensed under the terms of the CC BY-NC 4.0 license as published by Creative Commons: https://creativecommons.org/licenses/by/4.0/legalcode
*/

#include "Shader.h"

using namespace std;

Shader::Shader(const char * vertexPath, const char * fragmentPath, std::vector<std::string> extraCode) :
	BaseShader(extraCode),
	vertexPath(vertexPath),
	fragmentPath(fragmentPath)
{
	// Retrieve the vertex/fragment source code from the filepaths
	string vertexCode = loadShaderCode(vertexPath);
	string fragmentCode = loadShaderCode(fragmentPath);

	// Save the last modified times of the vertex and fragment shader
	vertexModifiedTime = getModificationTime(vertexPath);
	fragmentModifiedTime = getModificationTime(fragmentPath);
	
	// Make the shader program
	makeProgram(vertexCode.c_str() , fragmentCode.c_str(), ID);

	// Get shader uniforms
	getActiveUniforms();

	// Bind global uniforms
	bindGlobalUniforms();
}

bool Shader::update()
{
	// Exit function if vertex and fragment shaders have not been modified
	long long int newVTime = getModificationTime(vertexPath);
	long long int newFTime = getModificationTime(fragmentPath);
	if (newVTime == vertexModifiedTime && newFTime == fragmentModifiedTime)
		return false;
	
	// Retrieve the vertex/fragment source code from the filepaths
	string vertexCode = loadShaderCode(vertexPath);
	string fragmentCode = loadShaderCode(fragmentPath);

	// Save the last modified times of the vertex and fragment shader
	vertexModifiedTime = getModificationTime(vertexPath);
	fragmentModifiedTime = getModificationTime(fragmentPath);

	// Make the shader program
	unsigned int newProgramID;
	if (makeProgram(vertexCode.c_str(), fragmentCode.c_str(), newProgramID))
	{
		glDeleteProgram(ID);
		ID = newProgramID;
		for (Uniform * uniform : uniformBindings)
			delete uniform;
		uniformBindings.clear();
		for (auto & pair : activeUniforms)
			delete pair.second;
		activeUniforms.clear();
		getActiveUniforms();
		bindGlobalUniforms();
		cout << "Shader " << vertexPath << " " << fragmentPath << " recompiled." << endl;
		return true;
	}
	glDeleteProgram(newProgramID);
	return false;
}

bool Shader::makeProgram(const char * vShaderCode, const char * fShaderCode, unsigned int & newProgramID)
{
	// Declarations
	unsigned int vertex, fragment;
	int vSuccess, fSuccess, pSuccess;
	char infoLog[512];

	// Create and compile the vertex shader.
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &vSuccess);
	if (!vSuccess)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED::" << vertexPath << "\n" << infoLog << std::endl;
	}

	// Create anmd compile the fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &fSuccess);
	if (!fSuccess)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED::" << fragmentPath << "\n" << infoLog << endl;
	}

	// Link the vertex and fragment shader in a shader program
	newProgramID = glCreateProgram();
	glAttachShader(newProgramID, vertex);
	glAttachShader(newProgramID, fragment);
	glLinkProgram(newProgramID);
	glGetProgramiv(newProgramID, GL_LINK_STATUS, &pSuccess);
	if (!pSuccess)
	{
		glGetProgramInfoLog(newProgramID, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED::" << vertexPath << "\n" << infoLog << endl;
	}

	// Clean up
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	// Return true if both shaders and the program successfully compiled
	return vSuccess && fSuccess && pSuccess;
}