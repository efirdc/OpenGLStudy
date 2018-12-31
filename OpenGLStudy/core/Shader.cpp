/*
Modification of the Shader class from: https://learnopengl.com
Originally authored by Joey de Vries: https://twitter.com/JoeyDeVriez
Licensed under the terms of the CC BY-NC 4.0 license as published by Creative Commons: https://creativecommons.org/licenses/by/4.0/legalcode
*/

#include "Shader.h"

using namespace std;

Shader::Shader(const char * vertexPath, const char * fragmentPath) :
	vertexPath(vertexPath),
	fragmentPath(fragmentPath)
{
	// Retrieve the vertex/fragment source code from the filepaths
	string vertexCode;
	string fragmentCode;
	const char * cVertexCode = getCode(vertexPath, vertexCode);
	const char * cFragmentCode = getCode(fragmentPath, fragmentCode);

	// Save the last modified times of the vertex and fragment shader
	vertexModifiedTime = getModificationTime(vertexPath);
	fragmentModifiedTime = getModificationTime(fragmentPath);
	
	// Make the shader program
	makeProgram(cVertexCode, cFragmentCode, ID);
}

bool Shader::update()
{
	// Exit function if vertex and fragment shaders have not been modified
	long long int newVTime = getModificationTime(vertexPath);
	long long int newFTime = getModificationTime(fragmentPath);
	if (newVTime == vertexModifiedTime && newFTime == fragmentModifiedTime)
		return false;
	
	// Update modification times
	vertexModifiedTime = newVTime;
	fragmentModifiedTime = newFTime;

	// Retrieve the vertex/fragment source code from the filepaths
	string vertexCode;
	string fragmentCode;
	const char * cVertexCode = getCode(vertexPath, vertexCode);
	const char * cFragmentCode = getCode(fragmentPath, fragmentCode);

	// Make the shader program
	unsigned int newProgramID;
	if (makeProgram(cVertexCode, cFragmentCode, newProgramID))
	{
		glDeleteProgram(ID);
		ID = newProgramID;
	}
	else
		glDeleteProgram(newProgramID);
	return true;
}

const char * Shader::getCode(const char * codePath, string & code)
{
	// Declare the shaderfile and make sure it can throw errors
	ifstream shaderFile;
	shaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	// Open the file, read the file buffer contents, close the file, and then convert the stream into a string
	try
	{
		shaderFile.open(codePath);
		stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		code = shaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ::" << codePath << endl;
	}
	
	// Return as a c string
	return code.c_str();
}

bool Shader::makeProgram(const char * vShaderCode, const char * fShaderCode, unsigned int & newProgramID)
{
	// Declarations
	unsigned int vertex, fragment;
	int vSuccess, fSuccess, pSuccess;
	char infoLog[512];

	// Create anmd compile the vertex shader.
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

long long int Shader::getModificationTime(const char * filePath)
{
	struct stat result;
	if (stat(filePath, &result) != 0)
		cout << "ERROR::SHADER::CANT_READ_FILE_STATUS::" << filePath << endl;
	return result.st_mtime;
}

void Shader::use()
{
	glUseProgram(ID);
}

int Shader::getUniformLocation(const std::string & name)
{
	// Get the location of the uniform if it already exists in the uniformLocations map
	auto it = uniformLocations.find(name);
	if (it != uniformLocations.end())
		return it->second;

	// Get the location
	int location = glGetUniformLocation(ID, name.c_str());
	uniformLocations[name] = location;
	return location;
}

void Shader::setBool(const std::string & name, bool value)
{
	glUniform1i(getUniformLocation(name), (int)value);
}
void Shader::setInt(const std::string & name, int value)
{
	glUniform1i(getUniformLocation(name), value);
}
void Shader::setFloat(const std::string & name, float value)
{
	glUniform1f(getUniformLocation(name), value);
}
void Shader::setVec2(const std::string & name, const glm::vec2 & value)
{
	glUniform2fv(getUniformLocation(name), 1, &value[0]);
}
void Shader::setVec2(const std::string & name, float x, float y)
{
	glUniform2f(getUniformLocation(name), x, y);
}
void Shader::setVec3(const std::string & name, const glm::vec3 & value)
{
	glUniform3fv(getUniformLocation(name), 1, &value[0]);
}
void Shader::setVec3(const std::string & name, float x, float y, float z)
{
	glUniform3f(getUniformLocation(name), x, y, z);
}
void Shader::setVec4(const std::string & name, const glm::vec4 & value)
{
	glUniform4fv(getUniformLocation(name), 1, &value[0]);
}
void Shader::setVec4(const std::string & name, float x, float y, float z, float w)
{
	glUniform4f(getUniformLocation(name), x, y, z, w);
}
void Shader::setMat2(const std::string & name, const glm::mat2 & mat)
{
	glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat3(const std::string & name, const glm::mat3 & mat)
{
	glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat4(const std::string & name, const glm::mat4 & mat)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}